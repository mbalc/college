#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

void ASSERT_EQ(auto a, auto b) {
  assert(a == b);
}

int main () {
  Sejf s1("aaa", 1), s2("bbb", 1);

  cout << s1[0];
  swap(s1, s2);
  cout << s1[0];
  swap(s1, s2);
  cout << s1[0];
  swap(s1, s2);
  cout << s1[0];
  cout << "\nexpected output: " << 9798-1-1 << "\n";

  return 0;
}
