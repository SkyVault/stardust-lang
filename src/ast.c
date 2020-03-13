#include "ast.h"

#if 0
/*
<program>    ::= <s-exp-list>
<s-exp>      ::= <object> | '(' <s-exp-list> ')'
<s-exp-list> ::= <s-exp> <s-exp-list> | <s-exp>
<object>     ::= <atom> | <number> | <string> | #t | #f
*/
#endif

StarObj* starParseSExpr(StarLex* lexer);
StarObj* starParseExpList(StarLex* lex);

//<s-exp>      ::= <atom> | '(' <s-exp-list> ')'
StarObj* starParseExpList(StarLex* lexer) {
    StarObj* list = malloc(sizeof(StarObj));
    StarObj* head = list;

    list->T = STAR_OBJ_LIST;

    while (notEof(lexer)) {
        StarTok token = starPeekToken(lexer);

        if (token.T == TOK_OPEN_PAREN) {
            StarObj* item = starParseSExpr(lexer);
            list->next = item;
            list = item;
        }

        if (isTerminal(&token)) {
            StarObj* item = starParseSExpr(lexer);
            list->next = item;
            list = item;
        }

        if (token.T == TOK_CLOSED_PAREN) {
            starNextToken(lexer);
            return head;
        }
    }

    return head;
}

StarObj* parseObject(StarLex* lexer) {
    StarTok token = starNextToken(lexer);

    StarObj* object = malloc(sizeof(StarObj));
    object->T = STAR_OBJ_NONE;

    if (token.T == TOK_ATOM) {
        object->T = STAR_OBJ_ATOM;
        object->len = token.end - token.start;
        object->data = malloc(object->len + 1);
        sprintf(object->data, "%.*s", (int)object->len, token.start);
        return object;
    }

    if (token.T == TOK_NUMBER) {
        object->T = STAR_OBJ_NUMBER;
        char buff[512];
        sprintf(buff, "%.*s", (int)(token.end-token.start), token.start);
        object->num = atof(buff);
        return object;
    }

    return object;
}

//<s-exp>      ::= <object> | '(' <s-exp-list> ')'
StarObj* starParseSExpr(StarLex* lexer) {
    StarTok token = starPeekToken(lexer);

    if (isTerminal(&token)) {
        return parseObject(lexer);
    }

    if (token.T == TOK_OPEN_PAREN) {
        starNextToken(lexer);
        return starParseExpList(lexer);
    }

    assert(0);
    return NULL;
}

//<program>    ::= <s-exp-list>
StarObj* starParseProgram(char* buff, size_t len) {
    assert(len > 0);

    StarLex lexer = starMakeAndLoadLexer(buff, len);

    StarObj* list = malloc(sizeof(StarObj));
    list->T = STAR_OBJ_LIST;

    StarObj* head = list;

    bool first = true;
    while (notEof(&lexer)) {
        StarObj* sub = starParseSExpr(&lexer);
        if (first) {
            head = list = sub;
            first = false;
        } else {
            list->next = sub;
        }
        list = sub;
    }

    return head;
}