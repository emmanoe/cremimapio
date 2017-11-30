=======
README
=======

Le programme maputil est capable de consulter, modifier, remplacer les informations d'une carte stockées dans un fichier <file>.


--------------------------Consulter les informations--------------------------

maputil <file> --getwidth		--> retourne la largeur actuelle de la carte <file>
maputil <file> --getheight		--> retourne la hauteur actuelle de la carte <file>
maputil <file> --getobjects		--> retourne  le nombre d’objets inclus dans la carte <file> 
maputil <file> --getinfo		--> permet d’afficher toutes les informations de la carte <file> en une commande
maputil <file> --pruneobjects		--> permet de supprimer du fichier <file> les objets qui ne sont pas sur la carte.
maputil <file> --getpresence		--> retourne le nombre d'objets présent sur la carte <file>
maputil <file> --getfile		--> retourne les donnees de la carte <file> de façon lisible

-------------------------Modifier la taille de la carte-----------------------

maputil <file> --setwidth <w>	--> Remplace la largeur de la carte <file> par la valeur de w
maputil <file> --setheight <h>	--> Remplace la hauteur de la carte <file> par la valeur de h


------------------------Remplacer les objets d'une carte----------------------

xargs ./maputil maps/saved.map --setobjects < util/objects.txt

--> xargs reinitialise la liste des objets qui composent la carte par ceux contenus dans le fichier objects.txt



------------------------Supprimer les objets inutilisés-----------------------

maputil <file> --pruneobjects	--> Supprime du fichier <file> les objets dont aucune occurence n’apparaît sur la carte.

