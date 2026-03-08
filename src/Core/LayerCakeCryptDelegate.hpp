#ifndef JELLY_CORE_LAYER_CAKE_CRYPT_DELEGATE_HPP_
#define JELLY_CORE_LAYER_CAKE_CRYPT_DELEGATE_HPP_

#include <cstddef>

#include "CryptMode.hpp"

namespace jelly {

class LayerCakeCryptDelegate {
 public:
  virtual ~LayerCakeCryptDelegate() = default;
  virtual bool SealData(const unsigned char* pSource,
    const unsigned char* pWorker,
                        unsigned char* pDestination,
                        std::size_t pLength,
                        CryptMode pMode) = 0;
  virtual bool UnsealData(const unsigned char* pSource,
    const unsigned char* pWorker,
                          unsigned char* pDestination,
                          std::size_t pLength,
                          CryptMode pMode) = 0;
};

}  // namespace jelly

#endif  // JELLY_CORE_LAYER_CAKE_CRYPT_DELEGATE_HPP_
