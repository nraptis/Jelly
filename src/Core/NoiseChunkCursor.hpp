#ifndef JELLY_CORE_NOISE_CHUNK_CURSOR_HPP_
#define JELLY_CORE_NOISE_CHUNK_CURSOR_HPP_

#include <cstddef>
#include <cstdint>

namespace jelly {

class NoiseChunkCursor {
 public:
  NoiseChunkCursor() = default;
  NoiseChunkCursor(const std::uint8_t* pNoise, std::size_t pNoiseLength)
      : mNoise(pNoise), mNoiseLength(pNoiseLength) {}

  void Reset(const std::uint8_t* pNoise, std::size_t pNoiseLength) {
    mNoise = pNoise;
    mNoiseLength = pNoiseLength;
    mAbsoluteIndex = 0;
  }

  bool IsValid() const { return mNoise != nullptr && mNoiseLength > 0; }

  void FillNext(std::uint8_t* pDestination, std::size_t pLength) {
    if (pDestination == nullptr || pLength == 0 || !IsValid()) {
      return;
    }
    for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
      const std::size_t aCycleIndex = mAbsoluteIndex % mNoiseLength;
      const std::size_t aNoiseIndex = (mNoiseLength - 1u) - aCycleIndex;
      pDestination[aIndex] = mNoise[aNoiseIndex];
      ++mAbsoluteIndex;
    }
  }

 private:
  const std::uint8_t* mNoise = nullptr;
  std::size_t mNoiseLength = 0;
  std::size_t mAbsoluteIndex = 0;
};

}  // namespace jelly

#endif  // JELLY_CORE_NOISE_CHUNK_CURSOR_HPP_
