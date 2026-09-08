
#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_MSG 1024
#define TAMANHO_TABULEIRO 8
#define TOTAL_NAVIOS 4

#define AGUA          '~'
#define BARCO         'S'
#define ACERTO        'H'
#define ERRO          'M'
#define DESCONHECIDO  '.'


// Comandos
#define CMD_PLAY  "PLAY"
#define CMD_JOIN "JOIN"
#define CMD_READY "READY"
#define CMD_POS "POS"
#define CMD_FIRE "FIRE"
#define CMD_HIT "HIT"
#define CMD_MISS "MISS"
#define CMD_SUNK "SUNK"
#define CMD_WIN "WIN"
#define CMD_LOSE "LOSE"
#define CMD_END   "END"


#endif
