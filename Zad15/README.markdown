Tworze nieduży segment poleceniem:
pamiec=shmget(klucz,256,0700|IPC_CREAT);

Dołączam do segmentu poleceniem:
adres1=shmat(pamiec,0,0);
adres2=shmat(pamiec,0,0);

Odczyt znaków z klawiatury i Zapis do segmentu poleceniem:
scanf("%s",adres1);
scanf("%s",adres2);

Odczyt z segmentu poleceniem:
printf("W funkcji 1 jest zapisane: %s\n",adres1);
printf("W funkcji 2 jest zapisane: %s\n",adres2);

Sprawdzanie
W terminalu nr 1 po stworzeniu segmentów i dołączeniu
zapisuje:
>proba
scanf("%s",adres1);

W terminalu nr 2 odczytuje:
printf("W funkcji 2 jest zapisane: %s\n",adres2);
>proba

wniosek: wartości są identyczne w obu segmentach.

Cały program:
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define klucz 10

int pamiec,wybor;
char *adres1;
char *adres2;
int segment2 = 0;
void koniec()
{
shmdt(adres1);
shmdt(adres2);
shmctl(pamiec,IPC_RMID,0);
exit(0);
}

void odczyt1()
{
printf("W funkcji 1 jest zapisane: %s\n",adres1);
}

void zapis1()
{
printf("Podaj łancuch zapisywany do funkcji 1: ");
scanf("%s",adres1);
}

void dolaczSegment2()
{
if(segment2 == 0)
{
adres2=shmat(pamiec,0,0);
segment2 = 1;
}
}

void odczyt2()
{
dolaczSegment2();
printf("W funkcji 2 jest zapisane: %s\n",adres2);
}

void zapis2()
{

printf("Podaj łancuch zapisywany do funkcji 2: ");
scanf("%s",adres2);
}


main()
{
pamiec=shmget(klucz,256,0700|IPC_CREAT);
adres1=shmat(pamiec,0,0);

for (;;)
{
printf("M E N U :\n");
printf("\n");
printf("0 - odczyt z pierwszej funkcji\n");
printf("1 - zapis do pierwszej funkcji\n");
printf("2 - odczyt z drugiej funkcji\n");
printf("3 - zapis do drugiej funkcji\n");
printf("4 - koniec\n");
printf("\n");
printf("Co wybierasz ? ");
scanf("%d",&wybor);
switch(wybor)
{
case 0: odczyt1(); break;
case 1: zapis1(); break;
case 2: odczyt2(); break;
case 3: zapis2(); break;
case 4: koniec(); break;
default: printf("Nie ma takiej opcji ! \n"); break;
}
}
}
