
#ifndef Q_GRAFICA_H
#define Q_GRAFICA_H

#include "game.h"

//prototipi funzioni e procedure
void stampa(position dato, char sprite[ALTEZZA][DIMENSIONE_OGGETTO]);
void pulizia_missile( position pos_missile);
position azzera();
int collisioni_nave(int pipe[2], position nave, position sparo);

#endif //Q_GRAFICA_H
