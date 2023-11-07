//
// contiene le funzioni per il gioco
//

#include "game.h"
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

//sprite dello sparo dei nemici
char sparo2 [ALTEZZA][DIMENSIONE_OGGETTO] =
        {"=    \0",
         "     \0",
         "     \0",
         "     \0",
         "     \0",
         "     \0"}
;

//sprite navicella amica
char navicella1[ALTEZZA][DIMENSIONE_OGGETTO] = {"|>   \0",
                                                "|->  \0",
                                                "|--->\0",
                                                "|--->\0",
                                                "|->  \0",
                                                "|>   \0"};

//sprite sparo della navicella amica
char sparo[ALTEZZA][DIMENSIONE_OGGETTO] = {"0    \0",
                                           "     \0",
                                           "     \0",
                                           "     \0",
                                           "     \0",
                                           "     \0"};

//sprite della navicella nemica di livello 1
char nav_nemico[ALTEZZA][DIMENSIONE_OGGETTO] =
        {" ^^^ \0",
         "+++++\0",
         "+++++\0",
         "+++++\0",
         "< - >\0",
         "*****\0"}
;

/**
 * funzione che gestisce la navicella amica i suoi movimenti attraverso i tatsti cursore e i suoi spari
 * @param arg
 * @return
 */
void *Navicella(void *arg) {
    parametri passaggio, passaggio2;
    char c;
    position navicella;
    pthread_t tsparoup, tsparodw;
    navicella.x = 2;   /* Coordinata X iniziale */
    navicella.y = MAXY / 2;   /* Coordinata Y iniziale */
    /* Lettura dei tasti cursore */
    while (1) {
        c = getch();

        //freccia in alto si sposta su
        if (c == UP && navicella.y > 0) {
            navicella.y -= 1;
        }

        //freccia in basso si sposta giu
        if (c == DW && navicella.y < MAXY - DIMENSIONE_OGGETTO) {
            navicella.y += 1;
        }

        navicella.id = ID_NAVE;
        navicella.thread_id = pthread_self();
        //scrittura nel buffer
        scrittura(navicella);

        passaggio.x = navicella.x;
        passaggio.y = navicella.y;
        passaggio.direzione = -1;

        passaggio2.x = navicella.x;
        passaggio2.y = navicella.y;
        passaggio2.direzione = 1;

        //spazio spara due missili in direzioni opposte crea un thread per ogni missile
        if(c == SPAZIO) {
            pthread_create(&tsparodw, NULL, bomba, &passaggio2);
            pthread_create(&tsparoup, NULL, bomba, &passaggio);
        }
    }
}

/**
 *funzione che gestisce lo sparo della navicella amica
 * @param passaggio
 * @return
 */
void *bomba(void *passaggio){
    position sparoUP;
    parametri *p;
    int i = 0;
    int newx = 0, newy = 0;
    int direzionex , direzioney ;
    direzionex = 3;
    p = (parametri*)passaggio;
    sparoUP.x = p->x + DIMENSIONE_OGGETTO;
    sparoUP.y = p->y +2;
    direzioney = p->direzione;
    sparoUP.id = ID_SPARO;
    while (i<(MAXX/3)-1) {
        newx = sparoUP.x + direzionex;
        newy = sparoUP.y + direzioney;
        if (newx >= MAXX || newx < 0) {
            direzionex *= -1;
        } else {
            sparoUP.x += direzionex;
        }

        if (newy >= MAXY || newy < 0) {
            direzioney *= -1;
        } else {
            sparoUP.y += direzioney;
        }
        usleep(70000);
        sparoUP.thread_id = pthread_self();
        scrittura(sparoUP);

        i++;
    }
}

/**
 * funzione che gestisce le navicelle nemiche e crea un thread per ogni sparo
 * @param passaggio
 * @return
 */
void *nemico(void *passaggio){
    int i, j=1, l=0;
    pthread_t tbomba;
    position nemico;
    parametri *p, passaggio2;
    pthread_mutex_lock(&mtx);
    p = (parametri*)passaggio;
    pthread_mutex_unlock(&mtx);
    i = p->x + p->y;
    nemico.x = p->x;
    nemico.y = p->y;
    nemico.id = ID_NEMICO;
    nemico.thread_id = pthread_self();
    scrittura(nemico);
    l = p->x * p->y;
    while (1){
        sleep(4);
        if(l % 7 == 0){
            //viene creato un thread per lo spar della nave nemica
            passaggio2.x = nemico.x;
            passaggio2.y = nemico.y + 2;
            pthread_create(&tbomba, NULL, bomba2, &passaggio2);
        }
        nemico.x--;
        if((j%2) == 0)
            nemico.y++;
        else
            nemico.y--;
        j++;
        scrittura(nemico);
        l++;
    }
}

