#include "firstpass.h"
#include "data_types.h"
#include "helper.h"
#include "error.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>

#define WORD 2
#define LONG 4

static SymbolTable tab;
static Symbol *currentDirective;
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

Symbol* addSection(SymbolTable* tab, const char* name ){

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
	node->symbol.type = ST_SECTION;
	node->symbol.sctype = LOCAL;
	node->symbol.offset = 0;
//	node->symbol.section = section;
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

		if(currentLine->params->is_long == 1 || (currentLine->params->next && currentLine->params->next->is_long == 1)
			|| (is_substr(currentLine->ins->ins_name, "HALT")) || !(is_substr(currentLine->ins->ins_name, "JMP")))
			return LONG;
		else{
			if(is_substr(currentLine->ins->ins_name, "JMP")){
				if(currentLine->params->ptype == REGIND_CON || currentLine->params->ptype == REGIND_SYM)
					return WORD + LONG;
				if(currentLine->params->ptype==PCREL || currentLine->params->ptype==MEMDIR_CON || currentLine->params->ptype==MEMDIR_SYM || currentLine->params->ptype==PCREL)
					return LONG;
				if(currentLine->params->ptype==REGDIR)
					return WORD;
				}
			}

	return 0;
}

int firstPass(Line* parsedFile){
	cnt = 0;
	tab.head = NULL;
	tab.tail = NULL;
	tab.n = 0;
	currentDirective = NULL;

	end = 0;
	currentLine = parsedFile;

	while(!end && currentLine){
		if((currentLine->label) != NULL){
			Symbol *symbol = findSymbol(&tab, currentLine->label);
			if(!symbol)
				addSymbol(&tab, currentLine->label, cnt, currentDirective->section, LOCAL,  currentDirective->num );
			else if(symbol->secNo != 0)
				error("mul def label");
			else {
				symbol->secNo = currentDirective->secNo;
				symbol->offset = cnt;
			}

		}

		if(currentLine->type == O_INSTRUCTION){

			if(!currentDirective)
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

		if(currentLine->type == O_DIRECTIVE){
			if(currentLine->dir->dirType == SECTION){
				if(findSymbol(&tab, currentLine->dir->dir))
					error("Multiple def same directive L199, firstpass.c");
				addSection(&tab, currentLine->dir->dir);
				currentDirective = findSymbol(&tab, currentLine->dir->dir);
				cnt = 0;
			}
			else if(currentLine->dir->dirType == IMPORT_EXPORT){
					Parameter* temp = currentLine->params;
					while(temp){
						if(temp->ptype != MEMDIR_SYM)
							error("Invalid operand(L208, firstpass.c)");
						Symbol* s = findSymbol(&tab, temp->symbol);
						if(s)
							s->sctype = GLOBAL;
						else
							addSymbol(&tab, temp->symbol, 0, currentDirective->section, GLOBAL,0 );
						temp = temp->next;
					}
			}
			else if(currentLine->dir->dirType == DATA){
				int n = currentLine->paramNo;
				int size = 0;
					if(strcmp(currentLine->dir->dir, ".CHAR") == 0)
						size = 1;
					else if(strcmp(currentLine->dir->dir, ".WORD") == 0)
						size = 2;
					else if(strcmp(currentLine->dir->dir, "DWORD") == 0 || strcmp(currentLine->dir->dir, ".LONG"))
						size = 4;

					cnt = size * n;
			}
			else if(currentLine->dir->dirType == SKIP){
				if(currentLine->paramNo != 1 || currentLine->params->ptype != IMMED_CON)
					error(("Invalid operation (L230, firstpass.c)"));
				cnt += currentLine->params->value;
			}
		}

		currentLine = currentLine -> next;
	}
	return 0;
}	