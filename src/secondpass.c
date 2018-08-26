#include "secondpass.h"
#include "firstpass.h"
#include "data_types.h"
#include "helper.h"
#include "error.h"
#include <string.h>
#include <stdlib.h>

#define WORD 2
#define LONG 4

typedef  unsigned  int UINT;
static int cnt;
Symbol* currentDirective;
Line* currentLine;
int end = 0;

//Realocation table
void addRecord(RelocationTable* tab, RelType type, long offset, int sym){

	RelocationRecNode* node = (RelocationRecNode*) malloc(sizeof(RelocationRecNode));

	if(!node){
		error("Memory alloc problem");
	}

	if(tab->head == NULL)
		tab->tail = tab->head = node;
	else
		tab->tail = tab->tail->next = node;

	node->record.rel_num = tab->cnt++;
	node->record.type = type;
	node->record.offset = offset;
	node->record.sym_num = sym;
}

int record_process(Symbol* sym, RelType relocation_type, int offset){
	UINT pat = 0;

	if(relocation_type == REL_16) {
        if (sym->sctype == GLOBAL) {
            pat = 0;
            addRecord(currreltab, relocation_type, offset, sym->num);
        }
        else {
            pat = (UINT) sym->val;
            addRecord(currreltab, relocation_type, offset, sym->secNo);
        }

	return pat;
    }
	else if(relocation_type == RELPC_16) {
		if(sym->sctype == GLOBAL) {
            pat = (UINT) (-2);
            addRecord(currreltab, relocation_type, offset, sym->num);
        }
		else {
            pat = (UINT) (sym->val - 2);
            addRecord(currreltab, relocation_type, offset, sym->secNo);
        }
		return pat;
	}else{
		error("Unrecognized realocation type(L65, secondpass)");
	}
}

void writeRelTabToFIle(FILE* file,int tabN, RelocationTable* retab, SymbolTable* tab) {

	for (int i =0; i < tabN; i++){
		char* name = "<UNKNOWN SECTION>";
		Symbol* sym = findSection(tab, retab[i].sec);
		if(sym == NULL)
			error("Symbol not found(L75, secondpass)");

		RelocationRecNode* rel = NULL;

		fprintf(file, "### %s RELOCATION TABLE ###\n", sym->name);
		fprintf(file, "%20s%20s%20s%20s\n", "INDEX", "REL_TYPE", "OFFSET", "VALUE");

		for(rel = retab[i].head; rel; rel = rel->next){
			RelocationRec record = rel->record;
			char type[15];
			switch(record.type){
				case REL_16: strcpy(type, "RELTYPE16");
				break;
				case RELPC_16: strcpy(type, "RELTYPEPC16");
				break;
			}

			fprintf(file, "%20ld%20s%20ld%20d\n", record.rel_num, type, record.offset, record.sym_num);
		}

		fputc('\n', file);
	}
}



void deleteRelocationTable(RelocationTable* tab, int ntabs){
	RelocationRecNode* temp = tab->head;

	while(ntabs){
		--ntabs;
		while(temp) {
			tab->head = tab->head->next;
			free(temp);
			temp = tab->head;
		}
	}
}

//second pass assembler
int encodeInstruction(UINT begin, UINT end, int islong){
	unsigned char buff[4] = {'\0'};
	char* buffer = &buff[0];
	char* instruction_in_bytes = (char*)&begin;
    Code* temp = (Code*) malloc(sizeof(Code));
    if(temp == NULL)
    	error("Null pointer exception (L120, secondpass)");

    *buffer++ = *(instruction_in_bytes + 1);
    *buffer++ = *instruction_in_bytes;

    if(islong == 1) {
        instruction_in_bytes = (char*) &end;
        *buffer++ = *instruction_in_bytes;
        *buffer++ = *(instruction_in_bytes + 1);
    }
	temp->islong = islong;
    strcpy(temp->ins, buff);

    if(program == NULL)
		program = temp;
	else {
		Code* first = program;
		while(first->next != NULL){
			first = first->next;
		}
		first->next = temp;
	}
}

