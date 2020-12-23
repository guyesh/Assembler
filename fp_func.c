#include "functionsFiles.h"

int islabelSavedWord(char* label)
{
    if(cmd_exists(label))
        return TRUE;
    if(label[0]=='r'&&label[1]>='1'&&label[1]<='7'&&label[2]=='\0')
        return TRUE;
    return FALSE;
}
/*the function gets a row from the file and returns TRUE if the row contains a label, FALSE otherwise
    if there is an illegal definition of a label, the function will print the compilation error and return ERROR*/
int hasSymbol(char* buff,int line){
    int i,j;
     int illegalWhiteS = FALSE; /*flag that says if there was a white block in the definition of the tag(in the middle of the name or near the ':')*/
     int illegalChar=FALSE;/*flag that says if there was a char that isn't digit or alpha*/
     int illegalFirstChar=FALSE; 
    char tmpLabel[LABEL_MAX_SIZE];
    for(i=0;i<strlen(buff)&&isspace(buff[i]);i++){}/*skips spaces at the begining*/
    if(!isalpha(buff[i])&&buff[i]!=':')
    {
       illegalFirstChar=TRUE;
    }

    for(j=0;i<strlen(buff)&&j<LABEL_MAX_SIZE+1;i++,j++)
    {
      
        if(isspace(buff[i]))/*if there was a white space in the definition of the symbol*/
        {
             illegalWhiteS = TRUE;
           
        }
        else if(!isalpha(buff[i])&&!isdigit(buff[i])&&buff[i]!=':')
        {
            illegalChar=TRUE;
   
        }
        else if(buff[i]==':')
        {
            tmpLabel[j]='\0';
            if(illegalFirstChar)
            {
                fprintf(stdout,"In line %d: Illegal label,label should start with an alphabeth char;\n",line);
                return ERROR;
            }
            if(illegalWhiteS)
            {
                  fprintf(stdout,"In line %d: Illegal ':',label shouldn't contain white spaces in its definition;\n",line);
                return ERROR;
            }
            if(illegalChar){
                fprintf(stdout,"In line %d: Illegal label,label should contain only digits and alphabethic chars;\n",line);
                return ERROR;
            }
            if(i==0||isspace(buff[i-1]))/*label dosent contain chars at all*/
            {
              fprintf(stdout,"In line %d: Illegal ':',label should contain at least one char;\n",line);
                return ERROR;  
            }
            if(islabelSavedWord(tmpLabel))
            {
                fprintf(stdout,"In line %d: Illegal label, label cannot be a name of a register or a command;\n",line);
                return ERROR; 
            }
            return TRUE;

        }
          tmpLabel[j]=buff[i];
    }
        if(j==LABEL_MAX_SIZE+1&&strstr(buff,":")!=NULL)/*if there was : after the first word and the word was longer then 31 - (j starts from 0)*/
        {
            fprintf(stdout,"In line %d: A label shouldn't be larger then 31 chars;\n",line);
                return ERROR;
        }
    return FALSE;

}
/*the function gets a line, if it contains .data or .string, it returns the coresponding dataType
,if there was an error it returns ERROR,FALSE otherwise */
int isDataInstruction(char* buffer,int line,int hasLabel)
{
    char* dataInLine = strstr(buffer, ".data");/*NULL if there aren't '.data' in the given line*/
    char* stringInLine=strstr(buffer, ".string");/*NULL if there aren't '.string' in the given line*/
    int externOrEntryInLine=strstr(buffer, ".extern")!=NULL||strstr(buffer, ".entry")!=NULL;/*checks if there is extern or entry in line in order to report it as error( if there is .data/string)*/
    int i=0;
    /*if both .data and .string exists in the line => error...*/
    if(dataInLine != NULL&& stringInLine != NULL) {
        fprintf(stdout,"In line %d: You cannot declare .data and .string in the same line;\n",line);
        return ERROR;
    }
    else if((dataInLine != NULL|| stringInLine != NULL)&&externOrEntryInLine)
    {
    fprintf(stdout,"In line %d: You cannot declare .data/.string and .entry/.extern in the same line;\n",line);
    return ERROR;
    }
    if((dataInLine!=NULL||stringInLine !=NULL)){
        if(hasLabel)
        {
            /*find the index of end of the label*/
            i = (int)( strchr(buffer, ':') - buffer)+1;
        }
    for(;&buffer[i]!=dataInLine&&&buffer[i]!=stringInLine;i++)/*one of them is null - it will run until the .data/string*/
    {
        if(!isspace(buffer[i]))
        {
            fprintf(stdout,"In line %d: Invalid text before .data/.string;\n",line);
        return ERROR;
        }
    }}


    if(dataInLine != NULL)/*only data exists in line*/
    {
        return DATA; 
    }
    else if(stringInLine !=NULL)
    {
        return STRING;
    }
    return FALSE;
}

