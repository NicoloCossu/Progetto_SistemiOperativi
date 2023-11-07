/*/
 * per il gioco
 */

#include "game.h"
//definizione sprite nave livello 2
char pulizia[ALTEZZA][DIMENSIONE_OGGETTO] = {
        "**  **",
        "**  **",
        "    \0",
        "    \0",
        "**  **",
        "**  **",
};

//definizione sprite nave amica
char navicella1[ALTEZZA][DIMENSIONE_OGGETTO] = {"|>   \0",
                                                "|->  \0",
                                                "|--->\0",
                                                "|--->\0",
                                                "|->  \0",
                                                "|>   \0"};

//definizione sprite sparo nave amica
char sparo[ALTEZZA][DIMENSIONE_OGGETTO] = {"0    \0",
                                           "     \0",
                                           "     \0",
                                           "     \0",
                                           "     \0",
                                           "     \0"};

//definizione sprite navicella nemica di livello 1
char nav_nemico[ALTEZZA][DIMENSIONE_OGGETTO] =
        {" ^^^ \0",
         "+++++\0",
         "+++++\0",
         "+++++\0",
         "< - >\0",
         "*****\0"}
;


//definizione srpite sparo delle navicelle nemiche
char sparo2 [ALTEZZA][DIMENSIONE_OGGETTO] =
        {"=    \0",
         "     \0",
         "     \0",
         "     \0",
         "     \0",
         "     \0"}
;


/**
 * la procedura navicella si occupa di gestire i movimenti della navicella amica attraverso i tasti freccia , con uno
 * switch, nel case "SPAZIO" crea due processi che generano le coordinate dello sparo della navicella attravesrso un'
 * apposita funzione;
 * riceve in ingresso la pipe nella quale scrive la posizione della navicella
 * @param pipe
 */
void navicella(int pipe[]){
    int pid_s, pid_s2;
    int i, x = 0, y = 1;
    position cpos;
    cpos.x = 2;   /* Coordinata X iniziale */
    cpos.y = MAXY / 2;   /* Coordinata Y iniziale */
    cpos.pid = getpid();
    cpos.id = ID_NAVE;
    close(pipe[LETTURA]);
    scrittura(pipe[SCRITTURA], cpos);
    /* Lettura dei tasti cursore */
    while (1) {
        char c;
        c = getch();
        //freccia in alto si sposta su
        if (c == UP && cpos.y > 1) {
            cpos.y -= 1;
        }
        //freccia in basso si sposta giu
        if (c == DW && cpos.y < MAXY - 1) {
            cpos.y += 1;
        }
        //spazio spara due missili in direzioni opposte creando un figlio per ogni missile
        if(c == SPAZIO) {
            pid_s = fork();
            switch (pid_s) {
                case -1:
                    perror("Errore nell'esecuzione della fork");
                    _exit(1);
                case 0:
                    //processo figlio missile verso l'alto
                    close(pipe[LETTURA]);
                    bomba(pipe[SCRITTURA], cpos, -1);
                    exit(0);
            }
            pid_s2 = fork();
            switch (pid_s2) {
                case -1:
                    perror("Errore nell'esecuzione della fork");
                    _exit(1);
                case 0:
                    //figlio del figlio missile verso il basso
                    close(pipe[LETTURA]);
                    bomba(pipe[SCRITTURA], cpos, 1);
                    exit(0);
            }

        }
        close(pipe[LETTURA]);
        scrittura(pipe[SCRITTURA], cpos);
    }
};



/**
 * la procedura bomba genera le coordinate delle due bombe sparate dalla navicella amica riceve in ingresso:
 * @param pipeout
 * @param pos_nav posizione navicella amica dalla quale partono i due missili
 * @param y direzione (su o giu a seconda del missile) in cui il missile dovrà andare
 */
