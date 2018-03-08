#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <array>
#include "log.h"
#include <vector>

class Barrier {
public:
  std::condition_variable cv;
  std::mutex cv_m; // This mutex is used for two purposes:
                   // 1) to synchronize accesses to i
                   // 2) for the condition variable cv

  int limit;
  int state = 0;
  Barrier(int num) : limit(num) { }

  void reach() {
    {
      std::unique_lock<std::mutex> lk(this->cv_m);
      ++state;
      cv.wait(lk, [this]{return this->state >= this->limit;});
    }
    cv.notify_all();
    // {
    //   std::unique_lock<std::mutex> lk(this->cv_m);
    //   state = 0;
    // }
  }

};


std::vector<std::thread> runners;
Barrier foo(4);

void waits()
{
  foo.reach();
  log("reached!");
}

void launches()
{
  for (int u = 0; u < 10; ++u) {
    log("push new...");
    runners.push_back(std::thread{waits});
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main()
{
  std::thread launcher = std::thread{launches};

  launcher.join();

  for (auto& t : runners)
    t.join();
}
