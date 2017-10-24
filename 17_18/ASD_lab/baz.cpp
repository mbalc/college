// Task: For each query [k] we want to determine and return an amount of money
// that will allow to buy any possible combination of [k] products (from a set
// of products with given, non-descending costs) of an odd summary cost, or
// return '-1' if there's no such set.

// Approach: To solve this, for each query we're going to find the sum of [k]
// largest costs (to do this in a constant time we'll preprocess suffix sums of
// our cost set) and then try to find a biggest possible odd summary cost by
// considering some corner cases (see code)

/* Example test:

  IN:
4
1 2 3 4
3
2
3
4

  OUT:
7
9
-1

*/

#include <cstdio>

static void processSuffixSums(int size, long long *table) {
  long long *ptr = table;
  ptr += size - 2;
  for (int i = 0; i < size - 1; ++i) {
    *ptr += *(ptr + 1);
    --ptr;
  }
  ++ptr;
  for (int i = 0; i < size; ++i) {
    ++ptr;
  }
}

static void fillMinMaxTable(int size, int table[][2][2], long long *values) {
  auto getPos = [size](int i, int j) {
    if (i == -1) i = 0;
    return j ? size - i - 1 : i;
  };

  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < 4; ++j) {
      int index = getPos(i, j / 2);
      table[index][j / 2][j % 2] =
          table[getPos(i - 1, j / 2)][j / 2][j % 2];  // copy precedessor
      if (values[index] % 2 == j % 2) table[index][j / 2][j % 2] = index;
    }
  }
}

long long getNewSum(long long oldSum, long long oldCost, long long newCost) {
  if (oldCost == -1ll || newCost == -1ll) return -1ll;
  return oldSum - oldCost + newCost;
}

long long max(long long a, long long b) { return a < b ? b : a; }

int main() {
  int prodQuantity, queryQuantity, query;
  scanf("%d", &prodQuantity);

  long long prodCost[prodQuantity];
  int minMaxEvenOdd[prodQuantity][2][2];
  /* table cell at [n][m][p] contains location index of the closest even
   * ([p] == 0) or odd ([p] == 1) product that has smaller ([m] == 0) or bigger
   * ([m] == 1) cost (and thus the location index) than item located at [n]-th
   * position*/

  for (int i = 0; i < prodQuantity; ++i) {
    scanf("%lld", &prodCost[i]);
    for (int j = 0; j < 4; ++j) minMaxEvenOdd[i][j / 2][j % 2] = -1;
  }

  fillMinMaxTable(prodQuantity, minMaxEvenOdd, prodCost);
  processSuffixSums(prodQuantity, prodCost);

  auto getValue = [prodQuantity, &prodCost](int index) {
    if (index < 0 || index >= prodQuantity) return -1ll;
    if (index == prodQuantity - 1) return prodCost[index];
    return prodCost[index] - prodCost[index + 1];
  };

  auto findOutput = [getValue, &prodCost, &minMaxEvenOdd](int index) {
    long long sum = prodCost[index];
    if (sum % 2 == 1) return sum;
    if (index == 0) return -1ll;  // we can't replace anything
    return max(
        getNewSum(
            sum,
            getValue(minMaxEvenOdd[index][1][0]),     // replace smallest
            getValue(minMaxEvenOdd[index - 1][0][1])  // even member of the
            ),                                        // sum with biggest
                                                      // odd product not
                                                      // within the sum
        getNewSum(sum, /* replacing odd with even */
                  getValue(minMaxEvenOdd[index][1][1]),
                  getValue(minMaxEvenOdd[index - 1][0][0])));

  };

  scanf("%d", &queryQuantity);

  for (int i = 0; i < queryQuantity; ++i) {
    scanf("%d", &query);
    query = prodQuantity - query;
    printf("%lld\n", findOutput(query));
  }

  return 0;
}
