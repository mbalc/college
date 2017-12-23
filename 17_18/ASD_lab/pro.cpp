#include <cstdio>
#include <queue>
#include <algorithm>

using namespace std;
const int LIM = 100007;

using my_container_t = vector<int>;

unsigned int parents[LIM], prices[LIM], n, m, k;
my_container_t sons[LIM], vec;

auto cmp = [](int a, int b) { return prices[a] > prices[b]; };
priority_queue<int, my_container_t, decltype(cmp)> nodes(cmp);

void load() {
  unsigned int from, to;
  scanf ("%u%u%u", &n, &m, &k);
  for (size_t i = 1; i <= n; i++) {
    scanf("%u", &prices[i]);
  }
  for (size_t i = 1; i <= m; i++) {
    scanf("%u%u", &to, &from);
    ++parents[to];
    sons[from].push_back(to);
  }
}

void init() {
  for (size_t i = 1; i <= n; i++) {
    if (parents[i] == 0) nodes.push(i);
  }
}

void doContract(int at) {
  for (int i : sons[at]) {
    --parents[i];
    if (parents[i] == 0) nodes.push(i);
  }

}

int main () {
  load();
  init();
  unsigned int out = 0, at;
  for (size_t i = 0; i < k; i++) {
    // printf("%d\n", nodes.size());
    at = nodes.top();
    out = max(out, prices[at]);
    // printf("%d\n", out);
    nodes.pop();
    doContract(at);
  }
  printf("%u\n", out);
  return 0;
}
