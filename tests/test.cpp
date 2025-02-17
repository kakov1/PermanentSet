#include "permanent_set.hpp"
#include <cstddef>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace hwt;

class EteTests : public testing::Test {
protected:
  std::string test(std::size_t test_number) {
    std::ifstream test_file("./in/" + std::to_string(test_number) + "test.in");

    test_file.exceptions(std::ifstream::failbit);

    PermanentSet<int> set;

    std::string request;
    int key;
    std::string answer;

    while (std::cin >> request) {
      if (request == "k") {
        std::cin >> key;

        set.insert(key);

      } else if (request == "s") {
        std::cin >> request;
        std::cin >> key;

        std::vector<int> versioned_keys = set.insert(key);

        for (auto &&key : versioned_keys) {
          answer += std::to_string(key) + " ";
        }

      } else {
        set.reset();
      }
    }

    return answer;
  }

  std::string get_answer(std::size_t test_number) {
    std::ifstream answer_file("./out/" + std::to_string(test_number) +
                              "test.out");

    answer_file.exceptions(std::ifstream::failbit);

    return std::string((std::istreambuf_iterator<char>(answer_file)),
                       std::istreambuf_iterator<char>());
  }
};

class InterfaceTests : public testing::Test {
protected:
  PermanentSet<int> set1;
  PermanentSet<int> set2;

  InterfaceTests() {
    for (int i = 1; i < 50; ++i) {
      set1.insert(i);
    }

    for (int i = 15; i >= 1; --i) {
      set2.insert(i);
    }
  }
};

TEST_F(InterfaceTests, TestConstructors) {
  PermanentSet<int> set_copy1 = set1;
  PermanentSet<int> set_copy2;

  std::cout << set1.get_root()->height_ <<std::endl;
  std::cout << set_copy1.get_root()->height_<<std::endl;

  std::cout << (static_cast<const SearchTree<int> &>(set_copy1) ==
                   static_cast<const SearchTree<int> &>(set1))
            << std::endl;

  /// set_copy1.is_equal(set_copy1.get_root(), )

  set1 = set1;
  set1 = std::move(set1);

  ASSERT_TRUE(set1 == set_copy1);
  ASSERT_FALSE(set1 == set_copy2);
  ASSERT_FALSE(set1 == set2);

  PermanentSet<int> set_copy4 = std::move(set_copy1);
  PermanentSet<int> set_copy5 = std::move(set2);

  ASSERT_TRUE(set1 == set_copy4);
  ASSERT_FALSE(set1 == set_copy5);
}

TEST_F(InterfaceTests, TestAssign) {
  PermanentSet<int> set_copy1;
  PermanentSet<int> set_copy2;

  set_copy1 = set1;
  set_copy2 = set2;

  ASSERT_TRUE(set1 == set_copy1);
  ASSERT_FALSE(set1 == set_copy2);

  PermanentSet<int> set_copy3 = std::move(set_copy1);
  PermanentSet<int> set_copy4 = std::move(set_copy2);

  ASSERT_TRUE(set1 == set_copy3);
  ASSERT_FALSE(set1 == set_copy4);
}

TEST_F(EteTests, test) {
  for (int i = 1; i <= 6; ++i)
    ASSERT_TRUE(test(i) == get_answer(i));
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}