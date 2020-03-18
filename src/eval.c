#include "eval.h"

StarObj* evalObj(StarObj* obj, StarObj* env) {
    if (obj->T == STAR_OBJ_ATOM) {
        // Get value from env

        char key[512];
        sprintf(key, "%.*s", (int)obj->len, obj->data);
        return starGetFromEnv(env, key);
    }

    if (obj->T == STAR_OBJ_NUMBER) return obj;
    if (obj->T == STAR_OBJ_STRING) return obj;

    return obj;
}

StarObj evalList(StarObj* branch, StarObj* env) {
    if (!branch->value) {
        printf("Failed to evaluate an empty list");
        return STAR_NUM(0);
    }

    StarObj* first = evalObj(branch->value, env);

    if (!first) {
        printf("Failed to find (%.*s) in the env\n",
               (int)branch->value->len, branch->value->data);
        return STAR_NUM(0);
    }

    //StarObj* args = evalObj(first->next, env);
    
    return STAR_NUM(1);
}

StarObj evalProgn(StarObj* tree, StarObj* env) {

    StarObj* list = tree->value;
    StarObj* branch = list;

    while (branch) {
        evalList(branch, env);
        branch = branch->next;
    }
    
    return STAR_NUM(100);
}

StarObj starEval(StarEnv* env, char* buff, size_t len) {
    StarObj* tree = starParseProgram(buff, len);

    StarObj result = evalProgn(tree, env);
    
    // Leak
    free(tree);

    return result;
}
