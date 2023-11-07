//
// Created by Noemi on 07/02/22.
//

#include "grafica.h"
//sprite nemico di secondo livello
char nav_nemico2[ALTEZZA][DIMENSIONE_OGGETTO] = {
        "**  **",
        "**  **",
        "    \0",
        "    \0",
        "**  **",
        "**  **",
};

/**
 * procedura che stampa un oggetto riceve in inresso:
 * @param dato dati oggetto da stampare
 * @param sprite la rispettiva sprite
 */
void stampa(position dato, char sprite[][DIMENSIONE_OGGETTO]){
    int i;
    for (i = 0; i < ALTEZZA; i++) {
        mvprintw((dato.y) + i, (dato.x), "%s", sprite[i]);
    }
}

/**
 * stampa un nemico di livello 2
 * @param dato
 */
void stampa_nemico(position dato){
    int i, j;
    for (i = 0; i < ALTEZZA; i++) {
        for (j = 0; j < DIMENSIONE_OGGETTO; j++) {
            mvprintw((dato.y) + i, (dato.x) + j, "%c", nav_nemico2[i][j]);
        }
    }

}


void stampapulizia(position dato){
    int i, j;
    for (i = 0; i < ALTEZZA + 1; i++) {
        for (j = 0; j < DIMENSIONE_OGGETTO + 1; j++) {
            mvaddch((dato.y) + i, (dato.x) + j, ' ');
        }
    }
};

/**
 * fa in modo che lo sparo non lasci la "scia" e lascia vedere un solo oggetto
 * @param pos_missile
 */
void pulisci_sparo( position pos_missile){
    mvprintw((pos_missile.y)-1, (pos_missile.x)+3, "%s"," \0");
    mvprintw((pos_missile.y)-1, (pos_missile.x)-3, "%s"," \0");
    mvprintw((pos_missile.y)+1, (pos_missile.x)+3, "%s"," \0");
    mvprintw((pos_missile.y)+1, (pos_missile.x)-3, "%s"," \0");
    mvprintw((pos_missile.y)+1, (pos_missile.x), "%s"," \0");
    mvprintw((pos_missile.y), (pos_missile.x)+3, "%s"," \0");
    mvprintw((pos_missile.y)-1, (pos_missile.x), "%s"," \0");
    mvprintw((pos_missile.y), (pos_missile.x)-3, "%s"," \0");
    refresh();
}

/**
 * verifica le collisioni tra la nave amica e gli spari dei nemici
 * @param nave
 * @param sparo
 * @return
 */
int collisioni_nave(position nave, position sparo){
    int j, k, kill_return;
    //scorro la sprite in altezza
    for (k=0; k< ALTEZZA; k++){
        //scorro la sprite in orizzontale
        for (j=0; j< DIMENSIONE_OGGETTO; j++){
            if(sparo.x == ((nave.x)+j) && sparo.y == ((nave.y)+k)){
                kill_return = pthread_cancel(sparo.thread_id);
                if(kill_return == 0){
                    return 1;
                }
            }
        }
    }
    return 0;
}


position azzera(){
    position dato;
    dato.y = -1;
    dato.x = -1;
    dato.id = -1;
    dato.thread_id = -1;
    return dato;
}
