#include "allfiles.h"
#include "linkedList.h"
#include "symbolTable.h"
#include "cmdInfo.h"

/*functions from fp_func that are used in first pass*/
void secondPass(int ICF,int DCF,SymbolTable* st,node_t* dataList,FILE* file,char* fileName);
int hasSymbol(char* buff,int line);
int isDataInstruction(char* buffer,int line,int hasLabel);
char* extractLabelFromExtern(char* line,int lineNum);
char* extractNamefromCommand(char* line,int lineNum,int isSymbol);
int has_entry_extern(char* line,int lineNum);
char* extractLabelFromRow(char* row);
int stringToData( node_t *dataList,char * line,int lineNum);
int dataToData( node_t *dataList,char * line,int lineNum);
enum addressingType opperand_get_type(char* oper);
int checksyntax_get_mem_len(char* cmd,char* line,int lineNum);
int lineIsEmpty(char* line);
void add_ICF_to_data(SymbolTable* st,int ICF);
int isLabelValid(SymbolTable *st, char* labelName,int lineNum);
