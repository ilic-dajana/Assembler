#ifndef _LEXER_H_
#define _LEXER_H_

#include "data_types.h"
#include "tables.h"



TokenNode* tokenizingFile(Buffer file);
void delete_tokens(TokenNode* tokFile);

#endif