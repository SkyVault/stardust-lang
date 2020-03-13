#include <stdio.h>
#include <string.h>

#include "tests.h"
#include "lexer.h"
#include "object.h"
#include "ast.h"

int main() {
    runTests();

    /* char b[] = "(1 (2 3) 4)"; */
    char b[] = "(1 2 3 4) (3 2 1)";

    StarObj* list = starParseProgram(b, strlen(b));
        
    return 0;
}
