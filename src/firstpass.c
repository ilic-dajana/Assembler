#include "firstpass.h"
#include "data_types.h"
#include "helper.c"
#include "error.h"
#include "parser.h"

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
	node->symbol.num = tab->n++;
	node->symbol.secNo = val;
	node->next = NULL;

	tab->last = tab->tail->next = node;


	return &node->symbol;
}

Symbol* addSection(SymbolTable* tab, long ndx, Section section ){

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
	node->symbol.offset = 0;
	node->section = section;
	node->symbol.num = tab->n++;
	node->next = NULL;
	node->symobol.secNo = node->symbol.num;
	tab->last = tab->tail->next = node;


	return &node->symbol;
}

Symbol* findSymbol(SymbolTable* tab, const char* name){
	Symbol* temp = tab->head;

	while(temp){
		if(strcmp(temp->symbol.name, name))
			return &temp->symbol;
	}
	return NULL;
}

Symbol* findSection(SymbolTable* tab, Section sec){
	Symbol* temp = tab->head;

	while(temp){
		if(temp->symbol.section == sec)
			return &temp->symbol;
	}
	return NULL;
}

void deleteSymbolTable(SymbolTable *tab){
	
	while(tab->head){
		Symbol* temp = tab->head;
		tab->head = tab->head->next 
		free(temp);
	}
	tab->last = NULL;
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

	while(!eof && line){
		if(currentLine->label != NULL){
			Symbol *symbol = findSymbol(&tab, currentLine->label);
			if(!symbol)
				addSymbol(&tab, currentLine->label, cnt, currentSymbol->section, LOCAL,  currentSymbol->n );
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
				Parameter* pram = currentLine->head;
				if(currentLine->ins->addrType == DST && (param->ptype == IMMED_SYM || param->addrType == IMMED_CON))
					error("Not compatible addr type with ins");
			
				if(((param->ptype == REGIND_CON || param->ptype == REGIND_SYM) || (param->ptype == PCREL)) && (currentLine->ins->ins == CALL || is_substr(currentLine->ins->name, "JMP")))
					error("JMP instruction with uncompatible addressing");

				if(currentLine->paramNo > 1){
					if(param->next->ptype == PCREL && !(currentLine->ins->ins == CALL || is_substr(currentLine->ins->name, "JMP")))
						error("pcrel with non jump func");

				}
			}

			cnt += sizeofinstruction();
		}

		if(currentLine->type = O_DIRECTIVE){
			
		}
		currentLine = currentLine -> next;
		
	}
}	