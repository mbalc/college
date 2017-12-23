#include "blimit.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <queue>
#include <set>
#include <vector>

const int NUL = -42;
using ador_t = std::set<int>;

class myInt {
private:
    int val;
public:
    myInt (int a) : val(a) {}
    myInt () : myInt(NUL) {}
    operator int& () { return val; }
};

std::map<std::pair<int, int>, int> Wdata;
std::map<int, std::vector<int>> N; // :>: order on vector elements (max begin())

std::queue<int> Q, R;
std::map<int, ador_t> T, S;
std::map<int, myInt> Slast;


int b_method;

int W(int a, int b) {
    if (a > b) std::swap(a, b);
    return Wdata[std::make_pair(a, b)];
}

int findX(int u) {
    int maxx = NUL, maxWeight = NUL;
    for (int v : N[u]) {
        // std::cout << v << N[u].size() << " le\n";
        int dist = W(v, u), wNode = Slast[v], wDist = W(v, wNode);
        if (T[u].find(v) == T[u].end() && dist > maxWeight && (
            dist > wDist || (dist == wDist && v > wNode)
        )) { // found better substitute
            maxx = v;
            maxWeight = W(u, v);
        }
    }
    std::cout << maxx << "ismax\n";

    return maxx;
}

void compute (int u) {
    int x, y;
    uint limit = bvalue(b_method, u);
    std::cout << "    computing " << u << " for limit" << limit << "\n";
    while (T[u].size() < limit) {
        x = findX(u);

        if (x == NUL) break;
        else {
            y = Slast[x];
            S[x].insert(u); // TODO update Slast
            uint xLimit = bvalue(b_method, x);
            if (S[x].size() > xLimit) {
                S[x].erase(--S[x].end());
            }
            if (S[x].size() == xLimit) {
                Slast[x] = *S[x].rbegin();
            }
            std::cout << "inserting" << x << " to " << u << "\n";
            T[u].insert(x);
            if (y != NUL) { // see also the FAQ
                std::cout << "erasing " << x << " from " << y << "\n";
                T[y].erase(x);
                R.push(y);
            }
        }
        std::cout << T[u].size() << "Tsize\n";
    }
    std::cout << T[u].size() << " is sizeof "  << " \n";
}

int reduce(ador_t &A, int n) {
    int out = 0;
    for (auto d : A) {
        out += W(d, n);
        std::cout << "reducing " << d << "\n";
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
            while (!Q.empty()) {
                compute(Q.front());
                Q.pop();
            }
            std::swap(Q, R); // Q is empty now
        }
        std::cout << S.size() << "pushed overall \n";
        for (auto p : S) {
            sum += reduce(p.second, p.first);
        }
        std::cout << "THE OUTPUT IS: " << sum / 2 << "\n\n";
        sum = 0;
        S.clear();
        T.clear();
        Slast.clear();
    }
    return 0;
}
