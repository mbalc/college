#include <cstdio>
using namespace std;

const int MAX_SIZ = 1007, mod = 1000000000;

static int n, in[MAX_SIZ];
static int tab[MAX_SIZ][MAX_SIZ][2]; // tab[i][j][k]: how many possibilities to
                                     // create substring of [in] from index
                                     // [i]-th to [k]-th when last number was
                                     // inserted on ([k] : right ? left)
static int sum(int a, int b) { return (a + b) % mod; }

int main() {
  scanf("%d", &n);

  for (int i = 0; i < n; ++i) {
    tab[i][i][0] = 1;
    scanf("%d", &in[i]);
  }
  for (int it = n - 1; it > 0; --it) {
    for (int i = 0, j = n - it; i < it; ++i, ++j) {
      tab[i][j][0] = sum((in[i] < in[i + 1] ? tab[i + 1][j][0] : 0),
                         (in[i] < in[j] ? tab[i + 1][j][1] : 0));
      tab[i][j][1] = sum((in[i] < in[j] ? tab[i][j - 1][0] : 0),
                         (in[j - 1] < in[j] ? tab[i][j - 1][1] : 0));
    }
  }
  printf("%d\n", sum(tab[0][n - 1][0], tab[0][n - 1][1]));
}