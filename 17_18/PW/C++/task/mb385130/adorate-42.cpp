#include "blimit.hpp"

#include <algorithm>
#include <atomic>  // TODO cleanup includes
#include <cassert>
#include <climits>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

const int NULL_EXPR = -42,
          PROC_BUFF_LIM = 512;  // a thread will process this many nodes before locking the main
                                // queue of nodes left to be processed again - locks interfere less

class SpinLock {
private:
  std::atomic_flag locked = ATOMIC_FLAG_INIT;
  // int howManyProcHaveLock = 0;

public:
  void lock() {
    while (locked.test_and_set(std::memory_order_acquire)) {}  // spin until we are the ones who set
                                                               // 'locked' to 'true' from 'false'

    // critical section entered, nobody else is here - assert(howManyProcHaveLock++ == 0);
  }

  void unlock() {
    // leaving critical section, still nobody's here - assert(--howManyProcHaveLock == 0);

    locked.clear(std::memory_order_release);
  }
};

using relation_t = std::pair<int, int>;  // first - weight of an edge, second - destination node
/* The order of relation_t members is important for default sorting algorithms to behave just
   the way we want them to - to imply :<: order in sets and when sorted with std::sort during
   'neighbours' structure preprocessing */

std::vector<int> explore;  // we process ordered neighbours once per bmethod - here we store last
                           // index considered during such traversal

std::vector<int> oldIndex;  // on the beginning of the runtime we reindex nodes to consecutive
                            // numbers from 0 to (total count of nodes - 1); here we store old
                            // indexes for blimit function to work properly

std::vector<std::vector<relation_t>> neighbours;  // neighbours[i] -> a set of relations i node has
std::queue<int> mainProcQueue;
std::set<int> reprocQueue;  // nodes stored here will need another processing

std::vector<std::set<relation_t>> suitors;
std::deque<SpinLock> L;  // a spinlock mutex for each member of 'suitors' structure

std::deque<std::atomic<size_t>> T;  // we only hold the size of the original T structure
SpinLock Qtex, Rtex;                // mutexes for mainProcQueue and reprocQueue, respectively

int threadCount;
std::string inputFilename;

int b_method;
size_t count;  // total node count (inititated during input analysis)

size_t getLim(int node) {
  return bvalue(b_method, oldIndex[node]);
}

relation_t last(int at) {  // requires a lock to be held on L[at]
  size_t lim = getLim(at);

  if (suitors[at].size() == lim) return *suitors[at].begin();

  return std::make_pair(NULL_EXPR, NULL_EXPR);
}

bool isEligible(int u, relation_t el, relation_t wRel) {  // requires a lock on L[el.second]
  int v = el.second;
  int dist = el.first, wNode = wRel.second, wDist = wRel.first;  // 'w' stands for 'worst'

  return ((dist > wDist) || ((dist == wDist) && (u > wNode)))  // v is not matched yet with u
         &&
         (suitors[v].find(std::make_pair(dist, u)) == suitors[v].end());  // W(v, u) :>: W(v, wNode)
}

std::pair<relation_t, relation_t> findX(int u) {  // important notice: hands over a lock held on
                                                  // suitors member that is relevant to the relation
                                                  // returned, so it must be explicitly unlocked
  const auto& uNeighbours = neighbours[u];
  int siz = uNeighbours.size();

  for (; explore[u] < siz; ++explore[u]) {
    const std::pair<int, int>& rel = uNeighbours[explore[u]];
    int v = rel.second;

    if (getLim(v) > 0) {
      L[v].lock();
      relation_t wRel = last(v);  // 'w' stands for 'worst'

      if (isEligible(u, rel, wRel)) return std::make_pair(wRel, rel);
      L[v].unlock();
    }
  }

  return std::make_pair(std::make_pair(NULL_EXPR, NULL_EXPR), std::make_pair(NULL_EXPR, NULL_EXPR));
}

