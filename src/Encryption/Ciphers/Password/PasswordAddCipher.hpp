#ifndef PEANUTBUTTER_PASSWORD_ADD_CIPHER_HPP_
#define PEANUTBUTTER_PASSWORD_ADD_CIPHER_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

#if defined(__AVX2__) || defined(__SSSE3__)
#include <immintrin.h>
#endif

#include "../../Crypt.hpp"
#include "../../../PeanutButter.hpp"

namespace peanutbutter {

// Input shape: pLength must be a multiple of BLOCK_GRANULARITY; password length must be a positive multiple of 16.

class PasswordAddCipher final : public Crypt {
 public:
  PasswordAddCipher(const unsigned char* pPassword, int pPasswordLength)
      : mPasswordBytes(BuildPasswordBytes(pPassword, pPasswordLength)) {}

  explicit PasswordAddCipher(const std::vector<unsigned char>& pPasswordBytes)
      : PasswordAddCipher(pPasswordBytes.data(),
                          static_cast<int>(pPasswordBytes.size())) {}

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const override {
    return ApplyAdd(pSource, pWorker, pDestination, pLength, pErrorMessage, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const override {
    return ApplySubtract(pSource, pWorker, pDestination, pLength, pErrorMessage,
                         pMode);
  }

 private:
  static std::vector<unsigned char> BuildPasswordBytes(const unsigned char* pPassword,
                                                       int pPasswordLength) {
    if (pPassword == nullptr || pPasswordLength <= 0 || (pPasswordLength % 16) != 0) {
      return {};
    }
    return std::vector<unsigned char>(pPassword, pPassword + pPasswordLength);
  }

  static void SetError(std::string* pErrorMessage, const char* pMessage) {
    if (pErrorMessage != nullptr) {
      *pErrorMessage = pMessage;
    }
  }

  bool ValidateAndExpand(const unsigned char* pSource,
                         unsigned char* pWorker,
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
    if (pSource == nullptr || pWorker == nullptr || pDestination == nullptr) {
      SetError(pErrorMessage, "source/worker/destination must not be null");
      return false;
    }
    if (pSource == pDestination || pSource == pWorker || pDestination == pWorker) {
      SetError(pErrorMessage, "source/worker/destination must not alias");
      return false;
    }
    if (mPasswordBytes.empty()) {
      SetError(pErrorMessage, "password length must be a positive multiple of 16");
      return false;
    }
    const std::size_t aPasswordLength = mPasswordBytes.size();
    const std::size_t aFirst = std::min(aPasswordLength, pLength);
    std::memcpy(pWorker, mPasswordBytes.data(), aFirst);
    std::size_t aFilled = aFirst;
    while (aFilled < pLength) {
      const std::size_t aCopy = std::min(aFilled, pLength - aFilled);
      std::memcpy(pWorker + aFilled, pWorker, aCopy);
      aFilled += aCopy;
    }
    return true;
  }

  bool ApplyAdd(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const {
    if (!ValidateAndExpand(pSource, pWorker, pDestination, pLength, pErrorMessage)) {
      return pLength == 0u;
    }
    if (pLength == 0u) {
      return true;
    }
    switch (pMode) {
      case CryptMode::kNormal:
        for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
          pDestination[aIndex] =
              static_cast<unsigned char>(pSource[aIndex] + pWorker[aIndex]);
        }
        return true;
      case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
        {
          std::size_t aIndex = 0;
#if defined(__AVX2__)
          for (; aIndex + 32 <= pLength; aIndex += 32) {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(pDestination + aIndex),
                                _mm256_add_epi8(
                                    _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pSource + aIndex)),
                                    _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pWorker + aIndex))));
          }
#endif
          for (; aIndex + 16 <= pLength; aIndex += 16) {
            _mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination + aIndex),
                             _mm_add_epi8(
                                 _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource + aIndex)),
                                 _mm_loadu_si128(reinterpret_cast<const __m128i*>(pWorker + aIndex))));
          }
          for (; aIndex < pLength; ++aIndex) {
            pDestination[aIndex] =
                static_cast<unsigned char>(pSource[aIndex] + pWorker[aIndex]);
          }
        }
        return true;
#else
        SetError(pErrorMessage, "SIMD mode is not available");
        return false;
#endif
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        {
          std::size_t aIndex = 0;
          for (; aIndex + 16 <= pLength; aIndex += 16) {
            vst1q_u8(pDestination + aIndex,
                     vaddq_u8(vld1q_u8(pSource + aIndex), vld1q_u8(pWorker + aIndex)));
          }
          for (; aIndex < pLength; ++aIndex) {
            pDestination[aIndex] =
                static_cast<unsigned char>(pSource[aIndex] + pWorker[aIndex]);
          }
        }
        return true;
#else
        SetError(pErrorMessage, "NEON mode is not available");
        return false;
#endif
    }
    return false;
  }

  bool ApplySubtract(const unsigned char* pSource,
                     unsigned char* pWorker,
                     unsigned char* pDestination,
                     std::size_t pLength,
                     std::string* pErrorMessage,
                     CryptMode pMode) const {
    if (!ValidateAndExpand(pSource, pWorker, pDestination, pLength, pErrorMessage)) {
      return pLength == 0u;
    }
    if (pLength == 0u) {
      return true;
    }
    switch (pMode) {
      case CryptMode::kNormal:
        for (std::size_t aIndex = 0; aIndex < pLength; ++aIndex) {
          pDestination[aIndex] =
              static_cast<unsigned char>(pSource[aIndex] - pWorker[aIndex]);
        }
        return true;
      case CryptMode::kSimd:
#if defined(__AVX2__) || defined(__SSSE3__)
        {
          std::size_t aIndex = 0;
#if defined(__AVX2__)
          for (; aIndex + 32 <= pLength; aIndex += 32) {
            _mm256_storeu_si256(reinterpret_cast<__m256i*>(pDestination + aIndex),
                                _mm256_sub_epi8(
                                    _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pSource + aIndex)),
                                    _mm256_loadu_si256(reinterpret_cast<const __m256i*>(pWorker + aIndex))));
          }
#endif
          for (; aIndex + 16 <= pLength; aIndex += 16) {
            _mm_storeu_si128(reinterpret_cast<__m128i*>(pDestination + aIndex),
                             _mm_sub_epi8(
                                 _mm_loadu_si128(reinterpret_cast<const __m128i*>(pSource + aIndex)),
                                 _mm_loadu_si128(reinterpret_cast<const __m128i*>(pWorker + aIndex))));
          }
          for (; aIndex < pLength; ++aIndex) {
            pDestination[aIndex] =
                static_cast<unsigned char>(pSource[aIndex] - pWorker[aIndex]);
          }
        }
        return true;
#else
        SetError(pErrorMessage, "SIMD mode is not available");
        return false;
#endif
      case CryptMode::kNeon:
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
        {
          std::size_t aIndex = 0;
          for (; aIndex + 16 <= pLength; aIndex += 16) {
            vst1q_u8(pDestination + aIndex,
                     vsubq_u8(vld1q_u8(pSource + aIndex), vld1q_u8(pWorker + aIndex)));
          }
          for (; aIndex < pLength; ++aIndex) {
            pDestination[aIndex] =
                static_cast<unsigned char>(pSource[aIndex] - pWorker[aIndex]);
          }
        }
        return true;
#else
        SetError(pErrorMessage, "NEON mode is not available");
        return false;
#endif
    }
    return false;
  }

  std::vector<unsigned char> mPasswordBytes;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_PASSWORD_ADD_CIPHER_HPP_
