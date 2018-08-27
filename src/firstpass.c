#include "firstpass.h"
#include "data_types.h"
#include "helper.h"
#include "error.h"
#include "parser.h"

#include <stdlib.h>
#include <string.h>

#define WORD 2
#define LONG 4

static long cnt;
static long end;
static Line* currentLine;
Symbol* currentDirective;
extern SymbolTable* tab;

Symbol* addSymbol(SymbolTable* tab,const char* name, long secNo, ScopeType sctype, long val){

	SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));
	
	if(!node)
		error("Memory allocation problem");

    if(tab->head == NULL){
        tab->head = node;
        tab->tail = node;
    }
    else
        tab->tail = tab->tail->next = node;

	strcpy(node->symbol.name, name);
	node->symbol.type = ST_SYMBOL;
	node->symbol.sctype = sctype;
	node->symbol.num = tab->n++;
	node->symbol.secNo = secNo;
	node->symbol.val = val;
	node->next = NULL;

	return &node->symbol;
}
Symbol* addSection(SymbolTable* tab, const char* name) {

	SymbolNode* node = (SymbolNode*) malloc(sizeof(SymbolNode));

	if(!node)
		error("Memory allocation problem");

    if(tab->head == NULL){
        tab->head = node;
        tab->tail = node;
    }
    else
        tab->tail = tab->tail->next = node;

    strcpy(node->symbol.name, name);
	node->symbol.type = ST_SECTION;
	node->symbol.num = (tab->n)++;
	node->symbol.secNo = node->symbol.num;
	node->symbol.sctype = LOCAL;
	node->symbol.val = 0;
	node->next = NULL;


	return &node->symbol;
}


Symbol* findSymbol(SymbolTable* tab, const char* name){
    if(tab == NULL)
        return NULL;

	SymbolNode* temp = tab->head;

	while(temp){
		if(strcmp(temp->symbol.name, name)==0)
			return &temp->symbol;
		temp = temp->next;
	}
	return NULL;
}

Symbol* findSection(SymbolTable* tab, long secNo) {
    if(tab == NULL)
        return NULL;
	SymbolNode* temp = tab->head;

	while(temp) {
		if(temp->symbol.secNo == secNo && temp->symbol.type == ST_SECTION)
			return &temp->symbol;
		temp=temp->next;
	}
	return NULL;
}

void writeSymTabToFile(SymbolTable* tab, FILE* file){
	SymbolNode* temp = NULL;

	fprintf(file, "### SYMBOL TABLE ###\n");
	fprintf(file, "%10s%20s%10s%10s%10s%10s\n", "INDEX", "NAME", "TYPE", "SECTION", "VALUE", "LOKAL?");
	int i =0;
	for(temp = tab->head; temp; temp = temp->next, i++){
        Symbol sym = temp->symbol;
        char type[10];
        char scope[7];
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

        fprintf(file, "%10d%20s%10s%10ld%10ld%10s\n", sym.num, sym.name, type,  sym.secNo, sym.val, scope);

	}
    fputc('\n', file);
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

int sizeofinstruction(){

		if(is_substr(currentLine->ins->ins_name, "RET")) {
			return WORD;
		}
		else if(is_substr(currentLine->ins->ins_name, "JMP")) {
			if(currentLine->params->ptype == REGIND_CON || currentLine->params->ptype == REGIND_SYM)
				return WORD + LONG;
			if(currentLine->params->ptype == PCREL || currentLine->params->ptype == MEMDIR_SYM
			    || currentLine->params->ptype == MEMDIR_CON)
				return LONG;
			if(currentLine->params->ptype == REGDIR)
				return WORD;
		}
		else {
			if((currentLine->params && currentLine->params->is_long == 1) || (currentLine->params->next && currentLine->params->next->is_long == 1))
				return LONG;
			else
				return WORD;
		}
	return 0;
}

int firstPass(Line* parsedFile){
	cnt = 0;
	tab = (SymbolTable*) malloc(sizeof(SymbolTable));
	tab->head = NULL;
	tab->tail = NULL;
	tab->n = 1;
	currentDirective = NULL;
	end = 0;
	currentLine = parsedFile;

	while(!end && currentLine){
		// LABEL PASS
		if(currentLine->label[0] != '\0'){
			if(!currentDirective)
				error("UNDEFINED SECTION (L167, firstpass.c");

			Symbol *symbol = findSymbol(tab, currentLine->label);

			if(!symbol)
				addSymbol(tab, currentLine->label, currentDirective->secNo, LOCAL, cnt);
			else if(symbol->secNo != 0)
				error("mul def label");
			else {
				symbol->secNo = currentDirective->secNo;
				symbol->val = cnt;
			}
		}
		//INSTRUCTION PASS
		if(currentLine->type == O_INSTRUCTION){

			if(!currentDirective)
				error("Unrecognized section (L186, firstpass)");

			if(currentLine->paramNo != currentLine->ins->paramNo )
				error("Too few arguments for instruction(L188, firstpass)");

			if(currentLine->paramNo > 0){
				Parameter* param = currentLine->params;
				if(currentLine->ins->addrType == DST && (param->ptype == IMMED_SYM || param->ptype == IMMED_CON))
					error("Not compatible addr type with instruction (L193,firstpass)");
			
				if( (param->ptype == PCREL) && !(currentLine->ins->ins == CALL || is_substr(currentLine->ins->ins_name, "JMP")))
					error("Instruction with incompatible addressing(L196, firstpass)");
				if((param->ptype == REGIND_CON || param->ptype == REGIND_SYM) && (currentLine->ins->ins == CALL || is_substr(currentLine->ins->ins_name, "JMP")))
					error("JMP instruction with incompatible addressing(L196, firstpass)");
				if(currentLine->paramNo > 1){
					if((param->next->ptype == PCREL && !(currentLine->ins->ins == CALL || is_substr(currentLine->ins->ins_name, "JMP"))) || (param->is_long ==1 && param->next->is_long == 1))
						error("Instruction failed (L200, firstpass)");
				}
			}
			cnt += sizeofinstruction();
		}
		//DIRECTIVE PASS
		if(currentLine->type == O_DIRECTIVE){
			if(currentLine->dir->dirType == SECTION){
				if(findSymbol(tab, currentLine->dir->dir))
					error("Multiple def same directive L199, firstpass.c");
				addSection(tab, currentLine->dir->dir);
				currentDirective = findSymbol(tab, currentLine->dir->dir);
				cnt = 0;
			}
			else if(currentLine->dir->dirType == IMPORT_EXPORT){
					Parameter* temp = currentLine->params;
					while(temp){
						if(temp->ptype != MEMDIR_SYM)
							error("Invalid operand(L208, firstpass.c)");
						Symbol* s = findSymbol(tab, temp->symbol);
						if(s)
							s->sctype = GLOBAL;
						else
							addSymbol(tab, temp->symbol,  0, GLOBAL , 0);
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

					cnt += size * n;
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
                unsigned int toalign =(unsigned int) ~(~0x0000 << n);
                while(cnt & toalign)
                    cnt++;
			}
		}
		currentLine = currentLine -> next;
	}
	return 0;
}	