#include <stdio.h>
#include <stdlib.h>
#include "Fila.h"

int main() {
    Fila* fila = criar_fila(); // Cria uma fila vazia
    int idade;
    
    FILE* arq_leitura = fopen("clientes.txt", "r"); // Abre o arquivo clientes.txt para leitura
    if (arq_leitura == NULL) { // Se houver erro no arquivo, encerrar o programa
        exit(1);
    }

    while (fscanf(arq_leitura, "%d", &idade) != EOF) { //Ler enquanto houve idade no arquivo de leitura
        inserir_na_fila(fila, idade); // Insere cada idade lida na fila
    }
    fclose(arq_leitura); // Fecha o arquivo de leitura

    
    organizar_fila(fila); // Organiza a fila

    printf("A fila do atendimento será na ordem das seguintes idades:\n");
    while (fila->prim != NULL) { // Remove os elementos um a um
        printf("%d ", remover_elementos_fila(fila)); // Remove e imprime os elementos
    }
    putchar('\n'); //quebra de linha

    liberar_fila(fila); //Libera a memoria alocada para a fila

    return 0;
}
