#ifndef PARSER
#define PARSER
#include "lexer.h"
#include "read.h"
#include "wrappers.h"

#define ERR(msg, ...) \
    fprintf(stderr, msg, ##__VA_ARGS__); \
    exit(EXIT_FAILURE);   

typedef struct CreateInformation
{
    char *keyword_name;
    // ToDo: More fields.
} _CreateInfo;

typedef struct Parser
{
    _Token *token;
    _CreateInfo **ci;
    int amnt;
    _Lexer *lex;
} _Parser;

void parse_create(_Parser *p)
{
    p->token = get_token(p->lex);

    if(p->token->id == l_par)
    {
        p->token = get_token(p->lex);

        p->ci[p->amnt]->keyword_name = p->token->token_val;
        p->amnt++;
        p->ci[p->amnt] = (_CreateInfo *)calloc(1, sizeof(*p->ci[p->amnt]));

        p->token = get_token(p->lex);

        if(p->token->id == r_par)
        {
            p->token = get_token(p->lex);
            if(p->token->id == semi_colon)
                return;

            ERR("Expected `;`")
            //fprintf(stderr, "Expected `;`");
            //exit(EXIT_FAILURE);
        }

        fprintf(stderr, "Expected `)`.");
        exit(EXIT_FAILURE);
    }
}

void parse_configure(_Parser *p)
{
    p->token = get_token(p->lex);
    char is_valid_keyword = 0;

    if(p->token->id == l_par)
    {
        p->token = get_token(p->lex);
    
        for(int i = 0; i < sizeof(p->ci)/sizeof(p->ci[0]); i++)
            {
                if(strcmp(p->token->token_val, p->ci[i]->keyword_name)==0)
                {
                    is_valid_keyword = 1;
                    break;
                }
            }

        if(is_valid_keyword == 1)
        {
            p->token = get_token(p->lex);

            if(p->token->id == r_par)
            {
                
                return;
            }
            
        }
        ERR("Expected keyword made from `CREATE` to be passed as parameter for `CONFIGURE`.");
        //fprintf(stderr, "Expected keyword made from `CREATE` to be passed as parameter for `CONFIGURE`.");
        //exit(EXIT_FAILURE);
    }
    fprintf(stderr, "Expected `(`.");
    exit(EXIT_FAILURE);
}

_Parser *run_parser(_Lexer *lex);

void parse_syntax(_Parser *p)
{
    p->token = get_token(p->lex);

    if(p->token->id == colon)
    {
        p->token = get_token(p->lex);
        if(p->token->id == string)
        {
            {
                /* Mini lexer/parser for separate file. */
                char *data = read_file(p->token->token_val);
                _Lexer *lex = init_lexer(data);
                _Parser *pars = run_parser(lex);

                p->ci = pars->ci;
            }
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, "Expected string.");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "Colon is required after `syntax`.");
    exit(EXIT_FAILURE);
}

_Parser *run_parser(_Lexer *lex)
{
    _Parser *p = (_Parser *)calloc(1, sizeof(*p));
    
    p->token = get_token(lex);
    p->lex = lex;
    p->amnt = 0;
    p->ci = (_CreateInfo **)calloc(1, sizeof(*p->ci));
    p->ci[p->amnt] = (_CreateInfo *)calloc(1, sizeof(*p->ci[p->amnt]));
    
    while(p->token->id != EndOF)
        {
            switch(p->token->id)
                {
                    case CREATE: {
                        parse_create(p);
                        break;
                    }
                    case CONFIGURE: {
                        parse_configure(p);
                        break;
                    }
                    case syntax: {
                        parse_syntax(p);
                        break;
                    }
                    case EndOF: goto end;
                    default: goto end;
                }
            p->token = get_token(p->lex);
        }
end:
    return p;
}

#endif