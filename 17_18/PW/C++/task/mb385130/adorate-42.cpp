#include "blimit.hpp"

#include <iostream>
#include <climits>
#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <map>
#include <queue>
#include <set>
#include <vector>

const int NUL = -42;
using ador_t = std::set<int, std::function<bool(int, int)>>;


std::map<std::pair<int, int>, int> Wdata;
std::map<int, std::vector<int>> N; // :>: order on vector elements (max begin())

std::queue<int> Q;
std::map<int, ador_t> S;
std::map<int, std::set<int>> T;

int b_method;

int W(int a, int b) {
    if (a > b) std::swap(a, b);
    if (a == NUL) return 0;
    std::map<std::pair<int, int>, int>::iterator out = Wdata.find(std::make_pair(a, b));
    if (out == Wdata.end()) return 0; // TODO NUL?
    return out->second;
}

void init(int dx) {
    S.emplace(dx, ador_t([dx](int a, int b) {
      if (W(dx, a) == W(dx, b)) return a > b;
      return W(dx, a) > W(dx, b);
    }));
}

int last(int at) {
  init(at);
  uint lim = bvalue(b_method, at);
  if (lim == 0) return INT_MAX;
  if (S[at].size() == lim) return *--S[at].end();
  return NUL;
}

int findX(int u) {
    int maxx = NUL, maxWeight = NUL;
    const auto &vec = N[u];
    for (int v : vec) {
        if (bvalue(b_method, v) > 0) {
            init(v);
            int dist = W(v, u), wNode = last(v), wDist = W(v, wNode);
            if (T[u].find(v) == T[u].end() && ( // is better than max found so far
                dist > maxWeight || (dist == maxWeight && v > maxx)
            ) && ( // W(v, u) :>: W(v, wNode)
                dist > wDist || (dist == wDist && u > wNode)
            )) { // found better substitute
                maxx = v;
                maxWeight = W(u, v);
            }
        }
    }
    // std::cerr << maxx << "ismax\n";

    return maxx;
}

void compute (int u) {
    int x, y;
    uint limit = bvalue(b_method, u);
    // std::cerr << "    computing " << u << " for limit " << limit << "\n";
    auto &ad = T[u];
    while (ad.size() < limit) {
        x = findX(u);

        if (x == NUL) break;
        else {
            init(x);
            y = last(x);
            // std::cerr << "inserting" << x << " to " << u << "\n";
            auto &rel = S[x];
            rel.insert(u); // TODO update Slast
            ad.insert(x);
            if (y != NUL) { // see also the FAQ
                // std::cerr << "erasing " << x << " from " << y << "\n";
                rel.erase(y);
                T[y].erase(x);
                Q.push(y);
            }
        }
        // std::cerr << T[u].size() << "Tsize\n";
    }
    // std::cerr << T[u].size() << " is sizeof "  << " \n";
}

int reduce(std::set<int> &A, int n) {
    int out = 0;
    for (auto d : A) {
        out += W(d, n);
        // std::cerr << "reducing " << d << "\n";
    }
    return out;
}

int main(int argc, char* argv[]) {
    int sum = 0;
    if (argc != 4) {
        std::cerr << "usage: "<<argv[0]<<" thread-count inputfile b-limit"<< std::endl;
        return 1;
    }

    int thread_count = std::stoi(argv[1]);
    int b_limit = std::stoi(argv[3]);
    std::string input_filename{argv[2]};

    std::ifstream infile(input_filename);
    std::stringstream buf;
    std::string line;
    while (std::getline(infile, line)) {
        if (line[0] != '#') buf << line << " ";
    }

    int from, to, weight;

    while (buf >> from >> to >> weight) {
        if (from > to) std::swap(from, to);
        Wdata.insert(std::make_pair(std::make_pair(from, to), weight));
        N[from].push_back(to);
        N[to].push_back(from);
    }

    for (b_method = 0; b_method <= b_limit; b_method++) {
        for (auto p : N) Q.push(p.first);
        while (!Q.empty()) {
            compute(Q.front());
            Q.pop();
        }
        std::cerr << S.size() << "pushed overall \n";
        for (auto p : T) {
            // std::cerr << "among " << p.first << "\n";
            sum += reduce(p.second, p.first);
        }
        std::cerr << "\nAND THE OUTPUT IIIS!:\n";
        std::cout << sum / 2 << "\n";
        std::cerr << "\n";

        sum = 0;
        S.clear();
        T.clear();
    }
    return 0;
}
