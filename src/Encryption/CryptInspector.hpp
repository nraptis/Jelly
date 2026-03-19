#ifndef PEANUTBUTTER_ENCRYPTION_CRYPT_INSPECTOR_HPP_
#define PEANUTBUTTER_ENCRYPTION_CRYPT_INSPECTOR_HPP_

#include <cstddef>
#include <vector>

namespace peanutbutter {

struct CryptDebugInfo {
  bool mUsesEncryption = false;
  std::vector<std::size_t> mLayerCipherCounts;
};

class CryptInspector {
 public:
  virtual ~CryptInspector() = default;
  virtual CryptDebugInfo GetDebugInfo() const = 0;
};

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_ENCRYPTION_CRYPT_INSPECTOR_HPP_
