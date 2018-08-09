#include "lexer.h"
#include "helper.h"
#include "data_types.h"
#include "tables.h"

static char* from_ptr = NULL;
static char* to_ptr = NULL;
int line = 0;

void delete_tokens(TokenizedFile file){
	TokenNode* temp ;
	TokenNode* first = file;

	while(first){
		temp = first;
		first = first->next;
		free(temp);
	}

}

Token getNextToken(){
	Token newToken = {NULL, UNDEFINED};

	while(*from_ptr){
		if(from_ptr == ' ' && *from_ptr!='\n')
			from_ptr++;
	}

	if(*from_ptr == '+')
		newToken.token_type = PLUS;
	else if(*from_ptr == '-')
		newToken.token_type = MINUS;
	else if(*from_ptr == '*')
		newToken.token_type = ASTERISK;
	else if(*from_ptr == '$')
		newToken.token_type = DOLLAR;
	else if(*from_ptr == ';'){
		newToken.token_type = COMMENT;
		while(*from_ptr != '\n' && *from_ptr !='\0')
			from_ptr++;
	}
	else if(*from_ptr == ':')
		newToken.token_type = COLON;
	else if(*from_ptr == '&')
		newToken.token_type = AMPERSAND;
	else if(*from_ptr == ',')
		newToken.token_type = COMMA;
	else if(*from_ptr == '[')
		newToken.token_type = LBRACKET;
	else if(*from_ptr == ']')
		newToken.token_type = RBRACKET;
	else if(*from_ptr == '\0')
		newToken.token_type = EOF;
	else if(*from_ptr == '\n'){
		newToken.token_type = NEWLINE
		line++;
	}else{
		Token tok = {to_ptr, UNDEFINED};
		*to_ptr++ = *from_ptr;

		if(!isalnum(*from_ptr) && *from_ptr!='_' && *from_ptr != '.'){
			*to_ptr++ = '\0';
			newToken = tok;
		}

		while(isalnum(*from_ptr) || *from_ptr == '_' || *from_ptr == '.'){
			*to_ptr++ = *from_ptr;
		}
		*to_ptr++ ='\0';

		--from_ptr;
		if(isdigit(*tok.name)){
			if(is_num_decimal(tok.name)) tok.token_type = LITERAL;
		}else{
			if(strlen(tok.name) > 31 ) tok.token_type = UNDEFINED;
			else if(strlen(tok.name) > 7) tok.token_type =  SYMBOL;
			else if(search_for_instruction(tok.name)!=NULL) tok.token_type = INSTRUCTION;
			else if(search_for_directive(tok.name) != NULL) tok.token_type = DIRECTIVE;
			else if(sear_for_registers(tok.name)!= NULL) tok.token_type = REGISTER;
			else tok.token_type = SYMBOL;
		}
		newToken = tok;
	}
	++from_ptr;

	return newToken;
}

TokenNode* sort_list(){
	TokenNode *first = NULL;
	TokenNode *last = NULL;

	while(true){
		Token nextTok = getNextToken();
		TokenNode *node = NULL;
		node = malloc(sizeof(TokenNode));
		if(!node)
			return NULL;

		if(nextTok.token_type == EOF){
			if(last->token_type != NEWLINE){
				nextTok.token_type = NEWLINE;
				node->tok = t;
				last->next = node;
				last = node;

			}else
				free(node);
		break;
	}
	node->tok = t;
	node->next = NULL;
	if(!first) 
		first=last =node;
	else 
		last = last->next = node;
}
	return first;
}