
#include "main.h"
/*the function gets arguments from command line and checks if the arguments input is valid(more then one file)*/
int check_args(int argc)
{
    /*checking if there is at least one asm file*/
    if( argc < 2 ) {
      printf("You need to provide at-least one as file.\n");
        return ERROR;
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    FILE* file;
    int i;
    char fileName[FILE_NAME_MAX];/* tmp string to hold the file name*/
    if(check_args(argc)==ERROR)
        return 0;
    /*for runs over all the given files as arguments, opens each, compiles, and close it*/
    for(i=0;i<argc-1;i++)
    {
        sprintf(fileName,"%s.as",argv[i+1]);
        /* the use of \033[;m are for coloring the notes */
         if ((file = fopen(fileName,"r")) == NULL){
                printf("\033[0;31mError! opening file: \'%s.as\'\033[0m\n",argv[i+1]);

                /* Program exits if the file pointer returns NULL.(Error opening the file)*/
                exit(1);/*TODO decide if I want to change it to break*/
        } 
         printf("\n****\033[01;35mcompiling file: '%s.as'\033[0m****\033[0;31m\n",argv[i+1]);
        first_pass(file,fileName);

        fclose(file);/*closing the current file*/
    
    }
   return 0;
}