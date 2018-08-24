#ifndef _FIRSTPASS_H_
#define _FIRSTPASS_H_
#include "data_types.h"
#include <stdio.h>
//symbol table

#define SYM_MAX  32

typedef enum{
	ST_SECTION,
	ST_SYMBOL,
	ST_NON,
} SymbolType;


typedef struct 
{
	SymbolType type;
	char name[SYM_MAX];
	ScopeType sctype;
	long val;
	int num;
	long secNo;
}Symbol;

typedef struct SymNode{
	Symbol symbol;
	struct SymNode* next;
} SymbolNode;

typedef struct symtab {
	long n;
	SymbolNode* head;
	SymbolNode* tail;
}SymbolTable;

SymbolTable* tab;

Symbol* addSymbol(SymbolTable* tab, const char* name,long secNo, ScopeType sctype, long val );
Symbol* addSection(SymbolTable* tab, const char* name);

Symbol* findSymbol(SymbolTable* tab, const char* symbol);
Symbol* findSection(SymbolTable* tab, long secNo);

void writeSymTabToFile(SymbolTable* tab, FILE* file);

void deleteSymbolTable(SymbolTable *tab);
int sizeofinstruction();

int firstPass(Line* parsedFile);

#endif