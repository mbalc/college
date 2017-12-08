// Task: You are given a binary tree. For each query [node, dist], find and
// return an index of any node with a distance to [node] equal to [dist], or
// return -1 if there is no such node.

/* Example test:

  IN:
8
3 4
-1 6
2 5
-1 -1
7 -1
-1 -1
8 -1
-1 -1
6
1 3
1 4
1 5
6 1
6 4
6 5

  OUT:
6
8
-1
2
4
8

*/

#include <cstdio>
#include <vector>

using namespace std;

const unsigned int MAX_LCA = 20;

struct Node {
  vector<int> sons;
  vector<int> lca;

  int maxDownNode = 0;
  int maxUpNode = 0;
  int maxDownLength = 0;
  int maxUpLength = 0;

  int turnDist = 0;
};

int n, m;
Node* tab;

void initLCA() {
  for (unsigned int i = 1; i <= MAX_LCA; ++i) {
    for (int nd = 1; nd <= n; ++nd) {
      Node* node = &tab[nd];
      if (node->lca.size() >= i && tab[node->lca[i - 1]].lca.size() >= i) {
        node->lca.push_back(tab[node->lca[i - 1]].lca[i - 1]);
      }
    }
  }
}

void initDown(int at) { // compute longest paths that begin by going downwards
  tab[at].maxDownNode = at;
  tab[at].maxDownLength = 0;
  for (int son : tab[at].sons) {
    initDown(son);
    if (tab[son].maxDownLength >= tab[at].maxDownLength) {
      tab[at].maxDownLength = tab[son].maxDownLength + 1;
      tab[at].maxDownNode = tab[son].maxDownNode;
    }
  }
}

void initUp(int at) {
  tab[at].maxUpLength= 0;
  tab[at].maxUpNode = at;
  tab[at].turnDist = 0;

  if (tab[at].lca.size() > 0) {
    int father = tab[at].lca[0];
    tab[at].maxUpLength = tab[father].maxUpLength + 1;
    tab[at].maxUpNode = tab[father].maxUpNode;
    tab[at].turnDist = tab[father].turnDist + 1;

    int sibling = -1;
    for (int son : tab[father].sons) {
      if (son != at) sibling = son;
    }
    if (sibling != -1 && tab[sibling].maxDownLength + 2 > tab[at].maxUpLength) {
      tab[at].maxUpNode = tab[sibling].maxDownNode;
      tab[at].maxUpLength = tab[sibling].maxDownLength + 2;
      tab[at].turnDist = 1;
    }
  }
  for (int son : tab[at].sons) {
    initUp(son);
  }
}

int lca (int from, int dist) {
  // printf ("\n");
  for (int i = 0; dist > 0; ++i) {
    // printf ("D%d\n", from - tab);
    if (dist % 2) {
      from = tab[from].lca[i];
    }
    dist /= 2;
  }
  // printf ("RR D%d\n", from - tab);
  return from;
}

void initAll(int at) {
  if (tab[at].maxDownLength > tab[at].maxUpLength) {
    tab[at].maxUpLength = tab[at].maxDownLength;
    tab[at].maxUpNode = tab[at].maxDownNode;
    tab[at].turnDist = 0;
  }
  for (int son : tab[at].sons) {
    initAll(son);
  }
}

int main() {
  scanf ("%d", &n);
  tab = new Node[7+n];
  int a, b;
  for (int i = 1; i <= n; ++i) {
    scanf ("%d%d", &a, &b);
    if (a != -1) {
      tab[i].sons.push_back(a);
      tab[a].lca.push_back(i);
    }
    if (b != -1) {
      tab[i].sons.push_back(b);
      tab[b].lca.push_back(i);
    }
  }

  initLCA();
  initDown(1);
  initUp(1);
  initAll(1);

  // for (int i = 1; i <= n; ++i) {
  //   printf ("%d: ", i);
  //   for (Node* ptr : tab[i].lca) {
  //     printf ("%d, ", ptr - tab);
  //   }
  //   printf ("\n");
  // }

  scanf("%d", &m);
  int out;
  for (int i = 0; i < m; ++i) {
    scanf ("%d%d", &a, &b);
    if (tab[a].maxUpLength >= b) {
      if (tab[a].turnDist >= b) {
        out = lca(a, b);
        // printf("case11");
      }
      else {
        out = lca(tab[a].maxUpNode, tab[a].maxUpLength - b);
        // printf("case22");
      }
    }
    else out = -1;
    printf ("%d\n", out);
  }
}

// odl_w_dol[v] - liczymy od liści
//
// odl_w_gore[v]=max(odl_w_gore[ojciec[v]]+1, odl_w_dol[brat[v]]+2) - od góry
//
// drzewo lca
//
// if (odl_w_gore[ojciec[v]]+1 > odl_w_dol[brat[v]]+2)
// 	gdzie_skrecic[v] = gdzie_skrecic[ojciec[v]]
// else gdzie_skrecic[v] = brat[v]
