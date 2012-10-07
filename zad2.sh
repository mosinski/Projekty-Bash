#!/bin/sh 
while true 
do

echo "\nMenu\n"
echo "0-Wyjście z programu"
echo "1-Zapisywanie do pliku"
echo "2-Odczytywanie z pliku"
echo -n "> "

read d

if [ $d -eq 0 ]
 then echo "Koniec Programu"; exit 4
fi

case "$d" in
  "1") echo 'Napisz wiadomość:'
	while true 
	do
	read r
	if [ $r = "0" ]
 	then
	echo "Koniec Zapisywania\n"; 
	break
	else
	echo "$r" >> komunikat.txt
	fi
        done;;
  "2") cat komunikat.txt ;
	echo "\n";;
  *) echo "Niewłaściwy wybór!"
esac
   done
   exit 0
