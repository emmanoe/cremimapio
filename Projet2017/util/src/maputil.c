#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

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
            int current_width,current_height,current_objects;
            int obj;

            if(!strcmp(argv[2],valid_options[4])){
                
                get(fd,&current_height,"",POS_height*sizeof(unsigned),0);
                get(fd,&current_width,"",POS_width*sizeof(unsigned),0);
                get(fd,&current_objects,"",POS_objects*sizeof(unsigned),0);
                
                if( current_height > val){
                    lseek(fd,3*sizeof(unsigned),SEEK_SET);
                    int jmp;
                    for(int i = 0; i < current_objects; i++){
                        read(fd,&jmp,sizeof(int));
                        lseek(fd,jmp*sizeof(char)+5*sizeof(int),SEEK_CUR);
                    }
                    for(int i = 0; i < current_height*current_width; i++);
                    //[À SUIVRE] 2 tête de lectures ? R/D
                    
                }else{
                    obj = MAP_OBJECT_NONE;
                    lseek(fd,0,SEEK_END);
                    for(int x = 0; x < current_width; x++){
                        for(int y = 1 + current_height; y < current_height; y++){
                            write(fd,&x,sizeof(int));
                            write(fd,&y,sizeof(int));
                            write(fd,&obj,sizeof(int));
                        }
                    }
                }
                
                set(fd,&val,POS_height*sizeof(unsigned));
            }
            
            if(!strcmp(argv[2],valid_options[5])){
                set(fd,&val,POS_width*sizeof(unsigned));
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
