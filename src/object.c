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

StarObj* starNewEnv() { return starNewObj(STAR_OBJ_TABLE); }

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


StarObj* starPutInEnv(StarEnv* env, StarObj* obj, char* key, size_t key_len) {
    assert(env->T == STAR_OBJ_TABLE);

    /*
      string field is the key;
      value is the stored object
      next points to the next one
    */

    StarObj* this = malloc(sizeof(StarObj));
    this->value = obj;

    this->len = key_len;
    this->data = key;

    while (env->next != NULL) env = env->next;

    env->next = this;

    return obj;
}

StarObj* starGetFromEnv(StarEnv* env, const char* key) {
    while (env->next != NULL) {
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
