#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

#ifdef PADAWAN


static void verifier(int cond, char *msg){
   if(!cond){
      perror(msg);
      exit(1);
   }
}


void map_new (unsigned width, unsigned height)
{
  map_allocate (width, height);

  for (int x = 0; x < width; x++)
    map_set (x, height - 1, 0); // Ground

  for (int y = 0; y < height - 1; y++) {
    map_set (0, y, 1); // Wall
    map_set (width - 1, y, 1); // Wall
  }

  map_object_begin (8);

  // Texture pour le sol
  map_object_add ("images/ground.png", 1, MAP_OBJECT_SOLID);
  // Mur
  map_object_add ("images/wall.png", 1, MAP_OBJECT_SOLID);
  // Gazon
  map_object_add ("images/grass.png", 1, MAP_OBJECT_SEMI_SOLID);
  // Marbre
  map_object_add ("images/marble.png", 1, MAP_OBJECT_SOLID | MAP_OBJECT_DESTRUCTIBLE);
  // Herbe
  map_object_add ("images/herb.png", 1, MAP_OBJECT_AIR);
  // Petit plancher flottant
  map_object_add ("images/floor.png", 1, MAP_OBJECT_SEMI_SOLID);

  //Les fleurs
  map_object_add ("images/flower.png", 1, MAP_OBJECT_AIR);
  //Les pieces
  map_object_add ("images/coin.png", 20, MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE);


  map_object_end ();
}

void map_save (char *filename)
{
  int out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
  if(out !=-1){
    /* On releves les propriétés qui sont actuellment sur la carte */
    /*
      unsigned w = map_width();
      unsigned h = map_height();
      int nb_obj = map_objects();
      int test=0;
      int get_obj[(map_width()*map_height())];
      int i=0;
      char *name=NULL;
      int nb_sprit=0;
      int map_obj=0;
    */
    
    
    
    /* On ecrit sur le fichier les éléments relevés. */
      /*
      test = write(out, &w ,sizeof(w));
      verifier(test != -1, "Echec width");
      test = write(out, &h ,sizeof(h));
      verifier(test != -1, "Echec height");
      test = write(out, &nb_obj ,sizeof(h));
      verifier(test != -1, "Echec nbr_object");
      //Matrice de la map
      for(int x=0; x<w; x++){
         for(int y=0; y<h; y++){
            get_obj[i] = map_get(x,y);
            test = write(out, &get_obj[i], sizeof(get_obj[i]));
            verifier(test != -1, "Echec contains");
            i++;
         }
      }
      //Caracteristiques de chaque type d'objet
      for(int x=0; x<w; x++){
         for(int y=0; y<h; y++){
            name = map_get_name(map_get(x,y));
            nb_sprit = map_get_frames(map_get(x,y));
            switch((map_get(x,y)){
                  case :
                     break;
                     default:
                        break;


               }
         }
      }
      close(out);
      */
   }
   else{
      perror("Echec open()-> map_save");
      exit(1);
   }

   //fprintf (stderr, "Sorry: Map save is not yet implemented\n");

}

void map_load (char *filename)
{
  int in = open(filename, O_RDONLY);
  if(in !=-1){
    /* On releves les propriétés de la map */
    /*
      unsigned w, h;
      read(in, &w ,sizeof(w));
      read(in, &h ,sizeof(h));
    */
    
    /* On ecrit les éléments relevé sur la maps. */
    /*
      map_allocate (w, h);
      for(int x=0; x<w; x++){
      for(int y=0; y<h; y++){
      char c;
      test = read(in, &c, sizeof(c));
      verifier(test != -1, "");
      map_set(x,y, atoi(c));
      }
      }
      
      close(in);
    */
  }
  else{
    perror("Echec open()-> map_load");
    exit(1);
  }
  //exit_with_error ("Map load is not yet implemented\n");
}

#endif
