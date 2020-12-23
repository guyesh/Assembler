#include "firstPass.h"


/*the function preforms the first pass algoritem of the assembler. on a given (open) file*/
/****it assumes the file is open*/
void first_pass(FILE* file,char* fileName)
{
    int DC=0;
    int IC=MEMORY_START_POINT;
    int hasSymbolflag=FALSE;
    int lineNum=0;
    int rtnTmp;/*tmp var thats holds the functions return value*/
    int errorCnt=0;
    SymbolTable st;
    char buffer[BUFF_LEN+1];/*plus one for \0*/
    node_t* dataList= (node_t *) mallocAndCheck(sizeof(node_t));/*creating the first node, so dataList will get an address, the node will be removed at the end of first pass*/
    st.index=0;/*initialize st index*/
    st.table=NULL;/*initialize st table to NULL*/
    dataList->next=NULL;/*head of the list*/
    rewind(file);/*makes sure the file pointer is on the start of the file*/
        
    
    while(fgets(buffer, BUFF_LEN+1, file))
    {
        lineNum++;
        
        /*making sure line is shorter then 81 chars*/
        if(strlen(buffer)==BUFF_LEN&&buffer[BUFF_LEN-1]!='\n'&&buffer[BUFF_LEN-1]!=EOF)
        {
            char tmpC;
            fprintf(stdout,"In line %d: Line is longer then %d chars;\n",lineNum,BUFF_LEN-1);
            errorCnt++;
            while((tmpC=fgetc(file))!='\n'&&tmpC!=EOF);/* skips line*/
        }

        /* --------- checking if line is empty or if its a note--------*/
        if(lineIsEmpty(buffer))/*if line is empty or note skip it*/
            continue;

        /*---------checks if the first field is a label and turning on the label flag-----------*/
        
        if((rtnTmp=hasSymbol(buffer,lineNum)))/*TODO : symbol is 31 char long(max) ends with :*/
        {
            hasSymbolflag=TRUE;
        }
        else
        {
             hasSymbolflag=FALSE;
        }
        
        if(rtnTmp==ERROR)
        {
            errorCnt++;
            continue;
        }
        
        /*-----------checks if it is a data instruction--------*/
        
        rtnTmp=isDataInstruction(buffer,lineNum,hasSymbolflag);/*constains STRING=2/DATA=1/FALSE=0*/
        if(rtnTmp==ERROR)
        {
            errorCnt++;
            continue;
        }
        if(rtnTmp!=FALSE)
        {
            if(hasSymbolflag)
            {    
                char* labelName=extractLabelFromRow(buffer);/*setting a pointer to hold the new string so it could be free after adding symbol*/

                if(addSymbol(&st,labelName,DC,none,data,lineNum)==ERROR)/*adding the symbol+ checking if there was an error*/
                {
                     free(labelName);
                    errorCnt++;
                    continue;
                }
                free(labelName);
            }
            if(rtnTmp==STRING)
            {
                rtnTmp=stringToData(dataList,buffer,lineNum);
                DC+=rtnTmp;
               if(rtnTmp==ERROR)/*if it wasn't an error dc+=returned data counter , if there was an error dc value doesn't matter because it wont get to the second pass*/
               {
                   errorCnt++;
                   continue;
               }  
            }
            else if(rtnTmp==DATA)
            {
                rtnTmp=dataToData(dataList,buffer,lineNum);
                DC+=rtnTmp;
                if(rtnTmp==ERROR)/*if it wasn't an error dc+=returned data counter , if there was an error dc value doesn't matter because it wont get to the second pass*/
               {
                   errorCnt++;
                   continue;
               }  
            }
            continue;
        }

        /*-------------handle entry/extern------------------*/
        
        if((rtnTmp=has_entry_extern(buffer,lineNum)))
        {
            char* labelName;/*setting a pointer to hold the new label */
            if(rtnTmp==ERROR)
            {
                errorCnt++;
                continue;
            }
            /*checks if its an extern instruction*/
            if((labelName=extractLabelFromExtern(buffer,lineNum))!=FALSE)
            {
                if(labelName[0]=='\0'||!isLabelValid(&st,labelName,lineNum))/*the function outputs " " when there isn't a tag*/
                {
                    free(labelName);
                    errorCnt++;
                    continue;
                }
              
                addSymbolExtern(&st,labelName,0,external,code,lineNum);
                free(labelName);
            }
            continue;
        }
          /*--------------this is a code instruction------------*/
        {
            char* cmd= extractNamefromCommand(buffer,lineNum,hasSymbolflag);
            int L;/* will contain the amount of extra words needed for command*/
            if(cmd[0]=='\0')/*extractNamefromCommand() returns "" if there was an error*/
            {
                free(cmd);
                errorCnt++;
                continue;
            }
            else if(!cmd_exists(cmd))
            {
                  fprintf(stdout,"In line %d: Unrecognized command '%s';\n",lineNum,cmd);
                  errorCnt++;
                free(cmd);
                continue;

            }
            if(hasSymbolflag)
            {    
                char* labelName=extractLabelFromRow(buffer);/*setting a pointer to hold the new string so it could be free after adding symbol*/

                if(addSymbol(&st,labelName,IC,none,code,lineNum)==ERROR)/*adding the symbol+ checking if there was an error*/
                {
                     free(labelName);
                      free(cmd);
                        errorCnt++;
                        continue;
                }
                free(labelName);
            }
            L=checksyntax_get_mem_len(cmd,buffer,lineNum);
            if(L==ERROR)
            {
                free(cmd);
                 errorCnt++;
                 continue;
            }
            IC+=L;
            free(cmd);


        }
        
       /* printf("%s",buffer);HELPS FOR DEBUGING*/
    
    }
    /* removing the head of the list:( it doesn't contain value) 
     it was set in order to get an address for the list so it could be passed to functions*/
    {
        node_t* tmp=dataList->next;
        free(dataList);
        dataList=tmp;
    }
    add_ICF_to_data(&st,IC);
    /*printTable(&st); HELPS FOR DEBUGING*/
    if(errorCnt==0)
    {
       secondPass(IC,DC,&st,dataList,file,fileName);
    }
    else
    {
        printf("\n\033[1;31m***\033[0mFound \033[0;33m%d lines with \033[01;33merrors\033[0m, stops compilation of file '%s' in the first pass\033[1;31m***\033[0m\n",errorCnt,fileName);
    }
    
      freeST(&st);
    freeList(dataList);
}