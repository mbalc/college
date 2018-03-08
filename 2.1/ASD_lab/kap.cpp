#include <cstdio>
#include <climits>
#include <set>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

const int max_count = 220007;
const size_t INF = ULLONG_MAX;
unsigned int n;
bool vted[max_count];
unsigned long long dist[max_count];

struct pt {
  long long x;
  long long y;
  vector <int> link;
};

struct Cmp {
  bool operator()(int p1, int p2) {
    if (dist[p1] == dist[p2])
      return p1 < p2;
    return dist[p1] < dist[p2];
  }
};


pt tab[max_count];
int dex[max_count];

void read() {
  scanf("%u\n", &n);
  for (size_t i = 1; i <= n; i++) {
    scanf("%lld%lld\n", &tab[i].x, &tab[i].y);
    dex[i] = i;
  }
}

template <typename F>
void link(F get) {
  int last = 1;
  for (size_t i = 1; i <= n+1; i++) {
    if (i == n+1 || get(tab[dex[last]]) != get(tab[dex[i]])) {
      for (size_t k = last; k < i - 1; k++) {
        tab[dex[k]].link.push_back(dex[k+1]);
      }
      tab[dex[i - 1]].link.push_back(dex[last]);
      if (i != n + 1) for (size_t k = last; k < i; k++) {
        tab[dex[k]].link.push_back(dex[i]);
        tab[dex[i]].link.push_back(dex[k]);
      }
      last = i;
    }
  }
}

void init() {
  sort(dex+1, dex+n+1, [](int a, int b){ if (tab[a].x == tab[b].x) return tab[a].y < tab[b].y; return tab[a].x < tab[b].x; });
  link([](pt a){ return a.x; });
  sort(dex+1, dex+n+1, [](int a, int b){ if (tab[a].y == tab[b].y) return tab[a].x < tab[b].x; return tab[a].y < tab[b].y; });
  link([](pt a){ return a.y; });
}

long long getDist(int p1, int p2) {
  return min(abs(tab[p1].x - tab[p2].x), abs(tab[p1].y - tab[p2].y));
}

long long dijkstra() {
  dist[1] = 0;
  int src;
  set<int, Cmp> nodes;
  nodes.insert(1);
  while (!nodes.empty()) {
    src = *nodes.begin();
    // printf ("run %d\n", src);
    nodes.erase(src);
    vted[src] = 1;
    for (int tgt : tab[src].link) {
      if (!vted[tgt]) {
        if (dist[src] + getDist(src, tgt) < dist[tgt]) {
          nodes.erase(tgt);
          // printf("set from %d to %d in %d\n", src, tgt, getDist(src, tgt) + dist[src]);
          dist[tgt] = dist[src] + getDist(src, tgt);
          nodes.insert(tgt);
        }
      }
    }
  }
  return dist[n];
}

int main() {
  for (size_t i = 1; i <= max_count - 1; i++)
    dist[i] = INF;
  read();
  init();
  printf("%lld\n", dijkstra());
  return 0;
}
