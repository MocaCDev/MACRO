#ifndef LEXER
#define LEXER
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum token_id_ {
    /* Keywords. */
    DEFAULT,
    CREATE,
    CONFIGURE,
    set,
    as,
    use,
    Word,
    syntax,
    /* Punctuation/Symbols. */
    l_par,
    r_par,
    l_sqrbrack,
    r_sqrbrack,
    semi_colon,
    comma,
    string,
    colon,
    EndOF
  };

typedef struct Token {
    enum token_id_ id;
    char *token_val;
} _Token;

typedef struct Lexer {
    char *data;
    char curr;
    int index;
    _Token *curr_token;
} _Lexer;

typedef struct all_keywords {
    struct
    {
        char *token_value;
        enum token_id_ id;
    };
} _all_keywords;

typedef struct all_puncs {
    struct
    {
        char *token_value;
        enum token_id_ id;
    };
} all_puncs_;

_all_keywords all_keywords[] = {
    {(char *)"CREATE", CREATE},
    {(char *)"CONFIGURE", CONFIGURE},
    {(char *)"syntax", syntax},
    {(char *)"set", set},
    {(char *)"as", as},
    {(char *)"use", use},
};

all_puncs_ all_p[] = {
    {(char *)"(", l_par},
    {(char *)")", r_par},
    {(char *)"{", l_sqrbrack},
    {(char *)"}", r_sqrbrack},
    {(char *)";", semi_colon},
    {(char *)",", comma},
    {(char *)":", colon},
    {(char *)"\"", string}
};

void advance_lexer(_Lexer *lex)
{
    lex->index++;
    lex->curr = lex->data[lex->index];
}

char *get_word(_Lexer *lex)
{
    char *word = (char *)calloc(1, sizeof(*word));
    int index = 0;
  
    while(lex->curr != ' ' && lex->curr != '\0' && (lex->curr >= 'a' && lex->curr <= 'z') || (lex->curr >= 'A' && lex->curr <= 'Z'))
    {
        word[index] = lex->curr;
        index++;

        /* Realloc the memory to get next value. */
        word = (char *)realloc(
            word,
            (index+1) * sizeof(*word)
        );

        advance_lexer(lex);
    }
    lex->index--;
    lex->curr = lex->data[lex->index];

    return word;
}

_Token *init_token(char *val, enum token_id_ token, _Lexer *lex)
{
    _Token *t = (_Token *)calloc(1, sizeof(*t));
    t->id = token;
    t->token_val = val;
    lex->curr_token = t;

    advance_lexer(lex);
    return t;
}

_Token *get_token(_Lexer *lex)
{
    while(lex->index < strlen(lex->data) && lex->curr != '\0')
    {
        if(lex->curr == ' ')
        {
            while(lex->curr == ' ')
                advance_lexer(lex);

            continue;
        }

        if(lex->curr == '-')
        {
            advance_lexer(lex);
            if(lex->curr == '-')
            {
                advance_lexer(lex);
                while(lex->curr != '\n' && lex->curr != '\0')
                    advance_lexer(lex);
            } else {
                fprintf(stderr, "Expected second `-` for comment.");
                exit(EXIT_FAILURE);
            }
          }

        if(lex->curr >= 'a' && lex->curr <= 'z' || lex->curr >= 'A' && lex->curr <= 'Z')
        {
            char *word = get_word(lex);
            for(size_t i = 0; i < sizeof(all_keywords) / sizeof(all_keywords[0]); i++)
            {
                if(strcmp(all_keywords[i].token_value, word)==0)
                {
                    return init_token(word, all_keywords[i].id, lex);
                }
            }
            return init_token(word, Word, lex);
        }

        if(lex->curr == '"')
        {
            advance_lexer(lex);
            char *_string = (char *)calloc(1, sizeof(*_string));
            int index = 0;
            
            while(lex->curr != '"')
            {
                _string[index] = lex->curr;
                index++;

                _string = (char *)realloc(
                    _string,
                    (index+1) * sizeof(*_string)
                );
                advance_lexer(lex);
            }
            advance_lexer(lex);

            return init_token(_string, string, lex);
        }
        
        char *p_word = (char *)calloc(2, sizeof(*p_word));
        p_word[0] = lex->curr;
        p_word[1] = '\0';
        for(size_t i = 0; i < sizeof(all_p) / sizeof(all_p[0]); i++)
        {
            if(strcmp(p_word, all_p[i].token_value)==0)
                return init_token(p_word, all_p[i].id, lex);
        }

        advance_lexer(lex);
    }

    return init_token((char *)"\0", EndOF, lex);
}

_Lexer *init_lexer(char *data) {
    _Lexer *lex = (_Lexer *)calloc(1, sizeof(*lex));
    
    lex->data = data;
    lex->index = 0;
    lex->curr = lex->data[lex->index];

    return lex;
}

#endif