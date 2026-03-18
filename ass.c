#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int makearg(char s[],char ***args){
    // if problem return -1 else return 1

    if(s==NULL){
        return -1;
    }   

    // char **argv = *args; wrong sequence 
    //  argv =(char**)malloc(1024 *sizeof(char*));

    char **argv =(char**)malloc(1024 *sizeof(char*));
    *args= argv;
    char *buffer = (char *)malloc(1024 *sizeof(char));

    int index = 0 ; //used during parse
    int aindex =0 ; // used for argv index

    for(int i=0 ; s[i]!='\0';i++){
     if(s[i] == ' ' ){
        if(index > 0){
            buffer[index] = '\0';
            argv[aindex] =(char*)malloc(sizeof(char)*(index+1));
            for(int j=0; j < (index + 1) ; j++ ){
                argv[aindex][j]= buffer[j];
            }
            index = 0;
            aindex++;


        }
     }else{
     
        buffer[index++] = s[i];
     }

   }

   if(index > 0){

        buffer[index] = '\0';

        argv[aindex] = (char *)malloc(index + 1);

        for(int j=0; j<=index; j++)
            argv[aindex][j] = buffer[j];

        aindex++;
    }


free(buffer);
 return aindex;
}
int main(){

    char **args=NULL;
    char str[]="ls -l file";
    int argc=makearg(str,&args);

    for(int i = 0 ; i < argc ; i++){
        printf("%s\n",args[i]);
    }

      for(int i = 0; i < argc; i++){// free each word
        free(args[i]);
    }
        free(args);



    return 0;
}