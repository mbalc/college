#include <iostream>
#include <thread>
#include <future>
#include <exception>
#include <fstream>
#include <locale>
#include <string>
#include <list>
#include <codecvt>

static const int THREAD_COUNT = 5;
std::wstring word;

int grep(std::string filename, std::wstring word) {
    std::locale loc("pl_PL.UTF-8");
    std::wfstream file(filename);
    file.imbue(loc);
    std::wstring line;
    unsigned int count = 0;
    while (getline(file, line)) {
        for (auto pos = line.find(word,0);
             pos != std::string::npos;
             pos = line.find(word, pos+1))
            count++;
    }
    return count;
}

void f(std::list<std::string> &filenames, std::promise<int> &that) {
    int count = 0;
    for (auto filename : filenames) {
        count += grep(filename, word);
    }
    
    that.set_value(count);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::locale loc("pl_PL.UTF-8");
    std::wcout.imbue(loc);
    std::wcin.imbue(loc);

    std::getline(std::wcin, word);

    std::wstring s_file_count;
    std::getline(std::wcin, s_file_count);
    int file_count = std::stoi(s_file_count);

    std::list<std::string> filenames[THREAD_COUNT]{};

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    for (int file_num = 0; file_num < file_count; file_num++) {
        std::wstring w_filename;
        std::getline(std::wcin, w_filename);
        std::string s_filename = converter.to_bytes(w_filename);
        filenames[file_num % THREAD_COUNT].push_back(s_filename);
    }

    std::promise<int> promises[THREAD_COUNT];
    std::future<int> futures[THREAD_COUNT];
    std::list<std::thread> threads;
    
    int total = 0;
    
    try {
	for (int i = 0; i < THREAD_COUNT; ++i) {
	  futures[i] = promises[i].get_future();
	  threads.push_back(std::thread(
	    [&filenames, &promises, i] () {
	      f(filenames[i], promises[i]);
	    }
	  ));
	}

	for (int i = 0; i < THREAD_COUNT; ++i) {
	  total += futures[i].get();
	}
    }
    catch(const std::exception& e) {
        std::cout << "exception from a thread: "<< e.what() << std::endl;
    }
    
    for (auto &thr : threads) {
      thr.join();
    }

    std::wcout << total << std::endl;
}
