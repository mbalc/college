#include <cstdio>
#include <ctime>
#include <cstdlib>
using namespace std;

const int testValues [3][6] = {{0, 5, 200, 600, 600, 1000},
                               {0, 22, 30, 35, 35, 40},
                               {0, 40, 40, 12, 65, 50}};

int main () {
    srand (404);

    int testCase;
    scanf ("%d", &testCase);

    int nodeCount = testValues[0][testCase];
    int wLimit = testValues[1][testCase]; //maximum edge weight
    int edgeDensity = testValues[2][testCase]; //choose from range 0% - 100%

    printf ("%d\n", nodeCount);

    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            if (i == j) printf ("0 ");
            else if (rand() % 100 < edgeDensity)
                printf ("%d ", rand() % wLimit + 1);
            else printf ("0 ");
        }
        printf ("\n");
    }
    return 0;
}