/*checks if this is an extern instruction returns the label if true and false otherwise*/
char* extractLabelFromExtern(char* line,int lineNum)
{
   char* ptr;
    if((ptr =strstr(line,".extern"))!=NULL)/*check if the row has .extern*/
    {
        int i;
        int s=-1;/*will hold the index of the fist char in the label*/
        /*making sure there is no invalid text before extern*/
        for(i=0;&line[i]!=ptr;i++)
        {
            if(!isspace(line[i]))
            {
                  char* returnlabel =mallocAndCheck(sizeof(char));
                        fprintf(stdout,"In line %d: Invalid text before .extern;\n",lineNum);
                         returnlabel[0]='\0';
                        return returnlabel;
            }
        }
        for(i=7;ptr[i]!='\0';i++)/* searching for label */
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
                memset(returnlabel, 0, i-s+1);/*zeroing the string, otherwise strncpy causes problems with memory*/
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
         fprintf(stdout,"In line %d: You must announce a label after .extern;\n",lineNum);
         returnlabel[0]='\0';
        return returnlabel;
        }
    
    }
  
    return FALSE;
}


/*the function get a line(string) with a command and return the command name, if there isn't one function will return "" and prompt the error*/
char* extractNamefromCommand(char* line,int lineNum,int isSymbol)
{
    int i=0,j=0;
    char* cmd=mallocAndCheck((CMD_MAX_LEN+1)*sizeof(char));
    if(isSymbol)/* if True then there must be : in line*/
    {
        line=strstr(line,":")+1;
    }
    for(;line[i]!='\0';i++)
    {
        if(j==CMD_MAX_LEN-1&&!isspace(line[i]))
        {
            cmd[0]='\0';
            fprintf(stdout,"In line %d: Unrecognized command;\n",lineNum);
            return cmd;

        }
        else if(j!=0&&isspace(line[i]))
        {
            cmd[j]='\0';
            return cmd;
        }
        else if(!isspace(line[i]))
        {
            cmd[j]=line[i];
            j++;
        }
    }
    if(j==0)
    {
        /* must be a line with a label with nothing after it (LABEL:     ). we will prompt an error*/
        cmd[0]='\0';
        fprintf(stdout,"In line %d: You cannot declare a label on empty line;\n",lineNum);
        return cmd;
    }
    /* will get here only if -line- wont have \n at the end or any other white block*/
    cmd[j]='\0';
    return cmd;
}

int has_entry_extern(char* line,int lineNum)
{
    int is_entry= strstr(line,".entry")!=NULL;
    int is_extern= strstr(line,".extern")!=NULL;
    if(is_entry&&is_extern)
    {
        fprintf(stdout,"In line %d: You can not declare entry and extern in the same line;\n",lineNum);
        return ERROR;
    }
    if(is_entry||is_extern)
    {
        return TRUE;
    }
    return FALSE;

}

/*gets a row with a valid label and extracts its name; the function assume the label was checked and there are no spaces in between the name of the label(only at the beginging)*/
char* extractLabelFromRow(char* row)
{
    int i,j;
    int labelMaxLen=strchr(row, ':')-row;/*strchr(row, ':')-row ==> index of ':'  (max size for the label)*/
     char* label=mallocAndCheck(sizeof(char)*(labelMaxLen+1));
    for(i=0,j=0;i<strlen(row);i++)
    {
        if(row[i]==':')
        {
            label[j]='\0';
            break;
        }
        else if(!isspace(row[i]))
        {   label[j]=row[i];
            j++;
        }
    }
    return label;
}

