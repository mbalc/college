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

using rel_t = std::pair<int, int>;
using ador_t = std::set<rel_t>;

std::vector<std::vector<rel_t> > N;

// std::map<std::pair<int, int>, int> Wdata;
// std::map<int, std::vector<int>> N; // TODO :>: order on vector elements (max
// begin())

std::queue<int> Q;
std::vector<ador_t> S;
std::vector<std::set<int> > T;

int  b_method;
uint count;

std::pair<int, int> last(int at) {
  uint lim = bvalue(b_method, at);

  if (S[at].size() == lim) return (*S[at].begin());

  return std::make_pair(NUL, NUL);
}

rel_t findX(int &i, int u) {
  int maxx = NUL, maxWeight = NUL;
  const auto& vec = N[u];
  int siz = vec.size();

  for (; i < siz; ++i) {
    const std::pair<int, int> &el = vec[i];
    int v = el.second;

    if (bvalue(b_method, v) > 0) {
      ++++ Xusage;
      rel_t wRel = last(v);
      int dist = el.first, wNode = wRel.second, wDist = wRel.first;

      if ((T[u].find(v) == T[u].end()) && ( // W(v, u) :>: W(v, wNode)
            (dist > wDist) || ((dist == wDist) && (u > wNode))
            )) {                            // found better substitute
        return std::make_pair(dist, v);

        // maxx      = v;
        // maxWeight = W(u, v);
      }
    }
  }

  // std::cerr << maxx << "ismax\n";

  return std::make_pair(NUL, NUL);
}

void compute(int u) {
  int x, xPath, y;
  rel_t z, maxX;

  uint limit = bvalue(b_method, u);

  // std::cerr << "    computing " << u << " for limit " << limit << "\n";
  auto& ad = T[u];

  int lastFoundX = 0;

  while (ad.size() < limit) {
    maxX = findX(lastFoundX, u);
    x = maxX.second;
    xPath = maxX.first;

    if (x == NUL) break;
    else {
      z = last(x);
      y = z.second;

      // std::cerr << "inserting" << x << " to " << u << "\n";
      auto& rel = S[x];
      rel.insert(std::make_pair(xPath, u)); // TODO update Slast
      ad.insert(x);

      if (y != NUL) {                         // see also the FAQ
        // std::cerr << "erasing " << x << " from " << y << "\n";
        rel.erase(z);
        T[y].erase(x);
        Q.push(y);
      }
    }

    ++lastFoundX;

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

  std::map<int, std::vector<rel_t> >   multiN;
  std::map<int, int> convert;

  while (filtered >> from >> to >> weight) {
    if (from > to) std::swap(from, to);
    multiN[from].push_back(std::make_pair(weight, to));
    multiN[to].push_back(std::make_pair(weight, from));
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
  for (auto el : multiN) {
    int at = convert[el.first];

    for (auto node : el.second) {
      N[at].push_back(std::make_pair(node.first, convert[node.second]));
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
