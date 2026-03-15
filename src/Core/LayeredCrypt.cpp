#include "LayeredCrypt.hpp"

#include <vector>

#include "../Jelly.hpp"

namespace jelly {

bool LayeredCrypt::ValidateTopLevel(const unsigned char* pSource,
                                    unsigned char* pWorker,
                                    unsigned char* pDestination,
                                    std::size_t pLength) {
  if (pLength == 0) {
    return true;
  }
  if (pLength != SB_L3_LENGTH) {
    return false;
  }
  if (pSource == nullptr || pWorker == nullptr || pDestination == nullptr) {
    return false;
  }
  if (pSource == pDestination || pSource == pWorker || pDestination == pWorker) {
    return false;
  }
  return true;
}

bool LayeredCrypt::SealData(const unsigned char* pSource,
                            unsigned char* pWorker,
                            unsigned char* pDestination,
                            std::size_t pLength,
                            CryptMode pMode) const {
  if (!ValidateTopLevel(pSource, pWorker, pDestination, pLength)) {
    return pLength == 0;
  }
  if (pLength == 0) {
    return true;
  }

  std::vector<unsigned char> aStageWorker(pLength);

  for (std::size_t aIndex = 0; aIndex < 4; ++aIndex) {
    const std::size_t aOffset = SB_L1_LENGTH * aIndex;
    if (!mLayer1.SealData(pSource + aOffset, pWorker + aOffset,
                          pDestination + aOffset, SB_L1_LENGTH, pMode)) {
      return false;
    }
  }

  for (std::size_t aIndex = 0; aIndex < 2; ++aIndex) {
    const std::size_t aOffset = SB_L2_LENGTH * aIndex;
    if (!mLayer2.SealData(pDestination + aOffset, aStageWorker.data() + aOffset,
                          pWorker + aOffset, SB_L2_LENGTH, pMode)) {
      return false;
    }
  }

  if (!mLayer3.SealData(pWorker, aStageWorker.data(), pDestination, SB_L3_LENGTH,
                        pMode)) {
    return false;
  }

  return true;
}

bool LayeredCrypt::UnsealData(const unsigned char* pSource,
                              unsigned char* pWorker,
                              unsigned char* pDestination,
                              std::size_t pLength,
                              CryptMode pMode) const {
  if (!ValidateTopLevel(pSource, pWorker, pDestination, pLength)) {
    return pLength == 0;
  }
  if (pLength == 0) {
    return true;
  }

  std::vector<unsigned char> aStageWorker(pLength);

  if (!mLayer3.UnsealData(pSource, pWorker, pDestination, SB_L3_LENGTH, pMode)) {
    return false;
  }

  for (std::size_t aIndex = 0; aIndex < 2; ++aIndex) {
    const std::size_t aOffset = SB_L2_LENGTH * aIndex;
    if (!mLayer2.UnsealData(pDestination + aOffset, aStageWorker.data() + aOffset,
                            pWorker + aOffset, SB_L2_LENGTH, pMode)) {
      return false;
    }
  }

  for (std::size_t aIndex = 0; aIndex < 4; ++aIndex) {
    const std::size_t aOffset = SB_L1_LENGTH * aIndex;
    if (!mLayer1.UnsealData(pWorker + aOffset, aStageWorker.data() + aOffset,
                            pDestination + aOffset, SB_L1_LENGTH, pMode)) {
      return false;
    }
  }

  return true;
}

}  // namespace jelly

