#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <inttypes.h>

int main(int argc,char **argv){
    struct dirent *d;
    int a_flag=0;
    int A_flag=0;
    DIR *dir;
    char *X=argv[1];
    if(argc==1){
        dir=opendir(".");
    }
    else if(argc>=2 && X[0]=='-'){
        dir=opendir(".");
        int i=1;
        while(i<argc){
            char *q=argv[i];
            if(q[1]=='a'){
                a_flag=1;
            }
            else if(q[1]=='A'){
                A_flag=1;
            }
            else{
                printf("Flag not handled\n");
                return 0;
            }
            i++;
        }
    }
    else{
        dir=opendir(argv[1]);
        int i=2;
        while(i<argc){
            char *q=argv[i];
            if(q[1]=='a'){
                a_flag=1;
            }
            if(q[1]=='l'){
                A_flag=1;
            }
            i++;
        }

    }   
    if(!dir){
        if(errno==ENOENT){
            perror("Directory does not exist");}
        else{
            perror("Error occured");
        }
        exit(EXIT_FAILURE);
    }
    d=readdir(dir);
    while(d!=NULL){
        char *x=d->d_name;
        if(a_flag==0 && A_flag==0 && x[0]=='.'){
            d=readdir(dir);
            continue;
        }
        if(a_flag==0 && A_flag==1 && (strcmp(x,".")==0 || strcmp(x,"..")==0)){
            d=readdir(dir);
            continue;
        }

        printf(" %s ",x);
        d=readdir(dir);
    }
    printf("\n");
}