/*gets a string with .string in it and copies the string into the data list
if there was a syntax error in the asm file then the function will prompt the error and return ERROR
the function assumes there is a .string in the given string*/
int stringToData( node_t *dataList,char * line,int lineNum)
{
    char* strptr=strstr(line, ".string");/*ptr to the current char in string-( '.string' must be in line because it was checked earlier)*/
    int i;
    int invertedCommasCnt=0;
    int strLen=0;
    for(i=7;strptr[i]!='\0';i++)/*strptr points to the starts of .string thus we need to set i to 7 -after .string*/
    {
        if(!isspace(strptr[i])&&invertedCommasCnt==0&&strptr[i]!='\"')/*checks if before the string there are chars(that aren't white)*/
        {
            fprintf(stdout,"In line %d: After '.string' you should provide a string (syntax: .string \"your string\");\n",lineNum);
            return ERROR;
        }
        else if(!isspace(strptr[i])&&invertedCommasCnt==2)/*checks if after the string there are more chars(that aren't white)*/
        {
           fprintf(stdout,"In line %d: Extraneous text after '.string' decleration;\n",lineNum);
            return ERROR;
        }
        else if(strptr[i]=='\"')
        {
           
           invertedCommasCnt++; 
            
           if(invertedCommasCnt==2)
           {
                int j=i+1;
                /* 
                ------------we need to check if it's the end of the string-------------
                checks if there is another '\"' after this one- and this one is part of the string*/
                for(;strptr[j];j++)
                {
                    if(strptr[j]=='\"')
                    {
                        j=-1;/* means its not the end of the string*/
                            break;
                    }
                }
                if(j!=-1)
                    push(dataList,(int)'\0');
                else
                {
                    push(dataList,(int)'\"');
                    invertedCommasCnt--;/*because its not the end of the string*/
                }
                strLen++;
           }
        }
        else if(invertedCommasCnt==1)
        {           
            push(dataList,(int)strptr[i]);
            strLen++;
        }
    }
    if(invertedCommasCnt==0)
    {
        fprintf(stdout,"In line %d: After '.string' you should provide a string (syntax: .string \"your string\");\n",lineNum);
        return ERROR;
    }
    if(invertedCommasCnt==1)
    {
        fprintf(stdout,"In line %d: After '.string' you forgot to close the string with inverted commas ' \" ' ;\n",lineNum);
        return ERROR;
    }
    return strLen;

}

/*gets a string with .data in it and copies the data into the data list
if there was a syntax error in the asm file then the function will prompt the error and return ERROR
the function assumes there is a .data in the given string*/
int dataToData( node_t *dataList,char * line,int lineNum)
{
    char* strptr=strstr(line, ".data");/*ptr to the current char in the string-( '.data' must be in line because it was checked earlier)*/
    int i,j=0;/* i is the index in strptr j is the index in tmp_number*/
    char tmp_number[BUFF_LEN]="";/* the max len of the number is the mwx len of the row*/
    int dataLen=0;
    int expectingComma=FALSE;/* if there was a white char after a number then a comma or \n must be followed*/
    for(i=5;strptr[i]!='\0';i++)/*strptr points to the starts of .data thus we need to set i to 5 -after .data*/
    {
        if(j==0&&isspace(strptr[i]))/*skips white spaces near the comma (from the right side)*/
        {
            continue;
        }
        if(j==0&&(strptr[i]=='-'||strptr[i]=='+'))
        {
            tmp_number[j]=strptr[i];
            j++;
        }
        else if(strptr[i]>='0'&&strptr[i]<='9')
        {
            if(expectingComma)
            {
                 fprintf(stdout,"In line %d: There is a missing comma apparently/ there are invalid white chars in between the number;\n",lineNum);
                 return ERROR;
            }
            tmp_number[j]=strptr[i];
            j++;
        }
        else if(isspace(strptr[i]))/* if there was a white char after a number then a comma or \n must be followed*/
        {
            expectingComma=TRUE;
        }
        else if(strptr[i]==',')
        {
            long int number;
            tmp_number[j]='\0';
            if(j==0)
            {
                 fprintf(stdout,"In line %d: Number must contain at least one digit;\n",lineNum);
                            return ERROR;
            }
            number=atoi(tmp_number);
            expectingComma=FALSE;
            if(number>(pow(2,WORD_SIZE)-1)/2||number<-(pow(2,WORD_SIZE)-1)/2)
                {

                            fprintf(stdout,"In line %d: Number is bigger then 2^%d;\n",lineNum,WORD_SIZE);
                            return ERROR;
                }
            push(dataList,number);
            dataLen++;
            j=0;/*reset the index of tmp_number, there is no need to delete data, next number will override*/
        }
        else/* invaid char*/
        {
             fprintf(stdout,"In line %d: Invalid char in the decleration of .data;\n",lineNum);
                 return ERROR;
        }
        

    }
    if(strptr[i]!=',')
    {
    long int number;
    tmp_number[j]='\0';
     if(j==0)
            {
                 fprintf(stdout,"In line %d: Number must contain at least one digit;\n",lineNum);
                            return ERROR;
            }
    number=atol(tmp_number);
    if(number>(pow(2,WORD_SIZE)-1)/2||number<-(pow(2,WORD_SIZE)-1)/2)
    {

                 fprintf(stdout,"In line %d: Number is bigger then 2^24;\n",lineNum);
                 return ERROR;
    }
    push(dataList,number);
    dataLen++;
    }

    

    return dataLen;
}



