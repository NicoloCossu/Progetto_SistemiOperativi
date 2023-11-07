/******************************************************************************************************************
 *Space Defender                                                                                                  *
 * versione thread, viene utilizato un buffer per la comunicazione tra i vari thread con dei semafori e dei mutex *
 * per le operazioni di lettura e scrittura.                                                                      *
 *                                                                                                                *
 **********************++++****************************************************************************************/
#include "game.h"
#include "comunicazione.h"

int main(){

    initscr();			/* Inizializza schermo di output */
    noecho();			/* Imposta modalità della tastiera */
    curs_set(0);		/* Nasconde il cursore */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_BLACK);//colore per cancellare
    bkgd(COLOR_PAIR(1));
    attron(COLOR_PAIR(1));

    int i, j = 1,x,y;

    //inizializzo il mutex
    sem_init(&semaforo_read, 0, 0);
    sem_init(&semaforo_write, 0, 0);

    sem_post(&semaforo_write);
    startgame();

    endwin();
    return 0;
}





















