#include "dict.h"
#include <cassert>
#include <string.h>

using namespace jnp1;

int main () {
  unsigned long dict;
  char buf[4] = "foo";
  dict = dict_new();
  dict_insert(dict, buf, buf);
  buf[0] = 'b';
  assert(dict_find(dict, "foo") != NULL);
  assert(strncmp(dict_find(dict, "foo"), "foo", 4) == 0);
  assert(strncmp(dict_find(dict, "foo"), "boo", 4) != 0);
  return 0;
}
