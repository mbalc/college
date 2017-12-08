#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

void ASSERT_EQ(auto a, auto b) {
  assert(a == b);
}

int main () {
  Sejf s("To jest złoto!", 2);
      if(!s.kontroler())assert(false);
      s[2];
      if(!s.kontroler())assert(false);
      s[42];
      if(!s.kontroler())assert(false);
      s[0];
      if(s.kontroler())assert(false);

      s += 2;
      if(!s.kontroler())assert(false);
      s *= 3;
      if(!s.kontroler())assert(false);
      s -= 2;
      for (unsigned i = 0; i < 4; ++ i) {
          if(!s.kontroler())assert(false);
          s[0];
      }
      if(s.kontroler())assert(false);

  return 0;
}
