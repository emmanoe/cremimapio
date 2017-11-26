#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <assert.h>

#include "../../include/map.h"
#include "../../include/error.h"

#define NB_OPTIONS 10

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
#define opt9 "--getfile"

void verif_ES(int fd,void *buf,size_t size,int isRead){
    if(isRead){
        assert((read(fd,buf,size))==size);
    }else{
        assert((write(fd,buf,size))==size);
    }
}

off_t get(int fd,int* val,char* s,int depl,int act){
    lseek(fd,depl*sizeof(unsigned),SEEK_SET);
    verif_ES(fd,val,sizeof(unsigned),1);
    if(act)
        printf("%s : %d\n",s,*val);
    return lseek(fd,0,SEEK_CUR);
}

void set(int fd,int* val,int depl){
    lseek(fd,depl*sizeof(unsigned),SEEK_SET);
    verif_ES(fd,val,sizeof(unsigned),0);
}

off_t getCoords(int fd,int nb_objects){
    lseek(fd,4*sizeof(unsigned),SEEK_SET);
    size_t jmp;
    for(int i = 0; i < nb_objects; i++){
        verif_ES(fd,&jmp,sizeof(size_t),1);
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
    verif_ES(fd,&h,sizeof(unsigned),1);
    verif_ES(fd,&w,sizeof(unsigned),1);
    verif_ES(fd,&nb_objts,sizeof(unsigned),1);
    verif_ES(fd,&nb,sizeof(unsigned),1);
    printf("h = %d, w = %d, objects = %d,nb = %d\n",h,w,nb_objts,nb);
    for(int i = 0; i < nb_objts; i++){
        verif_ES(fd,&jmp,sizeof(size_t),1);
        char* object_name = (char*)malloc(sizeof(char)*(jmp+1));
        
        for(int j = 0; j< jmp; j++){
            verif_ES(fd,&object_name[j],sizeof(char),1);
        }
        object_name[jmp]='\0';
        printf("%zu %s",jmp,object_name);
        free(object_name);
        for(int j = 0,prop; j< 5; j++){
            verif_ES(fd,&prop,sizeof(int),1);
            printf(" %d",prop);
        }
        puts("");
        
    }
    
    for(int i = 0,x,y,obj; i < nb; i++){
        verif_ES(fd,&x,sizeof(int),1);
        verif_ES(fd,&y,sizeof(int),1);
        verif_ES(fd,&obj,sizeof(int),1);
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
            verif_ES(fd,&jmp1,sizeof(size_t),1);
            lseek(fd,jmp1*sizeof(char)+5*sizeof(int),SEEK_CUR);
        }
        
        size_t save = lseek(fd,0,SEEK_CUR);
        
        for(int k = 0; k < 2; k++){
            
            verif_ES(fd,&jmp[k],sizeof(size_t),1);
            object_name[k]=(char*)malloc(sizeof(char)*(jmp[k]+1));
            
            for(int j = 0; j< jmp[k]; j++){
                verif_ES(fd,&object_name[k][j],sizeof(char),1);
            }
            object_name[k][jmp[k]]='\0';
            verif_ES(fd,&frames[k],sizeof(int),1);
            verif_ES(fd,&solidity[k],sizeof(int),1);
            verif_ES(fd,&destructible[k],sizeof(int),1);
            verif_ES(fd,&collectible[k],sizeof(int),1);
            verif_ES(fd,&generator[k],sizeof(int),1);
        }
        
        lseek(fd,save,SEEK_SET);
        
        for(int k = 1; k >= 0; k--){
            
            verif_ES(fd,&jmp[k],sizeof(size_t),0);
            
            for(int j = 0; j< jmp[k]; j++){
                verif_ES(fd,&object_name[k][j],sizeof(char),0);
            }
            verif_ES(fd,&frames[k],sizeof(int),0);
            verif_ES(fd,&solidity[k],sizeof(int),0);
            verif_ES(fd,&destructible[k],sizeof(int),0);
            verif_ES(fd,&collectible[k],sizeof(int),0);
            verif_ES(fd,&generator[k],sizeof(int),0);
            
            free(object_name[k]);
        }
    }
}

