#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define NB_OPTIONS 8

#define opt1 "--getinfo"
#define opt2 "--getheight"
#define opt3 "--getwidth"
#define opt4 "--getobjects"
#define opt5 "--setwidth"
#define opt6 "--setheight"
#define opt7 "--setobjects"
#define opt8 "--pruneobjects"

int main(int argc, char* argv[])
{
    
    char* error = "maputil";
    
    if(argc == 3){
        
        int fd = open(argv[1],O_RDWR);
        //verification( fd == -1,error);
        
        char* valid_options[NB_OPTIONS]={opt1,opt2,opt3,opt4,opt5,opt6,opt7,opt8};
        int val;
        
        int all_opt = 0;
        if(!strcmp(argv[2],valid_options[0]))
            all_opt = 1;
        
        if(!strcmp(argv[2],valid_options[1]) || all_opt ){
            lseek(fd,0*sizeof(unsigned),SEEK_SET);
            read(fd,&val,sizeof(unsigned));
            printf("height : %d\n",val);
        }
        
        if(!strcmp(argv[2],valid_options[2]) || all_opt ){
            lseek(fd,1*sizeof(unsigned),SEEK_SET);
            read(fd,&val,sizeof(unsigned));
            printf("width : %d\n",val);
        }
        
        if(!strcmp(argv[2],valid_options[3]) || all_opt){
            lseek(fd,2*sizeof(unsigned),SEEK_SET);
            read(fd,&val,sizeof(unsigned));
            printf("objects : %d\n",val);
        }
        
        close(fd);
        
    }else{
        fprintf(stderr,"error arguments\n");
        return EXIT_FAILURE;
    }
    
    return 0;
}
