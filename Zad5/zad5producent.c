#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>

void main(int argc, char *argv[]) { 
    int plik; 
    char c; 
    char bufor[1024]; 
    int i = 0; 
    
    fprintf(stdout,"Wiadomość do konsumenta aby zakończyć wciśnij ESC potem potwierdź Enter-em\n");
    fprintf(stdout, "Prosze wpisać tekst:\n"); 
    
    while(c=getchar()) 
    { 
            if(c == 27) 
            { 
            bufor[i++] = '\n'; //zakończenie linii przed wywołaniem ESC
            plik = open("dane.txt", O_CREAT | O_APPEND | O_WRONLY, S_IRWXU); 
            write(plik, bufor, i); 
       	    close(plik); 
            i = 0; 
	    }
            else 
            { 
            bufor[i] = c; 
            i++; 
            } 

    } 
}