void processNode(std::queue<int>& localR, int u) {
  int x, xPath, y;
  relation_t z, maxX;

  size_t limit = getLim(u);
  std::pair<relation_t, relation_t> res;

  while (T[u] < limit) {  // T[u] is declared as the size of the original T[u] structure
    res = findX(u);
    maxX = res.second;
    x = maxX.second;
    xPath = maxX.first;

    if (x == NULL_EXPR) break;

    if (isEligible(u, maxX, last(x))) {
      z = res.first;  // worst relation of x
      y = z.second;

      auto& rel = suitors[x];
      rel.insert(std::make_pair(xPath, u));
      ++T[u];

      if (y != NULL_EXPR) {
        rel.erase(z);
        --T[y];
        localR.push(y);
      }
    }

    L[x].unlock();  // locked previously by findX()
    ++explore[u];
  }
}

void worker() {  // TODO buffer multiple nodes, split this
  std::queue<int> localQ, localR;

  Qtex.lock();
  while (!mainProcQueue.empty()) {
    for (size_t i = 1; i <= PROC_BUFF_LIM && !mainProcQueue.empty(); i++) {
      localQ.push(mainProcQueue.front());
      mainProcQueue.pop();
    }
    Qtex.unlock();

    while (!localQ.empty()) {
      processNode(localR, localQ.front());
      localQ.pop();
    }

    Qtex.lock();  // to check (!mainProcQueue.empty()) in the next 'while' iteration
  }
  Qtex.unlock();

  Rtex.lock();
  while (!localR.empty()) {
    reprocQueue.insert(localR.front());
    localR.pop();
  }
  Rtex.unlock();
}

void synchronizedMakeSuitors() {
  std::vector<std::thread> threads;

  Qtex.lock();
  while (!mainProcQueue.empty()) {  // non-concurrent access
    for (int i = 1; i < threadCount; ++i) threads.emplace_back(worker);
    Qtex.unlock();
    worker();

    while (!threads.empty()) {
      threads.back().join();
      threads.pop_back();
    }

    Qtex.lock();
    for (int node : reprocQueue) mainProcQueue.push(node);
    reprocQueue.clear();
  }
  Qtex.unlock();
}

int reduce() {
  int out = 0;

  for (size_t i = 0; i < count; i++) {
    for (auto d : suitors[i]) out += d.first;
  }
  return out;
}

void initStructures(std::map<int, std::vector<relation_t>>& neighbourMap) {
  std::map<int, int> convert;

  count = 0;

  std::cerr << "nodes: ";

  for (auto& elem : neighbourMap) {  // consecutive elem.first values are in ascending order
    convert[elem.first] = count;
    ++count;

    neighbours.emplace_back();
    suitors.emplace_back();
    L.emplace_back();

    T.emplace_back(0);
    oldIndex.push_back(elem.first);
    explore.push_back(0);
  }
  std::cerr << "\n";

  // reindex neighbours
  for (auto& elem : neighbourMap) {
    for (auto& node : elem.second)
      neighbours[convert[elem.first]].push_back(std::make_pair(node.first, convert[node.second]));
  }

  for (size_t i = 0; i < count; ++i)
    std::sort(neighbours[i].rbegin(), neighbours[i].rend());  // ascending order
}

void parseInput() {
  int from, to, weight;

  std::stringstream filtered;

  std::map<int, std::vector<relation_t>> neighbourMap;

  std::ifstream infile(inputFilename);
  std::string line;

  while (std::getline(infile, line))
    if (line[0] != '#') filtered << line << " ";

  while (filtered >> from >> to >> weight) {
    if (from > to) std::swap(from, to);
    neighbourMap[from].push_back(std::make_pair(weight, to));
    neighbourMap[to].push_back(std::make_pair(weight, from));
  }

  initStructures(neighbourMap);
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "usage: " << argv[0] << " thread-count inputfile b-limit" << std::endl;
    return 1;
  }

  threadCount = std::stoi(argv[1]);
  inputFilename = argv[2];
  int bLimit = std::stoi(argv[3]);

  parseInput();

  for (b_method = 0; b_method <= bLimit; b_method++) {
    for (size_t i = 0; i < count; i++) mainProcQueue.push(i);

    std::cerr << "roll " << b_method << "\n";

    synchronizedMakeSuitors();

    std::cerr << "\nAND THE OUTPUT IIIS!:\n";
    std::cout << reduce() / 2 << "\n";

    for (size_t i = 0; i < count; i++) {
      suitors[i].clear();
      T[i] = 0;
      explore[i] = 0;
    }
  }

  return 0;
}
