#include "eval.h"

StarObj* starEvalObj(StarEnv* env, StarObj* obj) {
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

StarObj* evalList(StarEnv* env, StarObj* branch) {
    if (!branch->value) {
        printf("Failed to evaluate an empty list");
        return starAllocNum(0);
    }

    StarObj* args = branch->value;
    StarObj* first = starEvalObj(env, args);

    if (!first) {
        printf("Failed to find (%.*s) in the env\n",
               (int)branch->value->len, branch->value->data);
        return starAllocNum(0);
    }

    //StarObj* args = starEvalObj(first->next, env);
    StarObj* list = starAllocList();

    StarObj* evArgs = list;

    bool fst = true;
    while (args->next) {
        args = args->next;

        if (fst) {
            list->value = starEvalObj(env, args);
            list = list->value;
            fst = false;
        } else {
            list->next = starEvalObj(env, args);
            list = list->next;
        }
    }

    StarObj* result = first->func(env, evArgs);
    starRel(evArgs);
    return result;
}

StarObj* evalProgn(StarEnv* env, StarObj* tree) {
    StarObj* branch = tree->value;

    while (branch) {
        branch->T = STAR_OBJ_LIST;

        StarObj* result = evalList(env, branch);

        if (branch->next == NULL) {
            return result;
        } else {
            starRel(result);
        }

        branch = branch->next;
    }

    return NULL;
}

StarObj* starEvalProgn(StarEnv* env, StarObj* obj) {
    return evalProgn(env, obj);
}

StarObj* starEval(StarEnv* env, char* buff, size_t len) {
    StarObj* tree = starParseProgram(buff, len);

    StarObj* result = evalProgn(env, tree);
    
    starDeallocObj(tree);

    return result;
}
