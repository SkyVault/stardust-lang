#include "object.h"

StarObj* starNewObj(int T) {
    StarObj* obj = malloc(sizeof(StarObj));
    obj->T = T;
    obj->value = NULL;
    obj->next = NULL;
    obj->num = 0.0f;
    obj->len = 0;
    obj->data = NULL;
    return obj;
}
