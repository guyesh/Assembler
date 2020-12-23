#include "symbolTable.h"
#include "allfiles.h"

void printTable(SymbolTable *st)
{
    int i=0;
     printf("\n|tag|val|place|type|\n");
     for(;i<st->index;i++)/*runs over the table until st.index*/
    {
        printf("|%s|%ld|%d|%d|\n",st->table[i]->tag,st->table[i]->value,st->table[i]->place,st->table[i]->type);/*prints table*/
    }
}
/*checks if a label exists in the symbol table*/
int labelExists(SymbolTable *st,char* tag){
 
    int i=0;
    for(;i<st->index;i++)/*runs over the table until st.index, searches for tag*/
    {
        if(!strcmp(st->table[i]->tag,tag))
        {
            
            return TRUE;
        }

    }
    return FALSE;
}
/* if there is already a symbol with the same name it will do nothing*/
void addSymbolExtern(SymbolTable* st,char* tag,int value, sType type,sPlace place,int line)
{
    if(labelExists(st,tag))
    {
        return ;
    }
    {
    Symbol* new_s= mallocAndCheck(sizeof(Symbol));
    new_s->tag=(char*)mallocAndCheck(sizeof(char)*(strlen(tag)+1));
    memset(new_s->tag, 0, strlen(tag)+1);/*zeroing the string, if we wont strncpy may causes problems*/
    strcpy(new_s->tag,tag);/*copying the labelname to a new location - for encapsulation*/
    new_s->value=value;
    new_s->type=type;
    new_s->place=place;

    st->table=reallocAndCheck(st->table,sizeof(Symbol*)*(st->index+1));
    *(st->table+st->index)=new_s;
    st->index++;
    return ;
    }
    

}
/*the function add a symbol to the table returns ERROR if there was an error*/
int addSymbol(SymbolTable* st,char* tag,int value, sType type,sPlace place,int line)
{
    if(labelExists(st,tag))
    {
        fprintf(stdout,"In line %d: The label '%s' already exists;\n",line,tag);
        return ERROR;
    }
    {
    Symbol* new_s= mallocAndCheck(sizeof(Symbol));
    new_s->tag=(char*)mallocAndCheck(sizeof(char)*(strlen(tag)+1));
    memset(new_s->tag, 0, strlen(tag)+1);/*zeroing the string, if we wont strncpy may causes problems*/
    strcpy(new_s->tag,tag);/*copying the labelname to a new location - for encapsulation*/
    new_s->value=value;
    new_s->type=type;
    new_s->place=place;

    st->table=reallocAndCheck(st->table,sizeof(Symbol*)*(st->index+1));
    *(st->table+st->index)=new_s;
    st->index++;
    return TRUE;
    }
    

}
/*the function returns the value of the symbol, if it dosent exists return ERROR*/
long int get_value(SymbolTable *st,char* tag)
{
     int i=0;
    for(;i<st->index;i++)/*runs over the table until st.index, searches for tag*/
    {
        if(!strcmp(st->table[i]->tag,tag))
        {
            return st->table[i]->value;
        }
    }
    return ERROR;
}
void freeST(SymbolTable *st)
{
    if(st->index!=0)
    {
        int i;
        for(i=0;i<st->index;i++)
        {
            free((st->table[i])->tag);
        
            free(st->table[i]);
        }
        free(st->table);
        /*st wasn't allocated in the heap so doesn't need to be freed*/
    }
}

/* the function replace the type of a given symbol(by label) to entry
It returns TRUE when succeeding and false otherwise*/
int updateSymbolEntry(SymbolTable *st,char* tag)
{
     int i=0;
    for(;i<st->index;i++)/*runs over the table until st.index, searches for tag*/
    {
        if(!strcmp(st->table[i]->tag,tag))
        {
            st->table[i]->type=entry;
            return TRUE;
        }

    }
    return FALSE;
}

