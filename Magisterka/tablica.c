#include <stdio.h> 
#include <stdlib.h>
#include <ctype.h>

int main() { 
    FILE *plik; 
    int lock;  
    char dane[512]; 
    int i = 0; 
    char *name;
    char ch; 
    int v;
    int array[10];

    plik = fopen("dane.txt", "r"); 

    while (fscanf(plik, "%d", &v) == 1)
    {
    array[i++] = v;
    printf("%d\n",v);
    }
    fclose (plik);
    return 0;
}
