#include <stdio.h>
#include <stdlib.h>
#include <linux/msg.h>
#include <linux/ipc.h>

#define path "/tmp" 

struct komunikat
   {
      long typ;
      char dane[256];
   };
   
main()
   {
      struct komunikat kom;
      int kolejka, *wsk;
      key_t klucz; 

      klucz = ftok(path, 10);
      kolejka=msgget(klucz,0777|IPC_CREAT);

      msgrcv(kolejka,&kom,sizeof(int),0,0);
      wsk=(int*) kom.dane;
      printf("serwer: otrzymalem %5d od %5ld\n", *wsk, kom.typ);
      (*wsk)++;
      printf("serwer: odsylam %5d do %5ld\n", *wsk, kom.typ);
      msgsnd(kolejka,&kom,sizeof(int),0);
      sleep(5);
      msgctl(kolejka,IPC_RMID,0);
   }       
