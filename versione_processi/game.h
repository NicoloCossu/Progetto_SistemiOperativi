
#ifndef Q_GAME_H
#define Q_GAME_H
//inclusione librerie
#include "pipes.h"
#include "grafica.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//definizione macro del gioco
#define ID_NEMICO 2
#define ID_NAVE 3
#define ID_SPARO 4
#define ID_SPARONEMICO 5
#define ID_NEMICO2 6
#define MAXX 130
#define MAXY 35
#define DIMENSIONE_OGGETTO 6
#define ALTEZZA 6
#define N_NEMICI 12
#define SPAZIO 32
#define DIREZIONE_SPARONEM 3
#define	UP	65
#define DW	66
#define VITE 1

#define LETTURA 0
#define SCRITTURA 1

//prototipi funzioni e procedure
void navicella(int pipe[]);
void bomba(int pipeout ,  position pos_nav, int y);
void nemico(int pipein, int x, int y);
void bomba2(int pipeout,  position pos_nav);
void collisioni(int pipe[2], position nemico[], position sparo, int scontro[]);
int controllo_nemici(position nemico[]);
void area(int pipe[]);
void stampa_nemico(position dato);
void pulizia_colonne();
#endif //Q_GAME_H
