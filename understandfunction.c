#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fork,execvp
#include <sys/wait.h> // wait
#include <fcntl.h> // used for open(),close() and flags like O_WRONLY

int main(){

    // int fd=open("Testing.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
    int fd=open("Testing.txt",O_RDONLY);
    if(fd == -1){
        exit(0);
    }
    dup2(fd,0);
    char str[1024];
    fgets(str,sizeof(str),stdin); // readds from file as we redirrected standard input to file

    printf("%s\n",str);// ..pinnt the data we got from file

    
    close(fd);
    
   return 0;
}