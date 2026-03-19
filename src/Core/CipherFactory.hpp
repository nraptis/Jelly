#ifndef JELLY_CORE_CIPHER_FACTORY_HPP_
#define JELLY_CORE_CIPHER_FACTORY_HPP_

#include <cstddef>
#include <functional>
#include <memory>

#include "../Core/CryptMode.hpp"
#include "../Encryption/Crypt.hpp"

namespace peanutbutter {

using CipherFactory =
    std::function<std::unique_ptr<Crypt>(
        std::size_t pCaseIndex,
        CryptMode pMode)>;

using SizedCipherFactory =
    std::function<std::unique_ptr<Crypt>(
        std::size_t pLength,
        std::size_t pCaseIndex,
        CryptMode pMode)>;

using BlockCipherFactory =
    std::function<std::unique_ptr<Crypt>(
        int pBlockSize,
        std::size_t pCaseIndex,
        CryptMode pMode)>;

}  // namespace peanutbutter

#endif  // JELLY_CORE_CIPHER_FACTORY_HPP_
