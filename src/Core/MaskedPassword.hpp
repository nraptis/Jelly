#ifndef JELLY_CORE_MASKED_PASSWORD_HPP_
#define JELLY_CORE_MASKED_PASSWORD_HPP_

#include <cstddef>

namespace jelly {

bool MaskPasswordAray(unsigned char* pBytes,
                      const unsigned char* pPassword,
                      unsigned char* pWorker,
                      unsigned char* pMask,
                      std::size_t pLength);

bool MaskedPassword(unsigned char* pBytes,
                    const unsigned char* pPassword,
                    unsigned char* pWorker,
                    unsigned char pMask,
                    std::size_t pLength);

}  // namespace jelly

#endif  // JELLY_CORE_MASKED_PASSWORD_HPP_
