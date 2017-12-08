#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

int main () {

  string blah = "new";
  Sejf s1(blah, 2);
  assert(s1[1] == 'e');
  blah[1] = 'o';
  cout << blah << '\n';
  assert(s1[1] == 'e');
  assert(s1[1] == -1);
  assert(s1[2] == -1);

  return 0;
}
