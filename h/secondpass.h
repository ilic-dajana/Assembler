#ifndef _SECONDPASS_H_
#define _SECONDPASS_H_

#include "data_types.h"
#include <stdio.h>
#include "firstpass.h"

#define MAX_SEC 4

typedef enum{
 	REL_16,
 	RELPC_16,
}RelType;

typedef struct{
	RelType type;
	long offset;
	long rel_num;
	int sym_num;
}RelocationRec;

typedef struct RelNode{
	RelocationRec record;
	struct RelNode* next;
}	RelocationRecNode;

typedef struct{
	RelocationRecNode* head;
	RelocationRecNode* tail;
	long cnt;
	long sec;
}RelocationTable;

RelocationTable *currreltab;
RelocationTable reltabls[MAX_SEC];
int secN;

void writeRelTabToFIle(FILE* file, int tabN, RelocationTable* reltab, SymbolTable* tab);

void addRecord(RelocationTable* tab, RelType type, long offset, int sym);

void deleteRelocationTable(RelocationTable* tab, int ntabs);

void secondPass(Line* parsedFile);

typedef struct code {
    unsigned char ins[4];
    int islong;
    struct code* next;
}Code;

Code* program;

void writeCode(FILE* file);
#endif