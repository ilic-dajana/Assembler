#ifndef _CODE_H_
#define _CODE_H_

#include "data_types.h"

typedef struct code{
    int offset;
    int symbol;
    struct code* next;
}CodeNode;

typedef struct{
    CodeNode* head;
    CodeNode* tail;
    int cnt;
}Code;

void addNode(Code* cd, int offset, int symbol);

void programCode(Line* parsedOutput, char* filename);


#endif