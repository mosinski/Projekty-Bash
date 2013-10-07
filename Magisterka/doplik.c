#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE *file;
	char dane;
	char filename[20];
	
	printf("Podaj nazwę pliku: ");
	scanf("%s",filename);
	getchar();
	
	file = fopen(filename,"a+"); 
	
	printf("Wpisz dane: ");
	
	while(1){
	  scanf("%c",&dane);
	  
	  if(dane == '0'){
	   printf("Koniec\n");
	   fclose(file);
	   return 0;
	  }
	 
	fprintf(file,"%c",dane);
	
	}
}
