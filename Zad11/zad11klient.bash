#!/bin/bash 

myfifo=$HOME"/tmp/klientfifo" 

if [ $# -ne 2 ] 
then 
echo "Użycie skryptu: $0 [server] [liczba całkowita]" 
exit 0 
fi 

serverfifo="/home/studinf/"$1"/tmp/serwerfifo" 

if [ ! -p $serverfifo ]; then 
echo "Błąd krytyczny: Plik $serverfifo nie jest właściwym plikiem typu pipe!" 
exit 0 
fi 

echo $HOME $2 > $serverfifo 

    while true 
    do 
        if read line <$myfifo; then 
        echo $line 
        exit 0 
        fi 
    done
