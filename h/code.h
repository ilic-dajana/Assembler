#ifndef _CODE_H_
#define_CODE_H_


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

void programCode(Line* paserdOutput, const char* filename);


#endif