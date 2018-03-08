#include <algorithm>
#include <climits>
#include <cstdio>
#include <functional>
#include <set>
#include <tuple>
#include <vector>

using namespace std;

const int max_count = 240007;
const size_t INF = ULLONG_MAX;
uint n, m, k;

using path = tuple<int, int>;

vector<path> gph[max_count];

bool vted[max_count];
unsigned long long dist[max_count];

struct Cmp {
  bool operator()(int p1, int p2) {
    if (dist[p1] == dist[p2])
      return p1 < p2;
    return dist[p1] < dist[p2];
  }
};

void dijkstra() {
  dist[0] = 0;
  int src, tgt;
  set<int, Cmp> nodes;
  nodes.insert(0);
  while (!nodes.empty()) {
    src = *nodes.begin();
    nodes.erase(src);
    vted[src] = 1;
    for (path p : gph[src]) {
      tgt = get<0>(p);
      if (!vted[tgt]) {
        if (dist[src] + get<1>(p) < dist[tgt]) {
          nodes.erase(tgt);
          dist[tgt] = dist[src] + get<1>(p);
          nodes.insert(tgt);
        }
      }
    }
  }
}

long long findResult() {
  if (dist[n - 1] == INF)
    return -1;
  unsigned long long out = dist[n - 1];
  for (uint i = 1; i <= k; ++i)
    out = min(out, dist[(i * n) + n - 1]);
  return out;
}

int main() {
  for (size_t i = 0; i <= max_count - 1; i++)
    dist[i] = INF;
  int from, to, disc, price;
  scanf("%u%u%u", &n, &m, &k);
  for (size_t i = 1; i <= m; i++) {
    scanf("%d%d%d%d", &from, &to, &disc, &price);
    for (size_t i = 0; i <= k; i++) {
      gph[(i * n) + from].push_back(path((i * n) + to, price));
      if (i < k)
        gph[(i * n) + from].push_back(path((i * n) + n + to, price - disc));
    }
  }
  dijkstra();
  printf("%lld\n", findResult());
  return 0;
}
