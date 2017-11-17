#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#include "../../include/map.h"
#include "../../include/error.h"

#define NB_OPTIONS 8

#define POS_height 0
#define POS_width 1
#define POS_objects 2

#define opt0 "--getinfo"
#define opt1 "--getheight"
#define opt2 "--getwidth"
#define opt3 "--getobjects"
#define opt4 "--setheight"
#define opt5 "--setwidth"
#define opt6 "--setobjects"
#define opt7 "--pruneobjects"

void get(int fd,int* val,char* s,int depl,int act){
    lseek(fd,depl,SEEK_SET);
    read(fd,val,sizeof(unsigned));
    if(act)
        printf("%s : %d\n",s,*val);
}

void set(int fd,int* val,int depl){
    lseek(fd,depl,SEEK_SET);
    write(fd,val,sizeof(unsigned));
}

void getCoords(int fd,int nb_objects){
    lseek(fd,3*sizeof(unsigned),SEEK_SET);
    size_t jmp;
    for(int i = 0; i < nb_objects; i++){
        read(fd,&jmp,sizeof(size_t));
        lseek(fd,jmp*sizeof(char)+5*sizeof(int),SEEK_CUR);
    }
}

int main(int argc, char* argv[])
{
    
    char* error = "maputil";
    
    if(argc > 2){
        
        int fd = open(argv[1],O_RDWR);
        //verification( fd == -1,error);
        
        char* valid_options[NB_OPTIONS]={opt0,opt1,opt2,opt3,opt4,opt5,opt6,opt7};
        int val;
        int all_opt = 0;
        int valid = lseek(fd,0,SEEK_CUR);
        
        if(argc == 3){
            
            if(!strcmp(argv[2],valid_options[0]))
                all_opt = 1;
            
            if(!strcmp(argv[2],valid_options[1]) || all_opt ){
                get(fd,&val,"height",POS_height*sizeof(unsigned),1);
            }
            
            if(!strcmp(argv[2],valid_options[2]) || all_opt ){
                get(fd,&val,"width",POS_width*sizeof(unsigned),1);
            }
            
            if(!strcmp(argv[2],valid_options[3]) || all_opt){
                get(fd,&val,"objects",POS_objects*sizeof(unsigned),1);
            }
            
            if(valid == lseek(fd,0,SEEK_CUR)){
                fprintf(stderr,"invalid option : %s\n",argv[2]);
                return EXIT_FAILURE;
            }
            
        }else if(argc==4){
            
            val = atoi(argv[3]);
            int current_objects;
            int ret_1,ret_2,ret_3;
            int obj;
            off_t file_size = lseek(fd,0,SEEK_END);
            
            get(fd,&current_objects,"",POS_objects*sizeof(unsigned),0);
            
            if((!strcmp(argv[2],valid_options[4])) || (!strcmp(argv[2],valid_options[5])))
            {
                
                val = atoi(argv[3]);
                int old_value,depl;
                int ret_1,ret_2,ret_3;
                int obj;
                off_t file_size = lseek(fd,0,SEEK_END);
                
                if(!strcmp(argv[2],valid_options[4])){
                    depl = POS_height;
                }else{
                    depl = POS_width;
                }
                
                get(fd,&old_value,"",depl*sizeof(unsigned),0);
                
                getCoords(fd,current_objects);
                
                while(file_size > lseek(fd,0,SEEK_CUR)){
                    
                    int x,y,obj;
                    int supp = -1;
                    int cmp;
                    
                    read(fd,&x,sizeof(int));
                    read(fd,&y,sizeof(int));
                    read(fd,&obj,sizeof(int));
                    
                    if( depl == POS_height ){
                        cmp = y;
                    }else{
                        cmp = x;
                    }
                    
                    lseek(fd,-3*sizeof(int),SEEK_CUR);
                    
                    int diff = (old_value-val);
                    if( old_value > val){//Retrecisseent
                        if( cmp-diff < 0 ){
                            
                            x = supp;
                            y = supp;
                            
                        }else if( depl == POS_height ){
                            cmp -= diff;//décalage vers le haut : diff > 0
                        }
                    }else if( depl == POS_height ){//Agrandissement
                            cmp-=diff;//décalage vers le haut : diff < 0
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
                
                set(fd,&val,depl*sizeof(unsigned));
            }
            
            if((!strcmp(argv[2],valid_options[7])) ){
                
                int occObjects[current_objects];
                int x,y,obj;
                for(int i = 0; i < current_objects; occObjects[i]=0,i++);
                int cpt = 0;
                
                getCoords(fd,current_objects);
                
                while(file_size > lseek(fd,0,SEEK_CUR)){
                    read(fd,&x,sizeof(int));
                    read(fd,&y,sizeof(int));
                    read(fd,&obj,sizeof(int));
                    
                    occObjects[obj] = 1;
                }
                
                for(int i = 0; i < current_objects; i++){
                    if(!occObjects[i]){
                        /*WARNING :
                            - changements des identifiant en cas de suppressions
                            - pas de troue entre les données
                         */
                    }else{
                        cpt++;
                    }
                }
                
                set(fd,&cpt,POS_objects*sizeof(unsigned));
            }
            
            if((!strcmp(argv[2],valid_options[6])) ){
                //TODO
            }
            
            if(valid == lseek(fd,0,SEEK_CUR)){
                fprintf(stderr,"invalid option : %s\n",argv[2]);
                return EXIT_FAILURE;
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
