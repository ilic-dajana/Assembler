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

Token getNextToken(){
	Token newToken ;

	while(*from_ptr){
		if(*from_ptr == ' ' || *from_ptr=='\t')
			from_ptr++;
	}

	if(*from_ptr == '+')
		newToken.token_type = T_PLUS;
	else if(*from_ptr == '-')
		newToken.token_type = T_MINUS;
	else if(*from_ptr == '*')
		newToken.token_type = T_ASTERISK;
	else if(*from_ptr == '$')
		newToken.token_type = T_DOLLAR;
	else if(*from_ptr == ';'){
		newToken.token_type = T_COMMENT;
		while(*from_ptr != '\n' && *from_ptr !='\0')
			from_ptr++;
	}
	else if(*from_ptr == ':')
		newToken.token_type = T_COLON;
	else if(*from_ptr == '&')
		newToken.token_type = T_AMPERSAND;
	else if(*from_ptr == ',')
		newToken.token_type = T_COMMA;
	else if(*from_ptr == '[')
		newToken.token_type = T_LBRACKET;
	else if(*from_ptr == ']')
		newToken.token_type = T_RBRACKET;
	else if(*from_ptr == '\0')
		newToken.token_type = T_EOF;
	else if(*from_ptr == '\n'){
		newToken.token_type = T_NEWLINE;
		line++;
	}else{
			


		if(!isalnum(*from_ptr) && *from_ptr != '_' && *from_ptr !='.'){
			newToken.token_type  = T_UNDEFINED;
			newToken.name[0] = '\0';
		}
		int i = 0;
		while(isalnum(*(from_ptr++)) || *from_ptr == '_' || *from_ptr == '.')
		{
			newToken.name[i] = *from_ptr;
			i++;
			if(i >= 32)
				error("max size literal reached");
		}
		newToken.name[i] = '\0';

		if(isdigit(newToken.name[0])){
			if(is_num_decimal(newToken.name)) newToken.token_type = T_LITERAL;
		}
		else{
	        if (strlen(newToken.name) > 7) newToken.token_type = T_SYMBOL;
	        else if (search_for_instruction(newToken.name) != NULL) newToken.token_type = T_INSTRUCTION;
	        else if (search_for_directive(newToken.name) != NULL) newToken.token_type = T_DIRECTIVE;
	        else if (search_for_register(newToken.name) != NULL) newToken.token_type = T_REGISTER;
	        else newToken.token_type = T_SYMBOL;		

	    }

	}

	return newToken;
}

TokenNode* make_token_list(){
	TokenNode *first = NULL;
	TokenNode *last = NULL;

	while(1){
		Token nextTok = getNextToken();
		TokenNode *node = NULL;
		node = (TokenNode*) malloc(sizeof(TokenNode));
		if(!node){
			 error("Error while allocating memory!");
			}
		if(nextTok.token_type == T_EOF){
			if(last->tok.token_type != T_NEWLINE){
				nextTok.token_type = T_NEWLINE;
				node->tok = nextTok;
				last->next = node;
				last = node;

			}else
				free(node);
		break;
	}
	if(nextTok.token_type == T_UNDEFINED)
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