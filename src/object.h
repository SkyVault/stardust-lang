#ifndef STAR_OBJECT_H
#define STAR_OBJECT_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "star.h"

#define STAR_OBJ_TYPES(G)         \
    G(STAR_OBJ_NONE),             \
    G(STAR_OBJ_TABLE),            \
    G(STAR_OBJ_ATOM),             \
    G(STAR_OBJ_NUMBER),           \
    G(STAR_OBJ_STRING),           \
    G(STAR_OBJ_LAMBDA),           \
    G(STAR_OBJ_C_FUNC),           \
    G(STAR_OBJ_LIST),

enum StarObjE {STAR_OBJ_TYPES(GEN_ENUM)};
static const char* StarObjS[] = {STAR_OBJ_TYPES(GEN_STR)};

struct StarObj;
typedef struct StarObj* (*StarCFunc)(struct StarObj*);

typedef struct StarObj {
    int T;

    struct StarObj* value;
    struct StarObj* next;

    union {
        double num;
        struct { size_t len; char* data; }; // String or Atom
        struct { int num_args; StarCFunc func; }; // C Func
    };
} StarObj;

typedef StarObj StarEnv;

StarObj* starCar(StarObj* cons);
StarObj* starCdr(StarObj* cons);

StarObj* starNewObj(int T);
StarObj* starNewEnv();
StarObj starMakeEnv();

StarObj* starPutInEnv(StarEnv* env, StarObj* obj, char* key, size_t key_len);
StarObj* starGetFromEnv(StarEnv* env, const char* key);
bool starIsKeyInEnv(StarEnv* env, const char* key);
bool starIsObjInEnv(StarEnv* env, StarObj* obj);

#define STAR_NUM(n) (StarObj){.T=STAR_OBJ_NUMBER, .num=(n)};

#endif//STAR_OBJECT_H
