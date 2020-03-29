#include "tests.h"
#include "eval.h"
#include "lexer.h"
#include "object.h"
#include "mem.h"

#include <string.h>

#define CIMPLE_TESTS                           \
    CTEST(lexerTest)                           \
    CTEST(skipWhitespaceTest)                  \
    CTEST(numbersTest)                         \
    CTEST(atomTest)                            \
    CTEST(envTest)                             \
    CTEST(evalTest)                            \
    CTEST(memTest)

#include <cimple.h>

bool lexerTest(char* os) {

    char* words[] = {
        "()(", "hello", "world", "stardust", "yay", ")"
    };

    char b[] = "(hello (world {stardust}) yay)";
    StarLex lex = starMakeAndLoadLexer(b, strlen(b));

    while (lex.it != lex.end) {
        StarTok token = starNextToken(&lex);

        char out[512];
        sprintf(out, "%.*s", (int)(token.end-token.start), token.start);

        cimp_log(os, "%s", out);
    }

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
        cimp_log(os, "%s == %f", buff, tests[i]);

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

bool envTest(char* os) {
    {
        StarEnv env = starMakeEnv();
        StarObj number$set = STAR_NUM(32);

        starPutInEnv(&env, &number$set, "number");
        StarObj* number$get = starGetFromEnv(&env, "number");
        if (number$get->T != number$set.T || number$get->num != number$set.num) {
            return false;
        }
    }

    StarEnv env = starMakeEnv();

    #define N 100
    #define STR(n) #n

    for (int i = 0; i < N; i++) {
        StarObj* starO = malloc(sizeof(StarObj));
        *starO = STAR_NUM(i);

        char key[256];
        size_t n = sprintf(key, "%d", i);

        starPutInEnv(&env, starO, key);
    }

    for (int i = 0; i < N; i++) {
        int which = rand()%N;

        char key[256];
        sprintf(key, "%d", which);

        StarObj* res = starGetFromEnv(&env, key);

        if (res == NULL) {
            cimp_log(os, "[%d] res == NULL", i);
            return false;
        }

        if (res->T != STAR_OBJ_NUMBER) {
            cimp_log(os, "result is not a number but [%s]",
                     StarObjS[res->T]);
        }

        if (which != res->num) {
            cimp_log(os, "[%d] values do not match", i);
            return false;
        }
    }
    
    #undef STR
    #undef N

    return true;
}

StarObj* add(StarEnv* env, StarObj* args) {
    float result = 0;

    bool first = true;

    while (args) {
        StarObj* ev = starEvalObj(args, env);

        result += ev->num;

        if (first) {
            args = args->value;
            first = false;
        }
        else {
            args = args->next;
        }
    }

    printf("%f\n", result);
    return starAllocNum(result);
}

bool evalTest(char* os) {
    StarEnv env = starMakeEnv();

    StarObj cfn_add = STAR_CFUNC(add);
    StarObj cfn_eval_progn = STAR_CFUNC(starEvalProgn);
    starPutInEnv(&env, &cfn_add, "add");
    starPutInEnv(&env, &cfn_eval_progn, "progn");

    char program[] = R"STAR(
        (progn (add 1 2 3))
    )STAR";

    StarObj* result = starEval(&env, program, strlen(program));
    cimp_log(os, "(add 1 2 3 4 5 6) -> %f", result->num);
        
    if (result->num != (1 + 2 + 3 + 4 + 5 + 6)) return false;

    return true;
}

bool memTest(char* os) {
    StarObj* obj = starAllocNum(123);
    StarObj* snd = starRef(obj);
    snd->num += 3;
    starRel(snd);

    if (!obj) {
        cimp_log(os, "Object shouldn't be freed at this point");
        return false;
    }

    if (obj->refcount != 1) {
        cimp_log(os, "Objects refcount should be 1, but is %d", obj->refcount);
        return false;
    }

    obj = starRel(obj);

    if (obj != NULL) {
        cimp_log(os, "Object is expected to be null at this point");
        return false;
    }

    return true;
}

bool runTests() {
    return cimpleRunTests();
}
