#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>

int isDirectoryEmpty(char *dirname) {
  int n = 0;
  struct dirent *d;
  DIR *dir = opendir(dirname);
  while ((d = readdir(dir)) != NULL) {
    n++;
    if(n>2){
        break;
    }
  }
  closedir(dir);
  if (n <= 2)
    return 1;
  else
    return 0;
}

int main(int  argc,char **argv){
    
    int f_flag=0;
    int d_flag=0;
    int i=1;
    int noOfFlags=0;
    while(argv[i]!=NULL){
        if(strcmp(argv[i],"-f")==0){
            f_flag=1;
            noOfFlags+=1;
        }
        else if(strcmp(argv[i],"-d")==0){
            d_flag=1;
            noOfFlags+=1;
        }
        else if(argv[i][0]=='-'){
            printf("Flag not handled\n");
            return 0;
        }
        i++;
    }
    int noOfFiles=argc-1-noOfFlags;
    if(noOfFiles==0){
        printf("Please specify name of file\n");
        return 0;
    }
    else{
        int i=1;
        while(i<argc){
            if(strcmp(argv[i],"-f")==0 || strcmp(argv[i],"-d")==0){
                i++;
                continue;
            }
            char *path=malloc(sizeof(char)*512);
            getcwd(path,512);
            strcat(path,"/");
            strcat(path,argv[i]);
            int fd=access(path,F_OK);
            if(fd==-1 && f_flag==0){
                printf("%s\n",path);
                printf("File does not exist\n");
                return 0;
            }
            if(fd==-1 && f_flag==1){
                i++;
                continue;
            }   
            struct stat st;
            stat(path,&st);
            if(S_ISREG(st.st_mode)==0 && d_flag==0){
                printf("%s\n",path);
                printf("The given file is directory\n");
                return 0;
            }
            else if(S_ISREG(st.st_mode)==0 && d_flag==1){
                int x=isDirectoryEmpty(path);
                if(x==1){
                    int rm=remove(path);
                    if(rm==0){
                        printf("%s\n",path);
                        printf("The given directory has been removed\n");
                    }
                }
                else{
                    printf("%s\n",path);
                    printf("Cannot remove.Directory not empty\n");
                }
            }
            int rm=remove(path);
            if(rm==0){
                printf("%s\n",path);
                printf("The given file has been removed\n");
            }
        free(path);

        i++;
    }
}
}
