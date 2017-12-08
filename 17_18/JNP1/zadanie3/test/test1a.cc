#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

int main () {

  Sejf s1("aaa", 2);
  assert(s1[0] == 'a');
  (((s1 *= 3) -= 1) += 0) -= 0;
  assert(s1[0] == 'a');
  assert(s1[0] == 'a');
  assert(s1[0] == -1);

  return 0;
}
