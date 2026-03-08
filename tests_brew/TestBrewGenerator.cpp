#include "TestBrewGenerator.hpp"

#include <cstdint>
#include <random>

namespace jelly {

namespace {

std::mt19937& RandomEngine() {
  static std::random_device aDevice;
  static std::mt19937 aEngine(aDevice());
  return aEngine;
}

unsigned char RandomByte() {
  static std::uniform_int_distribution<int> aDistribution(0, 255);
  return static_cast<unsigned char>(aDistribution(RandomEngine()));
}

}  // namespace

std::vector<unsigned char> TestBrewGenerator::GenerateBytes(
    std::size_t pLength) {
  if ((pLength % SB_CIPHER_LENGTH_GRANULARITY) != 0) {
    return {};
  }
  std::vector<unsigned char> aResult(pLength);
  for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
    aResult[aIndex] = RandomByte();
  }
  return aResult;
}

std::vector<unsigned char> TestBrewGenerator::GenerateBytesBlock(
    int pBlockSize,
    int pBlockCount) {
  if (pBlockSize <= 0 || pBlockCount <= 0) {
    return {};
  }

  const std::size_t aLength = static_cast<std::size_t>(pBlockSize) *
                              static_cast<std::size_t>(pBlockCount);
  return GenerateBytes(aLength);
}

std::size_t TestBrewGenerator::NormalizeLength(std::size_t pLength) {
  if (pLength == 0) {
    return SB_CIPHER_LENGTH_GRANULARITY;
  }
  const std::size_t aRemainder = pLength % SB_CIPHER_LENGTH_GRANULARITY;
  if (aRemainder == 0) {
    return pLength;
  }
  return pLength + (SB_CIPHER_LENGTH_GRANULARITY - aRemainder);
}

}  // namespace jelly
