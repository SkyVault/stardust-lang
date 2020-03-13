#ifndef STAR_OBJECT_H
#define STAR_OBJECT_H

#include <stdlib.h>

#include "star.h"

#define STAR_OBJ_TYPES(G)         \
    G(STAR_OBJ_NONE),             \
    G(STAR_OBJ_ATOM),             \
    G(STAR_OBJ_NUMBER),           \
    G(STAR_OBJ_STRING),           \
    G(STAR_OBJ_LIST),

enum StarObjE {STAR_OBJ_TYPES(GEN_ENUM)};
static const char* StarObjS[] = {STAR_OBJ_TYPES(GEN_STR)};

struct StarObj;


typedef struct StarObj {
    int T;

    union {
        double num;
        struct { size_t len; char* data; }; // String or Atom
        struct { struct StarObj* value; struct StarObj* next; };
    };
} StarObj;

#define STAR_NUM(n) (StarObj){.T=STAR_VAL_NUMBER, .num=(n)};

#endif//STAR_OBJECT_H
