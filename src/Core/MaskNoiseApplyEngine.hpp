#ifndef JELLY_CORE_MASK_NOISE_APPLY_ENGINE_HPP_
#define JELLY_CORE_MASK_NOISE_APPLY_ENGINE_HPP_

#include <cstddef>
#include <cstdint>
#include <string>

#include "CryptMode.hpp"
#include "Mixer.hpp"

namespace jelly {

class MaskNoiseApplyEngine {
 public:
  static bool ApplyMaskAndNoiseChunked(const unsigned char* pSource,
                                       unsigned char* pWorker,
                                       unsigned char* pDestination,
                                       std::size_t pLength,
                                       const unsigned char* pMask,
                                       std::size_t pMaskLength,
                                       const unsigned char* pNoise,
                                       std::size_t pNoiseLength,
                                       MixerNoise* pMixer,
                                       std::string* pErrorMessage,
                                       CryptMode pMode);

  static bool ApplyMaskChunked(const unsigned char* pSource,
                               unsigned char* pWorker,
                               unsigned char* pDestination,
                               std::size_t pLength,
                               const unsigned char* pMask,
                               std::size_t pMaskLength,
                               Mixer* pMixer,
                               std::string* pErrorMessage,
                               CryptMode pMode);

  static bool ApplyNoiseChunked(const unsigned char* pSource,
                                unsigned char* pWorker,
                                unsigned char* pDestination,
                                std::size_t pLength,
                                const unsigned char* pNoise,
                                std::size_t pNoiseLength,
                                MixerNoise* pMixer,
                                std::string* pErrorMessage,
                                CryptMode pMode);
};

}  // namespace jelly

#endif  // JELLY_CORE_MASK_NOISE_APPLY_ENGINE_HPP_
