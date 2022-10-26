#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define delimiters " \t\r\n\a\f"

void pwd(char *arg0,char *arg1,char *arg2){
  int P_flag=0;
  int L_flag=0;
  if(arg1==NULL && arg2==NULL){
    char *path=malloc(sizeof(char *)*1024);
    char *s=getcwd(path,1024);
    printf("%s\n",s);
    return;
  }
  if((arg1!=NULL && strcmp(arg1,"-P")==0)|| (arg2!=NULL && strcmp(arg2,"-P")==0)){
      P_flag=1;
  }
  if((arg1!=NULL && strcmp(arg1,"-L")==0)|| (arg2!=NULL && strcmp(arg2,"-L")==0)){
    L_flag=1;
  }
  if(L_flag==0){
    char *path=malloc(sizeof(char *)*1024);
    char *s=getcwd(path,1024);
    printf("%s\n",s);
  }
  else{
    char *path=malloc(sizeof(char *)*1024);
    char *s=getcwd(path,1024);
    printf("%s\n",s);
  }
}

int exit1(){
  printf("Thank you for using Arnav's shell\n");
  return 0;
}

void cd(char *arg0,char *arg1,char *arg2,char *arg3){
  if(arg1==NULL || strcmp(arg1,"&t")==0){
    chdir(getenv("HOME"));
  }
  else if(strcmp(arg1,"--help")==0){
    printf("cd - Change current/working directory\n");
    printf("--help: prints information regarding cd\n");
    printf(".. to go one directory back\n");
    printf("Without any parameters cd changes environment to home directory\n");
  }
  else{
    if(chdir(arg1)!=0){
      perror("Specify a valid directory");
    }
  }
  printf("\n");
}

void echo(char *arg0,char **arg){
  int i=1;
  int n_flag=0;
  int help_flag=0;
  if(strcmp(arg[1],"-n")==0){
      n_flag=1;
  }
  else if(strcmp(arg[1],"--help")==0){
    help_flag=1;
  }
  else if(arg[1][0]=='-'){
    printf("Flag not handled\n");
    return;
  }
  if(help_flag==1){
    printf("echo - display a line of text\n-n     do not output the trailing newline\n--help display this help and exit\n");
  }
  else{
    if(n_flag==1){
      i++;
    }
    while(arg[i]!=NULL){
      printf("%s ",arg[i]);
      i++;
    }
    if(n_flag==1){
      printf("\n");
    }
    else{
      printf("\n\n");
    }
  }
}

int launch_external(char **args,char *str2)
{
  pid_t pid, wpid;
  int status;
  char str[128]="/home/arnav/Assignment1/";
  char *str3=malloc(sizeof(char)*128);
  strcpy(str3,str2);
  strcat(str3,args[0]);
  pid = fork();
  if (pid == 0) {
    if (execl(str3,args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8],NULL) == -1) {
      perror("Incorrect Command");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("Fork Error");
  } else {
    wpid=waitpid(0,NULL,0);
    free(str3);
  }

  return 1;
}

void *thread_execute(void *arg){
  char **arguments=arg;
  char *a=malloc(sizeof(char)*256);
  int i=0;
  strcat(a,arguments[i]);
  i=1;
  while(arguments[i]!=NULL){
    strcat(a,arguments[i]);
    strcat(a," ");
    i++;
  }
  system(a);
  free(a);
  pthread_exit(NULL);
  return NULL;
}

int execute(int ind,char **args,char *str)
{

  if (args[0] == NULL) {
    return 1;
  }
  else if(strcmp(args[0],"exit")==0){
    return exit1();
  }
  else if(strcmp(args[0],"cd")==0){
    cd(args[0],args[1],args[2],args[4]);
    return 1;
  }
  else if(strcmp(args[0],"pwd")==0){
    pwd(args[0],args[1],args[2]);
    return 1;
  }
  else if(strcmp(args[0],"echo")==0){
    echo(args[0],args);
    return 1;
  }
  else if(ind>1 && strcmp(args[ind-1],"&t")==0){
      int i=0;
      char **a=malloc(sizeof(char *)*256);
      a[i]=str;
      while(strcmp(args[i],"&t")!=0){
           a[i+1]=args[i];
           i++;
      }
      pthread_t thread;
      if(pthread_create(&thread,NULL,&thread_execute,a)!=0){
        perror("Error creating thread");
        return 0;
      }
      pthread_join(thread,NULL);
      return 1;

  }
  else{
      return launch_external(args,str);
  }
  
}

void loop_until_interrupt(char *str)
{
  char *cmd;
  char **args;
  int status=1;

  while(status!=0){
    printf(">> ");
    size_t buffer=0;
    int d=getline(&cmd,&buffer,stdin);
    if(d==-1 && feof(stdin)){
        perror("Interrupt ");
        exit(EXIT_SUCCESS);
    }
    else if(d==-1){
        perror("You ran into an error");
        exit(EXIT_FAILURE);
    }

    int index=0;
    int argno=0;
    char **arguments;
    arguments=malloc(sizeof(char *)*256);
    arguments[index]=strtok(cmd,delimiters);
    while(arguments[index]!=NULL){
        index+=1;
        arguments[index]=strtok(NULL,delimiters);
        argno++;
    }
    status=execute(argno,arguments,str);
    free(cmd);
    free(arguments);
  }
}

int main(int argc,char **argv){
    printf("Welcome to Arnav's shell\n");
    char str2[128];
    getcwd(str2,128);
    strcat(str2,"/");
    loop_until_interrupt(str2);
    return EXIT_SUCCESS;
}