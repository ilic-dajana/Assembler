#ifndef _SECONDPASS_H_
#define _SECONDPASS_H_

#include "data_types.h"
#include <stdio.h>
typedef enum{
 	REL_16,
 	RELPC_16,
}RelType;

typedef struct{
	RelType type;
	long offset;
	int rel_num;
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

extern RelocationTable *reltab;

void writeRelTabToFIle(FILE* file, RelocationTable* reltab);

int addRecord(RelocationTable* tab, RelType type, long offset, int sym);

void deleteRelocationTable(RelocationTable* tab, int ntabs);

void secondPass(Line* parsedFile);
#endif