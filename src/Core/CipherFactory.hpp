#ifndef JELLY_CORE_CIPHER_FACTORY_HPP_
#define JELLY_CORE_CIPHER_FACTORY_HPP_

#include <cstddef>
#include <functional>
#include <memory>

#include "CryptMode.hpp"
#include "LayerCakeCryptDelegate.hpp"

namespace jelly {

using CipherFactory =
    std::function<std::unique_ptr<Crypt>(
        std::size_t pCaseIndex,
        CryptMode pMode)>;

using BlockCipherFactory =
    std::function<std::unique_ptr<Crypt>(
        int pBlockSize,
        std::size_t pCaseIndex,
        CryptMode pMode)>;

}  // namespace jelly

#endif  // JELLY_CORE_CIPHER_FACTORY_HPP_
