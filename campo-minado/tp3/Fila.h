#ifndef FILA_H
#define FILA_H

typedef struct filano {
    int idade;        
    struct filano* prox;  
    struct filano* ant;   
} FilaNo;

// Definição da estrutura da fila
typedef struct fila {
    FilaNo* prim;     
    FilaNo* ult;     
} Fila;


Fila* criar_fila(); // Cria e incializa uma fila, não recebe parametros e retorna um ponteiro para uma estrutura Fila

void inserir_na_fila(Fila* f, int v); // Insere um novo nó na fila com a idade, recebe um ponteiro para a fila f e um valor inteiro v que é a idade e não retorna nada

void organizar_fila(Fila* f); // Organiza a fila, recebe um ponteiro para a fila f e não retorna nada

int remover_elementos_fila(Fila* f); // Remove o primeiro elemento da fila e retorna a idade, recebe um ponteiro para a fila f

void liberar_fila(Fila* f); // Libera toda memoria alocada para a fila, recebe um ponteiro para fila f e não retorna nada

#endif
