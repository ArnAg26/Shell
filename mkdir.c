#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc,char **argv){
    int i=1;
    int v_flag=0;
    int m_flag=0;
    int noOfFlags=0;
    int m_flagpos=-1;
    char mode;
    while(argv[i]!=NULL){
        if(strcmp(argv[i],"-v")==0){
            v_flag=1;
            noOfFlags++;
        }
        else if(argv[i][0]=='-' && argv[i][1]=='m'){
            m_flag=1;
            noOfFlags++;
            m_flagpos=i;
            mode=argv[i][3];
            }
        else if(argv[i][0]=='-'){
                printf("Flag not handled\n");
                return 0;
            }
        i++;
    }
    int noOfFiles=argc-1-noOfFlags;
    if(noOfFiles<1){
        printf("Please specify name of directory\n");
    }
    else{
        i=1;
        while(argv[i]!=NULL){
            if(strcmp(argv[i],"-v")==0 || i==m_flagpos){
                i++;
                continue;
            }
            char *path=malloc(sizeof(char)*512);
            getcwd(path,512);
            strcat(path,"/");
            strcat(path,argv[i]);
            int fd=access(path,F_OK);
            if(fd==0){
                printf("Directory already exists\n");
                return 0;
            }
            int check;
            if(m_flag==0){
                check=mkdir(path,0777);
                
            }
            else if(m_flag==1){
                if(mode=='r'){
                    check=mkdir(path,0400);
                    chmod(path,0444);
                }
                else if(mode=='w'){
                    check=mkdir(path,0200);
                    chmod(path,0222);
                }
                else if(mode=='x'){
                    check=mkdir(path,0100);
                    chmod(path,0111);
                }
                else{
                    printf("Incorrect mode type\n");
                    return 0;
                }
            }
            if(check==0 && v_flag==1){
                    printf("%s  %s","Directory created",path);
                    printf("\n");
                }
            else if(check!=0){
                printf("Directory not created.Some error.\n");
            }
            free(path);
            i++;
        }

        
    }
}