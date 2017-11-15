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
                lseek(fd,POS_height*sizeof(unsigned),SEEK_SET);
                read(fd,&val,sizeof(unsigned));
                printf("height : %d\n",val);
            }
            
            if(!strcmp(argv[2],valid_options[2]) || all_opt ){
                lseek(fd,POS_width*sizeof(unsigned),SEEK_SET);
                read(fd,&val,sizeof(unsigned));
                printf("width : %d\n",val);
            }
            
            if(!strcmp(argv[2],valid_options[3]) || all_opt){
                lseek(fd,POS_objects*sizeof(unsigned),SEEK_SET);
                read(fd,&val,sizeof(unsigned));
                printf("objects : %d\n",val);
            }
            
            if(valid == lseek(fd,0,SEEK_CUR)){
                fprintf(stderr,"invalid option : %s\n",argv[2]);
                return EXIT_FAILURE;
            }
                
        }else if(argc==4){
            
            val = atoi(argv[3]);
            
            if(!strcmp(argv[2],valid_options[4]))
                lseek(fd,POS_height*sizeof(unsigned),SEEK_SET);
            
            if(!strcmp(argv[2],valid_options[5]))
                lseek(fd,POS_width*sizeof(unsigned),SEEK_SET);
            
            if(valid != lseek(fd,0,SEEK_CUR) || (!strcmp(argv[2],valid_options[4])))
                write(fd,&val,sizeof(unsigned));
            else{
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