void writeCode(FILE* file){
	Code* temp = program;
	fprintf(file, "###PROGRAM CODE###\n");

	while(temp) {

			fprintf(file, "%02x ", (unsigned char) temp->ins[0]);
			fprintf(file, "%02x ", (unsigned char) temp->ins[1]);
		if(temp->islong) {
			fprintf(file, "%02x ", (unsigned char) temp->ins[2]);
			fprintf(file, "%02x " , (unsigned char) temp->ins[3]);
		}
		fputc('\n', file);
		temp = temp->next;
	}
}

UINT getEndParam(Parameter* param, UINT* begin, int offset){
	UINT end = 0;

	if(param->ptype == IMMED_CON) {
		*begin |= IMMED << offset;
		end = param->value;
	}
	else if(param->ptype == IMMED_SYM) {
		*begin |= IMMED << offset;
		Symbol* sym = findSymbol(tab, param->symbol);

		if(sym == NULL)
			error("Symbol not found!(L165,secondpass)");

		end =(UINT)record_process(sym, REL_16, cnt + WORD);
	}
	else if(param->ptype == MEMDIR_CON) {
		*begin |= MEMDIR <<offset;
		end = (UINT)param->value;
	}
	else if(param->ptype == MEMDIR_SYM) {
		*begin |= MEMDIR << offset;
		Symbol* sym = findSymbol(tab, param->symbol);
		if(sym == NULL)
			error("Symbol not found!(L176, secondpass)");
		end = (UINT) record_process(sym, REL_16, cnt + WORD);
	}
	else if(param->ptype == PCREL) {
		*begin |= REGINDPOM << offset;
		Symbol* sym = findSymbol(tab, param->symbol);

		if(sym == NULL)
			error("Symbol not found!(L185, secondpass)");

		end = (UINT) ((sym->secNo == currentDirective->secNo) ? (sym->val - cnt - LONG) :
											(record_process(sym, RELPC_16, cnt + WORD)));
	}
	else if(param->ptype == REGIND_CON) {
		*begin |= REGINDPOM << offset;
		end = (UINT) param->value;
	}
	else if(param->ptype == REGIND_SYM) {
		*begin |= REGINDPOM << offset;
		Symbol* sym = findSymbol(tab, param->symbol);

		if(sym == NULL)
			error("Symbol not found!(L170,secondpass)");

		end = (UINT) record_process(sym, REL_16, cnt + WORD);
	}
	else if(param->ptype == REGDIR) {
		*begin |= A_REGDIR<<offset;
	}
	else
		error("Invalid parameter type!(L207, secondpass)");

	return end;
}

void pseudoIns(UINT* begin, UINT* end) {

	if(is_substr(currentLine->ins->ins_name, "RET")) {
		*begin |= (currentLine->ins->cond << 14);
		*begin |= (0xA << 10);
		*begin |= (A_REGDIR << 8);
		*begin |= (0x7 << 5);
		*begin |= (A_REGDIR << 3);
		encodeInstruction(*begin, *end, 0);
	}

	if(is_substr(currentLine->ins->ins_name, "JMP")) {
		*begin |= currentLine->ins->cond << 14;
		*begin |= (((currentLine->params->ptype == PCREL) ? 0x0 : 0xd) << 10);
		*begin |= (A_REGDIR << 8);
		*begin |= (0x7 << 5);
        Parameter param = *currentLine->params;
        param.next = NULL;

		if(param.ptype == REGDIR) {
			*begin |= ((A_REGDIR << 3) | (param.regNo));
			encodeInstruction(*begin, *end, 0);
		}
		else if(param.ptype == MEMDIR_CON) {
			param.ptype = IMMED_CON;
			*end = getEndParam(&param, begin, 3);
			encodeInstruction(*begin, *end, 1);
		}
		else if(param.ptype == MEMDIR_SYM) {
			param.ptype = IMMED_SYM;
			*end = getEndParam(&param, begin, 3);
			encodeInstruction(*begin, *end, 1);
		}
		else if(param.ptype== PCREL) {
			Symbol* sym = findSymbol(tab, param.symbol);
			if(sym == NULL)
				error("Symbol not found");
			*begin |= IMMED << 3;
			if(sym->secNo == currentDirective->secNo)
				*end = (UINT)(sym->val - cnt - LONG);
			else
				*end = record_process(sym, RELPC_16, cnt + WORD);

			 encodeInstruction(*begin, *end, 1);
		}
		else
			error("Undefined parameter type!(L251, secondpass)");
	}
}

