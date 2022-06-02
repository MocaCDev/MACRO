#include <stdio.h>
#include "src/read.h"
#include "src/lexer.h"
#include "src/parser.h"
#include "src/util.h"

//extern void some_function();

int main(int argc, char *argv[]) {
    char *f_content = read_file("main.m");
    _Lexer *lexer = init_lexer(f_content);
    _Parser *p = run_parser(lexer);
    return 0;
}