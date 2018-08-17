#include "lexer.h"
#include "helper.h"
#include "data_types.h"
#include "tables.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


static char* from_ptr = NULL;
static int TOKEN_SIZE = 31;
int line = 0;

void delete_tokens(TokenNode* file){
	TokenNode* temp ;
	TokenNode* first = file;

	while(first){
		temp = first;
		first = first->next;
		free(temp);
	}

}

Token* getNextToken(){
	Token* newToken = (Token*)malloc(sizeof(Token));
	if(!newToken)
		error("Error while allocating memory");
	char* to_ptr;
	while(*from_ptr){
		if(*from_ptr == ' ' || *from_ptr=='\t')
			from_ptr++;
	}

	if(*from_ptr == '+')
		newToken->token_type = PLUS;
	else if(*from_ptr == '-')
		newToken->token_type = MINUS;
	else if(*from_ptr == '*')
		newToken->token_type = ASTERISK;
	else if(*from_ptr == '$')
		newToken->token_type = DOLLAR;
	else if(*from_ptr == ';'){
		newToken->token_type = COMMENT;
		while(*from_ptr != '\n' && *from_ptr !='\0')
			from_ptr++;
	}
	else if(*from_ptr == ':')
		newToken->token_type = COLON;
	else if(*from_ptr == '&')
		newToken->token_type = AMPERSAND;
	else if(*from_ptr == ',')
		newToken->token_type = COMMA;
	else if(*from_ptr == '[')
		newToken->token_type = LBRACKET;
	else if(*from_ptr == ']')
		newToken->token_type = RBRACKET;
	else if(*from_ptr == '\0')
		newToken->token_type = EOF;
	else if(*from_ptr == '\n'){
		newToken->token_type = NEWLINE;
		line++;
	}else{
		Token* tok =(Token*) malloc(sizeof(Token));
		tok->name = NULL;
		tok->token_type = UNDEFINED;
		 strcpy(*to_ptr, *from_ptr);

		if(!isalnum(*from_ptr) && *from_ptr!='_' && *from_ptr != '.'){
			*to_ptr++ = '\0';
			newToken =  tok;
		}

		while(isalnum(*from_ptr) || *from_ptr == '_' || *from_ptr == '.'){
			*to_ptr++ = *from_ptr++;
		}
		*to_ptr++ ='\0';

	
		tok->name = to_ptr; ///????
		to_ptr = NULL;
		if(sizeof(tok))
			tok->token_type = UNDEFINED;
		else{
			if(isdigit(*tok->name)){
				if(is_num_decimal(tok->name)) tok->token_type = LITERAL;
				}else{
					if(strlen(tok->name) > 31 ) tok->token_type = UNDEFINED;
					else if(strlen(tok->name) > 7) tok->token_type =  SYMBOL;
					else if(search_for_instruction(tok->name)!=NULL) tok->token_type = INSTRUCTION;
					else if(search_for_directive(tok->name) != NULL) tok->token_type = DIRECTIVE;
					else if(search_for_register(tok->name)!= NULL) tok->token_type = REGISTER;
					else tok->token_type = SYMBOL;
				}
	}
		newToken->name = tok->name;
		newToken->token_type = tok->token_type;
	}
	++from_ptr;

	return newToken;
}

TokenNode* make_token_list(){
	TokenNode *first = NULL;
	TokenNode *last = NULL;

	while(1){
		Token* nextTok = getNextToken();
		TokenNode *node = NULL;
		node = (TokenNode*) malloc(sizeof(TokenNode));
		if(!node){
			 error("Error while allocating memory!");
			}
		if(nextTok->token_type == EOF){
			if(last->tok->token_type != NEWLINE){
				nextTok->token_type = NEWLINE;
				node->tok = nextTok;
				last->next = node;
				last = node;

			}else
				free(node);
		break;
	}
	if(nextTok->token_type == UNDEFINED)
	{
		error("Error: UNDEFINED token");
	}
	node->tok = nextTok;
	node->next = NULL;
	if(!first) 
		first=last =node;
	else 
		last = last->next = node;
}
	return first;
}

TokenNode* tokenizingFile(Buffer filebuf){
	from_ptr = filebuf.buff;
	TokenNode* token_file ;

	line = 1;
	TokenNode* list_of_tokens = make_token_list();

	if(list_of_tokens == NULL)
		exit(1);
	else{
		token_file = (TokenNode*) list_of_tokens;	
	}

	line = 0;
	from_ptr = 0;
	return token_file;
}