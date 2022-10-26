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
        time_t t=time(NULL);
        struct tm *tm;
        tm=localtime(&t);
        printf("%s",ctime(&t));
    }
    else{
        int i=1;
        int u_flag=0;
        int r_flag=0;
        char *x=malloc(sizeof(char)+256);
        while(argv[i]!=NULL){
            if(strcmp(argv[i],"-u")==0){
                u_flag=1;
            }
            else if(argv[i][0]=='-' && argv[i][1]=='r'){
                r_flag=1;
                if(argv[i][2]=='='){
                    printf("No need to add = sign\n");
                    return 0;
                }
                int j=2;
                while(argv[i][j]!='\0'){
                    x[j-2]=argv[i][j];
                    j++;
                }
            }
            else if(argv[i][0]='-'){
                printf("Flag not handled\n");
                return 0;
            }
            i++;
        }
            if(u_flag==1){
                time_t utc;
		        struct tm *tm;
		        time(&utc);
		        tm = gmtime(&utc);
	            printf("%s", asctime(tm));
            }
            else if(r_flag==1){
                int fd=access(x,F_OK);
                if(fd==-1){
                    printf("%s\n",x);
                    printf("No such file or directory\n");
                }
                else{
                struct stat f;
                stat(x,&f);
                printf("%s\n",ctime(&f.st_mtime));
                }
            }
            free(x);
        }
 }