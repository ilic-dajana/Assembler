#include "firstpass.h"
#include "data_types.h"
#include "helper.c"
#include "error.h"
#include "parser.h"

#include <stdlib.h>

static SymbolTable tab;
static Symbol *currentSymbol;
static long cnt;
static long end;
static Line* currentLine;

Symbol* addSymbol(SymbolTable* tab,const char* name, long offset, Section section, ScopeType sctype, long val){

	if(!tab->head){
		SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));
		
		if(!node)
			error("Memory allocation problem");

		node->symbol.type = NON;		
		node->symbol.sctype = LOCAL;
		node->symbol.offset = 0;
		node->symbol.num = 0;
		node->symbol.section = SEC_NO_SECTION;

		tab->tail = tab->head = node;
		tab->head->next = NULL;
		tab->n = 1;
	}

	SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));

	if(!node)
		error("Memory allocation problem");

	strcpy(node->symbol.name, name);
	node->symbol.type = ST_SYMBOL;
	node->symbol.sctype = sctype;
	node->symbol.offset = offset;
	node->symbol.section = section;
	node->symbol.num = tab->n++;
	node->symbol.secNo = val;
	node->next = NULL;

	tab->tail = tab->tail->next = node;


	return &node->symbol;
}

Symbol* addSection(SymbolTable* tab, long ndx, Section section ){

	if(!tab->head){
		SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));
		
		if(!node)
			error("Memory allocation problem");

		node->symbol.type = NON;
		node->symbol.sctype = LOCAL;
		node->symbol.offset = 0;
		node->symbol.num = 0;
		node->symbol.section = SEC_NO_SECTION;
		
		tab->tail = tab->head = node;
		tab->head->next = NULL;
		tab->n = 1;
	}

	SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));

	if(!node)
		error("Memory allocation problem");

	
	node->symbol.type = ST_SECTION;
	node->symbol.sctype = LOCAL;
	node->symbol.offset = 0;
	node->symbol.section = section;
	node->symbol.num = tab->n++;
	node->next = NULL;
	node->symbol.secNo = node->symbol.num;
	tab->tail = tab->tail->next = node;


	return &node->symbol;
}

Symbol* findSymbol(SymbolTable* tab, const char* name){
	SymbolNode* temp = tab->head;

	while(temp){
		if(strcmp(temp->symbol.name, name))
			return &temp->symbol;
	}
	return NULL;
}

Symbol* findSection(SymbolTable* tab, long secNum){
	SymbolNode* temp = tab->head;

	while(temp){
		if(temp->symbol.secNo == secNum)
			return &temp->symbol;
	}
	return NULL;
}

void deleteSymbolTable(SymbolTable *tab){
	
	while(tab->head){
		SymbolNode* temp = tab->head;
		tab->head = tab->head->next ;
		free(temp);
	}
	tab->tail = NULL;
	tab->n = 0;
}

long sizeofinstruction(){
	if(currentLine->ins->ins != NON){

	}
}

int firstPass(Line* parsedFile){
	cnt = 0;
	tab.head = NULL;
	tab.tail = NULL;
	tab.n = 0;
	currentSymbol = NULL;

	end = 0;
	currentLine = parsedFile;

	while(!end && currentLine){
		if(currentLine->label != NULL){
			Symbol *symbol = findSymbol(&tab, currentLine->label);
			if(!symbol)
				addSymbol(&tab, currentLine->label, cnt, currentSymbol->section, LOCAL,  currentSymbol->num );
			else if(symbol->secNo != 0)
				error("mul def label");
			else {
				symbol->secNo = currentSymbol->secNo;
				symbol->offset = cnt;
			}

		}

		if(currentLine->type = O_INSTRUCTION){

			if(!currentSymbol)
				error("Unrecognized section");

			if(currentLine->paramNo != currentLine->ins->paramNo )
				error("top few arguments to instruction");

			if(currentLine->paramNo > 0){
				Parameter* param = currentLine->params;
				if(currentLine->ins->addrType == DST && (param->ptype == IMMED_SYM || param->ptype == IMMED_CON))
					error("Not compatible addr type with ins");
			
				if(((param->ptype == REGIND_CON || param->ptype == REGIND_SYM) || (param->ptype == PCREL)) && (currentLine->ins->ins == CALL || is_substr(currentLine->ins->ins_name, "JMP")))
					error("JMP instruction with uncompatible addressing");

				if(currentLine->paramNo > 1){
					if(param->next->ptype == PCREL && !(currentLine->ins->ins == CALL || is_substr(currentLine->ins->ins_name, "JMP")))
						error("pcrel with non jump func");

				}
			}

			cnt += sizeofinstruction();
		}

		if(currentLine->type = O_DIRECTIVE){
			
		}
		currentLine = currentLine -> next;
		
	}
	return 0;
}	