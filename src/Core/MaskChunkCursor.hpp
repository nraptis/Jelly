#ifndef JELLY_CORE_MASK_CHUNK_CURSOR_HPP_
#define JELLY_CORE_MASK_CHUNK_CURSOR_HPP_

#include <cstddef>
#include <cstdint>

namespace jelly {

class MaskChunkCursor {
 public:
  MaskChunkCursor() = default;
  MaskChunkCursor(const std::uint8_t* pMask, std::size_t pMaskLength)
      : mMask(pMask), mMaskLength(pMaskLength) {}

  void Reset(const std::uint8_t* pMask, std::size_t pMaskLength) {
    mMask = pMask;
    mMaskLength = pMaskLength;
    mAbsoluteIndex = 0;
  }

  bool IsValid() const { return mMask != nullptr && mMaskLength > 0; }

  void FillNext(std::uint8_t* pDestination, std::size_t pLength) {
    if (pDestination == nullptr || pLength == 0 || !IsValid()) {
      return;
    }
    for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
      const std::size_t aCycleIndex = mAbsoluteIndex % mMaskLength;
      const std::size_t aMaskIndex = (mMaskLength - 1u) - aCycleIndex;
      pDestination[aIndex] = mMask[aMaskIndex];
      ++mAbsoluteIndex;
    }
  }

 private:
  const std::uint8_t* mMask = nullptr;
  std::size_t mMaskLength = 0;
  std::size_t mAbsoluteIndex = 0;
};

}  // namespace jelly

#endif  // JELLY_CORE_MASK_CHUNK_CURSOR_HPP_
