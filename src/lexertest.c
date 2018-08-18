#include <stdio.h>
#include <stdlib.h>
#include "helper.h"
#include "lexer.h"
#include "data_types.h"


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

    return 0;
}