#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fork,execvp
#include <sys/wait.h> // wait
#include <fcntl.h> // used for open(),close() and flags like O_WRONLY

int main(){

    int fd=open("Testing.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
    if(fd == -1){
        exit(0);
    }
    dup2(fd,1);
     char * argv[]={ "ls","-l",NULL};
     close(fd);
    execvp("ls",argv);
    
   return 0;
}