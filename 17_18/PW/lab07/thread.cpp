#include <thread>
#include <iostream>
#include <chrono>

void f() {
    std::cout << "f(): ensemble on est trop" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "f() completes" << std::endl;
}

void g() {
    std::cout << "g(): ensemble on est trop" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    std::cout << "g() completes" << std::endl;
}

int main() {
    std::cout << "tout seul on n'est rien" << std::endl;
    std::thread t1{f}; // creates a thread executing function f
    std::thread t2{g}; // creates another thread executing function g
    std::cout << "main() completes" << std::endl;
}
