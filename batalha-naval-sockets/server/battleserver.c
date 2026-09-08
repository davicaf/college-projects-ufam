#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h> // Biblioteca para Threads

#include "../common/protocol.h"

#define PORT 8080
#define TOTAL_NAVIOS 4

// --- Estruturas de Dados ---
typedef struct {
    int socket_fd;
    char tabuleiro[8][8];
    int partes_restantes;
    bool pronto;
} Jogador;

typedef struct {
    Jogador jogadores[2];
    int turno_atual;
    int jogadores_prontos;
    pthread_mutex_t lock; // Mutex para proteger o acesso a esta struct
} EstadoJogo;

typedef struct {
    int jogador_idx;
    EstadoJogo* jogo;
} ThreadArgs;

// --- Protótipos ---
void inicializar_jogo(EstadoJogo *jogo);
int processar_tiro(EstadoJogo *jogo, int atacante_idx, char* comando_fire);
void* rotina_do_jogador(void* args);


int main() {
    int server_fd;
    struct sockaddr_in address;
    EstadoJogo jogo_compartilhado;

    inicializar_jogo(&jogo_compartilhado);
    pthread_mutex_init(&jogo_compartilhado.lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind falhou");
        exit(EXIT_FAILURE);
    }
    listen(server_fd, 2);
    printf("Servidor Concorrente iniciado na porta %d...\n", PORT);

    pthread_t threads[2];
    ThreadArgs thread_args[2];

    for (int i = 0; i < 2; i++) {
        printf("Aguardando Jogador %d...\n", i + 1);
        int client_sock = accept(server_fd, NULL, NULL);
        printf("Jogador %d conectado.\n", i + 1);
        
        pthread_mutex_lock(&jogo_compartilhado.lock);
        jogo_compartilhado.jogadores[i].socket_fd = client_sock;
        thread_args[i] = (ThreadArgs){ .jogo = &jogo_compartilhado, .jogador_idx = i };
        pthread_mutex_unlock(&jogo_compartilhado.lock);

        pthread_create(&threads[i], NULL, rotina_do_jogador, &thread_args[i]);
    }

    // A thread principal espera as threads dos jogadores terminarem
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("Fim de jogo. Encerrando servidor.\n");
    pthread_mutex_destroy(&jogo_compartilhado.lock);
    close(server_fd);

    return 0;
}

// --- Lógica do Jogo e da Thread ---

void inicializar_jogo(EstadoJogo *jogo) {
    jogo->turno_atual = 0;
    jogo->jogadores_prontos = 0;
    for (int i = 0; i < 2; i++) {
        jogo->jogadores[i].partes_restantes = 0;
        jogo->jogadores[i].pronto = false;
        for (int l = 0; l < 8; l++) for (int c = 0; c < 8; c++) {
            jogo->jogadores[i].tabuleiro[l][c] = AGUA;
        }
    }
}