int sizeOfinstruction(){

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

void secondPass(Line* parsedFile){
	cnt = 0;
	secN = 1;
	currentDirective = NULL;
	currentLine = parsedFile;
	currreltab = NULL;
	program = NULL;
	while(currentLine){
		if(currentLine->type == O_INSTRUCTION){
			UINT begin = 0;
			UINT end = 0;

			if(currentLine->ins->ins != NON) {
				begin |= currentLine->ins->cond << 14;
				begin |= currentLine->ins->ins << 10;

				if(currentLine->params != NULL) {

					if(currentLine->params->ptype == PCREL)
						begin |= ((0x7)<<5);
					else
						begin |= ((currentLine->params->regNo)<<5);

					end = getEndParam(currentLine->params, &begin, 8);

					if(currentLine->params->next != NULL) {
						if(currentLine->params->next->ptype == PCREL)
							begin |= 0x7;
						else
							begin |= currentLine->params->next->regNo;

						if(currentLine->params->next->is_long == 1)
							end = getEndParam(currentLine->params->next, &begin, 3);
						else
							getEndParam(currentLine->params->next, &begin, 3);
					}
					else {
						begin |= A_REGDIR << 3;
					}
				}
				else {
					begin |= A_REGDIR << 8;
					begin |= A_REGDIR << 3;
				}
				int islong = 0;
				if(sizeOfinstruction() == LONG)
					islong = 1;
				encodeInstruction(begin, end, islong);
			}
			else {
					pseudoIns(&begin, &end);
			}
       		 cnt += sizeOfinstruction();
		}
		else if(currentLine->type == O_DIRECTIVE){

			if(currentLine->dir->dirType == SKIP){
				int n = currentLine->params->value;

				for(; n > 0; n--)
					cnt++;
			}
			else if(currentLine->dir->dirType == ALIGN){
				int n = currentLine->params->value;
				UINT toalign =(UINT)~(~0x0000 << n);

				while(cnt & toalign)
					cnt++;
			}
			else if (currentLine->dir->dirType == DATA){
				int n = currentLine->paramNo;
				int size = 0;
				Parameter* param = currentLine->params;

				if(strcmp(currentLine->dir->dir, ".CHAR") == 0)
					size = 1;
				else if(strcmp(currentLine->dir->dir, ".WORD") == 0)
					size = 2;
				else if(strcmp(currentLine->dir->dir, "DWORD") == 0 || strcmp(currentLine->dir->dir, ".LONG"))
					size = 4;

				if(n == 0)
					cnt += size;
				else {
					while(param){
						cnt += size;
						param = param->next;
					}
				}
			}
			else if (currentLine->dir->dirType == IMPORT_EXPORT){

			}
			else if(currentLine ->dir->dirType == D_EOF){
				end = 1;
			}
			else if(currentLine->dir->dirType == SECTION){
					cnt = 0;
					currentDirective = findSymbol(tab, currentLine->dir->dir);
					if(currentDirective == NULL)
						error("Directive not in symbol table!(L376, secondpass)");

					if(currreltab != NULL) {
						++currreltab;
						secN++;
					}
					else
						currreltab = &reltabls[0];

					currreltab->sec = currentDirective->secNo;
					currreltab->cnt = 0;
			}
		}
		currentLine = currentLine->next;
	}
}
