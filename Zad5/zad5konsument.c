#include <fcntl.h> 
#include <stdio.h> 
#include <unistd.h> 

void main(int argc, char *argv[]) { 
    int plik; 
    char ch; 
    
    while(1) 
    { 
        if(plik = open("dane.txt", O_RDONLY)) 
        { 
            while (read(plik, &ch, 1) == 1) 
            { 
            fprintf(stdout, "%c", ch); 
        } 
        
        close(plik); 
        unlink("dane.txt"); 
        } 
        sleep(1); 
    } 
    
}
