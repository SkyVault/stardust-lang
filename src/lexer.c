#include "lexer.h"

StarLex starMakeLexer() {
    return (StarLex){
        .it = 0,
        .end = 0,
    };
}

void starLoadLexer(StarLex* lex, char* buffer, size_t len) {
    lex->it = buffer;
    lex->end = buffer + len;
}

StarLex starMakeAndLoadLexer(char* buffer, size_t len) {
    StarLex lexer = starMakeLexer();
    starLoadLexer(&lexer, buffer, len);
    return lexer;
}

void starSkipWS(StarLex* lex) {
    while (lex->it != lex->end && isspace(*(lex->it)))
        lex->it++;
}

StarTok starNextToken(StarLex* lex) {
    starSkipWS(lex);

    // Parse a number
    char ch = *(lex->it);

    char* start = lex->it;

    int integer = 1;
    int negative = 0;

    if (*(lex->it) == '-'){ negative = 1; lex->it++; }
    if (lex->it == lex->end) return StarEOF;

    if (*(lex->it) == '.'){ integer = 0; lex->it++; }
    if (lex->it == lex->end) return StarEOF;

    if (lex->it == lex->end || !isdigit(*(lex->it))){
        lex->it = start;
    } else {
        while (lex->it != lex->end) {
            if (*(lex->it) == '-') {
                if (!negative) {
                    negative = 1;
                } else {
                    StarTok tok = (StarTok){.T=TOK_NUMBER, .start=start, .end=lex->it};
                    lex->it++;
                    return tok;
                }
            } else if (*(lex->it) == '.') {
                if (integer) {
                    integer = 0;
                } else {
                    StarTok tok = (StarTok) {.T=TOK_NUMBER, .start=start, .end=lex->it};
                    lex->it++;
                    return tok;
                }
            } else if (!isdigit(*(lex->it)))
                return (StarTok) {.T=TOK_NUMBER, .start=start, .end=lex->it};
            lex->it++;
        }

        if (lex->it == lex->end && start != lex->it)
            return (StarTok) {.T=TOK_NUMBER, .start=start, .end=lex->it};
    }

    lex->it = start;

    // Lex single character tokens
    switch (ch) {
        case '(': return (StarTok) {.T=TOK_OPEN_PAREN,.start=lex->it++, .end=lex->it};
        case ')': return (StarTok) {.T=TOK_CLOSED_PAREN,.start=lex->it++, .end=lex->it};
        case '[': return (StarTok) {.T=TOK_OPEN_BRACKET,.start=lex->it++, .end=lex->it};
        case ']': return (StarTok) {.T=TOK_CLOSED_BRACKET,.start=lex->it++, .end=lex->it};
        case '{': return (StarTok) {.T=TOK_OPEN_BRACE,.start=lex->it++, .end=lex->it};
        case '}': return (StarTok) {.T=TOK_CLOSED_BRACE,.start=lex->it++, .end=lex->it};
    }

    // Lex atoms
    start = lex->it;
    while (lex->it != lex->end) {
        if (lex->it == lex->end || isspace((*lex->it)) ||
            (*lex->it) == ')' || (*lex->it) == ']' || (*lex->it) == '}') {
            return (StarTok){
                .T=TOK_ATOM,
                .start=start,
                .end=lex->it
            };
        }
        lex->it++;
    }
    if (start != lex->it)
        return (StarTok){.T=TOK_ATOM, .start=start, .end=lex->it};

    return StarEOF;
}

bool notEof(StarLex* lex) { return lex->it != lex->end; }

StarTok starPeekToken(StarLex* lex) {
    char* start = lex->it;
    StarTok result = starNextToken(lex);
    lex->it = start;
    return result;
}

bool isTerminal(StarTok* tok) {
    return tok->T == TOK_ATOM ||
           tok->T == TOK_NUMBER ||
           tok->T == TOK_BOOLEAN;
}
