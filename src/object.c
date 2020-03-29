#include "object.h"

StarObj* starNewEnv() {
    StarObj* o = starAllocObj();
    o->T = STAR_OBJ_TABLE;
    o->value = NULL;
    o->next = NULL;
    return o;
}

StarObj starMakeEnv() {
    StarObj obj;
    obj.T = STAR_OBJ_TABLE;
    obj.value = NULL;
    obj.next = NULL;
    obj.num = 0.0f;
    obj.len = 0;
    obj.data = NULL;
    return obj;
}


StarObj* starPutInEnv(StarEnv* env, StarObj* obj, const char* key) {
    assert(env->T == STAR_OBJ_TABLE);

    /*
      string field is the key;
      value is the stored object
      next points to the next one
    */

    StarObj* this = malloc(sizeof(StarObj));
    this->value = obj;

    this->len = strlen(key);

    const size_t len = strlen(key);
    this->data = malloc(len+1);
    this->data[len] = '\0';
    for (size_t i = 0; i < len; i++)
        this->data[i] = key[i];

    StarObj* place = env;
    while (place->next != NULL)
        place = place->next;
    place->next = this;

    return obj;
}

StarObj* starGetFromEnv(StarEnv* env, const char* key) {
    env = env->next;

    while (env != NULL) {
        if (env->len == strlen(key)){
            bool match = true;
            for (size_t i = 0; i < env->len; i++)
                if (key[i] != env->data[i]) {
                    match = false;
                    break;
                }
            if (match) return env->value;
        }

        env = env->next;
    }

    return NULL;
}

bool starIsKeyInEnv(StarEnv* env, const char* key) {
    return false;
}

bool starIsObjInEnv(StarEnv* env, StarObj* obj) {
    return false;
}

StarObj* starCar(StarObj* cons) {
    return cons;
}

StarObj* starCdr(StarObj* cons) {
    return cons->next;
}

StarObj* starCons(StarObj* list, StarObj* el) {
    el->value = list->value;
    el->next = list->next;
    list->value = NULL;
    return list;
}

void starListAdd(StarObj* list, StarObj* item) {
    StarObj* first = list->value;
    if (!first) {
        list->value = item;
        return;
    }

    while (first) {
        if (!first->next) {
            first->next = item;
            return;
        } 
        first = first->next;
    }
}

StarObj* starAllocNum(double num) {
    StarObj* o = malloc(sizeof(StarObj));
    o->T = STAR_OBJ_NUMBER;
    o->refcount = 1;
    o->num = num;
    return o;
}

StarObj* starAllocCFunc(StarCFunc func) {
    StarObj* o = malloc(sizeof(StarObj));
    o->T = STAR_OBJ_NUMBER;
    o->refcount = 1;
    o->func = func;
    return o;
}

StarObj* starAllocList() {
    StarObj* o = malloc(sizeof(StarObj));
    o->T = STAR_OBJ_LIST;
    o->value = NULL;
    o->next = NULL;
    o->refcount = 1;
    return o;
}

StarObj* starAllocObj() {
    StarObj* o = malloc(sizeof(StarObj));
    o->T = STAR_OBJ_NONE;
    o->refcount = 1;
    return o;
}

StarObj* starRef(StarObj* object) {
    object->refcount++;
    return object;
}

StarObj* starRel(StarObj* object) {
    object->refcount--;

    if (object->refcount <= 0){
        starDeallocObj(object);
    }

    return (object->refcount <= 0 ? NULL : object);
}

void starDeallocObj(StarObj* object) {
    switch (object->T) {
        case STAR_OBJ_NUMBER: {
            free(object);
            return;
        }

        case STAR_OBJ_ATOM:
        case STAR_OBJ_STRING: {
            if (object->data)
                free(object->data);
            object->data = NULL;
            free(object);
            return;
        }

        case STAR_OBJ_LIST: {
            StarObj* first = object->value;
            starRel(first);

            if (first && first->refcount > 0) {
                do  {
                    starRel(first);
                    first = first->next;
                } while(first);
            }

            return;
        }

        default: {
            printf("Unimplemented destructor for object type %s\n",
                   StarObjS[object->T]);
            return;
        }
    }
}

#define BUFF_SIZE (512)

void printObjRec(StarObj* o, char* os) {
    switch (o->T) {
        case STAR_OBJ_NUMBER: {
            sprintf(strlen(os)+os, "%f", o->num);
            return;
        }

        case STAR_OBJ_ATOM:
        case STAR_OBJ_STRING: {
            sprintf(strlen(os)+os, "%.*s", (int)o->len, o->data);
            return;
        }

        case STAR_OBJ_LIST: {
            sprintf(strlen(os)+os, "(");
            StarObj* it = o->value;
            while (it) {
                printObjRec(it, os);
                if (it->next != NULL)
                    sprintf(strlen(os)+os, " ");
                it = it->next;
            }
            sprintf(strlen(os)+os, ")");
            return;
        }
    }
}

void starPrintObj(StarObj* object) {
    char os[BUFF_SIZE] = {'\0'};
    printObjRec(object, os);
    printf("%s\n", os);
}
