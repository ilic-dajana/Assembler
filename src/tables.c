#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "data_types.h"
#include "tables.h"

const Instruction table_of_instructions[] = {
	{"CALL", 1, DST, CALL, AL }, {"CALLEQ" , 1, DST, CALL, EQ}, {"CALLGT", 1, DST, CALL, GT}, {"CALLNE", 1, DST, CALL, NE}, {"CALLAL", 1, DST, CALL, AL},
	{"ADD", 2, DST, ADD, AL}, {"ADDEQ", 2, DST, ADD, EQ}, {"ADDGT", 2, DST, ADD, GT}, {"ADDNE", 2, DST, ADD, NE}, {"ADDAL", 2, DST, ADD, AL},
	{"SUB", 2, DST, SUB, AL}, {"SUBEQ", 2, DST, SUB, EQ}, {"SUBGT", 2, DST, SUB, GT}, {"SUBNE", 2, DST, SUB, NE}, {"SUBAL", 2, DST, SUB, AL},
	{"MUL", 2, DST, MUL, AL}, {"MULEQ", 2, DST, MUL, EQ}, {"MULGT", 2, DST, MUL, GT}, {"MULNE", 2, DST, MUL, NE}, {"MULAL", 2, DST, MUL, AL},
	{"DIV", 2, DST, DIV, AL}, {"DIVEQ", 2, DST, DIV, EQ}, {"DIVGT", 2, DST, DIV, GT}, {"DIVNE", 2, DST, DIV, NE}, {"DIVAL", 2, DST, DIV, AL},
	{"CMP", 2, DST, CMP, AL}, {"CMPEQ", 2, DST, CMP, EQ}, {"CMPGT", 2, DST, CMP, GT}, {"CMPNE", 2, DST, CMP, NE}, {"CMPAL", 2, DST, CMP, AL},
	{"AND", 2, DST, AND, AL}, {"ANDEQ", 2, DST, AND, EQ}, {"ANDGT", 2, DST, AND, GT}, {"ANDNE", 2, DST, AND, NE}, {"ANDAL", 2, DST, AND, AL},
	{"OR", 2, DST, OR, AL}, {"OREQ", 2, DST, OR, EQ}, {"ORGT", 2, DST, OR, GT}, {"ORNE", 2, DST, OR, NE}, {"ORAL", 2, DST, OR, AL},
	{"NOT", 2, DST, NOT, AL}, {"NOTEQ", 2, DST, NOT, EQ}, {"NOTGT", 2, DST, NOT, GT}, {"NOTNE", 2, DST, NOT, NE}, {"NOTAL", 2, DST, NOT, AL},
	{"TEST", 2, DST, TEST, AL}, {"TESTEQ", 2, DST, TEST, EQ}, {"TESTGT", 2, DST, TEST, GT}, {"TESTNE", 2, DST, TEST, NE}, {"TESTAL", 2, DST, TEST, AL},
	{"HALT", 0, NONDEF, NON, AL}, {"HALTEQ", 0, NONDEF, NON, EQ}, {"HALTGT", 0, NONDEF, NON, GT}, {"HALTNE", 0, NONDEF, NON, NE}, {"HALTAL", 0, NONDEF, NON, AL},
	{"JMP", 1, DST, NON, AL}, {"JMPEQ", 1, DST, NON, EQ}, {"JMPGT", 1, DST, NON, GT}, {"JMPNE", 1, DST, NON, NE}, {"JMPAL", 1, DST, NON, AL},
	{"PUSH", 1, SRC, PUSH, AL}, {"PUSHEQ", 1, SRC, PUSH, EQ}, {"PUSHGT", 1, SRC, PUSH, GT}, {"PUSHNE", 1, SRC, PUSH, NE}, {"PUSHAL", 1, SRC, PUSH, AL},
	{"POP", 1, DST, POP, AL}, {"POPEQ", 1, DST, POP, EQ}, {"POPGT", 1, DST, POP, GT}, {"POPNE", 1, DST, POP, NE}, {"POPAL", 1, DST, POP, AL},
	{"MOV", 2, DST, MOV, AL}, {"MOVEQ", 2, DST, MOV, EQ}, {"MOVGT", 2, DST, MOV, GT}, {"MOVNE", 2, DST, MOV, NE}, {"MOVAL", 2, DST, MOV, AL},
	{"SHL", 2, DST, SHL, AL}, {"SHLEQ", 2, DST, SHL, EQ}, {"SHLGT", 2, DST, SHL, GT}, {"SHLNE", 2, DST, SHL, NE}, {"SHLAL", 2, DST, SHL, AL},
	{"SHR", 2, DST, SHR, AL}, {"SHREQ", 2, DST, SHR, EQ}, {"SHRGT", 2, DST, SHR, GT}, {"SHRNE", 2, DST, SHR, NE}, {"SHRAL", 2, DST, SHR, AL},
	{"IRET", 0, NONDEF, NON, AL}, {"IRETEQ", 0, NONDEF, NON, EQ}, {"IRETGT", 0, NONDEF, NON, GT}, {"IRETNE", 0, NONDEF, NON, NE}, {"IRETAL", 0, NONDEF, NON, AL},
	{"RET", 0, NONDEF, NON, AL}, {"RETEQ", 0, NONDEF, NON, EQ}, {"RETGT", 0, NONDEF, NON, GT}, {"RETNE", 0, NONDEF, NON, NE}, {"RETAL", 0, NONDEF, NON, AL},

};
const int size_table_ins = sizeof(table_of_instructions)/sizeof(table_of_instructions[0]);

const char*table_of_registers[] = {
	"R0",  "R1",  "R2",  "R3",
    "R4",  "R5",  "R6",  "R7",
    "R8",  "R9", "R10", "R11",
    "R12", "R13", "R14", "R15",
};

const int size_table_reg = sizeof(table_of_registers)/sizeof(table_of_registers[0]);


const Directive table_of_directives[] = {
	{ ".TEXT",   SECTION       },
    { ".DATA",   SECTION       },
    { ".RODATA", SECTION       },
    { ".BSS",    SECTION       },
    { ".GLOBAL", IMPORT_EXPORT },
    { ".END",    D_EOF           },
    { ".ALIGN",  ALIGN         },
    { ".SKIP",   SKIP          },
    { ".CHAR",   DATA      },
    { ".WORD",   DATA      },
    { ".LONG",   DATA      },
};

const int size_table_dir = sizeof(table_of_directives)/sizeof(table_of_directives[0]);
