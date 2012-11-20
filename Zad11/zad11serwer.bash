#!/bin/bash 
trap 'exit 0' SIGUSR1 
trap 'sygnał zablokowany: SIGHUP' SIGHUP 
trap 'sygnał zablokowany: SIGTERM' SIGTERM 

klientplikpipe=`mkfifo ~/tmp/klientfifo`
serwerplikpipe=`mkfifo ~/tmp/serwerfifo`

if [ !-e "~/tmp/klientfifo" ]
$klientplikpipe
fi
if [ !-e "~/tmp/serwerfifo" ]
$serwerplikpipe
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
    echo $WYNIK > $klientfifo 
    fi 
    exit 0 
fi
