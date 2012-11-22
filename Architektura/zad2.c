#define _OPEN_SYS
#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <semaphore.h>

int main() { 
    FILE *f;
    const int TR = 49; 
    const int FA = 48; 
    char slowo[10]; 
    char num1[10]; 
    char num2[10]; 
    char op[10]; 
    int x;


    if (( f = fopen("dane.txt", "r")) == NULL ) 
     {  
     printf("Brak pliku dane.txt!\n");
     return 0; 
     } 

    while (fscanf(f, "%s", slowo) != EOF) { 
        if (strcmp(slowo, "NOT") == 0) { 
            strncpy(op, "NOT", 10); 
            fscanf(f, "%s", num1); 
            printf("%s %s = ", op, num1); 
                for (x = 0; x < strlen(num1); x++) { 
                    if (num1[x] == TR) { num1[x] = FA; } else { num1[x] = TR; } 
                } 
            printf("%s\n", num1); 
        } else { 
            strncpy(num1, slowo, 10); 
            fscanf(f, "%s", op); 
            fscanf(f, "%s", num2); 
            printf("%s %s %s = ", num1, op, num2); 
            for (x = 0; x < strlen(num1); x++) { 
                if (strcmp(op, "AND") == 0) { 
                    if(num1[x] == TR && num2[x] == TR) { num1[x] = TR; } else { num1[x] = FA; } 
                } 
                if (strcmp(op, "NAND") == 0) { 
                    if(num1[x] == TR && num2[x] == TR) { num1[x] = FA; } else { num1[x] = TR; } 
                } 
                if (strcmp(op, "OR") == 0) { 
                    if(num1[x] == TR || num2[x] == TR) { num1[x] = TR; } else { num1[x] = FA; } 
                } 
                if (strcmp(op, "NOR") == 0) { 
                    if(num1[x] == TR || num2[x] == TR) { num1[x] = FA; } else { num1[x] = TR; } 
                } 
                if (strcmp(op, "XOR") == 0) { 
                    if(num1[x] == num2[x]) { num1[x] = FA; } else { num1[x] = TR; } 
                } 
            } 
            printf("%s\n", num1); 
        } 
    } 

    fclose(f); 

    return 0; 
} 
