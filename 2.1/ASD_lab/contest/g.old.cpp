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

class node {
public:
    vector<int> edges;
    int isVisited;
    void log() {
        printf ("[%s]: [",  isVisited != INF ? "+" : "-");
        for (int i = 0; i < edges.size(); ++i) printf ("%d ", edges[i]);
        printf ("\n");
    }
};

bool cmp (edge a, edge b) {
    return a.weight < b.weight;
}


edge gph[10007];
node nodes[1007];

void cleanup(int n) {
    for (int i=0; i < n; ++i) nodes[i].isVisited = INF;
}

pair <vector<int>, int> dfs (int v) {
    vector <int> q;
    vector <int> eq;
    vector <int> vis;
    int cnt = 0;

    q.push_back(0);
    eq.push_back(-1);

    while (!q.empty()) {
        int at = q.back();
        q.pop_back();

        vis.push_back(eq.back());
        eq.pop_back();

        nodes[at].isVisited = -1;
        ++cnt;

        printf ("visit %d\n", at);


        for (int i = nodes[at].edges.size() - 1; i >= 0; --i) {
            int e = nodes[at].edges[i];
            int to = (at == gph[e].to) ? gph[e].from : gph[e].to;
            if (nodes[to].isVisited > gph[e].weight && gph[e].isEnabled) {
                q.push_back(to);
                nodes[to].isVisited = gph[e].weight;
                eq.push_back(e);
            }
        }

    }
    return pair<vector<int>, int> (vis, cnt);
}


int main () {
    int n, m, a, b, c;
    scanf ("%d%d", &n, &m);
    for (int i = 0; i < m; ++i) {
        scanf ("%d%d%d", &gph[i].from, &gph[i].to, &gph[i].weight);
        --gph[i].from;
        --gph[i].to;
    }
    sort (gph, gph+m, cmp);

    for (int i = 0; i < m; ++i) {
        nodes[gph[i].from].edges.push_back(i);
        nodes[gph[i].to].edges.push_back(i);
    }

    for (int i=0; i < m; ++i) {
        printf ("%d> ", i);
        gph[i].log();
    }
    for (int i=0; i < n; ++i) {
        printf ("%d> ", i);
        nodes[i].log();
    }

    pair <vector<int>, int> out;
    for (int i = 0; i < m; ++i) {
        gph[i].isEnabled = true;
        out = dfs(0);
        if (out.second == n) {
            sort (out.first.begin(), out.first.end());
            break;
        }
        cleanup(n);
    }



    printf ("%d \n", gph[out.first[(n/2)]].weight);

    return 0;
}

