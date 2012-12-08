#!/bin/bash 
trap 'exit 0' SIGUSR1 
trap 'sygnał zablokowany: SIGHUP' SIGHUP 
trap 'sygnał zablokowany: SIGTERM' SIGTERM 

KLIENT=$HOME"/tmp/klientfifo"
SERWER=$HOME"/tmp/serwerfifo"


if [ ! -e $KLIENT ]
then
klientplikpipe=`mkfifo ~/tmp/klientfifo`
fi

if [ ! -e $SERWER ]
then
serwerplikpipe=`mkfifo ~/tmp/serwerfifo`
fi

if [ $# -ne 2 ] 
    then 
    myfifo=$HOME"/tmp/serwerfifo" 
    while true 
    do 
        if read line <$myfifo; then 
        bash $0 $line 
        fi 
    done 
else 
    klientfifo=$1"/tmp/klientfifo" 
    if [ -p $klientfifo ]; then 
    LICZBA=$2 
    WYNIK=`echo "obase=2;$LICZBA" | bc`
    sleep 10
    echo $WYNIK > $klientfifo 
    fi 
    exit 0 
fi
