#ifndef PEANUTBUTTER_ENCRYPTION_MASK_LIBRARY_HPP_
#define PEANUTBUTTER_ENCRYPTION_MASK_LIBRARY_HPP_

#include <cstddef>
#include <cstdint>

namespace peanutbutter {

class MaskLibrary {
 public:
  static std::uint8_t GetMask(int pIndex);
  static constexpr std::size_t kMaskCount = 128u;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_ENCRYPTION_MASK_LIBRARY_HPP_