void bomba(int pipeout ,  position pos_nav, int y){
    int i = 0;
    int newx = 0, newy = 0;
    int direzionex = 3, direzioney = y;

    position pos,posNem;
    pos.x = pos_nav.x + DIMENSIONE_OGGETTO;
    pos.y = pos_nav.y;
    pos.pid = getpid();
    pos.id = ID_SPARO;
    while (i < (MAXX/3)-1) {
        newx = pos.x + direzionex;
        newy = pos.y + direzioney;

        if (newx >= MAXX || newx < 0) {
            direzionex *= -1;
        } else {
            pos.x += direzionex;
        }

        if (newy >= MAXY || newy < 1) {
            direzioney *= -1;
        } else {
            pos.y += direzioney;
        }
        usleep(90000);
        write(pipeout, &pos, sizeof(pos));
        i++;
    }
    write(pipeout, &pos, sizeof(pos));
}

/**
 * la procedura nemico gestisce i nemici e genera i loro spari attraverso dei processi.riceve in ingtresso:
 * @param pipein
 * @param x
 * @param y
 */
void nemico(int pipein, int x, int y){
    int i = x+y, j=1, l=0;

    position nemico, dato;
    nemico.x = x;
    nemico.y = y;
    nemico.pid = getpid();
    pid_t sparo_nemico;
    nemico.id = ID_NEMICO;
    scrittura(pipein, nemico);
    l = x * y;
    while (1){
        sleep(3);
        if(l % 7 == 0){
            sparo_nemico = fork();
            switch (sparo_nemico) {
                case -1:
                    perror("Errore nell'esecuzione della fork");
                    _exit(1);
                case 0:
                    bomba2(pipein,nemico);
                    _exit(0);
            }
        }
        /*movimenti dei nemici*/
        nemico.x--;
        if((j%2) == 0)
            nemico.y++;
        else
            nemico.y--;
        j++;
        scrittura(pipein, nemico);
        l++;
    }

    return;
};


/**
 * la procedura bmba 2 si occupa della gestione dgli spari dei nemici , riceve in ingresso
 * @param pipeout
 * @param pos_nav posizione nemico da cui parte lo sparo
 */
void bomba2(int pipeout,  position pos_nav){
    int i = 0;
    int direzionex = DIREZIONE_SPARONEM;
    position posizione;
    posizione.x = pos_nav.x + DIMENSIONE_OGGETTO;
    posizione.y = pos_nav.y;
    posizione.pid = getpid();
    posizione.id = ID_SPARONEMICO;
    while (i < MAXX-1) {
        //aumento di una posizione verso sinistra e scrivo sulla pipe
        posizione.x -= direzionex;
        usleep(120000);
        write(pipeout, &posizione, sizeof(posizione));
        i++;
    }
}

/**
 * gestisce le collisioni tra lo sparo della navicella amica e le navicelle nemiche. riceev in ingresso:
 * @param pipe
 * @param nemico
 * @param sparo
 */
void collisioni(int pipe[2], position nemico[], position sparo, int scontro[]){
    int i,j,k, kill_return;
    for (i=0; i< N_NEMICI; i++){
        //scorro la sprite in altezza
        for (k=0; k< ALTEZZA; k++){
            //scorro la sprite in orizzontale
            for (j=0; j< DIMENSIONE_OGGETTO; j++){
                //verifico la collisione
                if(sparo.x == ((nemico[i].x)+j) && sparo.y == ((nemico[i].y)+k)){
                    kill_return = kill(sparo.pid, SIGKILL);
                    if(kill_return == 0){
                        attron(COLOR_PAIR(2));
                        stampa(nemico[i], nav_nemico);
                        attron(COLOR_PAIR(1));
                        nemico[i].id = ID_NEMICO2;
                        scontro[i]++;
                        break;
                    }
                }
            }
        }
    }
};

/**
 * controlla se i nemici spostandosi verso sinistra raggiungono la navicella amica
 * @param nemico
 * @return
 */
int controllo_nemici(position nemico[]){
    int i;
    for(i=0; i< N_NEMICI; i++){
        if(nemico[i].x == DIMENSIONE_OGGETTO){
            return 1;
        }
    }
    return 0;
}


/**
 * la funzione area gestisce l'are a di gioco e le varie collisioni. riceve in ingresso
 * @param pipe pipe
 */
