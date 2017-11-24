#include <algorithm>
#include <cstdio>
#include <cstring>
#include <limits.h>
using namespace std;

struct Node {
  long long sum;
  long long lazy;
};

static int n, m, p, q, cap, out;
static Node tab[4000009];
static long long diff;

static void setCap() {
  cap = 1;
  while (n > cap) {
    cap <<= 1;
  }
}

static long long evaluateLazy(long long lazy, int range,
                              long long currentValue) {
  switch (lazy) {
  case -1:
    return 0;
  case 1:
    return range;
  default:
    return currentValue;
  }
}

static int range(int left, int right) { return right - left + 1; }
static int childRange(int left, int right) { return range(left, right) / 2; }

static void updateNode(int i, int range) {
  tab[i].sum = evaluateLazy(tab[2 * i].lazy, range / 2, tab[2 * i].sum) +
               evaluateLazy(tab[2 * i + 1].lazy, range / 2, tab[2 * i + 1].sum);
}

static void setNode(int index, long long lazy, int left, int right) {
  tab[index].sum = evaluateLazy(lazy, range(left, right), tab[index].sum);
}

static void lazyInit(int index, int left, int right) {
  if (tab[index].lazy != 0) {
    long long ourLazy = tab[index].lazy;
    if (index < cap) {
      tab[2 * index].lazy = ourLazy;
      tab[2 * index + 1].lazy = ourLazy;
    }
    setNode(index, ourLazy, left, right);
    tab[index].lazy = 0;
  }
}

static void updateTree(int index, int left, int right) {
  // printf("%d %d   gd \n", left, right);
  lazyInit(index, left, right);

  if (q < left || right < p)
    return;

  if (p <= left && right <= q) {
    setNode(index, diff, left, right);
    if (index < cap) {
      tab[2 * index].lazy = diff;
      tab[2 * index + 1].lazy = diff;
    }
  } else {
    updateTree(2 * index, left, right - childRange(left, right));
    updateTree(2 * index + 1, left + childRange(left, right), right);
  }

  if (index < cap)
    updateNode(index, right - left + 1);
}

static void updateRange() { updateTree(1, 1, cap); }

static long long getTree(int index, int left, int right, int destL, int destR) {
  lazyInit(index, left, right);

  if (destR < left || right < destL)
    return 0ll;
  if (destL <= left && right <= destR)
    return tab[index].sum;

  long long a = getTree(2 * index, left, right - childRange(left, right), destL,
                        destR),
            b = getTree(2 * index + 1, left + childRange(left, right), right,
                        destL, destR);
  return a + b;
}

static long long getRange(int destL, int destR) {
  return getTree(1, 1, cap, destL, destR);
}

static void printLog(int lim, int range) {
  if (lim > 2)
    printLog(lim / 2, range * 2);
  int nth = lim / 2;
  for (int i = lim / 2; i < lim; ++i) {
    nth = i - lim / 2;
    printf("(%02d-%02d): {%lld<%lld>},  ", 1 + nth * range, (nth + 1) * range,
           tab[i].sum, tab[i].lazy);
  }
  printf("\n");
}

static void log() { printLog(2 * cap, 1); }

int main() {
  scanf("%d", &n);
  setCap();
  scanf("%d", &m);
  // log();
  char str[8];
  for (int iter = 0; iter < m; ++iter) {
    scanf("%d%d%s", &p, &q, str);
    if (str[0] == 'B')
      diff = 1;
    else
      diff = -1;
    // for (int j = 0; j < strlen(str); ++j) {
    //   printf("%d: (%d)\n", j, diff);
    // }
    updateRange();
    long long limits = getRange(1, n);
    // printf("limits: %lld %lld %lld < \n", limits.mini, limits.maxi,
    //  limits.lazy);
    printf("%lld\n", limits);
    // log();
  }
  return 0;
}

/*
  przeszukaj(pocz, / p * -1) == {
    przeszukaj(pocz, kin, W, lon) {}
            if (pocz == 0 && kon == len-1) {
            dirty[w]=1
            ile_bialych[w] = len
        } else {
            if (dirty[w]) {
                dirty(2*w)= dirty[2*w + 1] = true;
                dirty[w] = false
                ile_bialych[2*w]=ile_bialych[2*w+1]=ile_bialych[w]/2

            }
            przeszukaj(pocz, min(len/2-1, kon), 2*w, len/2)
            przeszukaj(max(len/2, pocz)-len/2, kon-len/2, 2*w+1, len/2)
            ile_bialych[w]=ile_bialych[2*w]+ile_bialych[2*w+1]
        }
    }
*/
