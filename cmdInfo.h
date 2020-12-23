
#define CMD_MAX_LEN 5/* stop =5 including '/0'*/
#define NUM_OF_COMMANDS 16
#define NO_OPERAND(x) x[0]==FALSE&&x[1]==FALSE&&x[2]==FALSE&&x[3]==FALSE /*checks if the valid operator's addressing is FALSE for all options = no operator needed*/
/*define a row in the table*/
typedef struct {
    int opcode;
    int funct;
    char name[5];/*4 is the max size of a command name (+ \0)*/
    
    /*there are 4 addressing options*/
    int src_addressing[4];
    int dest_addressing[4];

} command;
enum addressingType {IMMEDIATE,DIRECT,RELATIVE,REGISTER,NONE};
int cmd_exists(char* name);
int cmd_to_opcode(char* name);
int cmd_to_funct(char* name);
int check_valid_addressing(char* cmd,enum addressingType src,enum addressingType des,int lineNum);