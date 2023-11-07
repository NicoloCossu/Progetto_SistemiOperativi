/*/
 * progetto SPACE DEFENDER
 */

#include "game.h"
#include "pipes.h"
int main(){
    initscr();            /* Inizializza schermo di output */
    noecho();            /* Imposta modalità della tastiera */
    curs_set(0);/* Nasconde il cursore */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);//colore principale
    init_pair(2, COLOR_BLACK, COLOR_BLACK);//colore per cancellare
    bkgd(COLOR_PAIR(1));
    attron(COLOR_PAIR(1));
    pid_t pidNemico[N_NEMICI], pidN;
    int i, x = MAXX - 10, y = 1, j = 1;
    ////creazione pipe principale
    int p[2];
    createPipe(p);
    /*
    * creazione processo navicella amica
    */
    pidN = fork();
    switch (pidN) {
        case -1:
            perror("Errore nell'esecuzione della fork");
            _exit(1);
        case 0:
            navicella(p);
            _exit(0);
    }

    /*/
    * creazione dei processi nemici
    */
    for(i=0; i< N_NEMICI; i++){
        pidNemico[i] = fork();
        switch (pidNemico[i]) {
            case -1:
                perror("Errore nell'esecuzione della fork");
                _exit(1);
            case 0:
                close(p[LETTURA]);
                nemico(p[SCRITTURA],x,y);
        }
        if (j % 4 == 0) {
            x = x - DIMENSIONE_OGGETTO - DIMENSIONE_OGGETTO;
            y = 1;
        } else {
            y = y + DIMENSIONE_OGGETTO + 4;
        }
        j++;
    }

    /*/
     * chiamata alla funzione area di gioco
     */
    area(p);
    endwin();
    return 0;
}

/*
 * funzione per ripulire la schermata dai missili a destra e sinistra arrivati a fine corsa senza collisioni
 */
void pulizia_colonne(){
    int i;
    for (i=0; i < MAXY; i++){
        attron(COLOR_PAIR(2));
        mvprintw(i, 0, " ");
        mvprintw(i, MAXX-5, "xxxxxxxx      \0");
        attron(COLOR_PAIR(1));
    }
};