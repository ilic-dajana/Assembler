#include "parser.h"
#include "data_types.h"
#include "helper.h"
#include "tables.h"

#include <stdio.h>
#include <string.h>


static TokenNode* globalToken = NULL;
static Line* current  = NULL; 
static Token token;

Token getNextToken(){
	Token tok;
	if(!globalToken)
	{
		tok.name = NULL;
		tok.token_type = EOF;
		return rok;
	}else {
		tok.name = globalToken->tok.name;
		tok.token_type = globalToken->tok.token_type;
		globalToken = globalToken->next;
	}
	return tok;
}

int checkTokenType(TokenType type, Token token){
	if(token.token_type == type)
		return 1;
	else
		return 0;
}


void getLine(Token token){

	if(checkTokenType(SYMBOL, token)){
		line->label = token.name;

		token = getNextToken();

		if(checkTokenType(COLON, token))
			token=getNextToken();
		//error;
	}else if(checkTokenType(DIRECTIVE, token)){
		line->type = DIRECTIVE;
		line->dir = search_for_directives(token.name);
		token = getNextToken();
		getParameters();
	}else if(checkTokenType(INSTRUCTION, token)){
		line->type = INSTRUCTION;
		line->ins = search_for_directives(token.name);
		token = getNextToken();
		getParameters();
	}else if(checkTokenType(COMMENT, token))
			token = getNextToken();

	if(checkTokenType(NEWLINE, token))
		token = getNextToken();


}
int isTypeSym(TokenType tt){
	 if (tt == PLUS || tt == MINUS || tt == LITERAL || tt == AMPERSAND
            || tt == SYMBOL || tt == ASTERISK || tt == REGISTER || tt == DOLLAR){
	 	return 1;
	 }
	 return 0;
}

Parameter getParameter(){
	
}


void getParameters(){
	Parameter* head = NULL;
	Parameter* tail = NULL;
	int n;

	if(isTypeSym(token.token_type)){
		head = tail = (ParameterNode*) calloc(1, sizeof(ParameterNode));
		head->param = getParameter();
		n++;
	}
	else {
		line->paramNo = 0;
		line->params = NULL;
		return;
	}

	while(checkTokenType(COMMA)){
		token = getNextToken();
		tail->next = calloc(1. sizeof(ParameterNode));
		tail = tail->next;
		tail->param = getParameter();
		n++;
	}
		line->paramNo = n;
		line->params = head;


}

Line* parsing(TokenizedFile tokenFile){
	globalToken = tokenFile;
	line = NULL;
	
	Line* head = NULL; 
	Line* tail = NULL;
	token = getNextToken();

	while(1){
		if(token.token_type != EOF){
			current = (Line*) calloc(1, sizeof(Line));
			
			getLine(token);

			if(current->op !=  EMPTY){

				if(head)
					tail = tail->next = line;

				else
					first = tail = line;
			}
			else
				free(line);
		}else {
			break;
		}
	}
	return head;
}