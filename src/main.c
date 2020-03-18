#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "lexer.h"
#include "object.h"
#include "ast.h"
#include "eval.h"

StarObj* add(StarObj* args) {
    return 0;
}

int main() {
    runTests();

    char b[] = "(add 1 2 3)";
    StarEnv env = starMakeEnv();

    StarObj cfunc_add = (StarObj) {
        .T = STAR_OBJ_C_FUNC,
        .func = add
    };

    starPutInEnv(&env, &cfunc_add, "add", 3);

    StarObj result = starEval(&env, b, strlen(b));
        
    return 0;
}
