#include "permanent_set.hpp"
#include <cstddef>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace hwt;

const std::string TEST_DIR = std::string(TEST_DATA_DIR) + "/";

class EteTests : public testing::Test {
protected:
  std::string test(std::size_t test_number) {
    std::ifstream test_file(TEST_DIR + "in/" + std::to_string(test_number) + "test.in");

    test_file.exceptions(std::ifstream::badbit);

    PermanentSet<int> set;

    std::string request;
    int key;
    std::string answer;

    while (test_file >> request) {
      if (request == "k") {
        test_file >> key;

        set.insert(key);

      } else if (request == "s") {
        test_file >> request;
        test_file >> key;

        std::deque<int> versioned_keys = set.emplace(key);

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
    std::ifstream answer_file(TEST_DIR + "out/" + std::to_string(test_number) +
                              "test.out");

    answer_file.exceptions(std::ifstream::badbit);

    std::string buf, answer;

    while (answer_file >> buf) {
      answer += (buf + " ");
    }

    return answer;
  }
};

class InterfaceTests : public testing::Test {
protected:
  PermanentSet<int> set1;
  PermanentSet<int> set2;

  InterfaceTests() {
    for (int i = 1; i <= 50; ++i) {
      set1.emplace(i);
    }

    for (int i = 15; i >= 1; --i) {
      set2.emplace(i);
    }
  }
};

TEST_F(InterfaceTests, TestConstructors) {
  PermanentSet<int> set_copy1 = set1;
  PermanentSet<int> set_copy2;

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
  for (int i = 1; i <= 7; ++i) {
    ASSERT_TRUE(test(i) == get_answer(i));
  }
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}