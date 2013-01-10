#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>  // for semaphores 
#include <sys/shm.h>  // for shared memory 
#include <errno.h> 
#define MAXLEVEL 7 

// unia używana do semaforow 
union semun { 
    int val; 
    struct semid_ds *buf; 
    ushort *array; 
} arg; 

// initBoard: funkcja wypelniajaca na poczatku plansze spacjami 
char *initBoard(char *tab, int level) 
{ 
    int i; 

    for (i = 0; i < level * level; i++) 
        tab[i] = ' '; 
    return tab; 
} 

/* checkNextMovement: funkcja sprawdzajaca, czy można wykonac jeszcze ruch na planszy 
   (innymi slowy, czy znajduje sie na niej jeszcze wolne pole) */ 
int checkNextMovement(char *tab, int level) 
{ 
    int i; 

    for (i = 0; i < level * level; i++) 
        if (tab[i] == ' ') 
            return 1; 
    return 0; 
} 

// makeMove: funkcja wstawiajaca znak na polu podanym przez gracza 
char *makeMove(char *tab, char sign, int level) 
{ 
    int x, y, ok = 0; 
    do { 
        do { 
            printf("Podaj pozycje, na ktorej mam wykonac twoj ruch: "); 
            scanf("%d", &x); 
            scanf("%d", &y); 
            if (x < 1 || x > level || y < 1 || y > level) { 
                printf("Nieprawidlowe dane! Sprobuj jeszcze raz.\n"); 
                ok = 0; 
            } 
            else 
                ok = 1; 
        } 
        while (!ok); 
        if (tab[(x - 1) * level + (y - 1)] != ' ') { 
            printf("Te pole jest już zajete!\n"); 
            ok = 0; 
        } 
    } 
    while (!ok); 

    // dodaj znak do podanego pola 
    tab[(x - 1) * level + (y - 1)] = sign; 
    return tab; 
} 

/* displayBoard: funkcja wyswietlajaca cala plansze gry - dostosowana do 
   zmiennej liczby pol (okresla to zmienna level) */ 
void displayBoard(char *tab, int level, char mysign) 
{ 
    int i, j; 

    // wyswietl gorny brzeg 
    for (i = 0; i < level * 4 + 1; i++) 
        printf("_"); 
    printf("\n"); 

    for (i = 0; i < level; i++) { 
        for (j = 0; j < level; j++) { 
            printf("|"); 
            if (tab[i * level + j] == mysign) 
                printf("\x1b[32m"); 
            else 
                printf("\x1b[31m"); 
            printf(" %c\x1b[0m ", tab[i * level + j]); // wyswietl kolejne pola 
        } 
        printf("|\n"); 
        if (i < level -1) { 
            for (j = 0; j < level; j++) 
                printf("| - ");  // wyswietl odstepy 
            printf("|\n"); 
        } 
    } 
    // wyswietl dolny brzeg 
    for (i = 0; i < level * 4 + 1; i++) 
        printf("-"); 
    printf("\n"); 
} 

/* checkIfWon: funkcja sprawdzajaca wszystkie możliwe pozycje w poszukiwaniu sytuacji, 
   w ktorej ktorys gracz wygral - zwraca wygrany znak, jeżeli tak jest */ 
char checkIfWon(char *tab, int level) 
{ 
    int i, j, won = 0; 
    char sign; 

    // przejd przez wszystkie poziome linie 
    for (i = 0; i < level; i++) { 
        for (j = 0; j < level - 1; j++) { 
            // sprawd czy po kolei wszystkie pola poziomo sa sobie rowne 
            if (tab[i * level + j] != ' ' && tab[i * level + j] == tab[i * level + (j + 1)]) { 
                sign = tab[i * level + j]; 
                won = 1; 
            } 
            else { 
                won = 0; 
                break; 
            } 
        } 
        if (won) 
            break; 
    } 

    // analogicznie jak wyżej, tyle że sprawdzane sa poziome linie 
    if (!won) { 
        for (j = 0; j < level; j++) { 
            for (i = 0; i < level - 1; i++) { 
                if (tab[i * level + j] != ' ' && tab[i * level + j] == tab[(i + 1)* level + j]) {
                    sign = tab[i * level + j]; 
                    won = 1; 
                } 
                else { 
                    won = 0; 
                    break; 
                } 
            } 
            if (won) 
                break; 
        } 
    } 

    // sprawd linie ukosna z lewej gory na prawy dol 
    if (!won) { 
        for (i = 0; i < level - 1; i++) { 
            if (tab[i * level + i] != ' ' && tab[i * level + i] == tab[(i + 1) * level + (i + 1)]) { 
                    sign = tab[i * level + i]; 
                    won = 1; 
                } 
                else { 
                    won = 0; 
                    break; 
                } 
            } 
    } 

    // sprawd linie ukosna z prawej gory na lewy dol 
    if (!won) { 
        for (i = 0; i < level - 1; i++) 
            if (tab[i * level + level-1-i] != ' ' && tab[i * level + level-1-i] == tab[(i + 1) * level + level-2-i]) { 
                    sign = tab[i * level + level-1-i]; 
                    won = 1; 
                } 
                else { 
                    won = 0; 
                    break; 
                } 
    } 

    // jeżeli byla sytuacja wygranej, zwroc zwycieżajacy znak 
    if (won) { 
        return sign; 
    } 
    else 
        return 0; 
} 

