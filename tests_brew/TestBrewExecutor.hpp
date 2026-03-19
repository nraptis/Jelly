#ifndef JELLY_TESTS_BREW_TEST_BREW_EXECUTOR_HPP_
#define JELLY_TESTS_BREW_TEST_BREW_EXECUTOR_HPP_

#include <cstddef>
#include <string>

#include "../src/Core/CipherFactory.hpp"

namespace peanutbutter {

void ExecuteTestBrew_Flat(const CipherFactory& pFactory,
                          const std::string& pName,
                          std::string* pError);

void ExecuteTestBrew_FlatSized(const SizedCipherFactory& pFactory,
                               const std::string& pName,
                               std::string* pError);

void ExecuteTestBrew_Block(const BlockCipherFactory& pFactory,
                           int pBlockSize,
                           const std::string& pName,
                           std::string* pError);

}  // namespace peanutbutter

#endif
