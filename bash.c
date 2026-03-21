#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fork,execvp
#include <sys/wait.h> // wait
#include <fcntl.h> //open(),O_WRONLY flags etc
// #include <dirent.h>


#define MAX_ARGS 1024
#ifndef MAX_INPUT
#define MAX_INPUT 1024
#endif


void free_args(char **argv, int argc) {
    for (int i = 0; i < argc; i++)
        free(argv[i]);
    free(argv);
}

int makearg(char s[],char ***args){
    // if problem return -1 else return 1

    if(s==NULL){
        return -1;
    }   


    // char **argv = *args; wrong sequence 
    //  argv =(char**)malloc(1024 *sizeof(char*));

    char **argv =(char**)malloc(MAX_ARGS *sizeof(char*));
    if(!argv) return -1;
    *args= argv;
    char *buffer = (char *)malloc(MAX_INPUT *sizeof(char));

    int index = 0 ; //used during parse
    int aindex =0 ; // used for argv index

    for(int i=0 ; s[i]!='\0';i++){
     if(s[i] == ' ' || s[i] == '\t'){
        if(index > 0){
            buffer[index] = '\0';
            argv[aindex] =(char*)malloc(sizeof(char)*(index+1));
            memcpy(argv[aindex],buffer,index+1);
           index = 0;
            aindex++;

          if (aindex >= MAX_ARGS - 1) break;  // leave room for NULL

        }
     }else{
        buffer[index++] = s[i];
     }

   }

   if(index > 0 && aindex < MAX_ARGS -1){
        buffer[index] = '\0';
        argv[aindex] = (char *)malloc(index + 1);
        memcpy(argv[aindex],buffer,index+1);
        aindex++;
    }

 argv[aindex] = NULL;
free(buffer);
 return aindex;
}

void redirection_handler(char **args){ 
    // here we pass only pointer to pointer because we are only changing 
    // the content of args instead of where args as a whole points to.

    for(int i =0 ; args[i]!=NULL;i++){
        //args[i] == "<" in C u cannot directly compare sring it compares meomry addresses
        if(strcmp(args[i], "<") == 0){ // for the input 
            if(args[i+1] == NULL){
                fprintf(stderr,"Error, Missing the input direction\n");
                exit(1);
            }
            int fd = open(args[i+1],O_RDONLY);
            if(fd < 0){
                 perror(args[i+1]);
                 exit(1);
            }
            for(;args[i]!=NULL;i++){
                args[i]=NULL;
            }
            dup2(fd,0);
            close(fd);

            break;
        }else if(strcmp(args[i], ">") == 0){ //for overwritng
             if(args[i+1] == NULL){
                printf("%s","Error, Missing the output direction\n");
                exit(1);
            }
            int fd = open(args[i+1],O_WRONLY|O_CREAT|O_TRUNC,0644);
            if(fd < 0){
                 perror(args[i+1]);
                  exit(1);
            }
             for(;args[i]!=NULL;i++){
                args[i]=NULL;
            }
            dup2(fd,1);
            close(fd);


            break;
        }else if(strcmp(args[i], ">>") == 0){ // for append
             if(args[i+1] == NULL){
                printf("%s","Error, Missing the output direction\n");
                exit(1);
            }
            int fd = open(args[i+1],O_WRONLY|O_CREAT|O_APPEND,0644);
            if(fd < 0){
                 perror(args[i+1]);
                  exit(1);
            }
             for(;args[i]!=NULL;i++){
                args[i]=NULL;
            }
            dup2(fd,1);
            close(fd);


            break;
        }
    }


}

void execute(char **args){

    pid_t processid = fork();

    if(processid < 0){
        perror("Fork failed");
        return ;
    }else if(processid == 0){
        redirection_handler(args);
        execvp(args[0],args);
        perror(args[0]);
        exit(1);
    }else{
        waitpid(processid,NULL,0);
    }

}
int main(){
    char command[1024];
    char **argv = NULL;

    while(1){
        printf("%s","nrjshell$ ");
        fflush(stdout);   
        if(fgets(command,sizeof(command),stdin) == NULL){
            break; //error from standard input
        }
        command[strcspn(command,"\n")] = '\0';

        if(command[0] == '\0'){
            continue;
        }

        int argc =makearg(command,&argv);

        if(argc <= 0){
            continue; //Not enough argument is passed 
        }

        if (strcmp(argv[0], "cd") == 0) {
           if (argv[1] == NULL) {
               chdir(getenv("HOME")); // default home
            } else {
                    if (chdir(argv[1]) != 0) {
                        perror("cd failed");
                    }
            }
            free_args(argv,argc);
            continue; // skip execvp

        }else if(strcmp(argv[0], "exit") == 0) {
             free_args(argv,argc);
          break; // exit the shell
        }else{
             execute(argv);
        }
        free_args(argv,argc);
        argv = NULL;

      
     
    }


  




    return 0;
}