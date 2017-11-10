#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "map.h"
#include "error.h"

#ifdef PADAWAN

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
    // Fleur paysage
    map_object_add ("images/flower.png", 1, MAP_OBJECT_AIR);
    // Pieces à collecter
    map_object_add ("images/coin.png", 20, MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE);
    
    
    map_object_end ();
}

void verification(int condition,char* error){
    
    if(condition){
        perror(error);
        exit(EXIT_FAILURE);
    }
}

void map_save (char *filename)
{
    char* error = "map_save";
    
    FILE* fd = fopen(filename,"w");
    verification(fd==NULL,error);
    
    unsigned h = map_height();
    unsigned w = map_width();
    unsigned nb_objects = map_objects();
    
    fprintf(fd,"%d %d %d\n",h,w,nb_objects);

    for(int obj = 0; obj < nb_objects; obj++){
        fprintf(fd,"%zu %s %d %d ",strlen(map_get_name(obj)),map_get_name(obj),map_get_frames(obj),map_get_solidity(obj));
        fprintf(fd,"%d %d %d\n",map_is_destructible(obj),map_is_collectible(obj),map_is_generator(obj));
    }
    
    for(int x = 0; x < w; x++){
        for(int y = 0; y < h ; y++){
            fprintf(fd,"%d %d %d\n", x, y,map_get(x,y));
        }
    }
    printf("Saved in %s\n",filename);
    
    fclose(fd);
}

void map_load (char *filename)
{
    printf("Loaded since %s\n",filename);
}

#endif
