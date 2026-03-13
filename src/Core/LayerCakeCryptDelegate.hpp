#ifndef JELLY_CORE_LAYER_CAKE_CRYPT_DELEGATE_HPP_
#define JELLY_CORE_LAYER_CAKE_CRYPT_DELEGATE_HPP_

#include <cstddef>
#include <string>

#include "CryptMode.hpp"

namespace jelly {

class Crypt {
 public:
  virtual ~Crypt() = default;
  virtual bool SealData(const unsigned char* pSource,
                        unsigned char* pWorker,
                        unsigned char* pDestination,
                        std::size_t pLength,
                        std::string* pErrorMessage,
                        CryptMode pMode) const = 0;
  virtual bool UnsealData(const unsigned char* pSource,
                          unsigned char* pWorker,
                          unsigned char* pDestination,
                          std::size_t pLength,
                          std::string* pErrorMessage,
                          CryptMode pMode) const = 0;

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                CryptMode pMode) const {
    return SealData(pSource, pWorker, pDestination, pLength, nullptr, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  CryptMode pMode) const {
    return UnsealData(pSource, pWorker, pDestination, pLength, nullptr, pMode);
  }
};

class LayerCakeCryptDelegate : public Crypt {
 public:
  ~LayerCakeCryptDelegate() override = default;

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const final {
    if (pErrorMessage != nullptr) {
      pErrorMessage->clear();
    }
    return SealData(pSource, pWorker, pDestination, pLength, pMode);
  }

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const final {
    if (pErrorMessage != nullptr) {
      pErrorMessage->clear();
    }
    return UnsealData(pSource, pWorker, pDestination, pLength, pMode);
  }

  virtual bool SealData(const unsigned char* pSource,
                        unsigned char* pWorker,
                        unsigned char* pDestination,
                        std::size_t pLength,
                        CryptMode pMode) const = 0;
  virtual bool UnsealData(const unsigned char* pSource,
                          unsigned char* pWorker,
                          unsigned char* pDestination,
                          std::size_t pLength,
                          CryptMode pMode) const = 0;
};

}  // namespace jelly

#endif  // JELLY_CORE_LAYER_CAKE_CRYPT_DELEGATE_HPP_
