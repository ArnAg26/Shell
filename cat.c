#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>

int main(int argc,char **argv){
    if(argc==1){
        printf("No file specified\n");
    }
    else if(argc>1){
        int i=0;
        int n_flag=0;
        int E_flag=0;
        int noOfFlags=0;
        while(argv[i]!=NULL){
            if(strcmp(argv[i],"-n")==0){
                n_flag=1;
                noOfFlags++;
            }
            else if(strcmp(argv[i],"-E")==0){
                E_flag=1;
                noOfFlags++;
            }
            else if(argv[i][0]=='-'){
                printf("Flag not handled\n");
                return 0;
            }
            i++;
        }
        i=1;
        while(argv[i]!=NULL){
            if(strcmp(argv[i],"-n")==0 || strcmp(argv[i],"-E")==0){
                i++;
                //printf("%s",argv[i]);
                continue;
            }
            char *path=malloc(sizeof(char)*1024);
            getcwd(path,1024);
            strcat(path,"/");
            strcat(path,argv[i]);
            struct stat st;
            stat(path,&st);
            if(S_ISREG(st.st_mode)==0){
                printf("A file of this name does not exist in current working directory\n");
                return 0;
            }
            FILE *p;
            p=fopen(path,"r");
            ssize_t read;
            size_t len=0;
            char *line=NULL;
            if(p==NULL){
                printf("%s %s",path,"Error opening file,please check for permissions\n");
                i++;
                continue;
            }
            int fd=access(path,R_OK);
            if(fd==-1){
                printf("%s %s",path,"The file is not given read permissions\n");
                i++;
                continue;
            }
            int linectr=1;
            while((read=getline(&line,&len,p))!=-1){
                if(n_flag==0 && E_flag==0){
                    printf("%s",line);
                }
                else if(n_flag==1 && E_flag==0){
                    printf("%d %s",linectr,line);
                    linectr++;
                }
                else if(E_flag==1 && n_flag==1){
                    int length=strlen(line);
                    line[length-1]=0;
                    printf("%d %s %c",linectr,line,'$');
                    printf("\n");
                    linectr++;
                }
                else if(E_flag==1 && n_flag==0){
                    int length=strlen(line);
                    line[length-1]=0;
                    printf("%s%c",line,'$');
                    printf("\n");
                }
            }
            i++;

            }
        
    }
    printf("\n");
}