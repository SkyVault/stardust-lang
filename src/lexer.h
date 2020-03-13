#ifndef STAR_LEXER_H
#define STAR_LEXER_H

#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "star.h"

#define STAR_TOK_TYPES(G)                   \
    G(TOK_EOF),                             \
    G(TOK_ATOM),                            \
    G(TOK_NUMBER),                          \
    G(TOK_BOOLEAN),                         \
    G(TOK_OPEN_PAREN),                      \
    G(TOK_CLOSED_PAREN),                    \
    G(TOK_OPEN_BRACKET),                    \
    G(TOK_CLOSED_BRACKET),                  \
    G(TOK_OPEN_BRACE),                      \
    G(TOK_CLOSED_BRACE),

enum StarTokE {STAR_TOK_TYPES(GEN_ENUM)};
static const char* StarTokS[] = {STAR_TOK_TYPES(GEN_STR)};

typedef struct {
    int T;
    char* start;
    char* end;
} StarTok;

typedef struct {
    char* it;
    char* end;
} StarLex;

#define StarEOF (StarTok){.T=TOK_EOF,.start=0,.end=0}

StarLex starMakeLexer();
void starLoadLexer(StarLex* lex, char* buffer, size_t len);
StarLex starMakeAndLoadLexer(char* buffer, size_t len);

void starSkipWS(StarLex* lex);
StarTok starNextToken(StarLex* lex);

bool notEof(StarLex* lex);
bool isTerminal(StarTok* tok);

StarTok starPeekToken(StarLex* lex);

#endif//STAR_LEXER_H