// getKey: pomocnicza funkcja pobierajaca klucz dla semaforow/pamieci wspoldzielonej 
key_t getKey(char *path, char hlp_sgn) 
{ 
    key_t key; 
    if ((key = ftok(path, hlp_sgn)) == (key_t) - 1) { 
        printf("Wystapil blad ftok()! Identyfikator: %s\n", strerror(errno)); 
        exit(1); 
    } 
    return key; 
} 

// getSemId: pomocnicza funkcja zwracajaca id aktualnego semafora (o ile istnieje) 
int getSemId() 
{ 
    char cmd[50]; 
    FILE *fp; 
    int id, rval; 

    /* pobierz identyfikator przez polecenie ipcs -m (dla pamieci wspoldzielonej) 
       lub ipcs -s (dla semaforow) i zapisz do pliku */ 
    sprintf(cmd, "ipcs -s | grep korki | cut -d ' ' -f2 > ipcobj"); 
    system(cmd); 

    // otworz plik z id 
    if ((fp = fopen("ipcobj", "r")) == NULL) { 
        printf("Wystapil krytyczny blad! Nie można bylo odczytac pliku z potoku ipcs...\n"); 
        exit(1); 
    } 
    // wyciagnij identyfikator z pliku 
    rval = fscanf(fp, "%d\n", &id); 

    fclose(fp); 
    sprintf(cmd, "rm -f ipcobj"); // plik jest już niepotrzebny, wiec go usuń 
    system(cmd); 

    if (rval > 0) 
        return id; 
    else return -1; 
} 

/* getIPCId: funkcja tworzaca identyfikator dla semaforow lub pamieci wspoldzielonej, 
   w zależnosci od przekazanego parametru type */ 
int getIPCId(char *path, char *type, int *starting, int length) 
{ 
    int ipc_id; 
    union semun arg; 

    if (!strcmp(type, "sem")) { 
        // sprawd, czy istnieje już semafor - jesli nie, to go utworz 
        if ((ipc_id = semget(getKey(path, 'm'), 1, 0660 | IPC_CREAT | IPC_EXCL)) != -1) { 
            *starting = 1;  // parametr okreslajacy, czy użytkownik rozpoczyna gre 
            arg.val = 1; 
            if (semctl(ipc_id, 0, SETVAL, arg) == -1) { 
                printf("Wystapil blad semctl()! Identyfikator: %s\n", strerror(errno)); 
                exit(1); 
            } 
        } 
        // jesli semafor istnieje, to pobierz jego identyfikator 
        else if ((ipc_id = semget(getKey(path, 'm'), 1, 0660 | IPC_CREAT)) == -1) { 
            printf("Wystapil blad semget()! Identyfikator: %s\n", strerror(errno)); 
            exit(1); 
        } 
    } 
    else if (!strcmp(type, "shm")) { 
        // utworz identyfikator dla pamieci wspoldzielonej 
        if ((ipc_id = shmget(getKey(path, 'n'), length, 0660 | IPC_CREAT)) == -1) { 
            printf("Wystapil blad shmget()! Identyfikator: %s\n", strerror(errno)); 
            exit(1); 
        } 
    } 
    else 
        ipc_id = -1; 
    return ipc_id; 
} 

// blockSemaphore funkcja blokujaca semafor 
void blockSemaphore(int semid, struct sembuf *semops) 
{ 
    if (getSemId() == semid) { // zablokuj semafor, o ile istnieje 
        // zablokuj semafor 
        semops->sem_op = -1; 
        if (semop(semid, semops, 1) == -1) { 
            printf("[BL] Wystapil blad semop()! Identyfikator: %s\n", strerror(errno)); 
            exit(1); 
        } 
    } 
} 

// unblockSemaphore: funkcja odblokowujaca semafor 
void unblockSemaphore(int semid, struct sembuf *semops) 
{ 
    usleep(100000); 
    if (getSemId() == semid) { // odblokuj semafor, o ile istnieje 
        // odblokuj semafor 
        semops->sem_op = 1; // ustaw operacje na semaforze na dodatnia (zwalnia go) 
        if (semop(semid, semops, 1) == -1) { 
            printf("[UNBL] Wystapil blad semop()! Identyfikator: %s\n", strerror(errno)); 
            exit(1); 
        } 
    } 
} 

