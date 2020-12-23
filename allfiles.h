#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFF_LEN 81 /* this is the max len of a row in the file. max line+ 1 more for \n*/
#define FALSE 0
#define TRUE 1
#define ERROR -1
#define WORD_SIZE 24
#define MEMORY_START_POINT 100
#define LABEL_MAX_SIZE 31
#define FILE_NAME_MAX 100/*file name max size*/
enum dataType {DATA=1,STRING};
void * mallocAndCheck(size_t size);/* function to replace malloc, will do malloc and check if null*/
void * reallocAndCheck(void* ptr,size_t size);