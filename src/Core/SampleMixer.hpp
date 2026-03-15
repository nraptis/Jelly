#ifndef JELLY_CORE_SAMPLE_MIXER_HPP_
#define JELLY_CORE_SAMPLE_MIXER_HPP_

#include <array>
#include <cstddef>
#include <cstdint>

#include "Mixer.hpp"

namespace jelly {

class SampleMixer final : public Mixer {
 public:
  SampleMixer() = default;

  void Mix(const std::uint8_t* pSource,
           unsigned char* pDestination,
           std::size_t pLength) override;

 private:
  static constexpr std::size_t kKeyLength = 256;
  static const std::array<std::uint8_t, kKeyLength> kKey;
};

class SampleMixerNoise final : public MixerNoise {
 public:
  SampleMixerNoise() = default;

  void Mix(const std::uint8_t* pSource,
           unsigned char* pDestination,
           unsigned char* pNoise,
           std::size_t pLength) override;
};

}  // namespace jelly

#endif  // JELLY_CORE_SAMPLE_MIXER_HPP_
