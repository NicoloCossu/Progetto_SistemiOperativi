
#include "pipes.h"

/**
 * funzione per la creazione della pipe riceve in ingresso:
 * @param filedes file descriptor
 * @return
 */
int createPipe(int filedes[2]) {
    if (pipe(filedes) == -1) {
        perror("Errore nella creazione della pipe.");
        exit(1);
    }
}

/**
 * procedura per la scrittura sulla pipe attraverso la write, riceve in ingresso:
 * @param pipein
 * @param dato dato da scrivere, di tipo position
 */
void scrittura(int pipein,  position dato){
    write(pipein, &dato, sizeof(dato));
}

/**
 * funzione per la lettura da pipe attraverso la read riceve in ingresso:
 * @param pipeout
 * restutuisce il dato letto
 * @return
 */
position lettura(int pipeout){
    position dato;
    read(pipeout, &dato, sizeof(dato));
    return dato;
}
