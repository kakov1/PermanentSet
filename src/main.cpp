#include "permanent_set.hpp"

using namespace hwt;
int main() {
  PermanentSet<int> set;

  std::string request;
  int key;

  while (std::cin >> request) {
    if (request == "k") {
      std::cin >> key;

      if (!std::cin.good()) {
        throw std::runtime_error("Key must be integer.");
      }

      set.insert(key);

    } else if (request == "s") {
      std::cin >> request;

      if (!std::cin.good() || request != "k") {
        throw std::runtime_error("Next symbol should be \"k\"");
      }

      std::cin >> key;

      if (!std::cin.good()) {
        throw std::runtime_error("Key must be integer.");
      }

      std::vector<int> versioned_keys = set.insert(key);

      for (auto &&key : versioned_keys) {
        std::cout << key << " ";
      }

    } else if (request == "r") {
      set.reset();
    } else {
      throw std::runtime_error("Incorrect request.");
    }
  }

  std::cout << std::endl;
}
