# 🚢 Batalha Naval Multicliente com Sockets TCP

Implementação em C de uma versão simplificada do jogo Batalha Naval, com comunicação **cliente-servidor via sockets TCP** e um **servidor concorrente multithread** capaz de arbitrar uma partida entre dois jogadores conectados simultaneamente. Projeto final da disciplina de **Laboratório de Programação Avançada** (UFAM, 2025.1).

![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Sockets](https://img.shields.io/badge/TCP%2FIP-Sockets-blue?style=for-the-badge)
![pthreads](https://img.shields.io/badge/pthreads-concorr%C3%AAncia-orange?style=for-the-badge)

## 🎯 Sobre o projeto

Dois clientes se conectam a um servidor central, que gerencia o pareamento das partidas, valida jogadas e sincroniza os turnos entre os jogadores usando **threads** e um **mutex** para proteger o estado compartilhado da partida.

### Regras do jogo

- Tabuleiro 8×8 (colunas `A`–`H`, linhas `1`–`8`).
- Frota de cada jogador: 1 Submarino (tamanho 1), 2 Fragatas (tamanho 2) e 1 Destroyer (tamanho 3).
- Turnos alternados; cada jogada é uma coordenada (ex: `B4`).
- Resultado de cada tiro: água, acerto ou navio afundado.
- Vence quem afundar toda a frota adversária primeiro.

## 🔄 Protocolo de comunicação

Um protocolo textual simples, definido em [`common/protocol.h`](common/protocol.h), coordena cliente e servidor:

| Comando | Origem → Destino | Descrição |
|---|---|---|
| `JOIN <nome>` | Cliente → Servidor | Solicita entrada na partida |
| `POS <tipo> <x> <y> <orientação>` | Cliente → Servidor | Informa a posição de um navio durante o posicionamento |
| `READY` | Cliente → Servidor | Sinaliza que o jogador terminou de posicionar seus navios |
| `PLAY` | Servidor → Cliente | Indica que é a vez daquele jogador atirar |
| `FIRE <x> <y>` | Cliente → Servidor | Realiza um ataque à coordenada informada |
| `HIT` / `MISS` / `SUNK` | Servidor → Ambos | Resultado do ataque (acerto, água ou navio afundado) |
| `WIN` / `LOSE` | Servidor → Cliente | Resultado final da partida para cada jogador |
| `END` | Servidor → Ambos | Encerra a partida e a conexão |

**Fluxo típico de uma partida:**

```
Cliente A                      Servidor                      Cliente B
   │── JOIN Alice ────────────►│                              │
   │                           │◄──────────────── JOIN Bob ───│
   │◄── (posicionamento com POS × várias vezes) ─────────────►│
   │── READY ─────────────────►│◄───────────────────── READY ─│
   │◄── PLAY ──────────────────│                              │
   │── FIRE 3 4 ──────────────►│── HIT/MISS/SUNK ────────────►│ (broadcast p/ ambos)
   │            ... alterna PLAY/FIRE até a frota acabar ...   │
   │◄── WIN ───────────────────│──────────────────────── LOSE─►│
```

## 🏗️ Arquitetura

- **Servidor** ([`server/battleserver.c`](server/battleserver.c)): aceita as duas conexões, cria uma *thread* por jogador (`pthread_create`) e protege o estado compartilhado da partida (tabuleiros, turno atual) com um `pthread_mutex_t`. Processa `POS`, `READY` e `FIRE`, decide acertos/afundamentos e alterna os turnos.
- **Cliente** ([`client/battleclient.c`](client/battleclient.c)): interface via terminal para posicionar os navios e atirar, exibindo lado a lado o próprio tabuleiro e o tabuleiro de tiros contra o adversário.
- **Protocolo comum** ([`common/protocol.h`](common/protocol.h)): constantes, comandos e dimensões do tabuleiro compartilhadas entre cliente e servidor.

## ▶️ Como compilar e executar

```bash
make               # compila server/battleserver e client/battleclient

./server/battleserver           # em um terminal

./client/battleclient            # em outro terminal (jogador 1)
./client/battleclient            # em um terceiro terminal (jogador 2)
```

## 👤 Autor

Davi Carlos — Ciência da Computação, UFAM.
