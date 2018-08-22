#include "secondpass.h"
#include "firstpass.h"
#include "data_types.h"
#include "helper.h"
#include "error.h"
#include <string.h>
#include <stdlib.h>
typedef  unsigned  int UINT;
static char* to_ptr;
static int cnt;
Symbol* currentSection;
Line* currentLine;
int end = 0;
int relocation_patch(Symbol*sym, RelType relocation_type, int offset){
	UINT pat = 0;
	addRecord(&reltab, relocation_type, offset, sym);

	if(relocation_type == REL_16)
	{
		if(sym->sctype == GLOBAL)
			pat = 0;
		else
			pat = sym->num;
		return pat;
	}
	else if(relocation_type == RELPC_16){
		if(sym->sctype == GLOBAL)
			pat = 2-;
		else
			pat = (sym->num -2);
		return pat;
	}else{
		error("Unrecognized rel type");
	}
}
void writeRelTabToFile(FILE* file, int n, RelocationTable* tab ){

	for (int i =0; i < n; i++){
		char* name = "<UNKNOWN SECTION>";
		Symbol* sym = findSymbol(tab, reltab[i].sec);
		RelocationRecNode* rel = NULL;
		fprintf(file, "### %s RELOCATION TABLE ###\n", sym->secNo);
		for(rel = reltab[i].head; rel; rel = rel->next){
			RelocationRec record = rel->record;
			char type[15];
			switch(record.type){
				case REL_16: strcpy(type, "RELTYPE16");
				break;
				case RELPC_16: strcpy(type, "RELTYPEPC16");
				break;
			}

			fprintf(file, "%ld %s %ld %ld", record.rel_num, type, record.offset, record.sym_num);

		}

	}
}

void endSection(){
	if(currentSection == NULL)
		return;

}
int addRecord(RelocationTable* tab, RelType type, long offset, int sym){
	RelocationRecNode* node = (RelocationRecNode*) malloc(sizeof(RelocationRecNode));

	if(!node){
		error("Memory alloc problem");
		return 0; 
	}

	node->record.rel_num = tab->cnt++;
	node->record.type = type;
	node->record.offset = offset;
	node->record.sym_num = sym;

	if(!tab->head)
		tab->head = node;
	else
		tab->tail->next = node;
	tab->tail = node;
	tab->tail = NULL;

	return 1;
}
void deleteRelocationTable(RelocationTable* tab, int ntabs){
	RelocationRecNode* temp = tab->head;

	while(temp && ntabs){
		--ntabs;
		tab->head = tab->head->next;
		free(temp);
		temp = tab->head;
	}
	if(tab->head == NULL)
	{
		tab->tail = NULL;
		tab->cnt = 0;
	}
}

UINT getEndParam(Parameter* param, UINT* begin, int offset){
	UINT end = 0;
	Symbol* symbol = NULL;

	if(param->ptype == IMMED_CON)


}

void secondPass(Line* parsedFile){
	cnt = 0;
	currentSection = NULL;
	currentLine = parsedFile;

	while(currentLine){
		if(currentLine->type = O_INSTRUCTION){
			UINT begin = 0;
			UINT end = 0;

			if(currentLine->ins->ins != NON){
				begin |= currentLine->ins->cond << 14;
				end |= currentLine->ins->ins << 10;
				if(currentLine->params != NULL){
					if(currentLine->params->ptype == PCREL)
						begin |= (0x7<<5);
					else
						begin |= (currentLine->params->regNo<<5);
					end = getEndParam(currentLine->params, &begin, 8);
				}
			}

		}
		else if(currentLine->type = O_DIRECTIVE){
				if(currentLine->dir->dirType == SKIP){
					int n = currentLine->params->value;
					for(; n > 0; n--)
							cnt++;
				}
				else if(currentLine->dir->dirType == ALIGN){
					int n = currentLine->params->value;
					UINT  toalign = (0x0001 << n);
					while(cnt & toalign)
						cnt++;
				}
				else if (currentLine->type = DATA){
					int n = currentLine->paramNo;
					int size = 0;
					Parameter* param = currentLine->params;
					int value;

					if(strcmp(currentLine->dir->dir, ".CHAR") == 0)
						size = 1;
					else if(strcmp(currentLine->dir->dir, ".WORD") == 0)
						size = 2;
					else if(strcmp(currentLine->dir->dir, "DWORD") == 0 || strcmp(currentLine->dir->dir, ".LONG"))
						size = 4;

					if(n == 0)
						cnt + = size;
					else {
						while(param){
							if(param->ptype == IMMED_CON)
								value = param->value;
							else{
								Symbol* sym = findSymbol(&tab, param->symbol);
								value = relocation_patch(&sym, cnt, REL_16);
							}
						}
						cnt += size;
					}
				} if (currentLine->dir->dirType == IMPORT_EXPORT){
							continue;
				}
				else if(currentLine ->dir->dirType == D_EOF){
					end = 1;
				}
				else if(currentLine->dir->dirType == SECTION){
						endSection();
						cnt = 0;
						currentSection = findSymbol(&tab, currentLine->dir->dir);
				}
		}

	}

	endSection();

}