/**
 * funzione che gestisce la direzione degli spari delle navi nemiche
 * @param parametro
 * @return
 */
void *bomba2(void *parametro){
    int i = 0;
    int direzionex = 1;
    position posizione;
    parametri *p;
    pthread_mutex_lock(&mtx);
    p = (parametri*)parametro;
    pthread_mutex_unlock(&mtx);

    posizione.x = p->x + DIMENSIONE_OGGETTO;
    posizione.y = p->y;
    posizione.thread_id= pthread_self();
    posizione.id = ID_SPARONEMICO;
    while (i < MAXX + DIMENSIONE_OGGETTO + 2) {
        posizione.x -= direzionex;
        usleep(90000);
        scrittura(posizione);
        i++;
    }
}


/**
 * procedura cje riceve in ingresso il nemico e lo sparo della nave amica e controlla che ci sia stata una collisione
 * @param nemico
 * @param sparo
 */
void collisioni(position nemico[], position sparo, int scontro[]){
    int i,j,k, kill_return, kill2;
    for (i=0; i< N_NEMICI; i++){
        //scorro la sprite in altezza
        for (k=0; k< ALTEZZA; k++){
            //scorro la sprite in orizzontale
            for (j=0; j< DIMENSIONE_OGGETTO; j++){
                if(sparo.x == ((nemico[i].x)+j) && sparo.y == ((nemico[i].y)+k)){
                    kill_return = pthread_cancel(sparo.thread_id);
                    while(kill_return == 0){
                        attron(COLOR_PAIR(2));
                        stampapulizia(nemico[i]);
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
 * gestisce l'area di gioco
 */
void area(void ){
    position dato_letto, nave, nemico[N_NEMICI];
    int enemy=0, killnemico, scontro[N_NEMICI];

    int j = 0, i, vite = 1, controllo = 0;
    nave.y = MAXY/2;
    nave.x = 2;
    for (i=0; i< N_NEMICI; i++){
        nemico[i] = azzera();
        scontro[i] = 0;
    }

    do {
        /* Leggo dal buffer */
        dato_letto = lettura();
        if (vite <= 0) {
            clear();
            refresh();
            mvprintw(MAXY / 2, MAXX / 2, "GAME OVER");
            refresh();
            sleep(3);
            break;
        }

        //controlla se sono stati colpiti tutti i nemici stampa VITTORIA e termina il gioco
        if (enemy >= N_NEMICI) {
            clear();
            refresh();
            mvprintw(MAXY / 2, MAXX / 2, "VITTORIA");
            refresh();
            sleep(3);
            break;
        }

        //se i nemici raggiungono l'astrovave amica il gioco finisce
        controllo = controllo_nemici(nemico);
        if(controllo){
            clear();
            refresh();
            mvprintw(MAXY / 2, MAXX / 2, "GAME OVER");
            refresh();
            sleep(3);
            break;
        }
        switch(dato_letto.id){
            case ID_NAVE:
                attron(COLOR_PAIR(2));
                stampa(nave, navicella1);
                attron(COLOR_PAIR(1));
                stampa(dato_letto, navicella1);
                nave = dato_letto;
                break;
            case ID_SPARO:
                pulisci_sparo(dato_letto);
                if(dato_letto.x == MAXX){
                    //se non ci sono collisioni il thread dello sparo viene aliminato
                    attron(COLOR_PAIR(2));
                    stampapulizia(dato_letto);
                    attron(COLOR_PAIR(1));
                    pthread_cancel(dato_letto.thread_id);
                }else{
                    //se ci sono collisioni vengono getite attraverso l'apposita funzione
                    stampa(dato_letto, sparo);
                    collisioni(nemico, dato_letto, scontro);
                }
                break;
            case ID_NEMICO:
                if(j<N_NEMICI){
                    nemico[j] = dato_letto;
                }
                j++;
                for (i=0; i < N_NEMICI; i++){
                    if(nemico[i].thread_id == dato_letto.thread_id){
                        if(scontro[i] < 3){
                            if(nemico[i].id == ID_NEMICO2){
                                attron(COLOR_PAIR(2));
                                stampapulizia(nemico[i]);
                                attron(COLOR_PAIR(1));
                                nemico[i].x = dato_letto.x;
                                nemico[i].y = dato_letto.y;
                                nemico[i].thread_id = dato_letto.thread_id;
                                nemico[i].id = ID_NEMICO2;
                            }else{
                                attron(COLOR_PAIR(2));
                                stampapulizia(nemico[i]);
                                attron(COLOR_PAIR(1));
                                nemico[i].x = dato_letto.x;
                                nemico[i].y = dato_letto.y;
                                nemico[i].thread_id = dato_letto.thread_id;
                                nemico[i].id = ID_NEMICO;
                            }
                        }else{
                            /*se il nemico viene colpito viene chiamata una funzione per eliminare il thread*/
                            enemy++;
                            killnemico = pthread_cancel(nemico[i].thread_id);
                            if(killnemico == 0){
                                attron(COLOR_PAIR(2));
                                stampapulizia(nemico[i]);
                                attron(COLOR_PAIR(1));
                                nemico[i] = azzera();
                            }
                            attron(COLOR_PAIR(2));
                            stampapulizia(nemico[i]);
                            attron(COLOR_PAIR(1));
                        }
                    }
                }
            case ID_SPARONEMICO:
                pulisci_sparo(dato_letto);
                stampa(dato_letto, sparo2);
                vite = vite - collisioni_nave(nave, dato_letto);
                break;
        }
        //aggiorna l'output
        stampa(nave, navicella1);
        for (i = 0; i < N_NEMICI; i++){
            if(nemico[i].id == ID_NEMICO2){
                stampa_nemico(nemico[i]);
            }else{
                stampa(nemico[i], nav_nemico);
            }
        }
        pulizia_colonne();

        refresh();
    } while(1);

}


/**
 * procedura che si occupa della scrittura sul buffer di un dato passato come parametro
 * @param dato
 */
void scrittura(position dato){
    sem_wait(&semaforo_write);
    pthread_mutex_lock(&mtx);
    buffer[counter] = dato;
    counter = (counter+1) % DIM_BUFFER;
    pthread_mutex_unlock(&mtx);
    sem_post(&semaforo_read);
}

/**
 * funzione che si occupa della lettura da buffer e restituisce il dato appena letto
 * @return
 */
position lettura(){
    position dato_letto;
    sem_wait(&semaforo_read);
    pthread_mutex_lock(&mtx);
    dato_letto = buffer[counter_lettura];
    counter_lettura = (counter_lettura + 1) % DIM_BUFFER;
    pthread_mutex_unlock(&mtx);
    sem_post(&semaforo_write);
    return dato_letto;
}

/**
 * procedura che fa iniziare il gioco creando i thread della nave amica e dei nemici e invocando infine "area()"
 */
void startgame(){
    parametri coordinate[N_NEMICI];
    pthread_t tnavicella, tnemici[N_NEMICI];
    int terminanave, terminanemici;
    int i, j = 1,x,y;
    //inizializzazione indice mutex
    pthread_mutex_lock(&mtx);
    counter = 0;
    pthread_mutex_unlock(&mtx);

    pthread_mutex_lock(&mtx);
    counter_lettura = 0;
    pthread_mutex_unlock(&mtx);

    //creazione thread navicella
    pthread_create(&tnavicella, NULL, Navicella, NULL);

    x = MAXX - 10;
    y = 1;
    pthread_mutex_lock(&mtx);
    coordinate[0].x = x;
    coordinate[0].y = y;
    coordinate[0].direzione = -1;
    pthread_mutex_unlock(&mtx);
    //creazione nuovi thread nemici
    for(i=0; i< N_NEMICI; i++){
        pthread_create(&tnemici[i], NULL, nemico, &coordinate[i]);
        if (j % 4 == 0) {
            x = x - DIMENSIONE_OGGETTO - DIMENSIONE_OGGETTO;
            y = 1;
        } else {
            y = y + DIMENSIONE_OGGETTO + 4;
        }

        pthread_mutex_lock(&mtx);
        coordinate[j].y = y;
        coordinate[j].x = x;
        pthread_mutex_unlock(&mtx);
        j++;
    }
    area();
//terminazione delle risorse utilizzate
    terminanave = pthread_cancel(tnavicella);

    for (i=0; i < N_NEMICI; i++ )
        pthread_cancel(tnemici[i]);

    pthread_mutex_destroy(&mtx);
    sem_destroy(&semaforo_write);
    sem_destroy(&semaforo_read);
};

void pulizia_colonne(){
    int i;
    for (i=0; i < MAXY; i++){
        attron(COLOR_PAIR(2));
        mvprintw(i, 0, " ");
        mvprintw(i, MAXX-5, "xxxxxxxx      \0");
        attron(COLOR_PAIR(1));
    }
};

