#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_

#define MAX_NAME_SIZE 32

typedef struct{
	char* buff;
	int buffersize;
} Buffer;

// Token

typedef enum {
	T_UNDEFINED = 0,
	T_EOF,
	T_NEWLINE,
	T_INSTRUCTION,
	T_REGISTER,
	T_DIRECTIVE,
	T_SYMBOL,
	T_COMMENT,
	T_LITERAL,
	T_PLUS,
	T_MINUS,
	T_COMMA,
	T_COLON,
	T_DOLLAR,
	T_LBRACKET,
	T_RBRACKET,
	T_AMPERSAND,
	T_ASTERISK,
}TokenType;



typedef enum{
	IMMED = 0x0,
	A_REGDIR = 0x1,
	MEMDIR = 0x2,
	REGINDPOM = 0x3,
}Addressings;

typedef enum {
	NONDEF = 0,
	SRC,
	DST
} AddressingOp;

typedef enum {
	NON = -1,
	CALL = 0xB,
	ADD = 0x0,
	SUB = 0x1,
	MUL = 0x2,
	DIV = 0x3,
	CMP = 0x4,
	AND = 0x5,
	OR = 0x6,
	NOT = 0x7,
	TEST = 0x8,
	MOV = 0xD,
	IRET = 0xC,
	PUSH = 0x9,
	SHL = 0xE,
	SHR = 0xF,
	POP = 0xA,
} InstructionSymbol;

typedef enum {
	EQ = 0x0,
	NE = 0x1,
	GT = 0x2,
	AL = 0x3,
	
} Conditions;

typedef enum {
	NODEF,
	SECTION,
	IMPORT_EXPORT,
	D_EOF,
	DATA,
	ALIGN,
	SKIP,
} Directives;

typedef enum 
{
	GLOBAL,
	LOCAL,
} ScopeType;

typedef enum {
	NOPARAM,
	IMMED_CON,
	IMMED_SYM,
	MEMDIR_SYM,
	MEMDIR_CON,
	REGDIR,
	REGIND_CON,
	REGIND_SYM,
	PCREL
}ParameterType;


typedef struct pnode
{
	ParameterType ptype;
	char is_long;
	char regNo;
	union{
		char symbol[MAX_NAME_SIZE];
		int value;
	};
	struct pnode *next;
	
}Parameter;

typedef struct 
{
	char ins_name[MAX_NAME_SIZE];
	int paramNo;
	AddressingOp addrType;
	InstructionSymbol ins;
	Conditions cond;
}Instruction;

typedef struct 
{
	char dir[32];
	Directives dirType;
	
}Directive;

typedef enum{
	EMPTY,
	O_INSTRUCTION,
	O_DIRECTIVE,
	O_LABEL,
} opType;


typedef struct lnode
{
	char label[MAX_NAME_SIZE];
	union{
		const Instruction* ins;
		const Directive* dir;
	};
	int paramNo;
	Parameter *params;
	struct lnode *next;
	opType type;
} Line;



typedef struct {
	char name[MAX_NAME_SIZE];
	TokenType token_type;
} Token;

typedef struct token {
	Token tok;
	struct token *next;
} TokenNode;






#endif
