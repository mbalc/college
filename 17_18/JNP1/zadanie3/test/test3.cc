#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

int main () {

  Sejf s("abc", 0);
    assert(s[0] == -1);
    s += 2;
    assert(s[0] == 'a');
    s += -1;
    assert(s[0] == 'a');
    assert(s[0] == -1);

  return 0;
}
