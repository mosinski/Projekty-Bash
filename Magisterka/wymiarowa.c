#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *file;
	int i = 0;
	char c;
	int isEx = 0;

	file = fopen("dane.txt", "r");
	
	if(file) {
	  while(fscanf(file, "%c", &c) == 1) {
	    if(c == '\n')
		 printf("\n");
	    else{
	    	 isEx = 1;
		 printf("%c", c);
		}
	  }
	printf("\n");
	fclose(file);
	
	if(isEx == 0){
		printf("Błąd! Pusty plik.\n");
	}
	
	}
	else {
	    printf("Błąd! Brak pliku.\n");
	}
	return 0;
}
