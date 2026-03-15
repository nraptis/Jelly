#ifndef JELLY_CORE_MIXER_HPP_
#define JELLY_CORE_MIXER_HPP_

#include <cstddef>
#include <cstdint>

namespace jelly {

class MixerNoise {
 public:
  virtual ~MixerNoise() = default;
  virtual void Mix(const std::uint8_t* pSource,
                   unsigned char* pDestination,
                   unsigned char* pNoise,
                   std::size_t pLength) = 0;
};

class Mixer {
 public:
  virtual ~Mixer() = default;
  virtual void Mix(const std::uint8_t* pSource,
                   unsigned char* pDestination,
                   std::size_t pLength) = 0;
};

}  // namespace jelly

#endif  // JELLY_CORE_MIXER_HPP_
