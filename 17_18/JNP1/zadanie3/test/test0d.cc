#include <cstdio>
#include <cassert>
#include <iostream>
#include "sejf.h"
using namespace std;

int main () {

  cout << "** Expected output:  **\n"
       << "OK\n"
       << "test\n"
       << "OK\n"
       << "OK\n"
       << "ALARM: WLAMANIE\n"
       << "\n"
       << "** Actual output:  **\n";

  Sejf s1("aaa", 2);

  s1[2];
  auto k1 = s1.kontroler();
  cout << k1 << "test\n";
  s1[2];
  s1[3];
  s1[4];
  cout << k1;
  s1[3];
  cout << k1;
  s1[2];
  cout << k1;

  return 0;
}
