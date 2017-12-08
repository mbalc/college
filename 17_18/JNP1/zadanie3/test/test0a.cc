#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

int main () {

  Sejf s("a\xff", 3);

  cout << (short int)(unsigned char)'\xff' << '\n';


  assert(s[0] == 'a');
  assert(s[1] == 255);
  assert(s[1] == (int)((unsigned char) -1));
  assert(s[1] == -1);
  assert(s[1] != (int)((unsigned char) -1));

  return 0;

}
