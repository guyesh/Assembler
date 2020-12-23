#include "cmdInfo.h"
#include "allfiles.h"
#include <stdio.h>

/*the table from page 34:
╔════════╦═══════╦════════╦═══════════════════════════╦═══════════════════════════╗
║ opcode ║ funct ║ name   ║ src addressing            ║ dest addressing           ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 0      ║ 0     ║ "mov"  ║ {TRUE,TRUE,FALSE,TRUE}    ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 1      ║ 0     ║ "cmp"  ║ {TRUE,TRUE,FALSE,TRUE}    ║ {TRUE,TRUE,FALSE,TRUE}    ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 2      ║ 1     ║ "add"  ║ {TRUE,TRUE,FALSE,TRUE}    ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 2      ║ 2     ║ "sub"  ║ {TRUE,TRUE,FALSE,TRUE}    ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 4      ║ 0     ║ "lea"  ║ {FALSE,TRUE,FALSE,FALSE}  ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 5      ║ 1     ║ "clr"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 5      ║ 2     ║ "not"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 5      ║ 3     ║ "inc"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 5      ║ 4     ║ "dec"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 9      ║ 1     ║ "jmp"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,TRUE,FALSE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 9      ║ 2     ║ "bne"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,TRUE,FALSE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 9      ║ 3     ║ "jsr"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,TRUE,FALSE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 12     ║ 0     ║ "red"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,TRUE,FALSE,TRUE}   ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 13     ║ 0     ║ "prn"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {TRUE,TRUE,FALSE,TRUE}    ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 14     ║ 0     ║ "rts"  ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,FALSE,FALSE,FALSE} ║
╠════════╬═══════╬════════╬═══════════════════════════╬═══════════════════════════╣
║ 15     ║ 0     ║ "stop" ║ {FALSE,FALSE,FALSE,FALSE} ║ {FALSE,FALSE,FALSE,FALSE} ║
╚════════╩═══════╩════════╩═══════════════════════════╩═══════════════════════════╝
*/
command cmd_arr[]={{0,0,"mov",{TRUE,TRUE,FALSE,TRUE},{FALSE,TRUE,FALSE,TRUE}},{1,0,"cmp",{TRUE,TRUE,FALSE,TRUE},{TRUE,TRUE,FALSE,TRUE}},{2,1,"add",{TRUE,TRUE,FALSE,TRUE},{FALSE,TRUE,FALSE,TRUE}},{2,2,"sub",{TRUE,TRUE,FALSE,TRUE},{FALSE,TRUE,FALSE,TRUE}},{4,0,"lea",{FALSE,TRUE,FALSE,FALSE},{FALSE,TRUE,FALSE,TRUE}},{5,1,"clr",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,FALSE,TRUE}},{5,2,"not",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,FALSE,TRUE}},{5,3,"inc",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,FALSE,TRUE}},{5,4,"dec",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,FALSE,TRUE}},{9,1,"jmp",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,TRUE,FALSE}},{9,2,"bne",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,TRUE,FALSE}},{9,3,"jsr",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,TRUE,FALSE}},{12,0,"red",{FALSE,FALSE,FALSE,FALSE},{FALSE,TRUE,FALSE,TRUE}},{13,0,"prn",{FALSE,FALSE,FALSE,FALSE},{TRUE,TRUE,FALSE,TRUE}},{14,0,"rts",{FALSE,FALSE,FALSE,FALSE},{FALSE,FALSE,FALSE,FALSE}},{15,0,"stop",{FALSE,FALSE,FALSE,FALSE},{FALSE,FALSE,FALSE,FALSE}}};
/*the function checks if command exists return TRUE if exists FALSE otherwise*/
int cmd_exists(char* name)
{
    int cmd_arr_len=NUM_OF_COMMANDS;
    int i=0;
    for(;i<cmd_arr_len;i++)
    {
       
        if(strcmp(cmd_arr[i].name,name)==0)
            return TRUE;
    }
    return FALSE;
}
int cmd_to_funct(char* name)
{
    int cmd_arr_len=NUM_OF_COMMANDS;
    int i=0;
    for(;i<cmd_arr_len;i++)
    {
        if(strcmp(cmd_arr[i].name,name)==0)
            return cmd_arr[i].funct;
    }
    return ERROR;
}
int cmd_to_opcode(char* name)
{
    int cmd_arr_len=NUM_OF_COMMANDS;
    int i=0;
    for(;i<cmd_arr_len;i++)
    {
        if(strcmp(cmd_arr[i].name,name)==0)
            return cmd_arr[i].opcode;
    }
    return ERROR;
}

/* function gets the command name and the addressing type of two operators and returns TRUE if function is valid and ERROR if there was an error*/
int check_valid_addressing(char* cmd,enum addressingType src,enum addressingType des,int lineNum)
{
    int cmd_arr_len=NUM_OF_COMMANDS;
    int i=0;
    int rtn=TRUE;
 
    for(;i<cmd_arr_len;i++)
    {
        if(strcmp(cmd_arr[i].name,cmd)==0)
        {
            /* checks if function gets src opernad*/
            if(NO_OPERAND(cmd_arr[i].src_addressing)&&src!=NONE)
            {
                 fprintf(stdout,"In line %d: Function %s doesn't get a source operand;\n",lineNum,cmd);
                 rtn=ERROR;
                    
            }
            else if(src!=NONE&&!cmd_arr[i].src_addressing[src])
            {
                fprintf(stdout,"In line %d: Invalid addressing type(source operand) in function %s;\n",lineNum,cmd);
                rtn=ERROR;
            }
            else if(src==NONE&&!(NO_OPERAND(cmd_arr[i].src_addressing)))
            {
                fprintf(stdout,"In line %d: Function %s suppose to get a source operand;\n",lineNum,cmd);
                rtn=ERROR;
            }           
            if(NO_OPERAND(cmd_arr[i].dest_addressing)&&des!=NONE)
            {
                 fprintf(stdout,"In line %d: Function %s doesn't get a target operand;\n",lineNum,cmd);
                 rtn=ERROR;
                    
            }
            else if(des!=NONE&&!cmd_arr[i].dest_addressing[des])
            {
                fprintf(stdout,"In line %d: Invalid addressing type(target operand) in function %s;\n",lineNum,cmd);
                rtn=ERROR;
            }
            else if(des==NONE&&!(NO_OPERAND(cmd_arr[i].dest_addressing)))
            {
                fprintf(stdout,"In line %d: Function %s suppose to get a target operand;\n",lineNum,cmd);
                rtn=ERROR;
            }
            
            return rtn;
        }
        
    }
    return FALSE;/*command not found*/
}