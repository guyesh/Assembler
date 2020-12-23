#include "allfiles.h"
/* this function will call malloc with the given size, and check if there was a memory allocation error*/
void * mallocAndCheck(size_t size)
{
    void * rtn=malloc(size);
    if(rtn==NULL)
    {
        printf("\033[01;33m\nMemory allocation error - exiting...\033[0m");
        exit(0);
    }
    return rtn;
}
void * reallocAndCheck(void* ptr,size_t size)
{
    void * rtn=realloc(ptr,size);
    if(rtn==NULL)
    {
        printf("\033[01;33m\nMemory allocation error - exiting...\033[0m");
        exit(0);
    }
    return rtn;
}