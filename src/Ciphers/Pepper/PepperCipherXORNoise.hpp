#ifndef JELLY_CIPHERS_PEPPER_PEPPER_CIPHER_XOR_NOISE_HPP_
#define JELLY_CIPHERS_PEPPER_PEPPER_CIPHER_XOR_NOISE_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "../../Core/LayerCakeCryptDelegate.hpp"
#include "../../Core/MaskNoiseApplyEngine.hpp"
#include "../../Core/SampleMixer.hpp"
#include "../../Jelly.hpp"

namespace jelly {

class PepperCipherXORNoise final : public LayerCakeCryptDelegate {
 public:
  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                CryptMode pMode) const override {
    std::string aError;
    SampleMixerNoise aMixer;
    return MaskNoiseApplyEngine::ApplyMaskAndNoiseChunked(
        pSource, pWorker, pDestination, pLength, kExampleMask.data(),
        kExampleMask.size(), kExampleNoise.data(), kExampleNoise.size(),
        &aMixer, &aError, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  CryptMode pMode) const override {
    std::string aError;
    SampleMixerNoise aMixer;
    return MaskNoiseApplyEngine::ApplyMaskAndNoiseChunked(
        pSource, pWorker, pDestination, pLength, kExampleMask.data(),
        kExampleMask.size(), kExampleNoise.data(), kExampleNoise.size(),
        &aMixer, &aError, pMode);
  }

 private:
  static constexpr std::size_t kMaskLength = SB_CIPHER_LENGTH_GRANULARITY * 12;
  static constexpr std::size_t kNoiseLength = SB_CIPHER_LENGTH_GRANULARITY * 12;
  static const std::array<std::uint8_t, kMaskLength> kExampleMask;
  static const std::array<std::uint8_t, kNoiseLength> kExampleNoise;
};

inline const std::array<std::uint8_t, PepperCipherXORNoise::kMaskLength>
    PepperCipherXORNoise::kExampleMask = [] {
      std::array<std::uint8_t, kMaskLength> aMask{};
      std::uint32_t aState = 0xA5B35713u;
      for (std::size_t aIndex = 0; aIndex < aMask.size(); ++aIndex) {
        aState ^= (aState << 13);
        aState ^= (aState >> 17);
        aState ^= (aState << 5);
        const std::uint8_t aValue =
            static_cast<std::uint8_t>((aState ^ (aState >> 8)) & 0xFFu);
        aMask[aIndex] =
            (aValue == 0u) ? static_cast<std::uint8_t>(0xA3u) : aValue;
      }
      return aMask;
    }();

inline const std::array<std::uint8_t, PepperCipherXORNoise::kNoiseLength>
    PepperCipherXORNoise::kExampleNoise = [] {
      std::array<std::uint8_t, kNoiseLength> aNoise{};
      std::uint32_t aState = 0xC8E9124Du;
      for (std::size_t aIndex = 0; aIndex < aNoise.size(); ++aIndex) {
        aState ^= (aState << 13);
        aState ^= (aState >> 17);
        aState ^= (aState << 5);
        aNoise[aIndex] = static_cast<std::uint8_t>(aState & 0xFFu);
      }
      return aNoise;
    }();

}  // namespace jelly

#endif  // JELLY_CIPHERS_PEPPER_PEPPER_CIPHER_XOR_NOISE_HPP_
