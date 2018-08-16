#ifndef _DATA_TYPES_H_
#define _DATA_TYPES_H_





typedef struct{
	char* buff;
	int buffersize;
} Buffer;

// Token

typedef enum {
	UNDEFINED,
	EOF,
	NEWLINE,
	INSTRUCTION,
	REGISTER,
	DIRECTIVE,
	SYMBOL,
	COMMENT,
	LITERAL,
	PLUS,
	MINUS,
	COMMA,
	COLON,
	DOLLAR,
	LBRACKET,
	RBRACKET,
	AMPERSAND,
	ASTERISK,
}TokenType;



typedef enum{
	IMMED = 0x0,
	A_REGDIR = 0x1,
	MEMDIR = 0x2,
	REGINDPOM = 0x3,
}Addressings;

typedef enum {
	NONDEF ,
	SRC,
	DST
} AddressingOp;

typedef enum {
	NON = -1,
	CALL = 0x0,
	ADD = 0x1,
	SUB = 0x2,
	MUL = 0x3,
	DIV = 0x4,
	CMP = 0x5,
	AND = 0x6,
	OR = 0x7,
	NOT = 0x8,
	TEST = 0x9,
	MOV = 0xA,
	IRET = 0xB,
	PUSH = 0xC,
	SHL = 0xD,
	SHR = 0xE,
	POP = 0xF,

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
	START,
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
		const char* symbol;
		int value;
	};
	struct pnode *next;
	
}Parameter;

typedef struct 
{
	const char *ins_name;
	int paramNo;
	Addressings addrType;
	InstructionSymbol ins;
	Conditions cond;
}Instruction;

typedef struct 
{
	const char *dir;
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
	const char *label;
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
	char* name;
	TokenType token_type;
} Token;

typedef struct token {
	Token* tok;
	struct token *next;
} TokenNode;






#endif