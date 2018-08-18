#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "lexer.h"
#include "data_types.h"
#include "parser.h"

int main(int argc, char* argv[]){

    const char* filename = "test.txt";

    Buffer file = loadFromFile(filename);

    TokenNode* tf = tokenizingFile(file);

    TokenNode* head = tf;
    while(head)
    {
        if(head->tok.token_type == T_NEWLINE){
            printf("[TT_NEWLINE]\n");

        }else if(head->tok.token_type == T_SYMBOL)
        {
            printf("[TT_SYMBOL]       # %s\n", head->tok.name);

        }
        else if(head->tok.token_type == T_INSTRUCTION)
        { printf("[TT_INSTRUCTION]  # %s\n", head->tok.name);
        }
        else if(head->tok.token_type == T_REGISTER)
        {  printf("[TT_REGISTER]     # %s\n", head->tok.name);
        }
        else if(head->tok.token_type == T_DIRECTIVE)
        {  printf("[TT_DIRECTIVE]    # %s\n", head->tok.name);
        }
        else if(head->tok.token_type == T_LITERAL)
        {printf("[TT_LITERAL]      # %s\n", head->tok.name);
        }
        else if(head->tok.token_type ==T_COMMENT)
        {printf("[TT_COMMENT]\n");
        }
        else if(head->tok.token_type == T_COMMA)
        {printf("[TT_COMMA]        # ,\n");
        }
        else if(head->tok.token_type == T_COLON)
        {  printf("[TT_COLON]        # :\n");
        }
        else if(head->tok.token_type == T_ASTERISK)
        { printf("[TT_ASTERISK]     # *\n");
        }
        else if(head->tok.token_type == T_DOLLAR)
        {printf("[TT_DOLLAR]       # $\n");
        }
        else if(head->tok.token_type == T_LBRACKET)
        { printf("[TT_LBRACKET]     # [\n");
        }
        else if(head->tok.token_type == T_RBRACKET)
        {  printf("[TT_RBRACKET]     # ]\n");
        }
        else if(head->tok.token_type == T_PLUS)
        {printf("[TT_PLUS]         # +\n");
        }
        else if(head->tok.token_type == T_MINUS)
        {printf("[TT_MINUS]        # -\n");
        }
        else{
            printf("Test failed.\n");
            exit(1);

        }
        head = head->next;
    }

    //parser test

    Line* curr = parsing(tf);

    while (curr)
    {
        putchar('{'); putchar('\n');

        printf("\tLabel: %s\n", curr->label ? curr->label : "---");

        if (curr->type != EMPTY)
        {
            printf("\tMnemonic: %s\n",
                   (curr->type == EMPTY ? curr->ins->ins_name : curr->dir->dir));
            printf("\tNum. parameters: %d\n", curr->paramNo);
            printf("\tParameters:\n");
            printf("\t[\n");
            Parameter *pcurr = curr->params;
            while (pcurr)
            {
                printf("\t\t{\n");

                printf("\t\t\tType: ");
                switch (pcurr->ptype)
                {
                    case IMMED_CON:
                        printf("%s\n", "PAR_IMMED_LITERAL");
                        printf("\t\t\tNum. value: %d\n", pcurr->value);
                        break;
                    case IMMED_SYM:
                        printf("%s\n", "PAR_IMMED_SYMVAL");
                        printf("\t\t\tSymbol: %s\n", pcurr->symbol);
                        break;
                    case MEMDIR_SYM:
                        printf("%s\n", "PAR_MEMDIR_SYM");
                        printf("\t\t\tSymbol: %s\n", pcurr->symbol);
                        break;
                    case MEMDIR_CON:
                        printf("%s\n", "PAR_MEMDIR_LITERAL");
                        printf("\t\t\tNum. value: %d\n", pcurr->value);
                        break;
                    case REGDIR:
                        printf("%s\n", "PAR_REGDIR");
                        break;
                    case REGIND_CON:
                        printf("%s\n", "PAR_REGIND_LITERAL");
                        printf("\t\t\tNum. value: %d\n", pcurr->value);
                        break;
                    case REGIND_SYM:
                        printf("%s\n", "PAR_REGIND_SYM");
                        printf("\t\t\tSymbol: %s\n", pcurr->symbol);
                        break;
                    case PCREL:
                        printf("%s\n", "PAR_PCREL");
                        printf("\t\t\tSymbol: %s\n", pcurr->symbol);
                        break;
                    default:
                        break;
                }

                printf("\t\t\tLength: %s\n", pcurr->is_long ? "Long" : "Short");
                if (pcurr->regNo == -1)
                    printf("\t\t\tRegister: UNUSED\n");
                else
                    printf("\t\t\tRegister: %d\n", pcurr->regNo);

                printf("\t\t},\n");
                pcurr = pcurr->next;
            }
            printf("\t]\n");
        }
        putchar('}'); putchar(','); putchar('\n');
        curr = curr->next;
    }

}