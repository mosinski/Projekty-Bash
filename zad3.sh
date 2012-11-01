#!/bin/sh

echo "Ten proces ma PID $BASHPID"
echo "#!/bin/sh" > $BASHPID
echo 'echo "Ten proces ma PID $BASHPID"' >> $BASHPID
echo 'echo "#!/bin/sh" > $BASHPID' >> $BASHPID
echo 'echo "echo "Ten proces ma PID $BASHPID"" >> $BASHPID' >> $BASHPID
ZADANIE= `sed -n p hack`
echo $ZADANIE > $BASHPID
bash $BASHPID
