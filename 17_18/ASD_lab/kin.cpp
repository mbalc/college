#include <cstdio>
using namespace std;

using numb_t = long long;

const long long MOD = 1000000000;

class Tree {

  size_t ceilPwr(size_t n) {
    size_t out = 1;
    while (out < n) {
      out <<= 1;
    }
    return out * 2;
  }

public:
  size_t cap;
  numb_t *tab;

  Tree(size_t n) {
    this->cap = ceilPwr(n);
    this->tab = new numb_t[this->cap + 7];
  }
  size_t dex(size_t index) { return (cap / 2) + index - 1; }
  size_t parentDex(size_t index) { return index / 2; }

  numb_t *leftSon(numb_t *from) { return from + (from - this->tab); }
  numb_t *rightSon(numb_t *from) { return this->leftSon(from) + 1; }

  void update(size_t index) {
    numb_t *node = &(this->tab[index]);
    *node = *leftSon(node) + *rightSon(node);
    if (index > 1) {
      update(index / 2);
    }
  }

  void set(size_t index, numb_t value) {
    // printf("setting");
    size_t place = this->dex(index);
    this->tab[place] = value;
    if (index > 1)
      update(place / 2);
  }

  numb_t get(size_t from, size_t to) {
    // printf("getting from %d to %d\n", from, to);
    numb_t result = this->composeGet(this->dex(from), this->dex(to));
    // printf("gotten %lld\n", result);
    return result;
  }

  numb_t composeGet(size_t from, size_t to) {
    // printf("  call %d to %d\n", from, to);
    if (from > to)
      return 0;
    if (from == to) {
      // printf("    E;;add %d \n", from);
      return this->tab[to];
    }

    numb_t sum = 0;
    switch (from % 2) {
    case 0: {
      from = this->parentDex(from);
      break;
    }
    case 1: {
      sum += this->tab[from];
      // printf("    L: add %d \n", from);
      from = this->parentDex(from) + 1;
      break;
    }
    }

    switch (to % 2) {
    case 0: {
      sum += this->tab[to];
      // printf("    R: add %d \n", to);
      to = this->parentDex(to) - 1;
      break;
    }
    case 1: {
      to = this->parentDex(to);
      break;
    }
    }

    return (sum + this->composeGet(from, to)) % MOD;
  }

  void reset() {
    for (size_t i = 1; i <= (this->cap) / 2; ++i)
      this->set(i, 0);
  }

  void printLog(size_t lim) {
    if (lim > 2)
      this->printLog(lim / 2);
    for (size_t i = lim / 2; i < lim; ++i) {
      // printf("%lu: %lld ", i, this->tab[i]);
    }
    // printf("\n");
  }

  void log() { this->printLog(this->cap); }
};

int main() {
  size_t n, k;
  scanf("%lu%lu", &n, &k);
  Tree tree(n);
  numb_t k_inversions[n + 1], input[n + 1];

  for (size_t i = 1; i <= n; ++i) {
    k_inversions[i] = 1;
    scanf("%lld", &input[i]);
  }

  // printf("read!\n");

  for (size_t k_i = 2; k_i <= k; ++k_i) {
    for (size_t it = 1; it <= n; ++it) {
      numb_t val = input[it];
      tree.set(val, k_inversions[val]);
      k_inversions[val] = tree.get(val + 1, n);
      tree.log();
    }
    tree.reset();
    tree.log();
    // printf("[ ");
    // for (size_t it = 1; it <= n; ++it) {
    // printf("%lld ", k_inversions[it]);
    // }
    // printf("]\nk++\n\n");
  }

  numb_t sum = 0;
  for (int i = 1; i <= n; ++i)
    sum = (sum + k_inversions[i]) % MOD;

  printf("%lld\n", sum);

  return 0;
}

// for (j = 2; j <= k; ++j) {
//   reset(tree);
//   for (i = 1; i <= n; ++i) {
//     wynik[j, a[i]] = tree.suma(a[i], n);
//     tree.set(i, wynik(j - 1, i));
//   }
// }
