#include <cassert>
#include <string.h>
#include "dict.h"

using namespace jnp1;

namespace {
    int streq(const char* s1, const char* s2, size_t len) {
        if (s1 == s2)
            return 1;

        if (s1 == NULL || s2 == NULL)
            return 0;

        return strncmp(s1, s2, len) == 0;
    }

    unsigned long test() {
        unsigned long id = dict_new();
        dict_insert(id, "k0", "a");
        return id;
    }

    unsigned long id = test();
}

int main() {
    const char* str = dict_find(id, "k0");
    assert(streq(str, "a", 2));
    dict_delete(id);
}
