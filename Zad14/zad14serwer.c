#include <stdio.h> 
#include <stdlib.h> 
#include <sys/msg.h> 
#include <sys/ipc.h> 
#include <unistd.h> 
#include <string.h> 
#include <signal.h> 
#include <errno.h> 
 
#define Klucz_wej 11 
#define Klucz_wyj 22 
#define DICTSIZE 12 
#define TRUE 1 
 
typedef struct { 
   char polish[100]; 
   char english[100]; 
} dictionary; 

typedef struct { 
   long pID; 
   char slowo[100]; 
} message; 

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

char *translate(char*); 
void endWork(int); 
void clQueve(int); 

int queveIn, queveOt; 

int main(int argc, char *argv[]) { 

   message communique; 
    
   queveIn = msgget(Klucz_wej, 0777 | IPC_CREAT);
   queveOt = msgget(Klucz_wyj, 0777 | IPC_CREAT); 
    

   signal (SIGINT, endWork); 
    
   while(TRUE){ 
      
      if(msgrcv(queveIn, &communique, sizeof(char)*110, 0, 0) == -1){ 
         printf("\nBłąd! Przy odbiorze słowa od klienta");
         exit(1); 
      } 
       
      printf("Odebrano słowo \"%s\" ", communique.slowo); 
      strcpy(communique.slowo, translate(communique.slowo)); 
      printf("\t\t... sending to PID %5ld \t answer \"%s\"\n", communique.pID, communique.slowo); 

      if(msgsnd(queveOt, &communique, sizeof(char)*110, 0) == -1){ 
         printf("\nBłąd! Przy wysyłaniu słowa do klienta");
         exit(2);
      } 
   } 

   clQueve(queveIn); 
   clQueve(queveOt);
	
   return 0; 
}        

char *translate(char* word){  
   int i; 

   for ( i = 0; i < DICTSIZE; i++) { 
      if(!strcmp(word, dictPolEng[i].polish)) 
         return dictPolEng[i].english; 
   } 

   return "BRAK";
} 

void endWork(int sig){ 
   signal (SIGINT, SIG_DFL); 
   clQueve(queveIn); 
   clQueve(queveOt); 
   exit(0); 
} 

void clQueve(int queve){ 
   if(!msgctl(queve, IPC_RMID, 0)) { 
      printf("\nBłąd! Podczas zamykania kolejki %d.\n", queve); 
      exit(3); 
   } 
}
