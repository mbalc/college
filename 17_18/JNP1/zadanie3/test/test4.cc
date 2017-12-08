#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

void ASSERT_EQ(auto a, auto b) {
  assert(a == b);
}

int main () {
  Sejf s1("aaa", 2);
    ASSERT_EQ(s1[0], 'a');
    (s1 *= 3) -= 1;
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[0], 'a');
    ASSERT_EQ(s1[0], -1);
  return 0;
}
