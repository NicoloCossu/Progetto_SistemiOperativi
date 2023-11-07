

#ifndef THREADS_GAME_H
#define THREADS_GAME_H
//inclusione librerie
#include "comunicazione.h"
#include "grafica.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//definizione macro
#define MAXX 130
#define MAXY 35
#define DIMENSIONE_NAV 6
#define DIMENSIONE_NEM 6
#define	UP	65		/* Cursore sopra */
#define DW	66		/* Cursore sotto */
#define SPAZIO 32  /*codice spazio*/
#define DIMENSIONE_OGGETTO 6
#define ALTEZZA 6
#define N_NEMICI 12
#define ID_NAVE 0
#define ID_SPARO 1
#define ID_NEMICO 2
#define ID_NEMICO2 3
#define ID_SPARONEMICO 4


//prototipi funzioni e procedure
void *Navicella(void *arg);
void *bomba(void *arg);
void *nemico(void *arg);
void *bomba2(void *parametro);
void collisioni(position nemico[], position sparo, int scontro[]);
int controllo_nemici(position nemico[]);
void area(void);
void stampa_nemico(position dato);
void scrittura(position dato);
position lettura();
void pulizia_colonne();
void startgame();
#endif //THREADS_GAME_H