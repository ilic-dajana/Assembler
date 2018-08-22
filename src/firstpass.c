#include "firstpass.h"
#include "data_types.h"
#include "helper.h"
#include "error.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>

#define WORD 2
#define LONG 4

static Symbol *currentDirective;
static long cnt;
static long end;
static Line* currentLine;

Symbol* addSymbol(SymbolTable* tab,const char* name, long offset, Section section, ScopeType sctype, long val){

	if(!tab->head){
		tab->head = (SymbolTable*) malloc(sizeof(SymbolTable));
		
		if(!tab->head)
			error("Memory allocation problem");

        tab->head->symbol.type = NON;
        tab->head->symbol.sctype = LOCAL;
        tab->head->symbol.offset = 0;
        tab->head->symbol.num = 0;
        tab->head->symbol.section = SEC_NO_SECTION;

		tab->tail = tab->head ;
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
    tab->tail = NULL;

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

void writeSymTabToFile(SymbolTable* tab, FILE* file){
	SymbolNode* temp = NULL;

	fprintf(file, "### SYMBOL TABLE ###\n");
	int i =0;
	for(temp = tab->head; temp; temp = temp->next, i++){
        Symbol sym = temp->symbol;
        char type[9];
        char scope[6];
        switch(sym.type){
            case ST_SECTION: strcpy(type, "SECTION");
            break;
            case ST_SYMBOL: strcpy(type, "SYMBOL");
            break;
            case ST_NON: strcpy(type, "UNDEFINED");
            break;
        }
        switch(sym.sctype){
            case GLOBAL: strcpy(scope, "GLOBAL");
            break;
            case LOCAL: strcpy(scope, "LOCAL");
            break;
        }

        fprintf(file, "%d %ld %s %s %ld %ld %s \n", i, sym.num, sym.name, type,  sym.secNo, sym.offset, scope )

	}
    fprintf(file, "\n");

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
	tab = NULL;
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
				addSymbol(&tab, currentLine->dir->dir);
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
					error("Invalid operation (L230, firstpass.c)");
				cnt += currentLine->params->value;
			}
			else if(currentLine->dir->dirType == D_EOF){
			    end = 1;
			}
			else if(currentLine->dir->dirType == ALIGN){
			    if(currentLine->paramNo != 1 || currentLine->params->ptype != IMMED_CON)
			        error("Invalid operation (L239, firstpass.c");
                int n = currentLine->params->value;
                unsigned int toalign = (0x0001 << n);
                while(cnt & toalign)
                    cnt++;
			}
		}
		currentLine = currentLine -> next;
	}
	return 0;
}	