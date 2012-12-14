#include <stdio.h> 
#include <stdlib.h> 
#include <sys/msg.h> 
#include <sys/ipc.h> 
#include <unistd.h> 
#include <string.h> 
#include <errno.h> 

// Stale predefiniowane 
#define Klucz_wej 11 
#define Klucz_wyj 22 
#define TRUE 1 

// Definiowanie struktur 
typedef struct { //stworzenie struktury składającej się z dwóch innych zmiennych
   char polish[100]; 
   char english[100]; 
} dictionary; 

typedef struct { //stworzenie struktury składającej się z dwóch innych zmiennych
   long pID; 
   char slowo[100]; 
} message; 

// Start programu 
int klawisz;   
int main(int argc, char *argv[]) { 

   // Zmienne programu 
   message communique; //struktura message do zmiennej communigue
   int queveIn, queveOt; //zmienne tylu int
   char buffer[100];  //zmienna typu char posłuży do przyjmowania słowa z klawiatury
    
   // Inicjowanie kolejek 
   queveIn=msgget(Klucz_wej, 0777); //zmienna postawiona za stworzenie kolejki o indetyfikatorze kolejki 11 i prawach dostępu do IPC 777
   queveOt=msgget(Klucz_wyj, 0777); //zmienna postawiona za stworzenie kolejki o indetyfikatorze kolejki 22 i prawach dostępu do IPC 777
   printf("\033[2J"); // Czyści ekran
   printf("\033[0;0f");
   printf("___________SŁOWNIK POLSKO ANGIELSKI___________\n");
   // Glowna petla programu 
   while (TRUE) { 
      // Pobieranie danych  
      printf("\nWpisz słowo w języku polskim, \naby zakończyć pracę wpisz KONIEC.\n\nTwoje słowo: "); 
      scanf("%s", buffer); //podstawienie wpisywanych znaków za zmienną buffer
      
      // Warunkowe zakonczenie pracy 
      if(!strcmp(buffer, "KONIEC")) { //jesli zmienna buffer == KONIEC to zaknnij program 
         exit(0); 
      } 

      // Przygotowanie danych do wysylki 
      strcpy(communique.slowo, buffer); //podstawienie zmiennej buffer do zmiennej slowo które stanowi strukture message
      communique.pID=getpid(); //pobiera pid procesu klienta i podstawia go do struktury comunique czyli message jako ID
      
      // Wysylanie danych 
      if(msgsnd(queveIn, &communique, sizeof(char)*110, 0) == -1) { //wykonanie wysyłania do kolejki i sprawdzenie od razu czy się powiodło  jeśli będzie błąd czyli -1 to wypisz napis i zakończ
         printf("\nBłąd! Podczas wysyłania słowa do serwera"); 
         exit(1); 
      } 
      
      // jeśli nie było błedów i program poszedł dalej to wypisz słowo wpisane
      printf("\nWpisane słowo polskie: \"%s\" ", buffer); 

      // Odbieranie wynikow 
      if(msgrcv(queveOt, &communique, sizeof(char)*110, getpid(), 0) == -1) { //jeśli odbieranie wyników się powiedzie przejdź dalej jeśli nei wypisz tekst i zamknij program
         printf("\nBłąd! Podczas odbierania słowa od serwera"); 
         exit(2); 
      } 
      
      // jeśli nie było błędów wypisz otrzymane słowo
      printf("- tłumaczenie: \"%s\".\n", communique.slowo);
   }  // ***** Koniec glownej petli 
return 0; 
} 
