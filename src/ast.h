#ifndef STAR_AST_H
#define STAR_AST_H

#include <stdio.h>
#include <assert.h>

#include "object.h"
#include "lexer.h"

StarObj* starParseProgram(char* buff, size_t len);

#endif//STAR_AST_H
