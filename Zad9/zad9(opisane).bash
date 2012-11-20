#!/bin/bash 

# Struktura kontrolna służąca do rozpoznania procesu macierzystego (wywołanego przez użytkownika)
if [ -z $mpid ]; then 
export mpid=$$ # Zmienna mpid przechowuje pid tego procesu 
fi 

# Instrukcja wywoływana w przypadku przekazania nieodpowiedniej liczby parametrów 
if [ $# != 2 ]; then 
echo "Błędne parametry!" 
echo "Poprawne użycie: $0 [plik] [katalog]"
exit 0 
fi 

zad_dir=`pwd`
# Zmienna określająca ścieżkę do pliku, którego istnienie oznacza, że coś znaleziono 
plik_kontrolny="/tmp/znalazlem"
# Przejdź do katalogu podanego jako drugi parametr jeżeli ten folder istnieje
if [ -d $2 ];then
cd $2 
else
echo "Katalog $2 który chcesz przeszukać nie istnieje!"
exit 0
fi
curr_dir=`pwd` 

# licznik katalogów [iterator po tablicy pids] 
dnum=0 
# definicja tablicy przechowującej pidy procesów potomnych 
declare -a pids 

# przejdź przez wszystkie wyniki polecenia ls dla aktualnego katalogu 
for filename in `ls $curr_dir`; do 
if [ -d $filename ]; then # jeżeli dana nazwa określa katalog, wywołaj znowu skrypt z nową ścieżką 
$zad_dir/$0 $1 $curr_dir/$filename & 
pids[dnum]=$! # dodaj pid nowego procesu do tablicy 
let dnum=dnum+1 

else #jeżeli plik nie jest katalogiem to sprawdzam czy jest szukanym plikiem

# jeżeli plik w danym katalogu jest szukanym plikiem, to wypisujemy odpowiedni komunikat 
if [ $filename = $1 ]; then 
echo "Znalazłem: $curr_dir/$filename" 
# jeżeli nie istnieje jeszcze plik określający znalezienie wyniku, to go utwórz 
if [ ! -e $plik_kontrolny ]; then 
touch $plik_kontrolny 
fi 
fi 
fi 
done 

# przejdź przez wszystkie pidy w tablicy i zakończ każdy z nich 
for i in ${pids[@]} 
do 
wait $i
done 

# jeżeli wywołany proces jest procesem macierzystym, to sprawdzamy, czy coś znaleziono 
if [ $$ -eq $mpid ]; then 
if [ ! -e $plik_kontrolny ]; then 
echo "Nie znaleziono $1 w katalogu $2" 
fi 
# usuwamy plik kontrolny (pomijając błąd, jeżeli nie istnieje) 
rm $plik_kontrolny
fi
