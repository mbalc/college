#include <algorithm>
#include <cstdio>

using namespace std;

static int n, m;

struct Player {
  bool teamSetup[57];
};

static bool cmp(const Player &a, const Player &b) {
  for (int i = 0; i < m; ++i) {
    if (a.teamSetup[i] != b.teamSetup[i])
      return a.teamSetup[i] < b.teamSetup[i];
  }
  return false;
}

Player tab[40007];

int main() {
  int input;
  scanf("%d%d", &n, &m);
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      scanf("%d", &input);
      --input;
      tab[input].teamSetup[i] = (j >= n / 2);
    }
    // for (int j = 0; j < n; ++j)
    //   printf("%d ", tab[j].teamSetup[i]);
    // printf("\n");
  }

  sort(tab, tab + n, cmp);

  for (int i = 0; i < n - 1; ++i) {
    if (!cmp(tab[i], tab[i + 1]) && !cmp(tab[i + 1], tab[i])) {
      printf("NIE\n");
      return 0;
    }
  }
  printf("TAK\n");

  return 0;
}
