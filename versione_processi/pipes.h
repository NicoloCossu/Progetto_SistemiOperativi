//
// Created by Nicolo on 13/01/2022.
//

#ifndef Q_PIPES_H
#define Q_PIPES_H
//definizione macro per la dimensione dei vari oggetti
#define DIMENSIONE_OGGETTO 6
#define ALTEZZA 6

//inclusione librerie
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * definizione struttura position contenente : coordinate oggetti, pid processo , identificatore dell'oggetto,
 * n° collisioni
 */
typedef struct  {
    int  x;	 /* Coordinata X */
    int  y;/* Coordinata Y */
    int pid;
    int id;
    int collisioni;
}position;

//prototipi di procedure e funzioni 
int createPipe(int filedes[2]);
position lettura(int pipeout);
void scrittura(int pipein,  position dato);
#endif //Q_PIPES_H
