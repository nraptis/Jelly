#ifndef PEANUTBUTTER_CASCADE_JUMP_XOR_CIPHER_HPP_
#define PEANUTBUTTER_CASCADE_JUMP_XOR_CIPHER_HPP_

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#if defined(__AVX2__) || defined(__SSSE3__)
#include <immintrin.h>
#endif

#include "../../Crypt.hpp"
#include "../../../PeanutButter.hpp"

namespace peanutbutter {

// Input shape: pLength must be a multiple of BLOCK_GRANULARITY; mask and jump lengths must be positive multiples of 16.

class CascadeJumpXORCipher final : public Crypt {
 public:
  CascadeJumpXORCipher(const unsigned char* pMask,
                       int pMaskLength,
                       const unsigned char* pJumps,
                       int pJumpLength)
      : mMask(pMask),
        mMaskLength(pMaskLength),
        mJumps(pJumps),
        mJumpLength(pJumpLength) {}

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const override {
    return Apply(pSource, pWorker, pDestination, pLength, pErrorMessage, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const override {
    return Apply(pSource, pWorker, pDestination, pLength, pErrorMessage, pMode);
  }

 private:
  bool Apply(const unsigned char* pSource,
             unsigned char* pWorker,
             unsigned char* pDestination,
             std::size_t pLength,
             std::string* pErrorMessage,
             CryptMode pMode) const {
    (void)pWorker;
    if (pErrorMessage != nullptr) {
      pErrorMessage->clear();
    }
    if (pLength == 0u) {
      return true;
    }
    if ((pLength % BLOCK_GRANULARITY) != 0u) {
      if (pErrorMessage != nullptr) {
        *pErrorMessage = "pLength must be a multiple of BLOCK_GRANULARITY";
      }
      return false;
    }
    if (pSource == nullptr || pDestination == nullptr || mMask == nullptr ||
        mJumps == nullptr) {
      if (pErrorMessage != nullptr) {
        *pErrorMessage = "null buffer in CascadeJumpXORCipher";
      }
      return false;
    }
    if (pSource == pDestination) {
      if (pErrorMessage != nullptr) {
        *pErrorMessage = "source and destination must not alias";
      }
      return false;
    }
    if (mMaskLength <= 0 || (mMaskLength % 16) != 0 || mJumpLength <= 0 ||
        (mJumpLength % 16) != 0) {
      if (pErrorMessage != nullptr) {
        *pErrorMessage =
            "mask and jump lengths must be positive multiples of 16";
      }
      return false;
    }

    const std::size_t aChunkCount = pLength >> 4;
    const std::size_t aMaskChunkCount =
        static_cast<std::size_t>(mMaskLength) >> 4;
    const bool aPowerOfTwoMaskChunks =
        (aMaskChunkCount & (aMaskChunkCount - 1u)) == 0u;
    std::size_t aMaskChunkIndex = 0u;
    std::size_t aJumpIndex = 0u;
    unsigned char aState[16] = {};

    for (std::size_t aChunk = 0; aChunk < aChunkCount; ++aChunk) {
      const unsigned char* aSourcePtr = pSource + (aChunk << 4);
      const unsigned char* aMaskPtr = mMask + (aMaskChunkIndex << 4);
      unsigned char* aDestinationPtr = pDestination + (aChunk << 4);
      unsigned char aDerived[16];

      switch (pMode) {
        case CryptMode::kNormal:
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDerived[aIndex] =
                static_cast<unsigned char>(aMaskPtr[aIndex] ^ aState[aIndex]);
            aDestinationPtr[aIndex] =
                static_cast<unsigned char>(aSourcePtr[aIndex] ^ aDerived[aIndex]);
          }
          break;
        case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
          {
            const __m128i aMask = _mm_loadu_si128(
                reinterpret_cast<const __m128i*>(aMaskPtr));
            const __m128i aStateVec = _mm_loadu_si128(
                reinterpret_cast<const __m128i*>(aState));
            const __m128i aDerivedVec = _mm_xor_si128(aMask, aStateVec);
            _mm_storeu_si128(reinterpret_cast<__m128i*>(aDerived), aDerivedVec);
            _mm_storeu_si128(
                reinterpret_cast<__m128i*>(aDestinationPtr),
                _mm_xor_si128(
                    _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                    aDerivedVec));
          }
#else
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDerived[aIndex] =
                static_cast<unsigned char>(aMaskPtr[aIndex] ^ aState[aIndex]);
            aDestinationPtr[aIndex] =
                static_cast<unsigned char>(aSourcePtr[aIndex] ^ aDerived[aIndex]);
          }
#endif
          break;
        case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
          {
            const uint8x16_t aDerivedVec =
                veorq_u8(vld1q_u8(aMaskPtr), vld1q_u8(aState));
            vst1q_u8(aDerived, aDerivedVec);
            vst1q_u8(aDestinationPtr,
                     veorq_u8(vld1q_u8(aSourcePtr), aDerivedVec));
          }
#else
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDerived[aIndex] =
                static_cast<unsigned char>(aMaskPtr[aIndex] ^ aState[aIndex]);
            aDestinationPtr[aIndex] =
                static_cast<unsigned char>(aSourcePtr[aIndex] ^ aDerived[aIndex]);
          }
#endif
          break;
      }

      for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
        aState[aIndex] = static_cast<unsigned char>(
            (aDerived[(aIndex + 1u) & 15u] << 1u) |
            (aDerived[aIndex] >> 7u));
      }

      const std::size_t aJump = static_cast<std::size_t>(mJumps[aJumpIndex]);
      ++aJumpIndex;
      if (aJumpIndex == static_cast<std::size_t>(mJumpLength)) {
        aJumpIndex = 0u;
      }
      aMaskChunkIndex += aJump;
      if (aPowerOfTwoMaskChunks) {
        aMaskChunkIndex &= (aMaskChunkCount - 1u);
      } else {
        aMaskChunkIndex %= aMaskChunkCount;
      }
    }
    return true;
  }

  const unsigned char* mMask = nullptr;
  int mMaskLength = 0;
  const unsigned char* mJumps = nullptr;
  int mJumpLength = 0;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_CASCADE_JUMP_XOR_CIPHER_HPP_
