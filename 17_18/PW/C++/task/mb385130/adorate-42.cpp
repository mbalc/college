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

using rel_t = std::pair<int, int>; // first - weight of an edge, second - destination node
// The order is important for default sorting algrorithms to behave just the way we want them to
using ador_t = std::set<rel_t>; // the worst adorator in terms of :<: order will be
                                // on the beginning of the set

std::vector<std::vector<rel_t> > N;
std::queue<int> Q;
std::vector<ador_t> S;
std::vector<size_t> T; // we only hold the size of the original T structure

std::vector<int> explore;
std::vector<int> oldIndex;

int  b_method;
size_t count;

size_t getLim (int node) {
  return bvalue(b_method, oldIndex[node]);
}

rel_t last(int at) {
  size_t lim = getLim(at);

  if (S[at].size() == lim) return (*S[at].begin());

  return std::make_pair(NUL, NUL);
}

std::pair<rel_t, rel_t> findX(int u) {
  const auto& vec = N[u];
  int siz = vec.size();

  for (; explore[u] < siz; ++explore[u]) {
    const std::pair<int, int> &el = vec[explore[u]];
    int v = el.second;

    if (getLim(v) > 0) {
      ++++ Xusage;
      rel_t wRel = last(v);
      int dist = el.first, wNode = wRel.second, wDist = wRel.first;

      if ((S[v].find(std::make_pair(el.first, u)) == S[v].end()) && ( // W(v, u) :>: W(v, wNode)
            (dist > wDist) || ((dist == wDist) && (u > wNode))
            )) {                            // found better substitute
        return std::make_pair(wRel, el);

        // maxx      = v;
        // maxWeight = W(u, v);
      }
    }
  }

  // std::cerr << maxx << "ismax\n";

  return std::make_pair(std::make_pair(NUL, NUL), std::make_pair(NUL, NUL));
}

void compute(int u) {
  int x, xPath, y;
  rel_t z, maxX;
  std::pair<rel_t, rel_t> res;

  size_t limit = getLim(u);

  // std::cerr << "    computing " << u << " for limit " << limit << "\n";

  while (T[u] < limit) { // T[u] is declared as the size of the original T[u] structure
    res = findX(u);
    maxX = res.second;
    x = maxX.second;
    xPath = maxX.first;

    if (x == NUL) break;
    else {
      z = res.first;
      y = z.second;

      // std::cerr << "inserting" << x << " to " << u << "\n";
      auto& rel = S[x];
      rel.insert(std::make_pair(xPath, u)); // TODO update Slast
      ++T[u];

      if (y != NUL) {                         // see also the FAQ
        // std::cerr << "erasing " << x << " from " << y << "\n";
        rel.erase(z);
        --T[y];
        Q.push(y);
      }
    }

    ++explore[u];

    // std::cerr << T[u].size() << "Tsize\n";
  }

  // std::cerr << T[u].size() << " is sizeof "  << " \n";
}

int reduce() {
  int out = 0;

  for (size_t i = 0; i < count; i++) {
    for (auto d : S[i]) {
      ++Rusage;
      out += d.first;

      // std::cerr << "reducing " << d << "\n";
    }
  }
  return out;
}

void analyzeInput(std::stringstream& filtered) {
  int from, to, weight;

  std::map<int, std::vector<rel_t> > multiN;
  std::map<int, int> convert;

  while (filtered >> from >> to >> weight) {
    if (from > to) std::swap(from, to);
    multiN[from].push_back(std::make_pair(weight, to));
    multiN[to].push_back(std::make_pair(weight, from));
  }

  count = 0;

  std::cerr << "nodes: ";

  for (auto &el : multiN) {
    // std::cerr << el.first << ", ";
    N.push_back(std::vector<std::pair<int, int> >());
    S.push_back(ador_t());
    T.push_back(0);
    convert[el.first] = count;
    oldIndex.push_back(0);
    explore.push_back(0);
    ++count;
  }
  std::cerr << "\n";

  // reindex N and Wdata
  for (auto &el : multiN) {
    int at = convert[el.first];
    oldIndex[at] = el.first;

    for (auto &node : el.second) {
      N[at].push_back(std::make_pair(node.first, convert[node.second]));
    }
    // el.second.clear();
  }

  for (size_t i = 0; i < count; ++i) {
    std::sort(N[i].rbegin(), N[i].rend()); // ascending order
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cerr << "usage: " << argv[0] << " thread-count inputfile b-limit" <<
      std::endl;
    return 1;
  }

  int thread_count = std::stoi(argv[1]);
  std::string input_filename{argv[2]};
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
    // std::cerr << S.size() << "pushed overall \n";

    std::cerr << "\nAND THE OUTPUT IIIS!:\n";
    std::cout << reduce() / 2 << "\n";
    std::cerr << "\n";

    for (size_t i = 0; i < count; i++) {
      S[i].clear();
      T[i] = 0;
      explore[i] = 0;
    }
  }

  std::cerr << "Total W usage: " << Wusage << "\n";
  std::cerr << "By findX: " << Xusage << "\n";
  std::cerr << "By S comparator: " << Susage << "\n";
  std::cerr << "During N sorting: " << Nusage << "\n";
  std::cerr << "During reductions: " << Rusage << "\n";

  return 0;
}
