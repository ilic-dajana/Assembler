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

Token getNextToken(){
	Token tok;
	if(!globalToken)
	{
		tok.name = NULL;
		tok.token_type = EOF;
		return tok;
	}else {
		tok.name = globalToken->tok->name;
		tok.token_type = globalToken->tok->token_type;
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
	long intpart = strtol(cons, NULL, 10);
	token = getNextToken();

	if(mark == 1){
		if(intpart == UINT_MAX + 1)
			return INT_MIN;
		else if(intpart > INT_MAX)
			error("Error const over max");
		else 
			return (int)cons * mark;
	}
	else {
		if(intpart > UINT_MAX + 1 )
			error("Error cost over min");
		else if(intpart == UINT_MAX + 1)
			return INT_MAX;
		else 
			return mark * (int)cons;
	}

	return -1;
}

int isTypeSym(TokenType tt){
	 if (tt == PLUS || tt == MINUS || tt == LITERAL || tt == AMPERSAND
            || tt == SYMBOL || tt == ASTERISK || tt == REGISTER || tt == DOLLAR){
	 	return 1;
	 }
	 return 0;
}

Parameter* getParameter(){
	Parameter* parameter = (Parameter*) malloc(sizeof(Parameter));
	int mark = 1;

	parameter->regNo = 0;

	if(token.token_type == DOLLAR){
		parameter->ptype = PCREL;
		token = getNextToken();
		if(checkTokenType(SYMBOL)){
			parameter->symbol = token.name;
			token = getNextToken();
		}
	}else if(token.token_type == ASTERISK){
		parameter->ptype = MEMDIR_CON;
		token = getNextToken();
		if(checkTokenType(PLUS))
			mark = 1;
		else if(checkTokenType(MINUS))
			mark = -1;
		token = getNextToken();

		parameter->value = checkIfCons(mark);
	}else if(token.token_type == AMPERSAND){
		parameter->ptype = IMMED_SYM;
		parameter->symbol = token.name;
		token = getNextToken();
	}else if(token.token_type == SYMBOL){
		parameter->ptype = MEMDIR_SYM;
		parameter->symbol = token.name;
		token = getNextToken();
	}else if(token.token_type == PLUS){
		mark = 1;
		parameter->ptype = NOPARAM;
		parameter->symbol = NULL;
		token = getNextToken();
	}else if(token.token_type == MINUS){
		mark = -1;
		parameter->ptype = NOPARAM;
		parameter->symbol = NULL;
		token = getNextToken();
	}else if(token.token_type == LITERAL){
		parameter->ptype = IMMED_CON;
		parameter->value = checkIfCons(mark);
	}else if(token.token_type == REGISTER ){
		parameter->ptype = REGDIR;
		parameter->regNo = atoi(token.name + 1);
		token = getNextToken();

		if(checkTokenType(LBRACKET)){

			token = getNextToken();

			if(token.token_type == PLUS){
			mark = 1;
			parameter->ptype = NOPARAM;
			parameter->symbol = NULL;
			token = getNextToken();
		}else if(token.token_type == MINUS){
			mark = -1;
			parameter->ptype = NOPARAM;
			parameter->symbol = NULL;
			token = getNextToken();
		}else if(token.token_type == LITERAL){
			parameter->ptype = IMMED_CON;
			parameter->value = checkIfCons(mark);
			}
		}else if(token.token_type == SYMBOL){
			parameter->ptype = MEMDIR_SYM;
			parameter->symbol = token.name;
			token = getNextToken();
		}else
			error("Unexpected token type");
		if(!checkTokenType(RBRACKET))
			error("Unexpected token type");
		else
			token = getNextToken();

	}else
		error("Parameter expected");

return parameter;
}


void getParameters(){
	Parameter* head = NULL;
	Parameter* tail = NULL;
	int n;

	if(isTypeSym(token.token_type)){
		head = tail = (Parameter*) calloc(1, sizeof(Parameter));
		if(!head || !tail)
			error("Error while allocating memory");
		head = getParameter();
		n++;
	}
	else {
		current->paramNo = 0;
		current->params = NULL;
		return;
	}

	while(checkTokenType(COMMA)){
		token = getNextToken();
		tail->next = calloc(1, sizeof(Parameter));
		if(tail->next == NULL)
			error("Error while allocating memory");
		tail = tail->next;
		tail= getParameter();
		n++;
	}
		current->paramNo = n;
		current->params = head;
}


void getcurrent(Token token){

	if(checkTokenType(SYMBOL)){
		current->label = token.name;

		token = getNextToken();

		if(checkTokenType(COLON))
			token=getNextToken();
		else error("Unexpected token");
	}else if(checkTokenType(DIRECTIVE)){
		current->type = O_DIRECTIVE;
		current->dir = search_for_directive(token.name);
		token = getNextToken();
		getParameters();
	}else if(checkTokenType(INSTRUCTION)){
		current->type = INSTRUCTION;
		current->ins = search_for_instruction(token.name);
		token = getNextToken();
		getParameters();
	}else if(checkTokenType(COMMENT))
			token = getNextToken();

	if(checkTokenType(NEWLINE))
		token = getNextToken();
	else
		error("Unexpected token");

}




Line* parsing(TokenNode* tokenFile){
	globalToken = tokenFile;
	current = NULL;
	
	Line* head = NULL; 
	Line* tail = NULL;
	token = getNextToken();

	while(1){
		if(token.token_type != EOF){
			current = (Line*) calloc(1, sizeof(current));
			if(current == NULL)
				error("Error while allocating memory");
			getcurrent(token);

			if(current->type !=  EMPTY){

				if(head)
					tail = tail->next = current;

				else
					head = tail = current;
			}
			else
				free(current);
		}else {
			break;
		}
	}
	return head;
}