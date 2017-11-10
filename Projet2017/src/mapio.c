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

int len_INT(int i){

  int cpt = 0;
  ( i < 0 ) ? ( i*(-1) & cpt++) : i;//WARNING
  do{
    cpt++;
  }while( (i/=10) > 0);

  return cpt;

}

void verification(int condition,char* error){
  
  if(condition){
    perror(error);
    exit(EXIT_FAILURE);
  }
  
}

void add_list(char** list_objts,int current_size,char* obj_name){

  char* error = "add_list";
  
  list_objts[current_size] = (char*)malloc(sizeof(char)*strlen(obj_name));
  verification(list_objts[current_size]==NULL,error);
  strcpy(list_objts[current_size],obj_name);
  
}

void add_tab(char** tab_objts,int current_size,int x,int y,int obj){

  char* error = "add_tab";
  char obj_name[len_INT(x)+len_INT(y)+len_INT(obj)+4];
  
  sprintf(obj_name,"%d %d %d\n",x,y,obj);
  tab_objts[current_size] = (char*)malloc(sizeof(char)*(len_INT(x)+len_INT(y)+len_INT(obj)+2));
  verification(tab_objts[current_size]==NULL,error);
  strcpy(tab_objts[current_size],obj_name);
  
}

int isInList(char** list_objts,char* obj_name){

  int i = 0;
  while(list_objts[i] != NULL){
    if(!strcmp(list_objts[i],obj_name) ){
      return 1;
    }
    i++;
  }
  return 0;
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

  int list_len = 0;
  int tab_len = 0;
  
  char** list_objects = (char**) malloc(nb_objects*sizeof(char*));
  verification(list_objects ==NULL,error);
  
  char** tab_objects = (char**) malloc((h*w)*sizeof(char*));
  verification(tab_objects ==NULL,error);
  
  for(int i = 0; i < nb_objects; i++){
    list_objects[i]=NULL;
  }

  for(int i = 0; i < h*w; i++){
    tab_objects[i]=NULL;
  }
  

  for(int x = 0; x < w; x++){
    for(int y = 0; y < h ; y++,tab_len++){
      int obj = map_get(x,y);
      add_tab(tab_objects,tab_len,x,y,obj);
      if( list_len < nb_objects && obj != MAP_OBJECT_NONE && !isInList(list_objects,map_get_name(obj))){
	add_list(list_objects,list_len,map_get_name(obj));
	fprintf(fd,"%zu %s %d %d ",strlen(list_objects[list_len]),list_objects[list_len],map_get_frames(obj),map_get_solidity(obj));
	fprintf(fd,"%d %d %d\n",map_is_destructible(obj),map_is_collectible(obj),map_is_generator(obj));
	list_len++;
      }
    }
  }

  for(int i = 0; i < tab_len; i++)//OPTIMISATION : tab write before list; load => lseek jump h*w; ?
    fprintf(fd,"%s", tab_objects[i]);

  for(int i=0; i < h*w; i++){
    if(i < nb_objects){
      free(list_objects[i]);
    }
    free(tab_objects[i]);
  }

  free(list_objects);
  free(tab_objects);

  printf("Saved in %s\n",filename);
  
  fclose(fd);
}

void map_load (char *filename)
{
  printf("Loaded since %s\n",filename);
}

#endif
