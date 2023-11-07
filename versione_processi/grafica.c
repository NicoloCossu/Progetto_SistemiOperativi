
#include "grafica.h"
//sprite navicella livello 2
char nav_nemico2[ALTEZZA][DIMENSIONE_OGGETTO] = {
        "**  **",
        "**  **",
        "    \0",
        "    \0",
        "**  **",
        "**  **",
};

/**
 * procedura per la stampa di un dato oggetto ricevuto in ingresso
 * @param dato da stampare
 * @param sprite
 */
void stampa(position dato, char sprite[][DIMENSIONE_OGGETTO]){
    int i;
    for (i = 0; i < ALTEZZA; i++) {
            mvprintw((dato.y) + i, (dato.x), "%s", sprite[i]);
    }
}

/**
 * stampa di un nemico di livello 2
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


/**
 * funzione che elimina "la scia" del missile in modo che si veda un unico oggetto
 * @param pos_missile
 */
void pulizia_missile(position pos_missile){
    mvprintw((pos_missile.y)-1, (pos_missile.x)+3, "%s"," \0");
    mvprintw((pos_missile.y)-1, (pos_missile.x)-3, "%s"," \0");
    mvprintw((pos_missile.y)+1, (pos_missile.x)+3, "%s"," \0");
    mvprintw((pos_missile.y)+1, (pos_missile.x)-3, "%s"," \0");
    mvprintw((pos_missile.y)+1, (pos_missile.x), "%s"," \0");
    mvprintw((pos_missile.y), (pos_missile.x)+3, "%s"," \0");
    mvprintw((pos_missile.y)-1, (pos_missile.x), "%s"," \0");
    mvprintw((pos_missile.y), (pos_missile.x)-3, "%s"," \0");
}


/**
 *  controlla se ci sono state collisioni e in quel caso elimina il processo
 * @param pipe
 * @param nave
 * @param sparo
 * @return
 */
int collisioni_nave(int pipe[2], position nave, position sparo){
    int j, k, kill_return;
        //scorro la sprite in altezza
        for (k=0; k< ALTEZZA; k++){
            //scorro la sprite in orizzontale
            for (j=0; j< DIMENSIONE_OGGETTO; j++){
                if(sparo.x == ((nave.x)+j) && sparo.y == ((nave.y)+k)){
                        kill_return = kill(sparo.pid, SIGKILL);
                        if(kill_return == 0){
                            return 1;
                        }
                    }
                }
            }
    return 0;
}

/*
 * creo un dato impostandolo a NULL -1
 */
position azzera(){
    position dato;
    dato.y = -1;
    dato.x = -1;
    dato.id = -1;
    dato.pid = -1;
    return dato;
}

