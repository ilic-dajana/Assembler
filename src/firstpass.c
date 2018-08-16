#include "firstpass.h"
#include "data_types.h"


Symbol* addEntry(SymbolTable* tab,const char* name, long offset, Section section, ScopeType sctype ){

	if(!tab->head){
		SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));
		
		if(!node)
			error("Memory allocation problem");

		node->symbol.type = NON;
		node->symbol.name = NULL;
		node->symbol.sctype = LOCAL;
		node->symbol.offset = 0;
		node->symbol.num = 0;
		node->symbol.section = NO_SECTION;

		tab->tail = tab->head = node;
		tab->head->next = NULL;
		tab->n = 1;
	}

	SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));

	if(!node)
		error("Memory allocation problem");

	strcpy(node->symbol.name, name);
	node->symbol.type = SYMBOL;
	node->symbol.sctype = sctype;
	node->symbol.offset = offset;
	node->section = section;
	node->n++;
	node->next = NULL;

	tab->last = tab->tail->next = node;


	return node->symbol;
}

Symbol* addSection(SymbolTable* tab, const char* name ){

	if(!tab->head){
		SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));
		
		if(!node)
			error("Memory allocation problem");

		node->symbol.type = NON;
		node->symbol.name = NULL;
		node->symbol.sctype = LOCAL;
		node->symbol.offset = 0;
		node->symbol.num = 0;
		node->symbol.section = NO_SECTION;

		tab->tail = tab->head = node;
		tab->head->next = NULL;
		tab->n = 1;
	}

	SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));

	if(!node)
		error("Memory allocation problem");

	strcpy(node->symbol.name, name);
	node->symbol.type = SECTION;
	node->symbol.sctype = sctype;
	node->symbol.offset = offset;
	node->section = section;
	node->n++;
	node->next = NULL;

	tab->last = tab->tail->next = node;


	return node->symbol;
}

