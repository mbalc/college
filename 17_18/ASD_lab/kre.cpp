#include <algorithm>
#include <cstdio>
#include <limits.h>
using namespace std;

const long long UPPER_LIMIT = 2000000000ll;
const long long LOWER_LIMIT = 0;

struct Node {
  long long mini, maxi, lazy;
};

static int n, m, p, q, cap, out;
static Node tab[2000009];
static long long diff;
static void setCap() {
  cap = 1;
  while (n > cap) {
    cap <<= 1;
  }
}
static void updateNode(int i) {
  tab[i].mini = min(tab[2 * i].mini + tab[2 * i].lazy,
                    tab[2 * i + 1].mini + tab[2 * i + 1].lazy);
  tab[i].maxi = max(tab[2 * i].maxi + tab[2 * i].lazy,
                    tab[2 * i + 1].maxi + tab[2 * i + 1].lazy);
}

static void initTree() {
  for (int i = cap - 1; i > 0; --i) {
    updateNode(i);
  }
}

static void incTree(int index, long long difference) {
  tab[index].mini += difference;
  tab[index].maxi += difference;
}

static void lazyInit(int index) {
  if (tab[index].lazy != 0) {
    long long ourLazy = tab[index].lazy;
    if (index < cap) {
      tab[2 * index].lazy += ourLazy;
      tab[2 * index + 1].lazy += ourLazy;
    }
    incTree(index, ourLazy);
    tab[index].lazy = 0;
  }
}
wre

static int childRange(int left, int right) { return (right - left + 1) / 2; }

static void updateTree(int index, int left, int right) {
  // printf("%d %d   gd \n", left, right);
  lazyInit(index);

  if (q < left || right < p)
    return;

  incTree(index, diff);

  if (p <= left && right <= q) {
    tab[2 * index].lazy += diff;
    tab[2 * index + 1].lazy += diff;
  } else {
    updateTree(2 * index, left, right - childRange(left, right));
    updateTree(2 * index + 1, left + childRange(left, right), right);
  }

  if (index < cap)
    updateNode(index);
}

static void updateRange() { updateTree(1, 1, cap); }

static Node getTree(int index, int left, int right, int destL, int destR) {
  lazyInit(index);

  if (destR < left || right < destL)
    return Node{LLONG_MAX, LLONG_MIN, 0};

  if (destL <= left && right <= destR) {
    return Node{tab[index].mini + tab[index].lazy,
                tab[index].maxi + tab[index].lazy, 0};
  } else {
    Node a = getTree(2 * index, left, right - childRange(left, right), destL,
                     destR),
         b = getTree(2 * index + 1, left + childRange(left, right), right,
                     destL, destR);
    return Node{min(a.mini, b.mini), max(a.maxi, b.maxi), 0};
  }
}

static Node getRange(int destL, int destR) {
  return getTree(1, 1, cap, destL, destR);
}

static long long getValue(int index) { return getRange(index, index).maxi; }

static int getEnds() {
  int o = 0;
  if (1 < p)
    o += (getValue(p - 1) < getValue(p));
  if (q < n)
    o += (getValue(q) < getValue(q + 1));
  return o;
}

static void printLog(int lim) {
  if (lim > 2)
    printLog(lim / 2);
  for (int i = lim / 2; i < lim; ++i) {
    // printf("(%d: {%lld;%lld<%lld>}),  ", i, tab[i].mini, tab[i].maxi,
    //  tab[i].lazy);
  }
  // printf("\n");
}

static void log() { printLog(2 * cap); }

int main() {
  scanf("%d", &n);
  setCap();
  for (int i = 0; i < n; ++i) {
    int index = i + cap;
    scanf("%lld", &tab[index].mini);
    tab[index].maxi = tab[index].mini;
    if (i > 0 && tab[index - 1].mini < tab[index].mini)
      ++out;
  }
  // printf("out : %d\n", out);
  initTree();
  scanf("%d", &m);
  // log();
  for (int iter = 0; iter < m; ++iter) {
    scanf("%d%d%lld", &p, &q, &diff);
    int oldEnds = getEnds();
    updateRange();
    Node limits = getRange(1, n);
    // printf("limits: %lld %lld %lld < \n", limits.mini, limits.maxi,
    //  limits.lazy);
    if (limits.mini < LOWER_LIMIT || UPPER_LIMIT < limits.maxi) {
      diff *= -1;
      updateRange();
      printf("%d\n", -1);
    } else {
      out -= oldEnds;
      out += getEnds();
      printf("%d\n", out);
    }
    // log();
  }
  return 0;
}
