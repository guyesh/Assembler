
#include "secondPass.h"

void secondPass(int ICF,int DCF,SymbolTable* st,node_t* dataList,FILE* file,char* fileName)
{
    
    int lineNum=0;
    int errorCnt=0;
    int rtnTmp;/*tmp var thats holds the functions return value*/
    char buffer[BUFF_LEN+1];
    long int lineAddress=MEMORY_START_POINT;
    char* file_name =mallocAndCheck(sizeof(char)*strlen(fileName)+4*sizeof(char));
    FILE* ob_file,* ext_file, *ent_file;
   strcpy(file_name,fileName);
   /* opening output files*/
   removeFileEx(file_name);
    strcat(file_name, ".ob");
     ob_file = fopen (file_name, "w+");
    removeFileEx(file_name);
    strcat(file_name, ".ext");
     ext_file = fopen (file_name, "w+");
    removeFileEx(file_name);
    strcat(file_name, ".ent");
     ent_file = fopen (file_name, "w+");

    fprintf(ob_file,"%d %d",ICF-MEMORY_START_POINT,DCF);
    rewind(file);/*makes sure the file pointer is on the start of the file*/
    while(fgets(buffer, BUFF_LEN+1, file))
    {
        lineNum++;
         /* --------- checking if line is empty or if its a note--------*/
        if(lineIsEmpty(buffer))/*if line is empty or note skip it*/
            continue;

        if(!isDataStrExtern(buffer))
        {
            if((rtnTmp=isEntry(buffer,lineNum)))
            {
                if(rtnTmp==ERROR)
                    errorCnt++;
                else
                {
                    char* entryLabel=getEntryLabel(buffer,lineNum);
                    if(entryLabel[0]=='\0')
                        errorCnt++;
                    else
                    {
                       if(!updateSymbolEntry(st,entryLabel))
                       {
                           errorCnt++;
                           fprintf(stdout,"In line %d: Cannot .entry when label doesn't exists in the current context;\n",lineNum);
                       }
                    }
                    free(entryLabel);
                }
                
            }
            else
            {
                
                rtnTmp=codeTheLine(buffer,lineNum,st,ob_file,ext_file,lineAddress);
                if(rtnTmp==ERROR)
                {
                    errorCnt++;
                }
                else
                    lineAddress+=rtnTmp;
            }

        }
        

    }
    lineAddress=codeData(dataList,ob_file,lineAddress);
    codeEntry(st,ent_file);
   
   printf("\033[0m");/* reset color of output*/
    fclose(ob_file);

    if(errorCnt==0)
    {
            printf("\033[1;32m\n***compilation of file '%s' is done, the files were created***\033[0m\n",fileName);
            
            /*deleting extern file if empty
            (trying to read last char if cant - file empty*/
              if (fseek(ext_file,-1,SEEK_CUR) != 0)
            {
                    fclose(ext_file);

                    removeFileEx(file_name);
                    strcat(file_name, ".ext");
                    if(remove(file_name)==0)
                        printf("no extern requests in file, deleting created extern file: \033[1;32mDeleted successfully\033[0m \n");
                    else
                    {
                         printf("no extern requests in file, deleting created extern file: file '%s' could not be deleted\n",file_name);
                    }
                    
                    
            }
            else
            {
                fclose(ext_file);
            }
            
            /*deleting entry file if empty*/
            if (fseek(ent_file,-1,SEEK_CUR) != 0)
            {
                    fclose(ent_file);
                    removeFileEx(file_name);
                    strcat(file_name, ".ent");
                    if(remove(file_name)==0)
                        printf("no .entry in file, deleting created entry file: \033[1;32mDeleted successfully\033[0m\n");
                    else
                    {
                         printf("no .entry in file, deleting created entry file: file '%s' could not be deleted\n",file_name);
                    }
            }
            else
            {
                fclose(ent_file);
            }
            
    }
    else
    {
        int removefilesStatus=0;
        printf("\n\033[1;31m***\033[0mFound \033[0;33m%d lines with \033[01;33merrors\033[0m, stops compilation of file '%s' in the second pass\033[1;31m***\033[0m\n --\033[0mremoving created files: ",errorCnt,fileName);
        /*removing created files*/
        {  
            /*closing unclosed files*/
            fclose(ent_file);
            fclose(ext_file);
            
            removeFileEx(file_name);
            strcat(file_name, ".ent");
            removefilesStatus+=remove(file_name);
            removeFileEx(file_name);
            strcat(file_name, ".ob");
            removefilesStatus+=remove(file_name);
            removeFileEx(file_name);
            strcat(file_name, ".ext");
            removefilesStatus+=remove(file_name); 
        }
        if (removefilesStatus == 0) 
            printf("\033[1;32mDeleted successfully\033[0m\n"); 
        else
            printf("ERROR: Unable to delete the/some files\n"); 
    }
    free(file_name);
}