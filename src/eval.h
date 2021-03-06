#ifndef STAR_EVAL_H
#define STAR_EVAL_H

#include <string.h>

#include "lexer.h"
#include "ast.h"
#include "object.h"

StarObj* starEvalObj(StarObj* env, StarObj* obj);
StarObj* starEvalProgn(StarObj* env, StarObj* obj);
StarObj* starEval(StarEnv* env, char* buff, size_t len);

#endif//STAR_EVAL_H
