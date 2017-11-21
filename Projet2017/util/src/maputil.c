#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../../include/map.h"
#include "../../include/error.h"

#define NB_OPTIONS 9

#define POS_height 0
#define POS_width 1
#define POS_objects 2
#define POS_presence 3

#define opt0 "--getinfo"
#define opt1 "--getheight"
#define opt2 "--getwidth"
#define opt3 "--getobjects"
#define opt4 "--setheight"
#define opt5 "--setwidth"
#define opt6 "--setobjects"
#define opt7 "--pruneobjects"
#define opt8 "--getpresence" 

off_t get(int fd,int* val,char* s,int depl,int act){
  lseek(fd,depl*sizeof(unsigned),SEEK_SET);
  read(fd,val,sizeof(unsigned));
  if(act)
    printf("%s : %d\n",s,*val);
  return lseek(fd,0,SEEK_CUR);
}

void set(int fd,int* val,int depl){
  lseek(fd,depl*sizeof(unsigned),SEEK_SET);
  write(fd,val,sizeof(unsigned));
}

off_t getCoords(int fd,int nb_objects){
  lseek(fd,4*sizeof(unsigned),SEEK_SET);
  size_t jmp;
  for(int i = 0; i < nb_objects; i++){
    read(fd,&jmp,sizeof(size_t));
    lseek(fd,jmp*sizeof(char)+5*sizeof(int),SEEK_CUR);
  }
  return lseek(fd,0,SEEK_CUR);
}

off_t getObjts(int fd){
  lseek(fd,4*sizeof(unsigned),SEEK_SET);
  return lseek(fd,0,SEEK_CUR);
}

void read_all(int fd){

  unsigned h,w,nb_objts,nb;
  size_t jmp;
  lseek(fd,0,SEEK_SET);
  read(fd,&h,sizeof(unsigned));
  read(fd,&w,sizeof(unsigned));
  read(fd,&nb_objts,sizeof(unsigned));
  printf("h = %d, w = %d, objects = %d\n",h,w,nb_objts);
  read(fd,&nb,sizeof(unsigned));
  for(int i = 0; i < nb_objts; i++){
    read(fd,&jmp,sizeof(size_t));
    char* object_name = (char*)malloc(sizeof(char)*jmp);
   
    for(int j = 0; j< jmp; j++){
      read(fd,&object_name[j],sizeof(char));
    }
    printf("%zu %s\n",jmp,object_name);
    lseek(fd,5*sizeof(int),SEEK_CUR);
    free(object_name);
  }
  int x,y,obj;
  for(int i = 0; i < nb; i++){
    read(fd,&x,sizeof(int));
    read(fd,&y,sizeof(int));
    read(fd,&obj,sizeof(int));
    printf("%d %d %d\n",x,y,obj);
  }
  
}

void exchange(int fd,int objts, int list1){
    
  if(list1 < objts-1 ){
    getObjts(fd);
    size_t jmp[2];
    int jmp1;
    char* object_name[2];
    int frames[2], solidity[2], destructible[2], collectible[2], generator[2];
        
    for(int i = 0; i < list1; i++){
      read(fd,&jmp1,sizeof(size_t));
      lseek(fd,jmp1*sizeof(char)+5*sizeof(int),SEEK_CUR);
    }
        
    size_t save = lseek(fd,0,SEEK_CUR);
       
    for(int k = 0; k < 2; k++){
            
      read(fd,&jmp[k],sizeof(size_t));
      object_name[k]=(char*)malloc(sizeof(char)*jmp[k]);
            
      for(int j = 0; j< jmp[k]; j++){
	read(fd,&object_name[k][j],sizeof(char));
      }
      read(fd,&frames[k],sizeof(int));
      read(fd,&solidity[k],sizeof(int));
      read(fd,&destructible[k],sizeof(int));
      read(fd,&collectible[k],sizeof(int));
      read(fd,&generator[k],sizeof(int));
    }
        
    lseek(fd,save,SEEK_SET);
        
    for(int k = 1; k >= 0; k--){
            
      write(fd,&jmp[k],sizeof(size_t));
            
      for(int j = 0; j< jmp[k]; j++){
	write(fd,&object_name[k][j],sizeof(char));
      }
      write(fd,&frames[k],sizeof(int));
      write(fd,&solidity[k],sizeof(int));
      write(fd,&destructible[k],sizeof(int));
      write(fd,&collectible[k],sizeof(int));
      write(fd,&generator[k],sizeof(int));
            
      free(object_name[k]);
    }
  }
}

