#include "functionsFiles.h"
/* this file contain the functions used in the second pass*/

/*the function gets a string and returns TRUE if there is a data/string/extern decleration in it*/
int isDataStrExtern(char* str)
{
    return( strstr(str, ".extern")!=NULL||strstr(str, ".string")!=NULL||strstr(str, ".data")!=NULL);
}
/*the function gets a string and returns TRUE if there is an entry decleration in it.
 in case of syntax error, a message will be prompt and the function will return ERROR*/
int isEntry(char* str,int lineNum)
{
     char* entryPtr= strstr(str, ".entry");
     int i=0;
     if(entryPtr==NULL)
        return FALSE;
    for(;str[i]!='\0'&& &str[i]!=entryPtr;i++)
    {
        /* a text before .entry decleration syntax error*/
        if(!isspace(str[i]))
        {
            fprintf(stdout,"In line %d: Illegal text before a .entry deceleration;\n",lineNum);
            return ERROR;
        }
    }
    return TRUE;
}
/* the function gets a string with .entry and return the symbol name
the function findes the edges indexes of the word and then calculate its length and allocating the needed memory 
!!! the function assumes the string has .entry in it 
the function returns ['\0'] when there is a syntax error. It will prompt the error as well */
char* getEntryLabel(char* str,int lineNum)
{
     char* ptr=strstr(str,".entry");
        int i;
        int s=-1;/*holds index of the fist char in the label*/
        for(i=6;ptr[i]!='\0';i++)/* searching for label */
        {
            if(s==-1&&!isspace(ptr[i]))
            {
                s=i;
            }
            else if(s!=-1&&isspace(ptr[i]))
            {
                int j=i;/*new index to check the text after the label*/
                /*making sure there is no extraneous text after the label*/
                for(;ptr[j]!='\0';j++)
                {
                    if(!isspace(ptr[j]))
                    {
                        char* returnlabel =mallocAndCheck(sizeof(char));
                        fprintf(stdout,"In line %d: Extraneous text after the label;\n",lineNum);
                         returnlabel[0]='\0';
                        return returnlabel;
                    }
                }

                
                {
                char* returnlabel =mallocAndCheck(sizeof(char)*(i-s+1));
                memset(returnlabel, 0,i-s+1);/*zeroing the string, otherwise strncpy causes problems with memory*/
                strncpy(returnlabel,ptr+s, i-s);
                returnlabel[i-s]='\0';
                return returnlabel;
                }
            }
        }

        if(s!=-1)
        {
            char* returnlabel =mallocAndCheck(sizeof(char)*(i-s+1));
                memset(returnlabel, 0, i-s+1);/*zeroing the string, otherwise strncpy causes problems with memory*/
                strncpy(returnlabel,ptr+s, i-s);
                returnlabel[i-s]='\0';
                return returnlabel;
        }

        {
        /*there is no label*/
        char* returnlabel =mallocAndCheck(sizeof(char));
         fprintf(stdout,"In line %d: You must declare a label after .entry;\n",lineNum);
         returnlabel[0]='\0';
        return returnlabel;
        }
    

}
/*the function gets a string represent a file name and removes the extention from it*/
char* removeFileEx(char* fileName) {
    char *lastExt;
    if (fileName == NULL) return NULL;
    lastExt = strrchr (fileName, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    return fileName;
}
/* the function will reverse a string*/
void strreverse(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)/* in case str is null*/
            return ;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
     
}
/* the function will convert a int to bits string. at the size of sizeOfOutput*/
/* the sizeofoutput is the size of (char* output) without the \0*/
void int_to_bits(char* output,long int num,int sizeOfOutput)
{
    int i;
    int isNegative=(num<0);
    num=isNegative?-num:num;
    for(i=0;i<sizeOfOutput;i++)    
    {    
        output[i]=(num%2)+'0';    
        num=num/2;    
    }    
    output[i]='\0';
strreverse(output);
    /* if the number is negative we will find the first 1 and replace all the 1 after it with 0 and 0 with 1*/
    if(isNegative)
    {
        int first_one_found=FALSE;
        for(i=sizeOfOutput-1;i>=0;i--)
        {
            if(output[i]=='1'&&!first_one_found)
            {
                first_one_found=TRUE;
            }
            else if(first_one_found)
            {
                if(output[i]=='1')
                    output[i]='0';
                else
                    output[i]='1';
            }

        }
    }
    /* reverse the string*/
    return ;
}
/* the function checks if there are non numbers digits in the string except '-'.
if there are it will return False; true otherwise*/
int check_if_number_valid(char* num)
{
    int i=0;
    if(num[0]=='-')
        i++;
    for(;num[i];i++)
    {
       
        if('0'>num[i]|| num[i]>'9')
            return FALSE;
    }
    return TRUE;

}
/* convert an operand to bits, 3 bits for an operand without extra word and 24 with
if there was an error function returns '\0' and prompts the error,
if detected an external symbol in direct addressing type it will write the symbol and current address to .ext file
---the function gets operExtraAddress- means the address of the *extra word* of operand  */
char* operand_to_bits(char* oper, enum addressingType addressing,SymbolTable* st,int lineNum,FILE* extFile,long int operExtraAddress)
{
    char* output;
    long int value;/* will be used to contain addresses/number (for direct/immediate)*/
    switch(addressing)
        {
            case REGISTER:
                output= mallocAndCheck(sizeof(char)*4);
                int_to_bits(output,atoi(oper+1),3);
                break;
            case DIRECT:
                
                if((value=get_value(st,oper))==ERROR)
                {
                     output= mallocAndCheck(sizeof(char));
                    fprintf(stdout,"In line %d: Unkown label(symbol) in direct addressing;\n",lineNum);
                     output[0]='\0';
                     
                }
                else
                {
                    output= mallocAndCheck(sizeof(char)*25);
                    int_to_bits(output,value,21);
                    
                    /*write A,R,E depended (checks if external by the address of the symbol)*/
                    if(value==0)/* this is an extern symbol*/
                    {
                        /* checks if its the first line - and thus know when to put '\n'- for new line
                        seek one char back if didn't succeed it first line and no \n will be written*/
                        if (fseek(extFile,-1,SEEK_CUR) != 0)
                        {
                            fprintf(extFile,"%s %07ld",oper,operExtraAddress);
                        }
                        else
                        {
                           fseek(extFile,1,SEEK_CUR);/*return to current*/
                            fprintf(extFile,"\n%s %07ld",oper,operExtraAddress); 
                        }
                        

                        output[21]='0';/*A*/
                        output[22]='0';/**R*/
                        output[23]='1';/*E*/
                    }
                    else
                    {
                        output[21]='0';/*A*/
                        output[22]='1';/**R*/
                        output[23]='0';/*E*/
                    }
                    output[24]='\0';
                   
                    
                }
                break;
            case IMMEDIATE:
               
                if(oper[1]=='\0')
                {
                    output= mallocAndCheck(sizeof(char));
                    fprintf(stdout,"In line %d: Operand can't be an empty number;\n",lineNum);
                        output[0]='\0';
                        return output;
                }

                if(!check_if_number_valid(oper+1))
                {
                        output= mallocAndCheck(sizeof(char));
                        fprintf(stdout,"In line %d: Operand is an invalid number - '%s';\n",lineNum,oper+1);
                        output[0]='\0';
                        return output;
                }
                value=atol(oper+1);
                if(value>(pow(2,WORD_SIZE-3)-1)/2||value<-(pow(2,WORD_SIZE-3)-1)/2)
                {
                             output= mallocAndCheck(sizeof(char));
                            fprintf(stdout,"In line %d: Operator number size is bigger then 2^%d;\n",lineNum,WORD_SIZE-3);
                             output[0]='\0';
                             return output;
                }
                 output= mallocAndCheck(sizeof(char)*25);
                 int_to_bits(output,value,21);
                    
                    /*write A,R,E */
                    output[21]='1';/*A*/
                    output[22]='0';/**R*/
                    output[23]='0';/*E*/
                    output[24]='\0';
                   
                    break;
            case RELATIVE:
            /* these commands only have destination Operand thus the address of the command is operExtraAddress-1 */
             if((value=get_value(st,oper+1))==ERROR)
                {
                    fprintf(stdout,"In line %d: Unkown label(symbol) in relative addressing;\n",lineNum);
                    output= mallocAndCheck(sizeof(char));
                     output[0]='\0';
                     
                }
                else if(value==0)/*its extern - invalid relative*/
                {
                    output= mallocAndCheck(sizeof(char));
                    fprintf(stdout,"In line %d: External label(symbol) cannot be in relative addressing;\n",lineNum);
                     output[0]='\0';
                }
                else
                {
                    output= mallocAndCheck(sizeof(char)*25);
                    int_to_bits(output,value-(operExtraAddress-1),21);
                     /*write A,R,E */
                    output[21]='1';/*A*/
                    output[22]='0';/**R*/
                    output[23]='0';/*E*/
                    output[24]='\0';
                }    
                break;

            default:/* when its NONE - no operand*/
                output=mallocAndCheck(sizeof(char)*4);
                strcpy(output,"000");
                break;



        }
        return output;
    
}
/* convert a string of binary number to hex*/
void hex(char* bin)
{
    char *pt = bin;
    int num = 0;
do {
    int b = *pt=='1'?1:0;
    num = (num<<1)|b;
    pt++;
} while (*pt);
sprintf(bin,"%06x", num);
}
/* the function gets the needed info to write the binary code and write it to the file,
 the function will get:
    - a string with 3 chars for operator with no extra words
    - a string with 24 chars for an operator with extra words
    - a string with zero chars for no operator */
