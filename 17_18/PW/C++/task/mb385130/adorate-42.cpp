#include "blimit.hpp"

#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <map>
#include <queue>
#include <set>
#include <vector>

const int NUL = -42;

static long long Wusage = 0, Xusage = 0, Susage = 0, Nusage = 0, Rusage = 0;
using ador_t = std::set<std::pair<int, int> >;

std::map<std::pair<int, int>, int> Wdata;
std::vector<std::vector<std::pair<int, int> > > N;

// std::map<std::pair<int, int>, int> Wdata;
// std::map<int, std::vector<int>> N; // TODO :>: order on vector elements (max
// begin())

std::queue<int> Q;
std::vector<ador_t> S;
std::vector<std::set<int> > T;

int  b_method;
uint count;

int W(int a, int b) {
  if (a > b) std::swap(a, b);

  if (a == NUL) return 0;

  ++Wusage;

  std::map<std::pair<int, int>,
           int>::iterator out = Wdata.find(std::make_pair(a, b));

  if (out == Wdata.end()) return 0;  // TODO NUL?

  return out->second;
}

int last(int at) {
  uint lim = bvalue(b_method, at);

  if (lim == 0) return INT_MAX;

  if (S[at].size() == lim) return (S[at].begin())->second;

  return NUL;
}

int findX(int u) {
  int maxx = NUL, maxWeight = NUL;
  const auto& vec = N[u];

  for (std::pair<int, int>el : vec) {
    int v = el.second;

    if (bvalue(b_method, v) > 0) {
      ++++ Xusage;
      int dist = el.first, wNode = last(v), wDist = W(v, wNode);

      if ((T[u].find(v) == T[u].end()) && ( // W(v, u) :>: W(v, wNode)
            (dist > wDist) || ((dist == wDist) && (u > wNode))
            )) {                            // found better substitute
        return v;

        // maxx      = v;
        // maxWeight = W(u, v);
      }
    }
  }

  // std::cerr << maxx << "ismax\n";

  return maxx;
}

void compute(int u) {
  int  x, y;
  uint limit = bvalue(b_method, u);

  // std::cerr << "    computing " << u << " for limit " << limit << "\n";
  auto& ad = T[u];

  while (ad.size() < limit) {
    x = findX(u);

    if (x == NUL) break;
    else {
      y = last(x);

      // std::cerr << "inserting" << x << " to " << u << "\n";
      auto& rel = S[x];
      rel.insert(std::make_pair(W(x, u), u)); // TODO update Slast
      ad.insert(x);

      if (y != NUL) {                         // see also the FAQ
        // std::cerr << "erasing " << x << " from " << y << "\n";
        rel.erase(std::make_pair(W(x, y), y));
        T[y].erase(x);
        Q.push(y);
      }
    }

    // std::cerr << T[u].size() << "Tsize\n";
  }

  // std::cerr << T[u].size() << " is sizeof "  << " \n";
}

int reduce(std::set<std::pair<int, int> >& A, int n) {
  int out = 0;

  for (auto d : A) {
    ++Rusage;
    out += d.first;

    // std::cerr << "reducing " << d << "\n";
  }
  return out;
}

void analyzeInput(std::stringstream& filtered) {
  int from, to, weight;

  std::map<std::pair<int, int>, int> multiW;
  std::map<int, std::vector<int> >   multiN;
  std::map<int, int> convert;

  while (filtered >> from >> to >> weight) {
    if (from > to) std::swap(from, to);
    multiW.insert(std::make_pair(std::make_pair(from, to), weight));
    multiN[from].push_back(to);
    multiN[to].push_back(from);
  }


  count = 0;

  std::cerr << "nodes: ";

  for (auto el : multiN) {
    // std::cerr << el.first << ", ";
    convert[el.first] = count;
    ++count;
  }
  std::cerr << "\n";

  for (size_t i = 0; i < count; i++) {
    N.push_back(std::vector<std::pair<int, int> >());
    T.push_back(std::set<int>());
    S.push_back(ador_t());
  }

  // reindex N and Wdata
  for (auto el : multiW) {
    std::pair<int, int> oldK = el.first;
    std::pair<int, int> newK =
      std::make_pair(convert[oldK.first], convert[oldK.second]);
    Wdata.insert(std::make_pair(newK, el.second));
  }

  for (auto el : multiN) {
    int at = convert[el.first];

    for (auto node : el.second) {
      N[at].push_back(std::make_pair(W(at, convert[node]), convert[node]));
    }
  }


  for (size_t i = 0; i < count; i++) {
    std::sort(N[i].rbegin(), N[i].rend());
  }
}

int main(int argc, char *argv[]) {
  int sum = 0;

  if (argc != 4) {
    std::cerr << "usage: " << argv[0] << " thread-count inputfile b-limit" <<
      std::endl;
    return 1;
  }

  int thread_count = std::stoi(argv[1]);
  std::string input_filename{ argv[2] };
  int b_limit = std::stoi(argv[3]);

  std::ifstream infile(input_filename);
  std::stringstream filtered;
  std::string line;

  while (std::getline(infile, line)) {
    if (line[0] != '#') filtered << line << " ";
  }

  analyzeInput(filtered);

  for (b_method = 0; b_method <= b_limit; b_method++) {
    for (size_t i = 0; i < count; i++) {
      Q.push(i);
    }

    std::cerr << "roll " << b_method << "\n";

    while (!Q.empty()) {
      compute(Q.front());
      Q.pop();
    }
    std::cerr << S.size() << "pushed overall \n";

    for (size_t i = 0; i < count; i++) {
      // std::cerr << "among " << p.first << "\n";
      sum += reduce(S[i], i);
    }
    std::cerr << "\nAND THE OUTPUT IIIS!:\n";
    std::cout << sum / 2 << "\n";
    std::cerr << "\n";

    sum = 0;

    for (size_t i = 0; i < count; i++) {
      S[i].clear();
      T[i].clear();
    }
  }

  std::cerr << "Total W usage: " << Wusage << "\n";
  std::cerr << "By findX: " << Xusage << "\n";
  std::cerr << "By S comparator: " << Susage << "\n";
  std::cerr << "During N sorting: " << Nusage << "\n";
  std::cerr << "During reductions: " << Rusage << "\n";

  return 0;
}
