#define _OPEN_SYS
#include <fcntl.h> 
#include <stdio.h> 
#include <unistd.h> 
#include <pwd.h> 

int main(int argc, char *argv[]) { 
    int plik; 
    int lock; 
    char ch; 
    char lockfile[512]; 
    char dane[512]; 
    char wynik[512]; 
    char bufor[1024]; 
    char *name;
    int i = 0; 
    struct passwd *pass; 
    uid_t  uid;
    pass = getpwuid(uid = getuid()); 
    name = pass->pw_name; 
 
    sprintf(lockfile, "/home/milosz/Pulpit/GitHub/Projekty-Bash/Zad6/%s/tmp/.lockfile", name);
    sprintf(dane, "/home/milosz/Pulpit/GitHub/Projekty-Bash/Zad6/%s/tmp/dane", name); 
    sprintf(wynik, "/home/milosz/Pulpit/GitHub/Projekty-Bash/Zad6/%s/tmp/wynik", name); 

    unlink(lockfile); 
    unlink(dane); 
    unlink(wynik); 
    
    while(1) 
    { 
    if(open(lockfile,O_EXCL) != -1) 
    { 
        if(open(dane,O_EXCL) != -1) 
        { 
                plik = open(dane, O_RDONLY); 
                printf("Wiadomość od "); 
                while (read(plik, &ch, 1) == 1) { 
            fprintf(stdout, "%c", ch); 
            } 
                close(plik); 
            unlink(dane); 

            
                printf("\nNapisz odpowiedź:\n");        
            
            i = 0; 
            while((ch=getchar()) != 27) 
            { 
                bufor[i] = ch; 
                i++; 
            } 

            plik = open(wynik, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU | S_IRWXG | S_IRWXO); 
            if (plik == -1) { 
            fprintf(stdout, "Nie udało się otworzyć pliku"); 
            return 1; 
            } else { 
            bufor[i++] = '\n';
                    write(plik, bufor, i); 
            close(plik); 
            } 
            unlink(lockfile); 
        } 
    } 
    sleep(1); 
    
    } 
    close(lock); 
    return 0; 
}