int codeTheWord(long int lineAddress,char* cmd,enum addressingType org_addressing,char* oper_org_content,enum addressingType des_addressing,char* oper_des_content,FILE* ob_file)
{
    char funct[6],opcode[7];
    char* org,*des;
    char* exWord1=NULL,*exWord2=NULL;
    char addressing_org_binary[3],addressing_des_binary[3];
    short int L=0;/* counts the number written lines*/
    char tmp_bin[24];
    int_to_bits(funct,cmd_to_funct(cmd),5);
    int_to_bits(opcode,cmd_to_opcode(cmd),6);
    /* extracting opers_content to exWord1/exWord2 and org/des strings - according to the oper_content len*/
    if(strlen(oper_org_content)==3||strlen(oper_org_content)==0)
    {
        org=oper_org_content;
        exWord1=NULL;
    }
    else
    {
        exWord1=oper_org_content;
        org=mallocAndCheck(sizeof(char)*4);
        strcpy(org,"000");
    }
    if(strlen(oper_des_content)==3||strlen(oper_des_content)==0)
    {
        des=oper_des_content;
        exWord2=NULL;
    }
    else
    {
        exWord2=oper_des_content;
        des=mallocAndCheck(sizeof(char)*4);
        strcpy(des,"000");
       
    }


/*in case operand doesn't exists*/
    if(des_addressing==NONE)
    {
        strcpy(addressing_des_binary,"00");
 
        
    }
    else
       int_to_bits(addressing_des_binary,des_addressing,2);

    if(org_addressing==NONE)
    {
       strcpy(addressing_org_binary,"00");

    }
    else
       int_to_bits(addressing_org_binary,org_addressing,2);
    

    
    
    sprintf(tmp_bin,"%s%s%s%s%s%s100",opcode,addressing_org_binary,org,addressing_des_binary,des,funct);
    hex(tmp_bin);
    fprintf(ob_file,"\n%07ld %s",lineAddress,tmp_bin);
    L++;
    if(exWord1!=NULL)
    {
        hex(exWord1);
        fprintf(ob_file,"\n%07ld %s",lineAddress+L,exWord1);
        L++;
        free(org);
    }
    if(exWord2!=NULL)
    {
        hex(exWord2);
        fprintf(ob_file,"\n%07ld %s",lineAddress+L,exWord2);
        L++;
        free(des);
    }

    return L;
}
/* the function gets the line to code and line number + symbol table and write it in binary to the given file
it returns ERROR if it detected an error, tnd the number of memory words otherwise*/
int codeTheLine(char* line,int lineNum,SymbolTable* st,FILE*  obFile,FILE* externFile,int lineAddress)
{
    char cmd[CMD_MAX_LEN+1];
    enum addressingType des_addressing;
    enum addressingType org_addressing;
    char* oper_des;/* destenetion operand = target*/
    char* oper_org;/*origin operand= source*/
    /* will hold the content of the origin in bits - it will be 3 bits for Register and 24 for operand with extra word*/
    char* oper_des_content;
    char* oper_org_content;
    int operand_max_size;/* will be calculated using cmd len;*/
    int i,j;
    int returnval;

/* ---- in this scope the string is being analyzed and the following vars will update correspondingly : cmd,oper_des,oper_org,adr_des,ade_org----*/
{
    /* skips the label in the string*/
    char* tmp=strstr(line, ":");
    if(tmp!=NULL)
        line=tmp+1;

    /*copying the cmd from the word to the cmd string - if we find an error that wasn't discovered in first pass we will prompt it now*/
    /* skips the first spaces*/
    
    for(i=0;line[i] != '\0'&& isspace(line[i]);i++);
    for(j = 0;line[i] != '\0'&& !isspace(line[i]); i++)
    {
        if(j<CMD_MAX_LEN)
        {
            cmd[j]=line[i];
            j++;
        }
    }
    cmd[j]='\0';
    if(j>=CMD_MAX_LEN||!cmd_exists(cmd))
    {
        fprintf(stdout,"In line %d: Unrecognized command;\n",lineNum);
        return ERROR;
    }

    /* the command is valid, we will fill oper_des and oper_org, part the syntax was checked in the first pass, we wont recheck it*/
    operand_max_size=sizeof(char)*(BUFF_LEN-strlen(cmd)+1);/*max size of operand*/

    /* ----------------filling org-operand---------------*/
    oper_org=mallocAndCheck(operand_max_size);
    for(;line[i] != '\0'&& isspace(line[i]);i++);
    /*if(line[i] != '\0') oper_org[0]='\0';*/
    for(j = 0;line[i] != '\0'&& !isspace(line[i])&& line[i]!=','&& j<operand_max_size-1; i++)
    {
            oper_org[j]=line[i];
            j++;
    }
    /*we need to increase i until ','*/
    for(;line[i] != '\0'&& isspace(line[i])&&line[i]!=',';i++);
    oper_org[j]='\0';
    
/* ----------------filling des-operand---------------*/
    if(line[i]==',')
        i++;
    oper_des=mallocAndCheck(operand_max_size);
    for(;line[i] != '\0'&& isspace(line[i]);i++);
    /*if(line[i] != '\0') oper_des[0]='\0';*/
    for(j = 0;line[i] != '\0'&& !isspace(line[i])&& line[i]!=','&& j<operand_max_size-1; i++)
    {
            oper_des[j]=line[i];
            j++;
    }
    oper_des[j]='\0';
    if(line[i]==',')
    {
        fprintf(stdout,"In line %d: Too many operands;\n",lineNum);
        return ERROR;
    }
    for(;line[i] != '\0';i++)
    {
        if(!isspace(line[i]))
        {
            
                free(oper_des);
                free(oper_org);
                fprintf(stdout,"In line %d: Extraneous text after command;\n",lineNum);
                return ERROR;
        }
    }

    /*---- updating addresing of operands---*/
    des_addressing=opperand_get_type(oper_des);
    org_addressing=opperand_get_type(oper_org);
}
    if(des_addressing==NONE)/* in case the first operand is empty we will want to replace org with des*/
        returnval = check_valid_addressing(cmd,NONE,org_addressing,lineNum);
    else
        returnval = check_valid_addressing(cmd,org_addressing,des_addressing,lineNum);
    /* addressing type error*/
    if(returnval==ERROR)
    {
        free(oper_des);
        free(oper_org);
        return ERROR;
    }

    oper_org_content=operand_to_bits(oper_org,org_addressing,st,lineNum,externFile,lineAddress+1);
    /* the extra word address depends if the former operand had a second word*/
    if(strlen(oper_org_content)==3)
        oper_des_content=operand_to_bits(oper_des,des_addressing,st,lineNum,externFile,lineAddress+1);
    else
        oper_des_content=operand_to_bits(oper_des,des_addressing,st,lineNum,externFile,lineAddress+2);
    /* if one of them returned an error*/
    if(oper_org_content[0]=='\0'||oper_des_content[0]=='\0')
    {
        returnval= ERROR;
    }
   
    /* in case the first operand is empty we will want to replace org with des*/
    if(des_addressing==NONE)
    {
        returnval=(returnval==ERROR)?ERROR:codeTheWord(lineAddress,cmd,des_addressing,oper_des_content,org_addressing,oper_org_content,obFile);
     
    }
    else{
        returnval=(returnval==ERROR)?ERROR:codeTheWord(lineAddress,cmd,org_addressing,oper_org_content,des_addressing,oper_des_content,obFile);
       
    }
     
    free(oper_org);
   free(oper_des);
    free(oper_des_content); 
    free(oper_org_content);
   
 
    return returnval;
}


/*function will write the data list into the object file
the function returns the address after adding the data to file*/
long int codeData(node_t * head,FILE* ob_file,long int address)
{
   node_t * current = head;

    char tmp[25];

    while (current != NULL) {
        int_to_bits(tmp,current->val,24);
        hex(tmp);
        fprintf(ob_file,"\n%07ld %s",address,tmp);
        address++;
        current = current->next;
    }    
    return address;
}
void codeEntry(SymbolTable* st,FILE* ent_file)
{
    int i=0;
    int isFirstLine=TRUE;/* flag that tells if its the first line written(there is no need in \n)*/
     for(;i<st->index;i++)/*runs over the table until st.index*/
    {
        if(st->table[i]->type==entry)
        {
            if(isFirstLine)
            {
                fprintf(ent_file,"%s %07ld",st->table[i]->tag,st->table[i]->value);
                isFirstLine=FALSE;
            }
            else
                fprintf(ent_file,"\n%s %07ld",st->table[i]->tag,st->table[i]->value);
        }
    }
}
