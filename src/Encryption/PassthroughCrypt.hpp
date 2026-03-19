#ifndef PEANUTBUTTER_ENCRYPTION_PASSTHROUGH_CRYPT_HPP_
#define PEANUTBUTTER_ENCRYPTION_PASSTHROUGH_CRYPT_HPP_

#include <cstddef>
#include <string>

#include "Crypt.hpp"
#include "CryptInspector.hpp"

namespace peanutbutter {

class PassthroughCrypt final : public Crypt, public CryptInspector {
 public:
  PassthroughCrypt() = default;

  bool SealData(const unsigned char* pSource,
                unsigned char* pWorker,
                unsigned char* pDestination,
                std::size_t pLength,
                std::string* pErrorMessage,
                CryptMode pMode) const override;

  bool UnsealData(const unsigned char* pSource,
                  unsigned char* pWorker,
                  unsigned char* pDestination,
                  std::size_t pLength,
                  std::string* pErrorMessage,
                  CryptMode pMode) const override;

  CryptDebugInfo GetDebugInfo() const override;

 private:
  static bool Copy(const unsigned char* pSource,
                   unsigned char* pDestination,
                   std::size_t pLength,
                   std::string* pErrorMessage);
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_ENCRYPTION_PASSTHROUGH_CRYPT_HPP_
