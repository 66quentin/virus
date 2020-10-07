#!/bin/bash
# fichier hote=$1 et virus=$2

nombre1=$(grep "" -c $1)
nombre2=$(grep "" -c $2)

iteration1=0
iteration2=0
ligne1="fichier1"
ligne2="fichier2"

touch F3.sh

while (("$iteration1" < "$nombre1")) || (("$iteration2" < "$nombre2"));
do

	if [ $(( $RANDOM % 2 )) == 1 ] && (("$iteration2" < "$nombre2"))
	then
		iteration2=$[iteration2+1]
		ligne2=$(sed "${iteration2}q;d" $2)
		commande=$(shuf -n1 -e /usr/bin/*)
		commande="${commande:9}"
		ligne2=$(echo $ligne2  | sed -e 's/^\w*\ *//')
		echo "$commande $ligne2 #@$iteration2">> F3.sh
	fi
	if (("$iteration1" < "$nombre1"))
	then
		iteration1=$[iteration1+1]
		ligne1=$(sed "${iteration1}q;d" $1)
	    	echo $ligne1 >> F3.sh
	fi
done
