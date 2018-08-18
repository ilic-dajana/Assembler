#include "parser.h"
#include "data_types.h"
#include "helper.h"
#include "tables.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

static TokenNode* globalToken = NULL;
static Line* current  = NULL; 
static Token token;

Token getNext(){
	Token tok;
	if(!globalToken){
		tok.token_type = EOF;
		return tok;
	}else{
		strcpy(tok.name, globalToken->tok.name);
		tok.token_type = globalToken->tok.token_type;
		globalToken = globalToken->next;
	}

	return tok;
}

int checkTokenType(TokenType type){
	if(token.token_type == type)
		return 1;
	else
		return 0;
}

int checkIfCons(int mark){
	const char* cons = token.name;
	int intpart = atoi(cons);

	if(mark == 1){
		if(intpart == UINT_MAX + 1)
			return INT_MIN;
		else if(intpart > INT_MAX)
			error("const over max(L44, parser.c)");
		else 
			return intpart * mark;
	}
	else {
		if(intpart > UINT_MAX + 1 )
			error("const over min(L50, parser.c)");
		else if(intpart == UINT_MAX + 1)
			return INT_MAX;
		else 
			return intpart * mark;
	}

	return -1;
}

int isTypeSym(TokenType tt){
	 if (tt == T_PLUS || tt == T_MINUS || tt == T_LITERAL || tt == T_AMPERSAND || tt == T_SYMBOL 
	 	  || tt == T_ASTERISK || tt == T_REGISTER || tt == T_DOLLAR){
	 	return 1;
	 }
	 return 0;
}

Parameter* getParameter(){
	Parameter* parameter = (Parameter*) malloc(sizeof(Parameter));
	int mark = 1;
	parameter->regNo = 0;

	if(token.token_type == T_DOLLAR){
		parameter->ptype = PCREL;
		token = getNext();
		if(checkTokenType(T_SYMBOL)){
			strcpy(parameter->symbol, token.name);
			token = getNext();
		}
	}
	else if(token.token_type == T_ASTERISK){
		parameter->ptype = MEMDIR_CON;
		token = getNext();

		if(checkTokenType(T_PLUS))
			mark = 1;
		else if(checkTokenType(T_MINUS))
			mark = -1;

		token = getNext();
		parameter->value = checkIfCons(mark);
	}
	else if(token.token_type == T_AMPERSAND){
		parameter->ptype = IMMED_SYM;
		strcpy(parameter->symbol, token.name);
		token = getNext();
	}
	else if(token.token_type == T_SYMBOL){
		parameter->ptype = MEMDIR_SYM;
		strcpy(parameter->symbol, token.name);
		token = getNext();
	}
	else if(token.token_type == T_PLUS){
		mark = 1;
		parameter->ptype = NOPARAM;
		token = getNext();
	}
	else if(token.token_type == T_MINUS){
		mark = -1;
		parameter->ptype = NOPARAM;
		token = getNext();
	}
	else if(token.token_type == T_LITERAL){
		parameter->ptype = IMMED_CON;
		parameter->value = checkIfCons(mark);
		token = getNext();
	}
	else if(token.token_type ==T_REGISTER ) {
        parameter->ptype = REGDIR;
        parameter->regNo = atoi(token.name + 1);
        token = getNext();

        if (checkTokenType(T_LBRACKET)){
            token = getNext();

            if (token.token_type == T_PLUS){
                mark = 1;
                parameter->ptype = NOPARAM;
                token = getNext();
            }
            else if (token.token_type == T_MINUS){
                mark = -1;
                parameter->ptype = NOPARAM;
                token = getNext();
            }
            else if (token.token_type == T_LITERAL){
                parameter->ptype = IMMED_CON;
                parameter->value = checkIfCons(mark);
                token = getNext();
            }
            else if (token.token_type == T_SYMBOL){
                parameter->ptype = MEMDIR_SYM;
                strcpy(parameter->symbol, token.name);
                token = getNext();
            }
            else
                error("Unexpected token type(L147, parser.c)");
            if (!checkTokenType(T_RBRACKET))
                error("Unexpected token type(L149, parser.c)");
            else
                token = getNext();
	    }
	}
	else
		error("Parameter expected(L155, parser.c)");

return parameter;
}


void getParameters(){
	Parameter* head = NULL;
	Parameter* tail = NULL;
	Parameter* temp = NULL;
	int n = 0;;

	if(isTypeSym(token.token_type)){
		tail = head = getParameter();
		n++;
	}
	else{
		current->paramNo = 0;
		current->params = NULL;
		return;
	}

	while(checkTokenType(T_COMMA)){
		token = getNext();
        temp = getParameter();
		tail = tail->next = temp;
		n++;
	}
		current->paramNo = n;
		current->params = head;
}

void getcurrent(){

	if(checkTokenType(T_SYMBOL)){
		strcpy(current->label, token.name);
    current->type = O_LABEL;
		token = getNext();
		if(checkTokenType(T_COLON))
			token=getNext();
		else error("Unexpected token(L195, parser.c)");
	}else if(checkTokenType(T_DIRECTIVE)){
		current->type = O_DIRECTIVE;
		current->dir = search_for_directive(token.name);
		token = getNext();
		getParameters();
	}else if(checkTokenType(T_INSTRUCTION)){
		current->type = O_INSTRUCTION;
		current->ins = search_for_instruction(token.name);
		token = getNext();
		getParameters();
	}else if(checkTokenType(T_COMMENT))
			token = getNext();

	if(checkTokenType(T_NEWLINE))
		token = getNext();
	else
		error("Unexpected token(L212, parser.c)");
}

Line* parsing(TokenNode* tokenFile){
	globalToken = tokenFile;
	current = NULL;	
	Line* head = NULL; 
	Line* tail = NULL;

	token = getNext();

	while(1){
		if(token.token_type != EOF){
			current = (Line*) calloc(1, sizeof(Line));
			if(current == NULL)
				error("NULL pointer exception(L227, parser.c)");

			getcurrent(token);

			if(current->type !=  EMPTY){
				if(head)
					tail = tail->next = current;
				else
					head = tail = current;
			}
			else
				free(current);
		}else{
			break;
		}
	}
	
	return head;
}