int main(int argc, char* argv[]){
    
  char* error = "maputil";
    
  if(argc > 2){
        
    char* valid_options[NB_OPTIONS]={opt0,opt1,opt2,opt3,opt4,opt5,opt6,opt7,opt8};
        
    int verif = 0;
    for(int i = 0; i < NB_OPTIONS; i++){
      if(!strcmp(argv[2],valid_options[i]))
	verif = 1;
    }
        
    if(!verif){
      fprintf(stderr,"invalid option : %s\n",argv[2]);
      return EXIT_FAILURE;
    }
        
    int fd = open(argv[1],O_RDWR);
    off_t file_size = lseek(fd,0,SEEK_END);
        
    int val;
    int all_opt = 0;
    int valid = lseek(fd,0,SEEK_CUR);
    int x,y,obj;

    int current_height;
    int current_width;
    int current_objects;
    int current_presence;

    get(fd,&current_height,"",POS_height ,0);
    get(fd,&current_width,"",POS_width,0);
    get(fd,&current_objects,"",POS_objects,0);
    get(fd,&current_presence,"",POS_presence,0);
        
    if(argc == 3){
            
      if(!strcmp(argv[2],valid_options[0]))
	all_opt = 1;
            
      if(!strcmp(argv[2],valid_options[1]) || all_opt ){
	get(fd,&val,"height",POS_height,1);
      }
            
      if(!strcmp(argv[2],valid_options[2]) || all_opt ){
	get(fd,&val,"width",POS_width,1);
      }
            
      if(!strcmp(argv[2],valid_options[3]) || all_opt){
	get(fd,&val,"objects",POS_objects,1);
      }

      if(!strcmp(argv[2],valid_options[8]) || all_opt){
	get(fd,&val,"presence",POS_presence,1);
      }
            
      if((!strcmp(argv[2],valid_options[7])) ){
                
	int occObjects[current_objects][2];
                
	int cpt = 0;
	for(int i = 0; i < current_objects; occObjects[i][1]=0,occObjects[i][0]=i,i++);
                
	getCoords(fd,current_objects);
                
	for(int i = 0; i < current_presence; i++){//Recherche des objets utilisÃ©s
	  read(fd,&x,sizeof(int));
	  read(fd,&y,sizeof(int));
	  read(fd,&obj,sizeof(int));
                    
	  occObjects[obj][1] = 1;
	}
		
	for(int i = 0; i < current_objects-cpt;){
	  if(!occObjects[i][1]){//Objet inutile
	    for(int k = i; k < current_objects-1;k++){//Placement en bas de la liste d'objet
	      int tmp[2];
                            
	      exchange(fd,current_objects,k);
                            
	      tmp[1] = occObjects[k][1];
	      tmp[0] = occObjects[k][0];
                            
	      occObjects[k][1] = occObjects[k+1][1];
	      occObjects[k][0] = occObjects[k+1][0];
                            
	      occObjects[k+1][1] = tmp[1];
	      occObjects[k+1][0] = tmp[0];
	    }
	    cpt++;//Incrementation du nombre de suppression
	  }else
	    i++;
	}
	getObjts(fd);
                
	size_t jmp1;
	for(int i = 0; i < current_objects-cpt; i++){//Placement aprÃ¨s le dernier elements utilisÃ©e
	  read(fd,&jmp1,sizeof(size_t));
	  lseek(fd,jmp1*sizeof(char)+5*sizeof(int),SEEK_CUR);
	}
		
	int fd2 = open(argv[1],O_RDONLY);//Tete de lecture
	getCoords(fd2,current_objects);
                
	for(int i = 0; i < current_presence; i++){//Arrangement des indices
                    
	  read(fd2,&x,sizeof(int));
	  read(fd2,&y,sizeof(int));
	  read(fd2,&obj,sizeof(int));
                    
	  for(int k = 0; k < current_objects-cpt;k++){


	    if(k >= current_objects-cpt){
	      continue;
	    }
		      
	    if(obj==occObjects[k][0]){
	      obj = k;;
	      break;
	    }
	  }
                    
	  write(fd,&x,sizeof(int));
	  write(fd,&y,sizeof(int));
	  write(fd,&obj,sizeof(int));
                    
	}
	close(fd2);
                
	ftruncate(fd,lseek(fd,0,SEEK_CUR));//Nettoyage de la fin du fichier
	val = current_objects - cpt;
	set(fd,&val,POS_objects);
      }
            
            
    }else if(argc==4){
            
      val = atoi(argv[3]);
            
      if((!strcmp(argv[2],valid_options[4])) || (!strcmp(argv[2],valid_options[5])))
	{
                
	  val = atoi(argv[3]);
	  int old_value,depl;
	  int cpt = 0;
                
	  if(!strcmp(argv[2],valid_options[4])){
	    if(val >= 12 && val <= 20)
	      depl = POS_height;
	    else{
	      puts("La hauteur est bornée de [12;20] !");
	      close(fd);
	      return EXIT_FAILURE;
	    }
	  }else{
	    if(val >= 30 && val <= 1024)
	      depl = POS_width;
	    else{
	      puts("La largeur est bornée de [30;1024] !");
	      close(fd);
	      return EXIT_FAILURE;
	    }
	  }
                
	  get(fd,&old_value,"",depl,0);
                
	  int fd2 = open(argv[1],O_RDONLY);
	  getCoords(fd,current_objects);
	  getCoords(fd2,current_objects);

	  int act = 1;
	  int save_obj;
                
	  for(int i = 0; i < current_presence; i++){
                    
	    int cmp;
		    
                    
	    read(fd2,&x,sizeof(int));
	    read(fd2,&y,sizeof(int));
	    read(fd2,&obj,sizeof(int));

	    if(act){
	      if(( depl == POS_height && y == 0 && x == 0 ) || ( depl == POS_width && y == old_value-1 && x == 0 )){
		save_obj = obj;
		act=0;
	      }
	    }
                    
	    if( depl == POS_height ){
	      cmp = y;
	    }else{
	      cmp = x;
	    }
                    
	    int diff = (old_value-val);
	    if( old_value > val){//Retrecisseent
	      if( (cmp-diff < 0 && depl == POS_height) || ( cmp > val && depl == POS_width) ){
		cpt++;
		continue;
                            
	      }else if( depl == POS_height ){
		cmp -= diff;//dÃ©calage vers le haut : diff > 0 => cmp dim.
	      }
	    }else if( depl == POS_height ){//Agrandissement
	      cmp-=diff;//dÃ©calage vers le bas : diff < 0 => cmp aug.
	    }
                    
	    if( depl == POS_height ){
	      y =  cmp;
	    }else{
	      x = cmp;
	    }
                    
	    write(fd,&x,sizeof(int));
	    write(fd,&y,sizeof(int));
	    write(fd,&obj,sizeof(int));
                    
	  }
	  close(fd2);
	  ftruncate(fd,lseek(fd,0,SEEK_CUR));

	  if(val-old_value > 0){
	    obj = save_obj;

	    if(depl == POS_height){
	      for(int y = 0; y < val-old_value; y++){

		x = 0;
		write(fd,&x,sizeof(int));
		write(fd,&y,sizeof(int));
		write(fd,&obj,sizeof(int));

		x = current_width-1;
		write(fd,&x,sizeof(int));
		write(fd,&y,sizeof(int));
		write(fd,&obj,sizeof(int));
		      
		current_presence+=2;
	      }
	    }else{

	      for(int x = old_value; x < val; x++){

		y = current_height-1 ;
		write(fd,&x,sizeof(int));
		write(fd,&y,sizeof(int));
		write(fd,&obj,sizeof(int));

		current_presence++;
	      }
		    
	    }
		  
		  
	  }
                
	  current_presence-= cpt;
	  set(fd,&current_presence,POS_presence);
	  set(fd,&val,depl);
	}
            
      if(valid == lseek(fd,0,SEEK_CUR)){
	fprintf(stderr,"invalid option : %s\n",argv[2]);
	return EXIT_FAILURE;
      }
            
    }else if(!((argc-3)%6)){
      if((!strcmp(argv[2],valid_options[6])) ){
                
	int nb_objects = (argc-2)/6;
                
	size_t tmp_size[nb_objects];
	char* tmp_name[nb_objects];
	int tmp_prop[nb_objects][5];
	int tmp_pts[current_presence][3];
                
	int cpt=0;
                
	for(int i = 0,k=3; i < nb_objects;k+=6, i++){
	  tmp_size[i] = strlen(argv[k]);
                    
	  tmp_name[i] = (char*) malloc(sizeof(char)*(1+tmp_size[i]));
	  strcpy(tmp_name[i],argv[k]);
                    
	  tmp_prop[i][0] = atoi(argv[k+1]);
                    
	  if(!strcmp("air",argv[k+2])){
	    tmp_prop[i][1] = MAP_OBJECT_AIR;
	  }else if(!strcmp("solid",argv[k+2])){
	    tmp_prop[i][1] = MAP_OBJECT_SOLID;
	  }else if(!strcmp("semi-solid",argv[k+2])){
	    tmp_prop[i][1] = MAP_OBJECT_SEMI_SOLID;
	  }else if(!strcmp("liquid",argv[k+2])){
	    tmp_prop[i][1] = MAP_OBJECT_LIQUID;
	  }
                    
	  tmp_prop[i][2] = (!strcmp("not-destructible",argv[k+3])) ? 0 : 1;
	  tmp_prop[i][3] = (!strcmp("not-collectible",argv[k+4])) ? 0 : 1;
	  tmp_prop[i][4] = (!strcmp("not-generator",argv[k+5])) ? 0 : 1;
                    
	}

	char* old_list[current_objects];//A FREE
	getObjts(fd);
	size_t jmp;
	for(int k = 0; k <current_objects;k++){
            
	  read(fd,&jmp,sizeof(size_t));
	  old_list[k]=(char*)malloc(sizeof(char)*jmp);
            
	  for(int j = 0; j< jmp; j++){
	    read(fd,&old_list[k][j],sizeof(char));
	  }
	  lseek(fd,5*sizeof(int),SEEK_CUR);
	}


	
                
	getCoords(fd,current_objects);
                
	for(int i = 0; i < current_presence; i++){//recuperation des donnÃ©es
	  read(fd,&x,sizeof(int));
	  read(fd,&y,sizeof(int));
	  read(fd,&obj,sizeof(int));
                    
	  tmp_pts[i][0] = x;
	  tmp_pts[i][1] = y;
	  tmp_pts[i][2] = obj;
	}
                
	ftruncate(fd,getObjts(fd));//nettoyage de la fin du fichier
                
	for(int i = 0; i < nb_objects; i++){//Ã©criture de la nouvelle liste d'objets
                    
	  write(fd,&tmp_size[i],sizeof(size_t));
	  for(int j = 0; j< tmp_size[i];j++){
	    write(fd,&tmp_name[i][j],sizeof(char));
	  }
	  write(fd,&tmp_prop[i][0],sizeof(int));
	  write(fd,&tmp_prop[i][1],sizeof(int));
	  write(fd,&tmp_prop[i][2],sizeof(int));
	  write(fd,&tmp_prop[i][3],sizeof(int));
	  write(fd,&tmp_prop[i][4],sizeof(int));
	}



        cpt =0;
                
	for(int i = 0; i < current_presence; i++){//insertion de la liste des coordonnÃ©es

	  if(!strcmp(tmp_name[tmp_pts[i][2]],old_list[tmp_pts[i][2]]))
	    ;
	  else{
	    int found = 0;
	    for(int k=0; k < nb_objects; k++){
	      if( 0 == strcmp(old_list[tmp_pts[i][2]],tmp_name[k])){
		tmp_pts[i][2]=k;
		found=1;
		printf("%s %s\n",old_list[tmp_pts[i][2]],tmp_name[k]);
		break;
	      }
	    }
	    if(!found){
	      continue;

	    }
	  }
	    
	     
	  write(fd,&tmp_pts[i][0],sizeof(int));
	  write(fd,&tmp_pts[i][1],sizeof(int));
	  write(fd,&tmp_pts[i][2],sizeof(int));
	}

	current_presence-=cpt;
	set(fd,&current_presence,POS_presence);
                
	set(fd,&nb_objects,POS_objects);
      }
    }
    else{
      fprintf(stderr,"error arguments\n");
      return EXIT_FAILURE;
    }
        
    close(fd);
  }
    
  return 0;
}
