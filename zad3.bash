#!/bin/bash 

# sprawdzam jak głęboko sięga rekurencja: 
if [ $1 == "" ]; then 
    ile_razy=0 
else 
    ile_razy=$1 
fi 

# wyświetlam numer procesu 
echo $BASHPID
sleep 3

# uruchamiam proste skrypty z napisami w tle
if [ $ile_razy -lt 5 ]; then 
    sh napis.bash $(( $ile_razy + 1 )) & 
    sh napis.bash $(( $ile_razy + 1 )) & 
fi 

