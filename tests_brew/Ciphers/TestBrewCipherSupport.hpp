#ifndef JELLY_TESTS_BREW_CIPHERS_TEST_BREW_CIPHER_SUPPORT_HPP_
#define JELLY_TESTS_BREW_CIPHERS_TEST_BREW_CIPHER_SUPPORT_HPP_

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "../../src/PeanutButter.hpp"

namespace peanutbutter {
namespace test_brew {

inline int ReportFlatResult(const std::string& pCipherName,
                            const std::string& pError);

inline int ReportBlockResult(const std::string& pCipherName,
                             int pBlockSize,
                             const std::string& pError);

inline std::uint32_t CaseBits(std::size_t pCaseIndex, std::uint32_t pSalt) {
  std::uint32_t aValue =
      static_cast<std::uint32_t>(pCaseIndex) ^ (pSalt * 0x9e3779b9u);
  aValue ^= aValue >> 16;
  aValue *= 0x7feb352du;
  aValue ^= aValue >> 15;
  aValue *= 0x846ca68bu;
  aValue ^= aValue >> 16;
  return aValue;
}

inline std::uint8_t CaseMask(std::size_t pCaseIndex, std::uint32_t pSalt) {
  return static_cast<std::uint8_t>(CaseBits(pCaseIndex, pSalt) & 0xffu);
}

inline int CaseShift(std::size_t pCaseIndex, std::uint32_t pSalt) {
  return static_cast<int>(CaseBits(pCaseIndex, pSalt) % 1025u) - 512;
}

inline int CaseCount(std::size_t pCaseIndex, std::uint32_t pSalt) {
  return 1 + static_cast<int>(CaseBits(pCaseIndex, pSalt) % 5u);
}

inline int CaseStride(std::size_t pCaseIndex, std::uint32_t pSalt) {
  return static_cast<int>(CaseBits(pCaseIndex, pSalt) % 5u);
}

template <typename tRun>
int RunForEveryBlockSize(const char* pCipherName, tRun pRun) {
  const int aBlockSizes[] = {static_cast<int>(8),
                             static_cast<int>(12),
                             static_cast<int>(16),
                             static_cast<int>(24),
                             static_cast<int>(32),
                             static_cast<int>(48)};

  for (int aBlockSize : aBlockSizes) {
    std::string aError;
    pRun(aBlockSize, aError);
    if (ReportBlockResult(pCipherName, aBlockSize, aError) != 0) {
      return 1;
    }
  }
  return 0;
}

inline int ReportFlatResult(const std::string& pCipherName,
                            const std::string& pError) {
  if (!pError.empty()) {
    std::cerr << pError << "\n";
    return 1;
  }
  std::cout << pCipherName << " passed\n";
  return 0;
}

inline int ReportBlockResult(const std::string& pCipherName,
                             int pBlockSize,
                             const std::string& pError) {
  if (!pError.empty()) {
    std::cerr << pCipherName << " block_size=" << pBlockSize << "\n";
    std::cerr << pError << "\n";
    return 1;
  }
  return 0;
}

}  // namespace test_brew
}  // namespace peanutbutter

#endif
