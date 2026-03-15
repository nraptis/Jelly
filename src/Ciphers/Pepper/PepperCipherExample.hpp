#ifndef JELLY_CIPHERS_PEPPER_PEPPER_CIPHER_EXAMPLE_HPP_
#define JELLY_CIPHERS_PEPPER_PEPPER_CIPHER_EXAMPLE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "../../Core/LayerCakeCryptDelegate.hpp"
#include "../../Core/MaskNoiseApplyEngine.hpp"
#include "../../Core/SampleMixer.hpp"
#include "../../Jelly.hpp"

namespace jelly {

class PepperCipherExample final : public LayerCakeCryptDelegate {
 public:
  PepperCipherExample() { BuildDeterministicMask(); }

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                CryptMode pMode) const override {
    std::string aError;
    SampleMixer aMixer;
    return MaskNoiseApplyEngine::ApplyMaskChunked(
        pSource, pWorker, pDestination, pLength, mMask.data(), mMask.size(),
        &aMixer, &aError, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  CryptMode pMode) const override {
    std::string aError;
    SampleMixer aMixer;
    return MaskNoiseApplyEngine::ApplyMaskChunked(
        pSource, pWorker, pDestination, pLength, mMask.data(), mMask.size(),
        &aMixer, &aError, pMode);
  }

 private:
  static constexpr std::size_t kMaskLength = SB_CIPHER_LENGTH_GRANULARITY * 12;

  void BuildDeterministicMask() {
    std::uint32_t aState = 0xA5B35713u;
    for (std::size_t aIndex = 0; aIndex < mMask.size(); ++aIndex) {
      aState ^= (aState << 13);
      aState ^= (aState >> 17);
      aState ^= (aState << 5);
      const std::uint8_t aMask =
          static_cast<std::uint8_t>((aState ^ (aState >> 8)) & 0xFFu);
      mMask[aIndex] = (aMask == 0u) ? static_cast<std::uint8_t>(0xA3u) : aMask;
    }
  }

  std::array<std::uint8_t, kMaskLength> mMask{};
};

}  // namespace jelly

#endif  // JELLY_CIPHERS_PEPPER_PEPPER_CIPHER_EXAMPLE_HPP_
