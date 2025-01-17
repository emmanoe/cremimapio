#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
    // Pieces Ãƒ  collecter
    map_object_add ("images/coin.png", 20, MAP_OBJECT_AIR | MAP_OBJECT_COLLECTIBLE);
    
    
    map_object_end ();
}

void verification(int condition,char* error){
    
    if(condition){
        perror(error);
        exit(EXIT_FAILURE);
    }
}

void fun(int bool,char* error,int fd, void* buf, void* buf2, void* buf3, void* buf4, void* buf5){
    
    if (bool){ // write
		if (buf!=NULL)
    		assert((write(fd,buf,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf2!=NULL)
      		assert((write(fd,buf2,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf3!=NULL)
      		assert((write(fd,buf3,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf4!=NULL)
      		assert((write(fd,buf4,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf5!=NULL)
      		assert((write(fd,buf5,sizeof(unsigned)))==sizeof(unsigned));
      	}
    else{ // read
    	if (buf!=NULL)
    		assert((read(fd,buf,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf2!=NULL)
      		assert((read(fd,buf2,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf3!=NULL)
      		assert((read(fd,buf3,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf4!=NULL)
      		assert((read(fd,buf4,sizeof(unsigned)))==sizeof(unsigned));
      	if (buf5!=NULL)
      		assert((read(fd,buf5,sizeof(unsigned)))==sizeof(unsigned));
      }
    }

void map_save (char *filename)
{
    char* error = "map_save";
    
    int fd = open(filename,O_WRONLY|O_CREAT|O_TRUNC,0666);
    verification(fd==-1,error);
    
    unsigned h = map_height();
    unsigned w = map_width();
    unsigned nb_objects = map_objects();
    unsigned nb_presence = 0;
    
    fun(1,"error",fd,&h,&w,&nb_objects,&nb_presence,NULL);
    
    for(int obj = 0; obj < nb_objects; obj++){
        
        size_t len_name = strlen(map_get_name(obj));
        char* name = map_get_name(obj);
        int frames = map_get_frames(obj);
        int solidity = map_get_solidity(obj);
        int destructible = map_is_destructible(obj);
        int collectible = map_is_collectible(obj);
        int generator = map_is_generator(obj);
        
        verification(write(fd,&len_name,sizeof(size_t))!=sizeof(size_t),error); // ERROR !!
        for(int i = 0; i< len_name;i++){
            verification(write(fd,&name[i],sizeof(char))!=sizeof(char),error);
        }
        fun(1,"error",fd,&frames,&solidity,&destructible,&collectible,&generator);
    }
    
    for(int x = 0; x < w; x++){
        for(int y = 0; y < h ; y++){
            
            int obj = map_get(x,y);
            
            if( obj != MAP_OBJECT_NONE){
                fun(1,"error",fd,&x,&y,&obj,NULL,NULL);
                nb_presence++;
            }
        }
    }
    lseek(fd,3*sizeof(unsigned),SEEK_SET);
    fun(1,"error",fd,&nb_presence,NULL,NULL,NULL,NULL);
    
    printf("Saved in %s\n",filename);
    
    close(fd);
}

void map_load (char *filename)
{
    char* error = "map_laod";
    unsigned h, w, nb_objects,nb_presence;
    int frames, solidity, destructible, collectible, generator;
    //char* object_name;
    size_t len_name;
    
    int fd = open(filename,O_RDONLY);
    verification(fd==-1,error);
    
    fun(0,"error",fd,&h,&w,&nb_objects,&nb_presence,NULL);
    
    map_allocate (w, h);
    map_object_begin (nb_objects);
    
    for (int i=0; i< nb_objects; i++){
        
        verification(read(fd,&len_name,sizeof(size_t))==-1,error);
        
        char object_name[len_name+1];
        
        for(int j = 0; j< len_name; j++){
            verification(read(fd,&object_name[j],sizeof(char))==-1,error);
        }
        fun(0,"error",fd,&frames,&solidity,&destructible,&collectible,&generator);
        
        int status = solidity;
        if(destructible){
            status |= MAP_OBJECT_DESTRUCTIBLE;
        }
        if(collectible){
            status |= MAP_OBJECT_COLLECTIBLE;
        }
        if(generator){
            status |= MAP_OBJECT_GENERATOR;
        }
        map_object_add (object_name, frames, status);
        
    }
    
    map_object_end ();
    for(int i =0;i < nb_presence;i++){
        
        int x, y, obj;
        
        fun(0,"error",fd,&x,&y,&obj,NULL,NULL);
        
        int cond = ( obj >= MAP_OBJECT_NONE && obj < (int)nb_objects);
        verification(!cond, error);
        if((x >=0 && x < w) && ( y >= 0 && y < h))
            map_set (x, y, obj);
        
    }
    
    printf("Loaded since %s\n",filename);
    
    close(fd);
    
}

#endif
