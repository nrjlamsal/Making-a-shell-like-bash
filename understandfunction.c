#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // fork,execvp
#include <sys/wait.h> // wait

int main(){

    char *argv[] ={"ls","-l",NULL};
    // argv ={"cd",NULL};

    execvp("ls",argv);
    return 0;
}