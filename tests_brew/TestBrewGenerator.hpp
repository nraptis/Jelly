#ifndef JELLY_TESTS_BREW_TEST_BREW_GENERATOR_HPP_
#define JELLY_TESTS_BREW_TEST_BREW_GENERATOR_HPP_

#include <cstddef>
#include <vector>

#include "../src/PeanutButter.hpp"

namespace peanutbutter {

class TestBrewGenerator {
 public:
  static std::vector<unsigned char> GenerateBytes(std::size_t pLength);

  static std::vector<unsigned char> GenerateBytesBlock(int pBlockSize,
                                                       int pBlockCount);

  static std::size_t NormalizeLength(std::size_t pLength);
};

}  // namespace peanutbutter

#endif
