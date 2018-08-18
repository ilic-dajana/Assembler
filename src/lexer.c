#include "lexer.h"
#include "helper.h"
#include "data_types.h"
#include "tables.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define  MAX_LITERAL 32
static char* from_ptr = NULL;


void delete_tokens(TokenNode* file){
	TokenNode* temp ;
	TokenNode* head = file;

	while(head){
		temp = head;
		head = head->next;
		free(temp);
	}
}

Token getNextToken(){
	Token newToken;

	for(int i =0; i < MAX_NAME_SIZE; i++)
	    newToken.name[i] = '\0';

	while(*from_ptr) {
		if (*from_ptr == ' ' || *from_ptr == '\t')
			from_ptr++;
		else
			break;
	}

	if (*from_ptr == '+')
		newToken.token_type = T_PLUS;
	else if (*from_ptr == '-')
		newToken.token_type = T_MINUS;
	else if (*from_ptr == '*')
		newToken.token_type = T_ASTERISK;
	else if (*from_ptr == '$')
		newToken.token_type = T_DOLLAR;
	else if (*from_ptr == ';') {
		newToken.token_type = T_COMMENT;
		while (*from_ptr != '\n' && *from_ptr != '\0')
			from_ptr++;
	} else if (*from_ptr == ':')
		newToken.token_type = T_COLON;
	else if (*from_ptr == '&')
		newToken.token_type = T_AMPERSAND;
	else if (*from_ptr == ',')
		newToken.token_type = T_COMMA;
	else if (*from_ptr == '[')
		newToken.token_type = T_LBRACKET;
	else if (*from_ptr == ']')
		newToken.token_type = T_RBRACKET;
	else if (*from_ptr == '\0')
		newToken.token_type = T_EOF;
	else if (*from_ptr == '\n') {
		newToken.token_type = T_NEWLINE;
	}
	else {
		if (!isalnum(*from_ptr) && *from_ptr != '_' && *from_ptr != '.') {
			newToken.token_type = T_UNDEFINED;
			return newToken;
		}
		int i = 0;
		while (isalnum(*(from_ptr)) || *from_ptr == '_' || *from_ptr == '.') {
            newToken.name[i++] = *from_ptr++;
            if (i >= MAX_LITERAL)
                error("Max size literal reached L75 Flexer.c");
        }

		if (isdigit(newToken.name[0])) {
			if (is_num_decimal(newToken.name)) newToken.token_type = T_LITERAL;
		}
		else {
			for(int j = 0; j < i; j++)
				newToken.name[j] = (char) toupper(newToken.name[j]);
			if(strlen(newToken.name) > 7) newToken.token_type =  T_SYMBOL;
			else if (search_for_instruction(newToken.name) != NULL) newToken.token_type = T_INSTRUCTION;
			else if (search_for_directive(newToken.name) != NULL) newToken.token_type = T_DIRECTIVE;
			else if (search_for_register(newToken.name) != NULL) newToken.token_type = T_REGISTER;
			else newToken.token_type = T_SYMBOL;
		}
		from_ptr--;
	}

	from_ptr++;
	return newToken;
}

TokenNode* make_token_list(){
	TokenNode *head = NULL;
	TokenNode *tail = NULL;

	while(1){
		Token nextTok = getNextToken();
		TokenNode *node = NULL;
		node = (TokenNode*) malloc(sizeof(TokenNode));
		if(!node){
			 error("NULL pointer exception(L106, lexer.c)");
			}

		if(!head)
			head = tail =node;
		else
			tail = tail->next = node;

		if(nextTok.token_type == T_EOF){
			if(tail->tok.token_type != T_NEWLINE){
				nextTok.token_type = T_NEWLINE;
				node->tok = nextTok;
				tail->next = node;
				tail = node;
				tail->next = NULL;
			}
			else
				free(node);
			break;
		}
		if(nextTok.token_type == T_UNDEFINED) {
			error("UNDEFINED token type (L127, lexer.c)");
		}
		node->tok = nextTok;
		node->next = NULL;
	}

	return head;
}

TokenNode* tokenizingFile(Buffer filebuf){
	from_ptr = filebuf.buff;

	TokenNode* list_of_tokens = make_token_list();

	if(list_of_tokens == NULL)
		error("NULL pointer exception(L142, lexer.c)");

	from_ptr = NULL;
	return list_of_tokens;
}