/* the function get an opperand as string and return its addressing type -- assumes the oper isn't empty */
enum addressingType opperand_get_type(char* oper)
{
    /* in case its a register*/
    if(oper[0]=='r'&&oper[1]!='\0'&&oper[1]>='0' &&oper[1]<='7'&&oper[2]=='\0')/* checking if oper is from the form r<0-7>*/
        return REGISTER;
    /* immediate addressing*/
    if(oper[0]=='#')
        return IMMEDIATE;
    /*relative addressing*/
    if(oper[0]=='&')
        return RELATIVE;
    if(oper[0]!='\0')
        return DIRECT;
    return NONE;


}
/*checks the syntax of command and return the number of memory words needed for the given line */
int checksyntax_get_mem_len(char* cmd,char* line,int lineNum)
{
    int i,j=0;
    char* operands_str=strstr(line,cmd)+strlen(cmd);
    char firstOp[BUFF_LEN];
    char secondOp[BUFF_LEN];
    int excpectingComma=0;
    for(i=0;operands_str[i]!='\0'&&operands_str[i]!=',';i++)
    {

        if(!isspace(operands_str[i]))
        {
            if(excpectingComma)
            {
                 fprintf(stdout,"In line %d: You are missing a comma apparently, you cannot have a 'space' in the middle of operand decleration;\n",lineNum);
                 return ERROR;
            }
            firstOp[j]=operands_str[i];
            j++;
        }
        else if(j!=0)
        {
            excpectingComma=1;
        }
        
    }
    firstOp[j]='\0';
    j=0;
    if(operands_str[i]==',')
    {i++;}
    for(;operands_str[i]!='\0'||(isspace(operands_str[i])&&j!=0);i++)
    {
        if(!isspace(operands_str[i]))
        {
            secondOp[j]=operands_str[i];
            j++;
        }
    }
    secondOp[j]='\0';
    /*checking there are no more chars*/
    for(;operands_str[i]!='\0';i++)
    {
        if(!isspace(operands_str[i]))
        {
             fprintf(stdout,"In line %d: Extraneous text after command;\n",lineNum);
                 return ERROR;
        }
    }

    /* calculating memory len after extracting operands from text*/
    {
        int memory_words=1;/*starts from one*/

        /*checking if firstOP isn't empty*/
        if(firstOp[0]!='\0')
        {   
            if(opperand_get_type(firstOp)!=REGISTER)
                memory_words++;/*amount of extra words for addressing type diffrent then register*/
            /* if its a 'register immediate addressing' there are no extra words to count*/
        }

        /*checking if secondOP isn't empty*/
        if(secondOp[0]!='\0')
        {   

            if(opperand_get_type(secondOp)!=REGISTER)
                memory_words++;/*amount of extra words for addressing type diffrent then register*/
            /* if its a 'register immediate addressing' there are no extra words to count*/
        }
        return memory_words;
    }


    




}


/* return TRUE if all the chars in the line are spaces or if the first char is ';'.
FALSE otherwise*/
int lineIsEmpty(char* line)
{
    int i=0;
    if(line[0]=='\0'||line[0]==';')
        return TRUE;
    for(;line[i]!='\0';i++)
    {
        if(!isspace(line[i]))
            return FALSE;
    }
    return TRUE;

}
void add_ICF_to_data(SymbolTable* st,int ICF)
{
    int i=0;
     for(;i<st->index;i++)/*runs over the table until st.index, searches for tag*/
    { 
        if((st->table)[i]->place==data)
            (st->table)[i]->value+=ICF;     
    }  
}
/* function gets a label string and checks if label is valid:
-start with alphabethic char
-contins only digits and alphabethics
-isn't declared yet (in the given symbol table
-less then 31 chars.
returns FALSE if label is not valid True otherwise
the function will prompt the error*/
int isLabelValid(SymbolTable *st, char* labelName,int lineNum)
{
    int i;
      if(labelExists(st,labelName))
        {
            fprintf(stdout,"In line %d: The label '%s' already exists;\n",lineNum,labelName);
                return FALSE;
    
        }
        if(strlen(labelName)>31)
        {
            fprintf(stdout,"In line %d: Label must be shorter then 31 chars;\n",lineNum);
            return FALSE;
        }
        if(!isalpha(labelName[0]))
        {
            fprintf(stdout,"In line %d: First char of label must be alphabethic;\n",lineNum);
            return FALSE;
        }
        for(i=0;labelName[i];i++)
        {
            if(!isalpha(labelName[i])&&!isdigit(labelName[i]))
            {
                fprintf(stdout,"In line %d: Label should only contain digits and alphabethic chars;\n",lineNum);
            return FALSE;
            }
        }
        return TRUE;


}