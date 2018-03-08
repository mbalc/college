#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;

const int INF = 9000000;

class edge {
public:
    int from;
    int to;
    int weight;

    bool isEnabled;

    void log() {
        printf (" [%s] %d -> %d (%d)\n", isEnabled ? "+" : "-", from, to, weight);
    }

    edge() {
        this->weight = false;
    }
};

bool cmp (edge a, edge b) {
    return a.weight < b.weight;
}


edge gph[10007];
pair<int, int> nodes[1007];

int master(int n) {
    int m = nodes[n].first;
    if (n == m) return m;

    return master(m);
}

void unify(int p, int q) {
    int a = master(p);
    int b = master(q);
    if (nodes[a].second < nodes[b].second) {
        nodes[a].second += nodes[b].second;
        nodes[a].first = b;
    }
    else {
        nodes[b].second += nodes[a].second;
        nodes[b].first = a;
    }
}




int main () {
    int n, m;
    scanf ("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf ("%d%d%d", &gph[i].from, &gph[i].to, &gph[i].weight);
        --gph[i].from;
        --gph[i].to;
    }
    sort (gph, gph+m, cmp);

    for (int i=0; i < n; ++i) nodes[i] = pair<int, int> (i, 1);


    int aim = n / 2;
    int i = -1;

    while (aim > 0) {
        ++i;
        int a = master (gph[i].from), b = master (gph[i].to);
        if (a != b) {
            unify (a, b);
            --aim;
        }
    }

    printf ("%d\n", gph[i].weight);


    return 0;
}

