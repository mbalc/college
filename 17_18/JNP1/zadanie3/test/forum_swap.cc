 #include <iostream>
 #include <algorithm>
 #include <cassert>
 #include "sejf.h"
 using namespace std;

int main() {
   Sejf s1("zawartosc", 2);
   Sejf s2("zawartosc", 0);
  auto k = s1.kontroler();
  std::swap(s1, s2);
  if (k) {
      cout << "TAK\n";
  } else {
      cout << "NIE\n";
  }
  assert (!k);
  return 0;
}