void area(int pipe[]){
    int i=0, vite = VITE, j = 0, killnemico;
    int enemy=0, controllo = 0;
    position dato_letto, nave, nemico[N_NEMICI];
    int scontro[N_NEMICI];
    for (i=0; i< N_NEMICI; i++){
        nemico[i] = azzera();
        scontro[i] = 0;
    }

    while(1){
        //controlla se sono stati colpiti tutti i nemici stampa VITTORIA e termina il gioco
        if(enemy >= N_NEMICI){
            clear();
            refresh();
            mvprintw(MAXY/2, MAXX/2, "VITTORIA");
            refresh();
            sleep(3);
            break;
        }

        //se le vite sono finite termina il gioco
        if(vite <= 0){
            clear();
            refresh();
            mvprintw(MAXY/2, MAXX/2, "GAME OVER");
            refresh();
            sleep(3);
            break;
        }

        //se i nemici raggiungono l'astrovave amica il gioco finisce
        controllo = controllo_nemici(nemico);
        if(controllo){
            clear();
            refresh();
            mvprintw(MAXY/2, MAXX/2, "GAME OVER");
            refresh();
            sleep(3);
            break;
        }

        /*leggiamo dalla pipe attravesro una funzione*/
        close(pipe[SCRITTURA]);
        dato_letto = lettura(pipe[LETTURA]);
        /*attraverso l'ID dell' oggetto questo viene riconosciuto e gestito*/
        switch (dato_letto.id) {

            //nave amica
            case ID_NAVE:
                attron(COLOR_PAIR(2));
                stampa(nave, navicella1);
                attron(COLOR_PAIR(1));
                nave = dato_letto;
                stampa(dato_letto, navicella1);
                break;

            //nemico
            case ID_NEMICO:
                //lettura nemici e memorizzazione di questi in un vettore nemico di tipo position
                if(j<N_NEMICI){
                    nemico[j] = dato_letto;
                }
                j++;

                for (i = 0; i < N_NEMICI; i++){
                    if(nemico[i].pid == dato_letto.pid){
                        if(scontro[i] < 3){
                            if(nemico[i].id == ID_NEMICO2){
                                attron(COLOR_PAIR(2));
                                stampa_nemico(nemico[i]);
                                attron(COLOR_PAIR(1));
                                nemico[i].x = dato_letto.x;
                                nemico[i].y = dato_letto.y;
                                nemico[i].pid = dato_letto.pid;
                                nemico[i].id = ID_NEMICO2;
                            }else{
                                attron(COLOR_PAIR(2));
                                stampa(nemico[i], nav_nemico);
                                attron(COLOR_PAIR(1));
                                nemico[i].x = dato_letto.x;
                                nemico[i].y = dato_letto.y;
                                nemico[i].pid = dato_letto.pid;
                                nemico[i].id = ID_NEMICO;
                            }
                            /*se il nemico viene colpito viene chiamata una funzione per eliminare il processo*/
                        }else{
                            killnemico = kill(nemico[i].pid, SIGKILL);
                            if(killnemico == 0){
                                attron(COLOR_PAIR(2));
                                stampa(nemico[i], pulizia);
                                attron(COLOR_PAIR(1));
                                nemico[i] = azzera();
                                enemy++;
                            }
                        }
                    }
                }
                break;
                //gestione sparo navicella amica
            case ID_SPARO:
                pulizia_missile(dato_letto);
                if(dato_letto.x == MAXX){
                    attron(COLOR_PAIR(2));
                    stampa(dato_letto, sparo);
                    attron(COLOR_PAIR(1));
                    kill(dato_letto.pid, SIGKILL);
                }else{
                    stampa(dato_letto, sparo);
                    collisioni(pipe, nemico, dato_letto, scontro);//in caso di collisione con una navicella nemica viene chiamata
                                                        // l'apposita funzione"collisioni"
                }
                break;
                //sparo nemico
            case ID_SPARONEMICO:
                pulizia_missile(dato_letto);
                stampa(dato_letto, sparo2);
                vite = vite - collisioni_nave(pipe, nave, dato_letto);
                break;
        }
        stampa(nave, navicella1);
        //stapa dei nemici aggiornati
        for (i = 0; i < N_NEMICI; i++){
            if(nemico[i].id == ID_NEMICO2){
                stampa_nemico(nemico[i]);
            }else{
                stampa(nemico[i], nav_nemico);
            }
        }
        pulizia_colonne();
        refresh();
    }
    clear();
};
