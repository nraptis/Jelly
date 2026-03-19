#ifndef PEANUTBUTTER_PASSWORD_JUMP_SUBTRACT_CIPHER_HPP_
#define PEANUTBUTTER_PASSWORD_JUMP_SUBTRACT_CIPHER_HPP_

#include <cstddef>
#include <cstdint>
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

// Input shape: pLength must be a multiple of BLOCK_GRANULARITY; password and jump lengths must be positive multiples of 16.

class PasswordJumpSubtractCipher final : public Crypt {
 public:
  PasswordJumpSubtractCipher(const unsigned char* pPassword,
                             int pPasswordLength,
                             const unsigned char* pJumps,
                             int pJumpLength)
      : mPassword(pPassword),
        mPasswordLength(pPasswordLength),
        mJumps(pJumps),
        mJumpLength(pJumpLength) {}

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const override {
    return Apply(pSource, pWorker, pDestination, pLength, pErrorMessage, pMode,
                 true);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const override {
    return Apply(pSource, pWorker, pDestination, pLength, pErrorMessage, pMode,
                 false);
  }

 private:
  bool Apply(const unsigned char* pSource,
             unsigned char* pWorker,
             unsigned char* pDestination,
             std::size_t pLength,
             std::string* pErrorMessage,
             CryptMode pMode,
             bool pEncrypt) const {
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
    if (pSource == nullptr || pDestination == nullptr || mPassword == nullptr ||
        mJumps == nullptr) {
      if (pErrorMessage != nullptr) {
        *pErrorMessage = "null buffer in PasswordJumpSubtractCipher";
      }
      return false;
    }
    if (pSource == pDestination) {
      if (pErrorMessage != nullptr) {
        *pErrorMessage = "source and destination must not alias";
      }
      return false;
    }
    if (mPasswordLength <= 0 || (mPasswordLength % 16) != 0 ||
        mJumpLength <= 0 || (mJumpLength % 16) != 0) {
      if (pErrorMessage != nullptr) {
        *pErrorMessage =
            "password and jump lengths must be positive multiples of 16";
      }
      return false;
    }

    const std::size_t aChunkCount = pLength >> 4;
    const std::size_t aPasswordChunkCount =
        static_cast<std::size_t>(mPasswordLength) >> 4;
    const bool aPowerOfTwoPasswordChunks =
        (aPasswordChunkCount & (aPasswordChunkCount - 1u)) == 0u;

    std::size_t aPasswordChunkIndex = 0u;
    std::size_t aJumpIndex = 0u;

    for (std::size_t aChunk = 0; aChunk < aChunkCount; ++aChunk) {
      const unsigned char* aSourcePtr = pSource + (aChunk << 4);
      const unsigned char* aPasswordPtr =
          mPassword + (aPasswordChunkIndex << 4);
      unsigned char* aDestinationPtr = pDestination + (aChunk << 4);

      switch (pMode) {
        case CryptMode::kNormal:
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDestinationPtr[aIndex] = static_cast<unsigned char>(
                pEncrypt ? (aSourcePtr[aIndex] - aPasswordPtr[aIndex])
                         : (aSourcePtr[aIndex] + aPasswordPtr[aIndex]));
          }
          break;
        case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
          if (pEncrypt) {
            _mm_storeu_si128(
                reinterpret_cast<__m128i*>(aDestinationPtr),
                _mm_sub_epi8(
                    _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                    _mm_loadu_si128(reinterpret_cast<const __m128i*>(aPasswordPtr))));
          } else {
            _mm_storeu_si128(
                reinterpret_cast<__m128i*>(aDestinationPtr),
                _mm_add_epi8(
                    _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                    _mm_loadu_si128(reinterpret_cast<const __m128i*>(aPasswordPtr))));
          }
#else
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDestinationPtr[aIndex] = static_cast<unsigned char>(
                pEncrypt ? (aSourcePtr[aIndex] - aPasswordPtr[aIndex])
                         : (aSourcePtr[aIndex] + aPasswordPtr[aIndex]));
          }
#endif
          break;
        case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
          if (pEncrypt) {
            vst1q_u8(aDestinationPtr,
                     vsubq_u8(vld1q_u8(aSourcePtr), vld1q_u8(aPasswordPtr)));
          } else {
            vst1q_u8(aDestinationPtr,
                     vaddq_u8(vld1q_u8(aSourcePtr), vld1q_u8(aPasswordPtr)));
          }
#else
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDestinationPtr[aIndex] = static_cast<unsigned char>(
                pEncrypt ? (aSourcePtr[aIndex] - aPasswordPtr[aIndex])
                         : (aSourcePtr[aIndex] + aPasswordPtr[aIndex]));
          }
#endif
          break;
      }

      const std::size_t aJump =
          static_cast<std::size_t>(mJumps[aJumpIndex]);
      ++aJumpIndex;
      if (aJumpIndex == static_cast<std::size_t>(mJumpLength)) {
        aJumpIndex = 0u;
      }
      aPasswordChunkIndex += aJump;
      if (aPowerOfTwoPasswordChunks) {
        aPasswordChunkIndex &= (aPasswordChunkCount - 1u);
      } else {
        aPasswordChunkIndex %= aPasswordChunkCount;
      }
    }

    return true;
  }

  const unsigned char* mPassword = nullptr;
  int mPasswordLength = 0;
  const unsigned char* mJumps = nullptr;
  int mJumpLength = 0;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_PASSWORD_JUMP_SUBTRACT_CIPHER_HPP_