int main(int argc, char **argv) 
{ 
    char *board = NULL, mysign, c, cmd[50]; 
    char *path = getenv("PWD"); // scieżka do tego pliku (dla funkcji ftok) 
    int shmid, semid; // identyfikatory dla pamieci wspoldzielonej i semaforow 
    int end = 0, starting = 0, level, win_state = 0; 
    // struktura używana przez funkcje semop() 
    struct sembuf semops = {0, -1, 0};  // dane na poczatek (ustawienie semafora) 

    printf("\nGra - Kolko i Krzyzyk\n"); 

    // I N I C J A L I Z A C J A   D A N Y C H  (semafory i pamiec wspoldzielona) 

    // pobierz id dla semafora i sprawd, czy użytkownik zaczyna jako pierwszy 
    semid = getIPCId(path, "sem", &starting, 0); 

    if (starting) { 
        if (argc != 3) { 
            printf("Uzycie programu: %s n S, gdzie n jest poziomem gry (3-7), a S jest znakiem używanym w grze:\n", argv[0]); 
            printf("X lub O.\n"); 
            semctl(semid, 0, IPC_RMID); 
            exit(1); 
        } 
        // pobierz dane z wejscia 
        else { 
            if ((level = atoi(argv[1])) < 3 || level > MAXLEVEL || ((mysign = argv[2][0]) != 'X' && mysign != 'O')) { 
                printf("Nieprawidlowy argument!\n"); 
                semctl(semid, 0, IPC_RMID); 
                exit(1); 
            } 
        } 

        printf("Rozpoczynasz gre jako pierwszy (-a)!\n\n"); 
        // zapisz informacje o grze dla drugiego użytkownika 
        sprintf(cmd, "echo \"%d %c\" > /home/korki/tmp/xo_info;", level, mysign); 
        system(cmd); 
        sprintf(cmd, "chmod 660 /home/korki/tmp/xo_info"); 
        system(cmd); 
    } 
    else { 
        printf("Rozpoczynasz gre jako drugi (-a)!\n\n"); 
        // pobierz dane z pliku od użytkownika, ktory rozpoczal cala gre 
        FILE *fp = fopen("/home/korki/tmp/xo_info", "r"); 
        if (!fp) { 
            printf("Nie moglem odczytac danych od pierwszego gracza!\n"); 
            exit(1); 
        } 
        fscanf(fp, "%d %c\n", &level, &mysign); 
        fclose(fp); 
        sprintf(cmd, "rm -f /home/korki/tmp/xo_info"); 
        system(cmd); 
        printf("Pierwszy gracz ustalil poziom gry na: %d i wybral znak %c, dlatego twoj to: ", level, mysign); 
        // ustaw przeciwny znak dla drugiego gracza 
        if (mysign == 'X') 
            mysign = 'O'; 
        else 
            mysign = 'X'; 
        printf("%c\n", mysign); 

    } 

    // pobierz id dla pamieci wspoldzielonej (ostatni parametr okresla rozmiar planszy w bajtach) 
    shmid = getIPCId(path, "shm", NULL, level * level * sizeof(char)); 

    // dolacz obszar pamieci do pamieci wspoldzielonej 
    if ((board = shmat(shmid, (char *)0, 0)) == (char *) -1) { 
        printf("Wystapil blad shmat()! Identyfikator: %s\n", strerror(errno)); 
        exit(1); 
    } 
    if (starting) 
        board = initBoard(board, level); // inicjuj plansze gry 

    printf("\nZaczynamy gre!\n"); 

    // W l A s C I W A   G R A 
    while (!end) { 
        // zablokuj semafor 
        blockSemaphore(semid, &semops); 
        // pokaż plansze 
        displayBoard(board, level, mysign); 
        // sprawd, czy ktos wygral 
        if ((c = checkIfWon(board, level)) != 0) { 
            end = 1; 
            if (c == mysign) 
                win_state = 1; // wygralismy :) 
            else 
                win_state = -1; // przegralismy :( 
        } 
        else { 

            // sprawd, czy można wykonac ruch (tzn. czy sa jeszcze wolne miejsca) 
            if (checkNextMovement(board, level)) { 
                // wykonaj ruch 
                board = makeMove(board, mysign, level); 
                displayBoard(board, level, mysign); 
                // sprawd, czy ktos wygral 
                if ((c = checkIfWon(board, level)) != 0) { 
                    end = 1; 
                    if (c == mysign) 
                        win_state = 1; // wygralismy :) 
                    else 
                        win_state = -1; // przegralismy :( 
                } 
            } 
            else 
                end = 1; 
        } 

        // odblokuj semafor 
        unblockSemaphore(semid, &semops); 

        if (!end) 
            printf("Czekam na ruch przeciwnika...\n"); 
    } 

    // wyswietl końcowy komunikat 
    if (win_state == 1) 
        printf("\nGratulacje! Wygrales (-as) ta trudna potyczke!\n"); 
    else if (win_state == -1) 
        printf("\nPrzykro mi, tym razem przegrales (-as)!\n"); 
    else 
        printf("\nTym razem nikt nie wygral - remis!\n"); 

    // S P R Z a T A N I E 
    shmdt(board);  // odlacz obszar od pamieci wspoldzielonej 

    if (starting) { 
        // usuń semafor 
        if (semctl(semid, 0, IPC_RMID) == -1) 
            printf("Wystapil blad semctl()! Identyfikator: %s\n", strerror(errno)); 

        // usuń segment pamieci wspoldzielonej 
        if (shmctl(shmid, IPC_RMID, 0) == -1) { 
            printf("Wystapil blad shmctl()! Identyfikator: %s\n", strerror(errno)); 
            exit(1); 
        } 
    } 
    return 0; 
}
