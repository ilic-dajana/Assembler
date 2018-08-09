//File:fileoptions.h

#include "helper.h"
#include "data_types.h"
#include "tables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


Buffer loadFromFile(const char* filename){
	Buffer buff;
	
	if(!filename) return NULL;
	
	FILE* file = fopen(filename, "r");
	if(!fp) return NULL;
	
	int size;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char* filebuff = (char*) malloc(size + 1);
	if(!buffer)
	{
			fclose(fp);
			return NULL;
	}
	
	fread(filebuff, size, sizeof(char),fp);
	filebuff[size]='\0';
	fclose(fp);
	
	buff.buff = filebuff;
	buff.buffersize = size;
	return buff;	
}

void freeBuff(Buffer filebuff){
	free(filebuff.buffer);
	filebuff.buffersize = 0;
}

int is_num_decimal(const char *number){
	
	if(!number) return -1;

	while(*number){
		if(!isdigit(*number)){
			return 0;
		}
		++number;
	}	
	return 1;
}

int is_substr(const char *str, const char *substr){

	if(!str || !substr){
		return -1;
	}

	if(strlen(substr) > strlen(str))
		return 0;

	while(*substr){
		if(*substr++ != *str++)
			return 0;
	}
	return 1;
}

const Instruction* search_for_instruction(const char *ins){

}

const Directive* search_for_directive(const char *dir){

}

const char *search_for_register(const char *reg){
	
}