#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "../common/protocol.h"

#define PORT 8080

// Funções de exibição do tabuleiro
void desenha_matriz(char tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    printf("    ");
    for (int col = 0; col < TAMANHO_TABULEIRO; col++) printf(" %c  ", 'A' + col);
    printf("\n");
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf(" %2d ", i + 1);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            char celula = tabuleiro[i][j];
            printf("[%c] ", celula == DESCONHECIDO ? ' ' : celula);
        }
        printf("\n");
    }
}
void exibe_tabuleiros(char meu_tabuleiro[8][8], char tabuleiro_tiros[8][8]) {
    system("clear || cls");
    printf("      === TABULEIRO DE TIROS (INIMIGO) ===\n");
    desenha_matriz(tabuleiro_tiros);
    printf("\n            === MEU TABULEIRO ===\n");
    desenha_matriz(meu_tabuleiro);
    printf("\n");
}
void inicializa_tabuleiros(char meu_tabuleiro[8][8], char tabuleiro_tiros[8][8]) {
    for (int i=0; i<8; i++) for (int j=0; j<8; j++) {
        meu_tabuleiro[i][j] = AGUA;
        tabuleiro_tiros[i][j] = DESCONHECIDO;
    }
}


int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[MAX_MSG] = {0};
    char meu_tabuleiro[8][8], tabuleiro_tiros[8][8];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("Conexão falhou.\n");
        return -1;
    }

    inicializa_tabuleiros(meu_tabuleiro, tabuleiro_tiros);
    exibe_tabuleiros(meu_tabuleiro, tabuleiro_tiros);
    printf("Conectado. Posicione seus navios.\n");

    int navios_a_posicionar[4] = {1, 2, 2, 3};
    for (int i = 0; i < TOTAL_NAVIOS; i++) {
        printf("\nPosicione o navio de tamanho %d\n", navios_a_posicionar[i]);
        char coord_str[4], orient_char;
        int x, y;

        while(1) {
            printf("Coordenada inicial (ex: A1): ");
            scanf("%s", coord_str);
            printf("Orientação (H/V): ");
            scanf(" %c", &orient_char);

            y = toupper(coord_str[0]) - 'A';
            x = atoi(&coord_str[1]) - 1;
            
            sprintf(buffer, "POS %d %d %c %d", x, y, toupper(orient_char), navios_a_posicionar[i]);
            send(sock, buffer, strlen(buffer), 0);
            
            memset(buffer, 0, MAX_MSG);
            read(sock, buffer, MAX_MSG);

            if(strncmp(buffer, "OK", 2) == 0){
                for (int k=0; k<navios_a_posicionar[i]; k++) {
                    if (toupper(orient_char) == 'H') meu_tabuleiro[x][y+k] = BARCO;
                    else meu_tabuleiro[x+k][y] = BARCO;
                }
                exibe_tabuleiros(meu_tabuleiro, tabuleiro_tiros);
                break;
            } else {
                printf("Posição inválida! Tente novamente.\n");
            }
        }
    }
    send(sock, "READY", strlen("READY"), 0);
    printf("Navios posicionados. Aguardando o outro jogador...\n");

    while (1) {
        memset(buffer, 0, MAX_MSG);
        int bytes_read = read(sock, buffer, MAX_MSG);
        if (bytes_read <= 0) break;

        if (strncmp(buffer, CMD_PLAY, strlen(CMD_PLAY)) == 0) {
            printf("Sua vez! Atire (ex: C5): ");
            char tiro_str[4];
            scanf("%s", tiro_str);
            int tx = atoi(&tiro_str[1]) - 1;
            int ty = toupper(tiro_str[0]) - 'A';
            sprintf(buffer, "FIRE %d %d", tx, ty);
            send(sock, buffer, strlen(buffer), 0);
        } else if (strncmp(buffer, CMD_HIT, strlen(CMD_HIT)) == 0 || strncmp(buffer, CMD_MISS, strlen(CMD_MISS)) == 0) {
            char res[5]; int res_x, res_y;
            sscanf(buffer, "%s %d %d", res, &res_x, &res_y);
            
            if (meu_tabuleiro[res_x][res_y] == BARCO) {
                meu_tabuleiro[res_x][res_y] = ACERTO;
            } else {
                 tabuleiro_tiros[res_x][res_y] = res[0];
            }
            exibe_tabuleiros(meu_tabuleiro, tabuleiro_tiros);
            printf("Resultado do último tiro: %s em (%c%d)\n", res, res_y + 'A', res_x + 1);
        } else if (strncmp(buffer, CMD_WIN, strlen(CMD_WIN)) == 0) {
            printf("\nPARABÉNS! VOCÊ VENCEU!\n");
            break;
        } else if (strncmp(buffer, CMD_LOSE, strlen(CMD_LOSE)) == 0) {
            printf("\nFIM DE JOGO! VOCÊ PERDEU.\n");
            break;
        }
    }

    close(sock);
    return 0;
}