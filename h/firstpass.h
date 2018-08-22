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

typedef enum{
	SEC_NO_SECTION,
	SEC_TEXT,
	SEC_DATA,
	SEC_RODATA,
	SEC_BSS,
	SEC_END,
} Section;

typedef struct 
{
	SymbolType type;
	char name[SYM_MAX];
	ScopeType sctype;
	long offset;
	long num;
	Section section;
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

extern SymbolTable* tab;

Symbol* addSymbol(SymbolTable* tab, const char* name, long offset, Section section, ScopeType sctype, long val );

Symbol* findSymbol(SymbolTable* tab, const char* symbol);

void writeSymTabToFile(SymbolTable* tab, FILE* file);


void deleteSymbolTable(SymbolTable *tab);

int firstPass(Line* parsedFile);

#endif