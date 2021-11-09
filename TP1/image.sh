#!/bin/bash

#Repertoire des images
IMAGEREP=$1
#Repertoire d'arriver des images
IMAGEWRITE=$2
#Resolution
RES=$3

#On test le nombre d'arguments
if [ "$#" -lt 2 ] ; then #2 arguments au minimum
	echo "Not enough arguments"
	echo "Should be: images <PATH IMAGES> <PATH NEW IMAGES> [RESOLUTION]"
	exit
fi

#Fonction pour clean les nom de fichier
function cleanu() {
	mv "$1" $(echo "$1" | sed -r "s/[[:space:]\'!]+//g") #On renome en enlevant !/'/" "
}

#fonction pour convertir toutes les images en .png
function convertir() {
	if [[ $(file -i $1 | awk '{print $2}') = image/* ]] ; then #On check si c'est une imagei
		case $RES in
			"")	convert $1 $IMAGEREP/$(basename ${1%.*}).png ;; #On converti
			*)	convert -resize $RES $1 $IMAGEREP/$(basename ${1%.*}).png ;; #On converti +RES
		esac
	
	else
		echo "'$1' is not an image"
	fi
}

#On check d'abord si le Nouveau Repertoire existe, sinon on le cree
if [ -d $IMAGEWRITE ] ; then
	
	#On converti toute les images en png
	for FILE in "$IMAGEREP"/*; do
		convertir "$FILE"
	done

	#On clean les noms de fichiers
	for FILE in "$IMAGEREP"/*; do
		cleanu "$FILE"
		#echo "$FILE"
	done

	for FILES in "$IMAGEREP"/*; do
		if [[ $FILES == *.png ]] ; then 
			cp $FILES $IMAGEWRITE
			chmod 740 $FILES
			echo "$FILES copied."
		else
			echo "$FILES not copied"
		fi
	done

else
	mkdir -p $IMAGEWRITE #On cree le repertoire
	echo "Directory $IMAGEWRITE created"
	echo "restarting script ..."
	exec ./image.sh $IMAGEREP $IMAGEWRITE $RES #On relance le script
fi


