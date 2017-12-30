#include "blimit.hpp"

#include <algorithm>
#include <cassert>
#include <atomic> // TODO cleanup includes
#include <climits>
#include <deque>
#include <iostream>
#include <fstream>
#include <functional>
#include <sstream>
#include <mutex>
#include <string>
#include <map>
#include <queue>
#include <set>
#include <thread>
#include <utility>
#include <vector>

class SpinLock {
private:

  std::atomic_flag locked = ATOMIC_FLAG_INIT;

  // std::mutex locked;

  // int testchck = 0;

public:

  void lock() {
    while (locked.test_and_set(std::memory_order_acquire));

    // locked.lock();

    // critical section entered, nobody else is here - assert(testchck++ == 0);
  }

  void unlock() {
    // leaving critical section, still nobody there - assert(--testchck == 0);

    // if (testchck == -1) {
    //   std::cerr << "O zgrozo!\n";
    // }
    locked.clear(std::memory_order_release);

    // locked.unlock();
  }
};

int threadCount;

const int NUL = -42, procLimit = 4, buffLimit = 16;

using rel_t = std::pair<int, int>; // first - weight of an edge,
                                   // second - destination node
// The order is important for default sorting algrorithms to behave just the way
// we want them to
using ador_t = std::set<rel_t>;    // the worst adorator in terms of :<: order
                                   // will be on the beginning of the set

std::vector<std::vector<rel_t> > N;
std::queue<int> Q;
std::set<int>   R;
std::vector<ador_t> S;

std::vector<int> explore;
std::vector<int> oldIndex;

std::deque<std::atomic<size_t> > T; // we only hold the size of the original T structure
std::deque<SpinLock> L;
SpinLock Qtex, Rtex;

int b_method;
size_t count;

size_t getLim(int node) {
  return bvalue(b_method, oldIndex[node]);
}

rel_t last(int at) { // requires a lock to be held on L[at]
  size_t lim = getLim(at);

  if (S[at].size() == lim) return *S[at].begin();

  return std::make_pair(NUL, NUL);
}

bool isEligible(int u, rel_t el, rel_t wRel) { // requires a lock to be held on L[el.second]
  int v = el.second;
  int dist = el.first, wNode = wRel.second, wDist = wRel.first;

  return (S[v].find(std::make_pair(dist, u)) == S[v].end()) && ( // v is not matched yet with u
    (dist > wDist) || ((dist == wDist) && (u > wNode)));         // W(v, u) :>: W(v, wNode)
}

std::pair<rel_t, rel_t>findX(int u) {                            // hands over a lock held on S
                                                                 // member that is relevant to the
                                                                 // relation returned, it must be
                                                                 // explicitly unlocked
  const auto& vec = N[u];
  int siz         = vec.size();

  for (; explore[u] < siz; ++explore[u]) {
    const std::pair<int, int>& el = vec[explore[u]];
    int v                         = el.second;

    if (getLim(v) > 0) {
      L[v].lock();
      rel_t wRel = last(v); // 'w' stands for 'worst'

      if (isEligible(u, el, wRel)) {
        return std::make_pair(wRel, el);
      }
      L[v].unlock();
    }
  }

  return std::make_pair(std::make_pair(NUL, NUL), std::make_pair(NUL, NUL));
}

void worker() { // TODO buffer multiple nodes
  int x, u, xPath, y;
  size_t limit;
  rel_t  z, maxX;

  std::queue<int> localR;
  std::pair<rel_t, rel_t> res, upd;

  Qtex.lock();

  while (!Q.empty()) {
    u = Q.front();
    Q.pop();
    Qtex.unlock();


    limit = getLim(u);


    while (T[u] < limit) { // T[u] is declared as the size of the original T[u] structure
      res   = findX(u);
      maxX  = res.second;
      x     = maxX.second;
      xPath = maxX.first;

      if (x == NUL) {
        break;
      }

      // Utex.unlock();

      if (isEligible(u, maxX, last(x))) {
        z = res.first; // worst relation of x
        y = z.second;

        auto& rel = S[x];
        rel.insert(std::make_pair(xPath, u));
        ++T[u];


        if (y != NUL) {
          rel.erase(z);
          --T[y];
          localR.push(y);
        }
      }

      L[x].unlock();
      ++explore[u];
    }
    Qtex.lock();
  }
  Qtex.unlock();
  Rtex.lock();

  while (!localR.empty()) {
    R.insert(localR.front());
    localR.pop();
  }
  Rtex.unlock();

  // std::cerr << T[u].size() << " is sizeof "  << " \n";
}

void synchronizedMakeSuitors() {
  std::vector<std::thread> threads;

  Qtex.lock();

  while (!Q.empty()) { // non-concurrent access
    for (int i = 1; i < threadCount; ++i) {
      threads.emplace_back(worker);
    }
    Qtex.unlock();
    worker();

    while (!threads.empty()) {
      threads.back().join();
      threads.pop_back();
    }
    Qtex.lock();

    for (int node : R) {
      Q.push(node);
    }
    R.clear();
  }

  Qtex.unlock();
}

int reduce() {
  int out = 0;

  for (size_t i = 0; i < count; i++) {
    for (auto d : S[i]) {
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

  for (auto& el : multiN) {
    // std::cerr << el.first << ", ";
    convert[el.first] = count;
    ++count;

    N.emplace_back();
    S.emplace_back();
    L.emplace_back();

    T.emplace_back(0);
    oldIndex.push_back(0);
    explore.push_back(0);
  }
  std::cerr << "\n";

  // reindex N and Wdata
  for (auto& el : multiN) {
    int at = convert[el.first];
    oldIndex[at] = el.first;

    for (auto& node : el.second) {
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

  threadCount = std::stoi(argv[1]);
  std::string inputFilename{ argv[2] };
  int bLimit = std::stoi(argv[3]);

  std::ifstream infile(inputFilename);
  std::stringstream filtered;
  std::string line;

  while (std::getline(infile, line)) {
    if (line[0] != '#') filtered << line << " ";
  }

  analyzeInput(filtered);

  for (b_method = 0; b_method <= bLimit; b_method++) {
    for (size_t i = 0; i < count; i++) {
      Q.push(i);
    }

    std::cerr << "roll " << b_method << "\n";

    synchronizedMakeSuitors();

    // std::cerr << S.size() << "pushed overall \n";

    std::cerr << "\nAND THE OUTPUT IIIS!:\n";
    std::cout << reduce() / 2 << "\n";

    // std::cerr << "\n";

    for (size_t i = 0; i < count; i++) {
      S[i].clear();
      T[i]       = 0;
      explore[i] = 0;
    }
  }

  return 0;
}
