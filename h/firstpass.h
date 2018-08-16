#ifndef _FIRSTPASS_H_
#define _FIRSTPASS_H_
#include "data_types.h"
//symbol table

#define SYM_MAX = 32

typedef enum{
	SECTION,
	SYMBOL,
	NON,
} SymbolType;

typedef enum{
	NO_SECTION,
	TEXT,
	DATA,
	RODATA,
	BSS,
	END,
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

typedef struct SymNode {
	Symbol symbol;
	SymNode* next;
} SymbolNode;

typedef struct symtab {
	long n;
	SymbolNode* head;
	SymbolNode* tail;
}SymbolTable;

extern SymbolNode* symbolTable;

Symbol* addSymbol(SymbolTable* tab, const char* name, long offset, Section section, ScopeType sctype, long val );

Symbol* addSection(SymbolTable* tab, const char* name);

Symbol* findSymbol(SymbolTable* tab, const char* symbol);

Symbol* findSection(SymbolTable* tab, long secNum);

void deleteSymbolTable(SymbolTable *tab);

int firstPass(Line* parsedFile);

#endif