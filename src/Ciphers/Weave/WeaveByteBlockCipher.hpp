#ifndef JELLY_WEAVE_BYTE_BLOCK_CIPHER_HPP_
#define JELLY_WEAVE_BYTE_BLOCK_CIPHER_HPP_

#include <algorithm>
#include <cstddef>
#include <vector>

#include "../../Core/LayerCakeCryptDelegate.hpp"
#include "../../Jelly.hpp"

namespace jelly {

class WeaveByteBlockCipher final : public LayerCakeCryptDelegate {
 public:
  WeaveByteBlockCipher(std::size_t pBlockSize,
                       int pCount,
                       int pFrontStride,
                       int pBackStride)
      : mBlockSize(pBlockSize),
        mCount(pCount),
        mFrontStride(pFrontStride),
        mBackStride(pBackStride) {}

  bool SealData(const unsigned char* pSource,
                const unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                CryptMode pMode) override {
    (void)pMode;
    return Apply(pSource, pDestination, pLength);
  }

  bool UnsealData(const unsigned char* pSource,
                  const unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  CryptMode pMode) override {
    (void)pMode;
    return Apply(pSource, pDestination, pLength);
  }

 private:
  bool Apply(const unsigned char* pSource,
             unsigned char* pDestination,
             std::size_t pLength) const {
    if (mBlockSize == 0) {
      return false;
    }
    if (pLength == 0) {
      return true;
    }
    if ((pLength % SB_CIPHER_LENGTH_GRANULARITY) != 0) {
      return false;
    }
    if (pSource == nullptr || pDestination == nullptr) {
      return false;
    }
    if (pSource == pDestination) {
      return false;
    }

    for (std::size_t aOffset = 0; aOffset < pLength; aOffset += mBlockSize) {
      const std::size_t aSpan = std::min(mBlockSize, pLength - aOffset);
      const std::vector<std::size_t> aMap =
          BuildMap(aSpan, mCount, mFrontStride, mBackStride);
      for (std::size_t aIndex = 0; aIndex < aSpan; ++aIndex) {
        pDestination[aOffset + aIndex] = pSource[aOffset + aMap[aIndex]];
      }
    }
    return true;
  }

  static std::size_t ClampPositiveCount(int pValue) {
    return static_cast<std::size_t>(pValue < 1 ? 1 : pValue);
  }

  static std::size_t ClampNonNegative(int pValue) {
    return static_cast<std::size_t>(pValue < 0 ? 0 : pValue);
  }

  static std::vector<std::size_t> BuildMap(std::size_t pLength,
                                           int pCount,
                                           int pFrontStride,
                                           int pBackStride) {
    std::vector<std::size_t> aMap(pLength);
    for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
      aMap[aIndex] = aIndex;
    }
    if (pLength < 2) {
      return aMap;
    }

    const std::size_t aCount = ClampPositiveCount(pCount);
    const std::size_t aFrontStride = ClampNonNegative(pFrontStride);
    const std::size_t aBackStride = ClampNonNegative(pBackStride);
    std::size_t aFront = 0;
    std::size_t aBack = pLength - 1;

    while (aFront < aBack) {
      std::size_t aSwaps = aCount;
      while (aSwaps > 0 && aFront < aBack) {
        std::swap(aMap[aFront], aMap[aBack]);
        --aSwaps;
        ++aFront;
        --aBack;
      }
      if (aFront >= aBack) {
        break;
      }
      std::size_t aSkips = aFrontStride;
      while (aSkips > 0 && aFront < aBack) {
        --aSkips;
        ++aFront;
      }
      if (aFront >= aBack) {
        break;
      }
      aSkips = aBackStride;
      while (aSkips > 0 && aFront < aBack) {
        --aSkips;
        --aBack;
      }
    }

    return aMap;
  }

  std::size_t mBlockSize;
  int mCount;
  int mFrontStride;
  int mBackStride;
};

}  // namespace jelly

#endif  // JELLY_WEAVE_BYTE_BLOCK_CIPHER_HPP_
