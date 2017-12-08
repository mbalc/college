#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

int main () {

  Sejf s1("aaa", 2);

  /** the next three lines should throw compilation errors **/
  // Sejf s4 = s1;
  // Sejf s5(s1);
  // s6 = s1;

  Sejf s2("bbb", 0);
  /** the next line should throw a compilation error **/
  // if (s1.kontroler() < s2.kontroler()) cout<<"wrong\n";

  return 0;

}
