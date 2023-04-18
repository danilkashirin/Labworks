#include "Allocator.h"
#include <list>
#include <set>
#include <iostream>
#include <fstream>

void VectorTest(size_t size, const std::string& filepath) {
  std::cout << "Vector Test\n"
            << "size: " << size << '\n'
            << "-----------------------------\n";
  std::ofstream fout;
  fout.open("/Users/danilkasirin/projects_C/Programming/labwork-9-danilkashirin/result.txt");
  fout << "Vector Test\n";

  auto start = static_cast<int> (clock());
  std::cout << "allocator start: " << start << '\n';

  if (filepath.empty()) {
    size_t n = size;
    Arguments::Parse(2, {2, n * 2},
                     {{n * 2, 2}});
    std::cout << "argument parsed\n";
  } else {
    Arguments::Parse(filepath);
    std::cout << "file parsed\n";
  }

  std::vector<int, Allocator<int>> allocator_cntr;
  for (int i = 0; i < size; i++) {
    allocator_cntr.push_back(i);
    if (i % 1000000 == 0) {
      fout << (int) ((static_cast<int> (clock()) - start) / 1e3) << ' ';
    }
  }
  fout << '\n';

  auto finish = static_cast<int> (clock());
  std::cout << "allocator finish: " << finish << '\n';
  std::cout << "allocator time: " << (finish - start) / 1e6 << '\n';
  std::cout << "-----------------------------\n";

  start = static_cast<int> (clock());
  std::cout << "basic start: " << start << '\n';
  std::vector<int> basic_cntr;
  for (int i = 0; i < size; i++) {
    basic_cntr.push_back(i);
    if (i % 1000000 == 0) {
      fout << (int) ((static_cast<int> (clock()) - start) / 1e3) << ' ';
    }
  }
  fout.close();
  finish = static_cast<int> (clock());
  std::cout << "basic finish: " << finish << '\n';
  std::cout << "basic time: " << (finish - start) / 1e6 << '\n';
  std::cout << "-----------------------------\n\n";
}

void ListTest(size_t size, const std::string& filepath) {
  std::cout << "List Test\n"
            << "size: " << size << '\n'
            << "-----------------------------\n";
  std::ofstream fout;
  fout.open("/Users/danilkasirin/projects_C/Programming/labwork-9-danilkashirin/result.txt");
  fout << "List Test\n";

  auto start = static_cast<int> (clock());
  std::cout << "allocator start: " << start << '\n';

  if (filepath.empty()) {
    size_t n = size;
    Arguments::Parse(2, {2, n * 2},
                     {{n * 2, 2}});
    std::cout << "argument parsed\n";
  } else {
    Arguments::Parse(filepath);
    std::cout << "file parsed\n";
  }

  std::list<int, Allocator<int>> allocator_cntr;
  for (int i = 0; i < size; i++) {
    allocator_cntr.push_back(i);
    if (i % 1000000 == 0) {
      fout << (int) ((static_cast<int> (clock()) - start) / 1e3) << ' ';
    }
  }
  fout << '\n';

  auto finish = static_cast<int> (clock());
  std::cout << "allocator finish: " << finish << '\n';
  std::cout << "allocator time: " << (finish - start) / 1e6 << '\n';
  std::cout << "-----------------------------\n";

  start = static_cast<int> (clock());
  std::cout << "basic start: " << start << '\n';
  std::list<int> basic_cntr;
  for (int i = 0; i < size; i++) {
    basic_cntr.push_back(i);
    if (i % 1000000 == 0) {
      fout << (int) ((static_cast<int> (clock()) - start) / 1e3) << ' ';
    }
  }
  fout.close();
  finish = static_cast<int> (clock());
  std::cout << "basic finish: " << finish << '\n';
  std::cout << "basic time: " << (finish - start) / 1e6 << '\n';
  std::cout << "-----------------------------\n\n";
}

void SetTest(size_t size, const std::string& filepath) {
  std::cout << "Set Test\n"
            << "size: " << size << '\n'
            << "-----------------------------\n";
  std::ofstream fout;
  fout.open("/Users/danilkasirin/projects_C/Programming/labwork-9-danilkashirin/result.txt");
  fout << "Set Test\n";

  auto start = static_cast<int> (clock());
  std::cout << "allocator start: " << start << '\n';

  if (filepath.empty()) {
    size_t n = size;
    Arguments::Parse(2, {2, n * 2},
                     {{n * 2, 2}});
    std::cout << "argument parsed\n";
  } else {
    Arguments::Parse(filepath);
    std::cout << "file parsed\n";
  }

  std::set<int, std::less<>, Allocator<int>> allocator_cntr;
  for (int i = 0; i < size; i++) {
    allocator_cntr.insert(i);
    if (i % 500000 == 0) {
      fout << (int) ((static_cast<int> (clock()) - start) / 1e3) << ' ';
    }
  }
  fout << '\n';

  auto finish = static_cast<int> (clock());
  std::cout << "allocator finish: " << finish << '\n';
  std::cout << "allocator time: " << (finish - start) / 1e6 << '\n';
  std::cout << "-----------------------------\n";

  start = static_cast<int> (clock());
  std::cout << "basic start: " << start << '\n';
  std::set<int> basic_cntr;
  for (int i = 0; i < size; i++) {
    basic_cntr.insert(i);
    if (i % 500000 == 0) {
      fout << (int) ((static_cast<int> (clock()) - start) / 1e3) << ' ';
    }
  }
  fout.close();
  finish = static_cast<int> (clock());
  std::cout << "basic finish: " << finish << '\n';
  std::cout << "basic time: " << (finish - start) / 1e6 << '\n';
  std::cout << "-----------------------------\n\n";
}

int main(int argc, const char** argv) {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  VectorTest(1e9, argc == 1 ? "" : argv[1]);
  //ListTest( 3* 1e8, argc == 1 ? "" : argv[1]);
  //SetTest(1e8, argc == 1 ? "" : argv[1]);
  return 0;
}

