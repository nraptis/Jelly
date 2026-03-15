#ifndef JELLY_CORE_ENCRYPTION_LAYER_HPP_
#define JELLY_CORE_ENCRYPTION_LAYER_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "LayerCakeCryptDelegate.hpp"

namespace jelly {

class EncryptionLayer final : public LayerCakeCryptDelegate {
 public:
  EncryptionLayer() = default;

  void AddCipher(std::unique_ptr<Crypt> pCipher);
  void ClearCiphers();
  std::size_t CipherCount() const;

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                CryptMode pMode) const override;

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  CryptMode pMode) const override;

 private:
  bool ApplyForward(const unsigned char* pSource,
                    unsigned char* pWorker,
                    unsigned char* pDestination,
                    std::size_t pLength,
                    CryptMode pMode) const;
  bool ApplyReverse(const unsigned char* pSource,
                    unsigned char* pWorker,
                    unsigned char* pDestination,
                    std::size_t pLength,
                    CryptMode pMode) const;

  std::vector<std::unique_ptr<Crypt>> mCiphers;
};

}  // namespace jelly

#endif  // JELLY_CORE_ENCRYPTION_LAYER_HPP_

