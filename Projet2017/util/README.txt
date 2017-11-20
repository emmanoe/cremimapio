=======
README
=======

Le programme maputil est capable de consulter, modifier, remplacer les informations d'une carte stockées dans un fichier <file>.


--------------------------Consulter les informations--------------------------

maputil <file> --getwidth		--> retourne la largeur actuelle de la carte <file>
maputil <file> --getheight		--> retourne la hauteur actuelle de la carte <file>
maputil <file> --getobjects		--> retourne  le nombre d’objets contenus dans la carte <file> 
maputil <file> --getinfo		--> permet d’afficher toutes les informations de la carte <file> en une commande


-------------------------Modifier la taille de la carte-----------------------

maputil <file> --setwidth <w>	--> Remplace la largeur de la carte <file> par la valeur de w
maputil <file> --setheight <h>	--> Remplace la hauteur de la carte <file> par la valeur de h


------------------------Remplacer les objets d'une carte----------------------

xargs ./util/maputil maps/saved.map --setobjects < util/objects.txt

--> xargs reinitialise la liste des objets qui composent la carte par ceux contenus dans le fichier objects.txt



------------------------Supprimer les objets inutilisés-----------------------

maputil <file> --pruneobjects	--> Supprime du fichier <file> les objets dont aucune occurence n’apparaît sur la carte.

