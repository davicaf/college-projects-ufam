# 💣 Campo Minado em C

Implementação em C do clássico jogo Campo Minado (Minesweeper) para o terminal, desenvolvida ao longo da disciplina de **Algoritmos e Estruturas de Dados I** (UFAM, 2024.2). O projeto evoluiu em três entregas (TP1 → TP3), cada uma agregando novos conceitos de estruturas de dados e boas práticas de C.

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Status](https://img.shields.io/badge/status-conclu%C3%ADdo-brightgreen?style=for-the-badge)

## 🕹️ Como o jogo funciona

- O jogador escolhe a dificuldade: **F**ácil (10×10, 3 minas), **M**édio (20×20, 6 minas) ou **D**ifícil (30×30, 9 minas).
- O tabuleiro é sorteado com minas em posições aleatórias e cada célula sem mina exibe a quantidade de minas adjacentes.
- A cada rodada o jogador informa uma coordenada `linha coluna` para revelar; acertar uma mina encerra o jogo, e revelar todas as células seguras vence a partida.

## 📁 Estrutura das entregas

| Pasta | O que foi adicionado |
|---|---|
| [`tp1/`](tp1) | Versão inicial: lógica principal do jogo em um único arquivo — geração do tabuleiro, alocação dinâmica de matrizes, colocação de minas, contagem de minas adjacentes e loop de jogo. |
| [`tp2/`](tp2) | Modularização do código em arquivos separados (`matriz.c/h`) e adição de um sistema de **log** (`log.c/h`) que registra o histórico de jogadas em arquivo, além de validações extras de entrada. |
| [`tp3/`](tp3) | Estrutura de dados auxiliar: uma **fila duplamente encadeada** (`Fila.c/h`) usada para organizar uma fila de atendimento por idade lida de um arquivo (`clientes.txt`), reaproveitando os conceitos de alocação dinâmica do jogo principal. |

## 🧠 Conceitos aplicados

- Alocação dinâmica de memória (matrizes de ponteiros, listas encadeadas).
- Modularização de código em C (headers `.h` + implementação `.c`).
- Manipulação de arquivos (leitura de dados e escrita de logs).
- Estruturas de dados lineares (fila duplamente encadeada) e algoritmos de ordenação.

## ▶️ Como compilar e executar

Cada entrega pode ser compilada isoladamente com `gcc`. Exemplos:

```bash
# TP1 — versão única
cd tp1
gcc -o campo_minado campo_minado.c
./campo_minado

# TP2 — versão modularizada com log
cd tp2
gcc -o campo_minado campo_minado.c matriz.c log.c
./campo_minado

# TP3 — fila de atendimento
cd tp3
gcc -o fila_atendimento fila_atendimento.c Fila.c
./fila_atendimento
```

## 👤 Autor

Davi Carlos — Engenharia de Software, UFAM.
