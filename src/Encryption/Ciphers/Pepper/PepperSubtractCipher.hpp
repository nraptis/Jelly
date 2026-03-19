#ifndef PEANUTBUTTER_PEPPER_SUBTRACT_CIPHER_HPP_
#define PEANUTBUTTER_PEPPER_SUBTRACT_CIPHER_HPP_

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

// Input shape: pLength must be a multiple of BLOCK_GRANULARITY; mask length must be a positive multiple of 16.

class PepperSubtractCipher final : public Crypt {
 public:
  PepperSubtractCipher(const unsigned char* pMask, std::size_t pMaskLength)
      : mMask(pMask), mMaskLength(pMaskLength) {}

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const override {
    (void)pWorker;
    return ApplySubtract(pSource, pDestination, pLength, pErrorMessage, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const override {
    (void)pWorker;
    return ApplyAdd(pSource, pDestination, pLength, pErrorMessage, pMode);
  }

 private:
  static void SetError(std::string* pErrorMessage, const char* pMessage) {
    if (pErrorMessage != nullptr) {
      *pErrorMessage = pMessage;
    }
  }

  bool Validate(const unsigned char* pSource,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage) const {
    if (pErrorMessage != nullptr) {
      pErrorMessage->clear();
    }
    if (pLength == 0u) {
      return true;
    }
    if ((pLength % BLOCK_GRANULARITY) != 0u) {
      SetError(pErrorMessage, "pLength must be a multiple of BLOCK_GRANULARITY");
      return false;
    }
    if (pSource == nullptr || pDestination == nullptr || mMask == nullptr) {
      SetError(pErrorMessage, "source/destination/mask must not be null");
      return false;
    }
    if (pSource == pDestination) {
      SetError(pErrorMessage, "source and destination must not alias");
      return false;
    }
    if (mMaskLength == 0u || (mMaskLength % 16u) != 0u) {
      SetError(pErrorMessage, "mask length must be a positive multiple of 16");
      return false;
    }
    return true;
  }

  bool ApplyAdd(const unsigned char* pSource,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const {
    if (!Validate(pSource, pDestination, pLength, pErrorMessage)) {
      return pLength == 0u;
    }
    if (pLength == 0u) {
      return true;
    }
    const std::size_t aChunkCount = pLength >> 4;
    const std::size_t aMaskChunkCount = mMaskLength >> 4;
    std::size_t aMaskChunkIndex = 0u;

    for (std::size_t aChunk = 0; aChunk < aChunkCount; ++aChunk) {
      const unsigned char* aSourcePtr = pSource + (aChunk << 4);
      const unsigned char* aMaskPtr = mMask + (aMaskChunkIndex << 4);
      unsigned char* aDestinationPtr = pDestination + (aChunk << 4);
      switch (pMode) {
        case CryptMode::kNormal:
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDestinationPtr[aIndex] = static_cast<unsigned char>(
                aSourcePtr[aIndex] + aMaskPtr[aIndex]);
          }
          break;
      case CryptMode::kSimd:
#if defined(__AVX2__)
          _mm_storeu_si128(
              reinterpret_cast<__m128i*>(aDestinationPtr),
              _mm_add_epi8(
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aMaskPtr))));
          break;
#elif defined(__SSSE3__)
          _mm_storeu_si128(
              reinterpret_cast<__m128i*>(aDestinationPtr),
              _mm_add_epi8(
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aMaskPtr))));
          break;
#else
          SetError(pErrorMessage, "SIMD mode is not available");
          return false;
#endif
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
          vst1q_u8(aDestinationPtr,
                   vaddq_u8(vld1q_u8(aSourcePtr), vld1q_u8(aMaskPtr)));
          break;
#else
          SetError(pErrorMessage, "NEON mode is not available");
          return false;
#endif
      }
      ++aMaskChunkIndex;
      if (aMaskChunkIndex == aMaskChunkCount) {
        aMaskChunkIndex = 0u;
      }
    }
    return true;
  }

  bool ApplySubtract(const unsigned char* pSource,
                     unsigned char* pDestination,
                     std::size_t pLength,
                     std::string* pErrorMessage,
                     CryptMode pMode) const {
    if (!Validate(pSource, pDestination, pLength, pErrorMessage)) {
      return pLength == 0u;
    }
    if (pLength == 0u) {
      return true;
    }
    const std::size_t aChunkCount = pLength >> 4;
    const std::size_t aMaskChunkCount = mMaskLength >> 4;
    std::size_t aMaskChunkIndex = 0u;

    for (std::size_t aChunk = 0; aChunk < aChunkCount; ++aChunk) {
      const unsigned char* aSourcePtr = pSource + (aChunk << 4);
      const unsigned char* aMaskPtr = mMask + (aMaskChunkIndex << 4);
      unsigned char* aDestinationPtr = pDestination + (aChunk << 4);
      switch (pMode) {
        case CryptMode::kNormal:
          for (std::size_t aIndex = 0; aIndex < 16; ++aIndex) {
            aDestinationPtr[aIndex] = static_cast<unsigned char>(
                aSourcePtr[aIndex] - aMaskPtr[aIndex]);
          }
          break;
      case CryptMode::kSimd:
#if defined(__AVX2__)
          _mm_storeu_si128(
              reinterpret_cast<__m128i*>(aDestinationPtr),
              _mm_sub_epi8(
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aMaskPtr))));
          break;
#elif defined(__SSSE3__)
          _mm_storeu_si128(
              reinterpret_cast<__m128i*>(aDestinationPtr),
              _mm_sub_epi8(
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aSourcePtr)),
                  _mm_loadu_si128(reinterpret_cast<const __m128i*>(aMaskPtr))));
          break;
#else
          SetError(pErrorMessage, "SIMD mode is not available");
          return false;
#endif
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
          vst1q_u8(aDestinationPtr,
                   vsubq_u8(vld1q_u8(aSourcePtr), vld1q_u8(aMaskPtr)));
          break;
#else
          SetError(pErrorMessage, "NEON mode is not available");
          return false;
#endif
      }
      ++aMaskChunkIndex;
      if (aMaskChunkIndex == aMaskChunkCount) {
        aMaskChunkIndex = 0u;
      }
    }
    return true;
  }

  const unsigned char* mMask = nullptr;
  std::size_t mMaskLength = 0;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_PEPPER_SUBTRACT_CIPHER_HPP_
