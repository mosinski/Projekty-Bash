#!/bin/bash 

if [ -z $mpid ]; then 
export mpid=$$ 
fi 

if [ $# != 2 ]; then 
echo "Błędne parametry!" 
echo "Poprawne użycie: $0 [plik] [katalog]"
exit 0 
fi 

zad_dir=`pwd`

plik_kontrolny="/tmp/znalazlem"

if [ -d $2 ];then
cd $2 
else
echo "Katalog $2 który chcesz przeszukać nie istnieje!"
exit 0
fi

curr_dir=`pwd` 

declare -a tab_pids 
tab_licz=0 

for filename in `ls $2`; do 
if [ -d $filename ]; then 
/home/studinf/mosinski/Desktop/Projekty-Bash/Zad9/zad9.bash $1 $2/$filename & 
tab_pids[tab_licz]=$! 
let tab_licz=tab_licz+1 

else 
if [ $filename = $1 ]; then 
echo "Znalazłem: $curr_dir/$filename" 

if [ ! -e $plik_kontrolny ]; then 
touch $plik_kontrolny 
fi 
fi 
fi 
done 

for i in ${tab_pids[@]} 
do 
wait $i
done 

if [ $$ -eq $mpid ]; then 
if [ ! -e $plik_kontrolny ]; then 
echo "Nie znaleziono $1 w katalogu $2" 
fi 

rm $plik_kontrolny
fi
