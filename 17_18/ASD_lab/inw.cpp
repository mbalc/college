#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

vector<unsigned long> v, o[1000007];

unsigned long tab[1000007], input[1000007], n;

unsigned long finde(unsigned long at) {
  if (tab[at] == at)
    return at;
  return finde(tab[at]);
}

bool cmp(const vector<unsigned long>& a, const vector<unsigned long>& b) {
  if (a.empty())
    return false;
  if (b.empty())
    return true;
  return a[0] < b[0];
}

int main() {
  scanf("%lu", &n);

  for (unsigned long i = 0; i < n; ++i) {
    scanf("%lu", &input[i]);
  }
  for (long i = n - 1; i >= 0; --i) {
    unsigned long a = input[i];
    tab[a] = a;
    if (v.empty() || v.back() > a) {
      v.push_back(a);
      tab[a] = a;
    } else {
      int top = v.back();
      tab[a] = top;
      v.pop_back();
      while (!v.empty() && a > v.back()) {
        tab[v.back()] = top;
        v.pop_back();
      }
      v.push_back(top);
    }
  }

  for (unsigned long i = 1; i <= n; ++i) {
    tab[i] = finde(i);
    o[tab[i]].push_back(i);
  }

  for (unsigned long i = 1; i <= n; ++i) {
    sort(o[i].begin(), o[i].end());
  }

  int count = 0;
  for (unsigned long i = 1; i <= n; ++i) {
    if (!o[i].empty()) {
      ++count;
    }
  }

  printf("%d\n", count);

  for (unsigned long i = 1; i <= n; ++i) {
    if (!o[i].empty()) {
      printf("%lu ", o[i].size());
      for (unsigned long el : o[i]) {
        printf("%lu ", el);
      }
      printf("\n");
    }
  }
  return 0;
}
