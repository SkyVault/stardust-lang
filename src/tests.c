#include "tests.h"
#include "lexer.h"

#include <string.h>

bool lexerTest(char* os);
bool skipWhitespaceTest(char* os);
bool numbersTest(char* os);
bool atomTest(char* os);

#define CIMPLE_TESTS                            \
    CTEST(lexerTest),                           \
    CTEST(skipWhitespaceTest),                  \
    CTEST(numbersTest),                         \
    CTEST(atomTest),                        

#include <cimple.h>

bool lexerTest(char* os) {
    return true;
}

bool skipWhitespaceTest(char* os) {
    char buffer[] = "    X";
    StarLex lex = starMakeAndLoadLexer(buffer, strlen(buffer));
    starSkipWS(&lex);
    return !(lex.it == lex.end || *(lex.it) != 'X');
}

bool numbersTest(char* os) {
    double tests[] = { -.123, 2, -23, 3.14159, .12};
    char buffer[] = "-.123 2 -23 3.14159 .12";
    StarLex lex = starMakeAndLoadLexer(buffer, strlen(buffer));

    int i = 0;
    while (lex.it != lex.end) {
        StarTok tok = starNextToken(&lex);

        char buff[256] = {0};
        sprintf(buff, "%.*s", (int)(tok.end-tok.start), tok.start);
        buff[strlen(buff)] = '\0';
        //cimp_log(os, "%s == %f", buff, tests[i]);
        sprintf(os+strlen(os), "%s%s == %f\n", CIMPLE_ARROW, buff, tests[i]);

        if (tok.T == TOK_EOF) {
            cimp_log(os, "Reached EOF...");
            return false;
        }

        if ((double)atof(buff) != tests[i]) {
            return false;
        }

        i++;
    }

    return true;
}

bool atomTest(char* os) {
    char* matchs[] = {"atom",  "test!here", "*what*"};
    char buff[] = "atom test!here *what*";
    StarLex lex = starMakeAndLoadLexer(buff, strlen(buff));

    int i = 0;
    while (lex.it != lex.end) {
        StarTok tok = starNextToken(&lex);
        if (tok.T != TOK_ATOM) {
            sprintf(os+strlen(os),
                    "%s Token is expected to be an ATOM, but is %d instead\n",
                    CIMPLE_ARROW,
                    tok.T);
            return false;
        }

        char word[512];
        sprintf(word, "%.*s", (int)(tok.end-tok.start), tok.start);

        if (strcmp(word, matchs[i]) != 0) {
            sprintf(os+strlen(os), "%s Token was supposed to be %s, but got %s\n",
                    CIMPLE_ARROW,
                    matchs[i],
                    word);
            return false;
        }

        i += 1;
    }
    return true;
}

bool runTests() { return cimpleRunTests(); }
