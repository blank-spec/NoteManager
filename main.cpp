#include <chrono>
#include <cstddef>
#include <functional>
#include <iostream>
#include <optional>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

#include "src/Interface.h"

using namespace std;
using Options = optional<vector<pair<int, int>>>;

struct pair_hash {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    return hash<T1>()(p.first) ^ (hash<T2>()(p.second) << 1);
  }
};

Options findNums(vector<int>& vec, int target) {
  unordered_set<int> seenNums;
  unordered_set<pair<int, int>, pair_hash> seenPair;
  vector<pair<int, int>> result;

  for (const auto& v : vec) {
    const int need = target - v;

    if (seenNums.find(need) != seenNums.end()) {
      pair<int, int> newPair = {max(need, v), min(need, v)};

      if (seenPair.find(newPair) == seenPair.end()) {
        result.push_back(newPair);
        seenPair.insert(newPair);
      }
    }
    seenNums.insert(v);
  }

  return result.empty() ? nullopt : Options{result};
}

void sayHello() { cout << "Hello world" << endl; }

int main() {
  Interface ui("C:/Bank/Note.txt");
  ui.run();

  return 0;
}
