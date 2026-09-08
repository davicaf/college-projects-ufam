#include <stdio.h> 
#include <stdlib.h> 
#include "Fila.h" 

Fila* criar_fila() { 
    Fila* tempfila = malloc(sizeof(Fila));
    if (tempfila == NULL) {
        printf("Erro ao criar a fila\n");
        exit(1);
    }
    tempfila->prim = NULL; 
    tempfila->ult = NULL; 
    return tempfila; 
} 

void inserir_na_fila(Fila* f, int v) { 
    FilaNo* n = malloc(sizeof(FilaNo)); // Aloca memória para um novo nó da fila
    if (n == NULL) { 
        exit(1); // Caso a alocação falhe, o programa encerra com erro
    }

    n->idade = v; // Atribui o valor v (a idade) ao novo nó
    n->prox = NULL; // O novo nó será o último, então seu ponteiro "prox" será NULL
    n->ant = f->ult; // O novo nó terá como anterior o nó que atualmente é o último da fila

    if (f->ult != NULL) { 
        f->ult->prox = n; // Se a fila não estiver vazia, o ponteiro "prox" do último nó aponta para o novo nó
    } else {
        f->prim = n; // Se a fila estiver vazia, o novo nó se torna o primeiro da fila
    }

    f->ult = n; // O novo nó agora é o último da fila
}


int verifica_ordem(Fila* f, int crescente) {
    if (f->prim == NULL){
        return 1;
    }; // Se a fila estiver vazia consideramos como já ordenada

    FilaNo* p = f->prim; //Variavel auxiliar para percorrer os nós da fila
    while (p->prox != NULL) { // Percorre enquanto houver um próximo elemento
        //Se um elemento for maior que o proximo não está crescente || Se um elemento for menor que o proximo não está decrescente
        if ((crescente && p->idade > p->prox->idade) || (!crescente && p->idade < p->prox->idade)) { 
            return 0; // Retorna 0 assim que uma desordem for encontrada
        }
        p = p->prox; // Passa para o próximo nó
    }
    return 1; // Retorna verdadeiro se a fila estiver ordenada
}


void organizar_fila(Fila* f) {
    if (f->prim == NULL || f->prim->prox == NULL) return; // Fila vazia ou com um único elemento, não há o que ajustar

    if (verifica_ordem(f, 1)) { // Verifica se a fila está em ordem crescente
        // Fila está em ordem crescente, inverte
        FilaNo* n = f->prim; // Variável auxiliar para percorrer os nós
        FilaNo* n2 = NULL; // Variável auxiliar para manipulação dos ponteiros

        while (n != NULL) { // Percorre a fila até o final
            n2 = n->ant; // Armazena o nó anterior
            n->ant = n->prox; // Inverte o ponteiro anterior, apontando para o próximo nó
            n->prox = n2; // Inverte o ponteiro próximo, apontando para o nó anterior
            n = n->ant; // Avança para o próximo nó
        }

        n2 = f->prim; // Após a inversão, o primeiro nó da fila será o último
        f->prim = f->ult; // O primeiro nó agora é o último
        f->ult = n2; // O último nó agora é o primeiro
    } else if (!verifica_ordem(f, 0)) { // Caso a fila não esteja nem em ordem crescente nem em ordem decrescente, ordenar
        // Fila está desordenada, ordenar
        int trocado; // Variável que controla se houve troca
        do {
            trocado = 0; // Inicializa a troca como falsa
            FilaNo* n = f->prim; // Variável auxiliar para percorrer a fila

            while (n->prox != NULL) { // Percorre a fila até o penúltimo nó
                if (n->idade < n->prox->idade) { // Se o nó atual for menor que o próximo, troca os valores
                    int temp = n->idade; // Armazena temporariamente o valor da idade do nó atual
                    n->idade = n->prox->idade; // Troca o valor da idade
                    n->prox->idade = temp; // Coloca o valor armazenado no próximo nó
                    trocado = 1; // Marca que houve troca
                }
                n = n->prox; // Avança para o próximo nó
            }
        } while (trocado); // Continua a ordenar até que não haja mais trocas
    }
}


int remover_elementos_fila(Fila* f) { 
    if (f->prim == NULL) { // Verifica se a fila está vazia
        exit(1); // Se estiver vazia, a função termina imediatamente com erro (exit 1)
    }

    FilaNo* n = f->prim; // Cria um ponteiro auxiliar para o primeiro nó da fila
    int i = n->idade; // Armazena a idade (valor) do primeiro nó, que será retornado após a remoção
    f->prim = n->prox; // Atualiza o ponteiro 'prim' para o próximo nó da fila, removendo o primeiro nó

    if (f->prim != NULL) { // Se a fila ainda tiver elementos (não estiver vazia após a remoção)
        f->prim->ant = NULL; // O novo primeiro nó não deve ter nó anterior
    } else { // Caso contrário, se a fila estiver vazia após a remoção
        f->ult = NULL; // O ponteiro é atualizado para NULL
    }

    free(n); // Libera a memória do nó removido
    return i; // Retorna o valor (idade)
}

void liberar_fila(Fila* f) { 
    FilaNo* n = f->prim; // Cria um ponteiro para o primeiro nó da fila
    FilaNo* p = NULL; // Ponteiro auxiliar para liberar a memória dos nós

    while (n != NULL) { // Enquanto ainda houver nós na fila
        p = n; // Armazena o nó atual em p
        n = n->prox; // Avança para o próximo nó na fila
        free(p); // Libera a memória do nó armazenado em p
    }

    free(f); // Libera a memória alocada para a estrutura da fila 
}
