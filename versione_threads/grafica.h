

#ifndef THREADS_GRAFICA_H
#define THREADS_GRAFICA_H

//inclusione libreria
#include "game.h"

//prototipi funzioni e procedure
void stampa(position dato, char sprite[ALTEZZA][DIMENSIONE_OGGETTO]);
void pulisci_sparo(position pos_missile);
position azzera();
int collisioni_nave(position nave, position sparo);
void stampapulizia(position dato);
#endif //THREADS_GRAFICA_H