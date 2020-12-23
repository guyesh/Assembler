#include "allfiles.h"
#include <math.h>
#include "linkedList.h"
#include "symbolTable.h"
#include "cmdInfo.h"

/* functions from fp_func used in second pass*/
int lineIsEmpty(char* line);


/* function from sp_func used in second pass*/
int isDataStrExtern(char* str);
int isEntry(char* str,int lineNum);
char* getEntryLabel(char* str,int lineNum);
char* removeFileEx(char* fileName);
void strreverse(char *str);
void int_to_bits(char* output,long int num,int sizeOfOutput);
int check_if_number_valid(char* num);
char* operand_to_bits(char* oper, enum addressingType addressing,SymbolTable* st,int lineNum,FILE* extFile,long int operExtraAddress);
void hex(char* bin);
int codeTheWord(long int lineAddress,char* cmd,enum addressingType org_addressing,char* oper_org_content,enum addressingType des_addressing,char* oper_des_content,FILE* ob_file);
int codeTheLine(char* line,int lineNum,SymbolTable* st,FILE*  obFile,FILE* externFile,int lineAddress);
long int codeData(node_t * head,FILE* ob_file,long int address);
void codeEntry(SymbolTable* st,FILE* ent_file);
