#include <cassert>
#include <cstdio>

using namespace std;

using Hash = long long;
using Result = int;

const long long MOD = 1000000007, INPUT_LIMIT = 300007;
const Hash HASH_BASE = 30;

const Result LESS = -1, MORE = 1, EQUAL = 0;

long long n, len;
Hash suffHashes[INPUT_LIMIT];
char txt[INPUT_LIMIT];
Hash pwrs[INPUT_LIMIT];

Hash modDiff(Hash a, Hash b) {
  return (a - b + MOD) % MOD;
}

Hash modMul(Hash a, Hash b) {
  return (a * b) % MOD;
}

void preprocess() {
  suffHashes[len] = 0;
  for (long long i = len - 1; i >= 0; --i) {
    suffHashes[i] = (modMul(suffHashes[i + 1], HASH_BASE) + (txt[i] - 'a' + 1)) % MOD;
  }

  pwrs[0] = 1;
  for (long long i = 1; i < len + 3; ++i) {
    pwrs[i] = modMul(pwrs[i - 1], HASH_BASE);
    // fprintf(stderr, "%d ", suffHashes[i]);
  }
  // fprintf(stderr, "\n");
}

Hash slice(size_t from, size_t to) {
  Hash power = pwrs[to - from];
  return modDiff(suffHashes[from], modMul(suffHashes[to], power));
}

Result compare(size_t a1, size_t a2, size_t b1, size_t b2) {
  int diffA = a2 - a1, diffB = b2 - b1;
  if (diffA < diffB) {
    return -compare(b1, b2, a1, a2);
  }

  if (diffB < 1)
    return EQUAL;

  if (diffB != diffA) {
    Result out = compare(a1, a1 + diffB, b1, b2);
    if (out == EQUAL)
      return MORE;
    return out;
  }

  if (diffA == 1) {
    long long cmp = txt[a1] - txt[b1];
    if (cmp > 0)
      return MORE;
    if (cmp == 0)
      return EQUAL;
    return LESS;
  }

  long long aMid = a1 + diffA / 2, bMid = b1 + diffA / 2;

  // Result out = compare(a1, aMid, b1, bMid);
  //
  // fprintf(stderr, "%d <=> %d == %d\n", out == EQUAL, slice(3, 6), slice(5, 8));
  // assert((out == EQUAL) == (slice(a1, aMid) == slice(b1, bMid)));

  if (slice(a1, aMid) == slice(b1, bMid))
    return compare(aMid, a2, bMid, b2);
  return compare(a1, aMid, b1, bMid);
}

int main() {
  size_t a1, a2, b1, b2;
  scanf("%lld%lld", &len, &n);
  scanf("%s", txt);

  preprocess();

  for (long long i = 0; i < n; ++i) {
    scanf("%lu%lu%lu%lu", &a1, &a2, &b1, &b2);
    --a1;
    --b1;

    Result out = compare(a1, a2, b1, b2);

    switch (out) {
      case LESS: printf("<\n"); break;
      case EQUAL: printf("=\n"); break;
      case MORE: printf(">\n"); break;
    }
  }
}
