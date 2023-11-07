//
// Created by Noemi on 07/02/22.
//

#ifndef THREADS_COMUNICAZIONE_H
#define THREADS_COMUNICAZIONE_H


//inclusione librerie
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

//definizione macro
#define DIMENSIONE_OGGETTO 6
#define ALTEZZA 6
#define DIMENSIONE_NAV 6
#define DIM_BUFFER 100

/* Struttura adoperata per veicolare le coordinate */
typedef struct {
    char c[DIMENSIONE_NAV]; /* Identificatore dell'entità che invia i dati */
    int  x;
    int  y;
    int id;
    pthread_t thread_id;
    int collisioni;
}position;

//struttura per passare i parametri coordinate x,y e la direzione dell' oggetto
typedef struct {
    int x;
    int y;
    int direzione;
}parametri;

//creazione buffer per la comunicazione tra i thread, mutex e semafori
position buffer[DIM_BUFFER];
int counter;
int counter_lettura;

sem_t semaforo_write;
sem_t semaforo_read;

#endif //THREADS_COMUNICAZIONE_H