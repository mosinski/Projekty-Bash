#include <fcntl.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <string.h> 
#include <pwd.h> 
//producent czyta i zapisuje 
int main(int argc, char *argv[]) { 
    int plik; 
    int result; 
    char ch; 
    char konto[32]; 
    char lockfile[512]; 
    char dane[512]; 
    char bufor[1024]; 
    int i = 0; 
    char wynik[512]; 
    char *name; 
    struct passwd *pass; 
    pass = getpwuid(getuid()); 
    name = pass->pw_name; 
    
    if(argc != 2) 
    { 
    fprintf(stdout, "Błąd: Podaj nazwę konta do którego chcesz napisać\n"); 
    return 1; 
    } 
    
    strcpy(konto, argv[1]); 
    sprintf(lockfile, "/home/milosz/Pulpit/GitHub/Projekty-Bash/Zad6/%s/tmp/.lockfile", konto); 
    sprintf(dane, "/home/milosz/Pulpit/GitHub/Projekty-Bash/Zad6/%s/tmp/dane", konto); 
    sprintf(wynik, "/home/milosz/Pulpit/GitHub/Projekty-Bash/Zad6/%s/tmp/wynik", konto); 
    
    while(open(lockfile,O_CREAT | O_EXCL, S_IRWXU | S_IRWXG | S_IRWXO) == -1) 
    { 
    printf("Serwer zajęty, proszę czekać\n"); 
    sleep(5); 
    } 
    
    fprintf(stdout, "Proszę wpisać tekst:\n"); 
    
    while((ch=getchar()) != 27) { 
            bufor[i] = ch; 
            i++; 
    } 

    plik = open(dane, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO); 
    if (plik == -1) { 
        fprintf(stdout, "Nie udało się otworzyć pliku"); 
    } else { 
        bufor[i++] = '\n'; //zakończenie linii przed wywołaniem ESC 
                write(plik, name, strlen(name)); 
                write(plik, ":\n", 2); 
                write(plik, bufor, i); 
        close(plik); 
    
    while(1) 
    { 
        if(open(wynik,O_EXCL) != -1) 
        { 
        result = open(wynik, O_RDONLY); 
        fprintf(stdout, "\nOdpowiedź od %s:\n", konto);        
            while (read(result, &ch, 1) == 1) { 
            fprintf(stdout, "%c", ch); 
        } 
        close(result); 
        unlink(wynik); 
        return 0; 
        } 
        sleep(1); 
    } 
    } 
    
    return 0; 
}