int main(int argc, char* argv[]){
    
    char* error = "maputil";
    
    if(argc > 2){
        
        char* valid_options[NB_OPTIONS]={opt0,opt1,opt2,opt3,opt4,opt5,opt6,opt7,opt8,opt9};
        
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
            
            if(!strcmp(argv[2],valid_options[9])){
                read_all(fd);
            }
            
            if((!strcmp(argv[2],valid_options[7])) ){
                
                int occObjects[current_objects][2];
                
                int cpt = 0;
                for(int i = 0; i < current_objects; occObjects[i][1]=0,occObjects[i][0]=i,i++);
                
                getCoords(fd,current_objects);
                
                for(int i = 0; i < current_presence; i++){//Recherche des objets utilisÃƒÂ©s
                    verif_ES(fd,&x,sizeof(int),1);
                    verif_ES(fd,&y,sizeof(int),1);
                    verif_ES(fd,&obj,sizeof(int),1);
                    
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
                for(int i = 0; i < current_objects-cpt; i++){//Placement aprÃƒÂ¨s le dernier elements utilisÃƒÂ©e
                    verif_ES(fd,&jmp1,sizeof(size_t),1);
                    lseek(fd,jmp1*sizeof(char)+5*sizeof(int),SEEK_CUR);
                }
                
                int fd2 = open(argv[1],O_RDONLY);//Tete de lecture
                getCoords(fd2,current_objects);
                
                for(int i = 0; i < current_presence; i++){//Arrangement des indices
                    
                    verif_ES(fd2,&x,sizeof(int),1);
                    verif_ES(fd2,&y,sizeof(int),1);
                    verif_ES(fd2,&obj,sizeof(int),1);
                    
                    for(int k = 0; k < current_objects-cpt;k++){
                        
                        
                        if(k >= current_objects-cpt){
                            continue;
                        }
                        
                        if(obj==occObjects[k][0]){
                            obj = k;;
                            break;
                        }
                    }
                    
                    verif_ES(fd,&x,sizeof(int),0);
                    verif_ES(fd,&y,sizeof(int),0);
                    verif_ES(fd,&obj,sizeof(int),0);
                    
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
                int old_value,depl,cpt = 0;
                
                if(!strcmp(argv[2],valid_options[4])){
                    if(val >= 12 && val <= 20)
                        depl = POS_height;
                    else{
                        puts("La hauteur est bornÃ©e de [12;20] !");
                        close(fd);
                        return EXIT_FAILURE;
                    }
                }else{
                    if(val >= 30 && val <= 1024)
                        depl = POS_width;
                    else{
                        puts("La largeur est bornÃ©e de [30;1024] !");
                        close(fd);
                        return EXIT_FAILURE;
                    }
                }
                
                get(fd,&old_value,"",depl,0);
                
                int fd2 = open(argv[1],O_RDONLY);
                getCoords(fd,current_objects);
                getCoords(fd2,current_objects);
                
                int act = 1,save_obj;
                
                for(int i = 0; i < current_presence; i++){
                    
                    int cmp;
                    verif_ES(fd2,&x,sizeof(int),1);
                    verif_ES(fd2,&y,sizeof(int),1);
                    verif_ES(fd2,&obj,sizeof(int),1);
                    
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
                            cmp -= diff;//dÃƒÂ©calage vers le haut : diff > 0 => cmp dim.
                        }
                    }else if( depl == POS_height ){//Agrandissement
                        cmp-=diff;//dÃƒÂ©calage vers le bas : diff < 0 => cmp aug.
                    }
                    
                    if( depl == POS_height ){
                        y =  cmp;
                    }else{
                        x = cmp;
                    }
                    
                    verif_ES(fd,&x,sizeof(int),0);
                    verif_ES(fd,&y,sizeof(int),0);
                    verif_ES(fd,&obj,sizeof(int),0);
                    
                }
                close(fd2);
                ftruncate(fd,lseek(fd,0,SEEK_CUR));
                
                if(val-old_value > 0){
                    obj = save_obj;
                    
                    if(depl == POS_height){
                        for(int y = 0; y < val-old_value; y++){
                            
                            x = 0;
                            verif_ES(fd,&x,sizeof(int),0);
                            verif_ES(fd,&y,sizeof(int),0);
                            verif_ES(fd,&obj,sizeof(int),0);
                            
                            x = current_width-1;
                            verif_ES(fd,&x,sizeof(int),0);
                            verif_ES(fd,&y,sizeof(int),0);
                            verif_ES(fd,&obj,sizeof(int),0);
                            
                            current_presence+=2;
                        }
                    }else{
                        
                        for(int x = old_value; x < val; x++){
                            
                            y = current_height-1 ;
                            verif_ES(fd,&x,sizeof(int),0);
                            verif_ES(fd,&y,sizeof(int),0);
                            verif_ES(fd,&obj,sizeof(int),0);
                            
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
                    
                    verif_ES(fd,&jmp,sizeof(size_t),1);
                    old_list[k]=(char*)malloc(sizeof(char)*(jmp+1));
                    
                    for(int j = 0; j< jmp; j++){
                        verif_ES(fd,&old_list[k][j],sizeof(char),1);
                    }
                    old_list[k][jmp]='\0';
                    lseek(fd,5*sizeof(int),SEEK_CUR);
                }
                
                getCoords(fd,current_objects);
                
                for(int i = 0; i < current_presence; i++){//recuperation des donnÃƒÂ©es
                    for(int k = 0; k < 3; k++)
                        verif_ES(fd,&tmp_pts[i][k],sizeof(int),1);
                }
                
                ftruncate(fd,getObjts(fd));//nettoyage de la fin du fichier
                
                for(int i = 0; i < nb_objects; i++){//ÃƒÂ©criture de la nouvelle liste d'objets
                    
                    verif_ES(fd,&tmp_size[i],sizeof(size_t),0);
                    for(int j = 0; j< tmp_size[i];j++){
                        verif_ES(fd,&tmp_name[i][j],sizeof(char),0);
                    }
                    for(int k = 0; k < 5; k++)
                        verif_ES(fd,&tmp_prop[i][k],sizeof(int),0);
                }
                
                
                
                cpt =0;
                int new_id[current_objects];
                for(int i = 0; i < current_objects; new_id[i]=-1, i++);
                
                for(int i = 0; i < current_objects; i++){
                    for(int k = 0; k < nb_objects; k++){
                        if(!strcmp(tmp_name[k], old_list[i])){
                            new_id[i]=k;
                            break;
                        }
                    }
                }
                
                for(int i = 0; i < current_objects; i++){
                    free(old_list[i]);
                }
                
                for(int k = 0; k < nb_objects; k++){
                    free(tmp_name[k]);
                }
                
                for(int i = 0; i < current_presence; i++){//insertion de la liste des coordonnÃƒÂ©es
                    
                    if(new_id[tmp_pts[i][2]]==-1){
                        cpt++;
                        continue;
                    }else{
                        tmp_pts[i][2] =new_id[tmp_pts[i][2]];
                    }
                    for(int k = 0; k < 3; k++)
                        verif_ES(fd,&tmp_pts[i][k],sizeof(int),0);
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
