#ifndef JELLY_ROTATE_MASK_BYTE_BLOCK_CIPHER_HPP_
#define JELLY_ROTATE_MASK_BYTE_BLOCK_CIPHER_HPP_

#include <cstddef>

#include "RotateMaskBlockCipher.hpp"

namespace peanutbutter {

// Input shape: pLength must be a multiple of BLOCK_GRANULARITY and the runtime block size; mask is a single byte.

template <std::size_t tBlockSize>
using RotateMaskByteBlockCipher = RotateMaskBlockCipher<tBlockSize>;

}  // namespace peanutbutter

#endif  // JELLY_ROTATE_MASK_BYTE_BLOCK_CIPHER_HPP_
