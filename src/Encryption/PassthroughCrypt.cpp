#include "PassthroughCrypt.hpp"

#include <cstring>

namespace peanutbutter {

bool PassthroughCrypt::SealData(const unsigned char* pSource,
                                unsigned char* pWorker,
                                unsigned char* pDestination,
                                std::size_t pLength,
                                std::string* pErrorMessage,
                                CryptMode pMode) const {
  (void)pWorker;
  (void)pMode;
  return Copy(pSource, pDestination, pLength, pErrorMessage);
}

bool PassthroughCrypt::UnsealData(const unsigned char* pSource,
                                  unsigned char* pWorker,
                                  unsigned char* pDestination,
                                  std::size_t pLength,
                                  std::string* pErrorMessage,
                                  CryptMode pMode) const {
  (void)pWorker;
  (void)pMode;
  return Copy(pSource, pDestination, pLength, pErrorMessage);
}

CryptDebugInfo PassthroughCrypt::GetDebugInfo() const {
  CryptDebugInfo aInfo;
  aInfo.mUsesEncryption = false;
  return aInfo;
}

bool PassthroughCrypt::Copy(const unsigned char* pSource,
                            unsigned char* pDestination,
                            std::size_t pLength,
                            std::string* pErrorMessage) {
  if (pLength == 0u) {
    return true;
  }
  if (pSource == nullptr || pDestination == nullptr) {
    if (pErrorMessage != nullptr) {
      *pErrorMessage = "PassthroughCrypt received a null buffer";
    }
    return false;
  }
  if (pSource != pDestination) {
    std::memcpy(pDestination, pSource, pLength);
  }
  return true;
}

}  // namespace peanutbutter
