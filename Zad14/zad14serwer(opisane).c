#include <stdio.h> 
#include <stdlib.h> 
#include <sys/msg.h> 
#include <sys/ipc.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h> 
#include <errno.h> 

// Stale predefiniowane 
#define Klucz_wej 11 
#define Klucz_wyj 22 
#define DICTSIZE 12 
#define TRUE 1 

// Definiowanie struktur 
typedef struct { 
   char polish[100]; 
   char english[100]; 
} dictionary; 

typedef struct { 
   long pID; 
   char slowo[100]; 
} message; 

// Zmienne globalne 
dictionary dictPolEng[DICTSIZE] = { 
   {"rower", "bike"}, 
   {"kot", "cat"}, 
   {"pies", "dog"}, 
   {"owca", "lamb"}, 
   {"stół", "table"}, 
   {"krzesło", "chair"}, 
   {"mleko", "milk"}, 
   {"zegar", "clock"}, 
   {"prysznic", "shower"}, 
   {"blisko", "near"}, 
   {"płeć", "sex"}, 
   {"dom", "house"} 
}; 

// Podprogramy 
char *translate(char*); 
void endWork(int); 
void clQueve(int); 

// Zmienne globalne 
int queveIn, queveOt; 

// Start programu    
int main(int argc, char *argv[]) { 

   // Zmienne programu 
   message communique; 
    
   // Inicjalizacja zmiennych 
   queveIn = msgget(Klucz_wej, 0777 | IPC_CREAT); //zmienna postawiona za stworzenie kolejki o indetyfikatorze kolejki 11 i prawach dostępu do IPC 777
   queveOt = msgget(Klucz_wyj, 0777 | IPC_CREAT); //zmienna postawiona za stworzenie kolejki o indetyfikatorze kolejki 22 i prawach dostępu do IPC 777
    
   // Obsluga zakonczenia pracy serwera - kill 
   signal (SIGINT, endWork); 
    
   // Główna pelta programu 
   while(TRUE){ 
      
      // Odczyt z kolejki - pobieranie danych 
      if(msgrcv(queveIn, &communique, sizeof(char)*110, 0, 0) == -1){ //czeka na pojawienie sie w kolejce 11 wiadomości jeśli się pojawi odczyta 
         printf("\nBłąd! Przy odbiorze słowa od klienta");  //jeśli odczytanie wywoła błąd zwróci napis
         exit(1); //i zamknie program
      } 
      
      // Komunikaty o pracy serwera 
      printf("Odebrano słowo \"%s\" ", communique.slowo); //wyświetla napis z odebranym słowem
      strcpy(communique.slowo, translate(communique.slowo)); //tu uruchamiany jest podprogram transplate a jego wynik z return podtawiany jest na nowo za commuunique.slowo z struktury message
      printf("\t\t... sending to PID %5ld \t answer \"%s\"\n", communique.pID, communique.slowo); //wyswietlany jest PID i to nowe słowo
      
      // Zapis do kolejki - wysylanie danych 
      if(msgsnd(queveOt, &communique, sizeof(char)*110, 0) == -1){ //tu jest wysyłany do kolejki wyjscia czyli 22
         printf("\nBłąd! Przy wysyłaniu słowa do klienta");  //jeśli sie nie uda wyświrtli się ten napis
         exit(2); //zakończy działanie programu
      } 
   } // ***** Koniec glownej petli 

   clQueve(queveIn); //zamykanie kolejek podprogramem clQueve
   clQueve(queveOt); //zamykanie kolejek podprogramem clQueve
	
   return 0; 
}        

// Podprogram - wyszukiwanie tlumaczenia 
char *translate(char* word){  //przeszukiwanie tablicy DICTSIZE slowem z translate(slowo) jeżeli słowo będzie wysptępować to zwróci jego odpowiednik
   int i; 

   for ( i = 0; i < DICTSIZE; i++) { 
      if(!strcmp(word, dictPolEng[i].polish)) 
         return dictPolEng[i].english; 
   } 

   return "BRAK"; //jeśli przeszuka całą tablice i nie znajdzie to zwróci słowo BRAK jako (communique.slowo)
} 

// Podprogram - konczacy prace, obslugujacy kill 
void endWork(int sig){ 
   signal (SIGINT, SIG_DFL); 
   clQueve(queveIn); 
   clQueve(queveOt); 
   exit(0); 
} 

// Podprogram - zamykajacy kolejki 
void clQueve(int queve){ 
   if(!msgctl(queve, IPC_RMID, 0)) { 
      printf("\nBłąd! Podczas zamykania kolejki %d.\n", queve); 
      exit(3); 
   } 
}