// Rotina que cada thread executa para um jogador
void* rotina_do_jogador(void* args) {
    ThreadArgs* t_args = (ThreadArgs*)args;
    EstadoJogo* jogo = t_args->jogo;
    int meu_idx = t_args->jogador_idx;
    int meu_socket = jogo->jogadores[meu_idx].socket_fd;
    char buffer[MAX_MSG];

    // --- FASE DE POSICIONAMENTO ---
    while (true) {
        int bytes_read = read(meu_socket, buffer, MAX_MSG);
        if (bytes_read <= 0) break;
        buffer[bytes_read] = '\0';

        pthread_mutex_lock(&jogo->lock);
        if (strncmp(buffer, CMD_POS, 3) == 0) {
            int x, y, tamanho; char orientacao;
            sscanf(buffer, "POS %d %d %c %d", &x, &y, &orientacao, &tamanho);
            
            bool valido = true; // TODO: Lógica de validação completa aqui
            if (valido) {
                for (int i = 0; i < tamanho; i++) {
                    if (orientacao == 'H') jogo->jogadores[meu_idx].tabuleiro[x][y + i] = BARCO;
                    else jogo->jogadores[meu_idx].tabuleiro[x + i][y] = BARCO;
                }
                jogo->jogadores[meu_idx].partes_restantes += tamanho;
                send(meu_socket, "OK", 2, 0);
            } else {
                send(meu_socket, "ERRO", 4, 0);
            }
        } else if (strncmp(buffer, CMD_READY, 5) == 0) {
            jogo->jogadores[meu_idx].pronto = true;
            jogo->jogadores_prontos++;
            printf("Jogador %d está pronto.\n", meu_idx + 1);
            pthread_mutex_unlock(&jogo->lock);
            break;
        }
        pthread_mutex_unlock(&jogo->lock);
    }
    
    // Sincronização: espera o outro jogador ficar pronto
    while (jogo->jogadores_prontos < 2) {
        sleep(1);
    }

    // --- FASE DE BATALHA ---
    bool jogo_terminou = false;
    while (!jogo_terminou) {
        pthread_mutex_lock(&jogo->lock);
        bool minha_vez = (jogo->turno_atual == meu_idx);
        pthread_mutex_unlock(&jogo->lock);

        if (minha_vez) {
            send(meu_socket, CMD_PLAY, strlen(CMD_PLAY), 0);
            int bytes_read = read(meu_socket, buffer, MAX_MSG);
            if (bytes_read <= 0) break;
            buffer[bytes_read] = '\0';
            
            pthread_mutex_lock(&jogo->lock);
            if (strncmp(buffer, CMD_FIRE, 4) == 0) {
                if (processar_tiro(jogo, meu_idx, buffer) == 1) {
                    jogo_terminou = true;
                }
            }
            pthread_mutex_unlock(&jogo->lock);
        } else {
            sleep(1); // Espera um pouco antes de checar o turno de novo
        }
        
        // Checa se o jogo terminou por ação do outro jogador
        pthread_mutex_lock(&jogo->lock);
        int oponente_idx = (meu_idx == 0) ? 1 : 0;
        if (jogo->jogadores[oponente_idx].partes_restantes == 0 || jogo->jogadores[meu_idx].partes_restantes == 0){
            jogo_terminou = true;
        }
        pthread_mutex_unlock(&jogo->lock);
    }
    return NULL;
}


int processar_tiro(EstadoJogo *jogo, int atacante_idx, char* comando_fire) {
    int x, y;
    if (sscanf(comando_fire, "FIRE %d %d", &x, &y) != 2 || x < 0 || x >= 8 || y < 0 || y >= 8) return 0;

    int defensor_idx = (atacante_idx == 0) ? 1 : 0;
    char* celula_ptr = &jogo->jogadores[defensor_idx].tabuleiro[x][y];
    char msg_resposta[MAX_MSG];

    if (*celula_ptr == BARCO) {
        *celula_ptr = ACERTO;
        jogo->jogadores[defensor_idx].partes_restantes--;
        sprintf(msg_resposta, "%s %d %d", CMD_HIT, x, y);
    } else {
        if (*celula_ptr == AGUA) *celula_ptr = ERRO;
        sprintf(msg_resposta, "%s %d %d", CMD_MISS, x, y);
        jogo->turno_atual = defensor_idx;
    }
    
    send(jogo->jogadores[0].socket_fd, msg_resposta, strlen(msg_resposta), 0);
    send(jogo->jogadores[1].socket_fd, msg_resposta, strlen(msg_resposta), 0);

    if (jogo->jogadores[defensor_idx].partes_restantes == 0) {
        send(jogo->jogadores[atacante_idx].socket_fd, CMD_WIN, strlen(CMD_WIN), 0);
        send(jogo->jogadores[defensor_idx].socket_fd, CMD_LOSE, strlen(CMD_LOSE), 0);
        return 1;
    }
    return 0;
}