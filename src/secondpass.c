/*#include "secondpass.h"
#include "data_types.h"
#include "helper.h"
#include "error.h"

#include <stdlib.h>

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

}*/