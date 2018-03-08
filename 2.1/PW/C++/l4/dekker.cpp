#include <array>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

const long counter = 5'000'000;

long x = 0;
std::atomic<int> flags[2];
std::atomic<int> turn{0};

void critical_section(void) {
  long y;
  y = x;
  y = y + 1;
  x = y;
}

void local_section(void) {}

void entry_protocol(int nr) {
  flags[nr] = true;

  while (flags[1 - nr]) {
    if (turn != nr) {
      flags[nr] = false;
      while (turn != nr) {
      }
      flags[nr] = true;
    }
  }
}

void exit_protocol(int nr) {
  turn = 1 - nr;
  flags[nr] = false;
}

void th(int nr) {
  for (long i = 0; i < counter; i++) {
    local_section();
    entry_protocol(nr);
    critical_section();
    exit_protocol(nr);
  }
}

void monitor() {
  long prev = 0;
  for (;;) {
    prev = x;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    if (prev == x)
      std::cout << "Deadlock! wants = " << flags[0] << "/" << flags[1] << " "
                << "waits " << turn << std::endl;
    else
      std::cout << "monitor: " << x << std::endl;
  }
}

int main() {
  std::cout << "main() starts" << std::endl;
  std::thread monitor_th{monitor};
  monitor_th.detach();
  std::thread t1{th, 0};
  std::thread t2{th, 1};
  t1.join(); // wait for t1 to complete
  t2.join(); // wait for t2 to complete
  std::cout << "main() completes: " << x << std::endl;
}
