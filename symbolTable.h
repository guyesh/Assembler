
typedef enum {entry,external,none} sType;/*describe the symbol type*/
typedef enum {code,data} sPlace;/*describe the symbol place*/


/*symbol is a row in the symbol table and contains data about the symbol(tag-name,value-address,place[code,data] and type[external,entry,none])*/
typedef struct {
    char* tag;
    long int value;/* the value is long int because int can be 2 bytes which not enough for 24 bits*/
    sType type;/*entry/extern/none*/
    sPlace place;/*code/data*/
} Symbol;
/*the symbol table, should contains all the symbols in the asm code*/
typedef struct {
    Symbol** table;/*Symbol's pointers array*/
    int index;/*tells the index for the next symbol to add*/
}  SymbolTable;

void printTable(SymbolTable *st);/*function that helps debuging*/
int labelExists(SymbolTable *st,char* tag);
int addSymbol(SymbolTable* st,char* tag,int value, sType type,sPlace place,int line);
void addSymbolExtern(SymbolTable* st,char* tag,int value, sType type,sPlace place,int line);
void freeST(SymbolTable *st);
int updateSymbolEntry(SymbolTable *st,char* tag);
long int get_value(SymbolTable *st,char* tag);

