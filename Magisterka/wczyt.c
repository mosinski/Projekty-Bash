#include <stdio.h> 
#include <stdlib.h>

int main() { 
    FILE *plik; 
    int lock;  
    char dane[512]; 
    int i = 0; 
    char *name;
    char ch; 
 

    plik = fopen("dane.txt", "r"); 
    printf("Plik: "); 

    fscanf (plik, "%s", dane);
    fclose (plik);
    printf("%s\n", dane);
    return 0;
}
