#include <cstdio>
#include <climits>

int min (int a, int b) {
    if (a > b) return b;
    return a;
}

void algorithm () {

    int nodeCount;
    scanf ("%d", &nodeCount);
    int shortestPaths [nodeCount][nodeCount];

    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            scanf ("%d", &shortestPaths[i][j]);
            if (shortestPaths[i][j] == 0) shortestPaths[i][j] = INT_MAX / 4;
        }
    }

    for (int i = 0; i < nodeCount; ++i) shortestPaths[i][i] = 0;

    for (int k = 0; k < nodeCount; ++k) {
        for (int i = 0; i < nodeCount; ++i) {
            for (int j = 0; j < nodeCount; ++j)
                shortestPaths[i][j] = min(shortestPaths[i][j], shortestPaths[i][k] + shortestPaths[k][j]);
        }
    }
}

int main () {
    algorithm ();
    return 0;
}
