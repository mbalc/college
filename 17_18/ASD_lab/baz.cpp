// For each query [k] we want to determine and return an amount of money that
// will allow to buy any possible combination of [k] products (from a set of
// products with given, non-descending costs) of an even summary cost, or return
// '-1' if there's no such set.

// To solve this, for each query we're going to find the sum of [k] largest
// costs (to do this in a constant time we'll preprocess suffix sums of our cost
// set) and then try to find a biggest possible odd summary cost by considering
// some corner cases (see code)

#include <cstdio>
#include <cstdlib>
#include <algorithm>

static void suffixSum(int size, int* table) {
  int* ptr = table;
  for (int i = 0; i < size - 1; ++i) {
    *(ptr + 1) += *ptr;
    ++ptr;
  }
}

class InputData {
 public:
  int prodQuantity, queryNumber;
  int* prodCost;
  InputData() {
    std::scanf("%d", &prodQuantity);

    prodCost = new int[prodQuantity];
    for (int i = 0; i < prodQuantity; ++i) {
      std::scanf("%d", &prodCost[i]);
    }
    suffixSum(prodQuantity, prodCost);

    std::scanf("%d", &queryNumber);
  }
};

int main() {
  InputData input;
  int query;

  for (int i = 0; i < input.queryNumber; ++i) {
    std::scanf("%d", &query);

    std::printf("%d\n", input.prodCost[query]);
  }

  return 0;
}
