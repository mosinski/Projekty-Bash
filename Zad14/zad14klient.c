#include <stdio.h> 
#include <stdlib.h> 
#include <sys/msg.h> 
#include <sys/ipc.h> 
#include <unistd.h> 
#include <string.h> 
#include <errno.h> 

#define Klucz_wej 11 
#define Klucz_wyj 22 
#define TRUE 1 

typedef struct { 
   char polish[100]; 
   char english[100]; 
} dictionary; 

typedef struct {
   long pID; 
   char slowo[100]; 
} message; 
  
int main(int argc, char *argv[]) { 

   message communique; 
   int queveIn, queveOt;
   char buffer[100];
    

   queveIn=msgget(Klucz_wej, 0777); 
   queveOt=msgget(Klucz_wyj, 0777); 
   printf("\033[2J");
   printf("\033[0;0f");
   printf("___________SŁOWNIK POLSKO ANGIELSKI___________\n");
  
   while (TRUE) { 
      
      printf("\nWpisz słowo w języku polskim, \naby zakończyć pracę wpisz KONIEC.\n\nTwoje słowo: "); 
      scanf("%s", buffer); 
      
 
      if(!strcmp(buffer, "KONIEC")) { 
         exit(0); 
      } 
 
      strcpy(communique.slowo, buffer); 
      communique.pID=getpid(); 
      
    
      if(msgsnd(queveIn, &communique, sizeof(char)*110, 0) == -1) { 
         printf("\nBłąd! Podczas wysyłania słowa do serwera"); 
         exit(1); 
      } 
      
      printf("\nWpisane słowo polskie: \"%s\" ", buffer); 
 
      if(msgrcv(queveOt, &communique, sizeof(char)*110, getpid(), 0) == -1) {
         printf("\nBłąd! Podczas odbierania słowa od serwera"); 
         exit(2); 
      } 
      
      printf("- tłumaczenie: \"%s\".\n", communique.slowo);
   } 
return 0; 
} 
