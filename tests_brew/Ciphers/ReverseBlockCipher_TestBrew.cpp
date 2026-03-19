#include <string>

#include "../../src/Encryption/Ciphers/Reverse/ReverseBlockCipher.hpp"
#include "../../src/PeanutButter.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {

template <std::size_t tBlockSize>
int RunCase() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Block(
      [](int, std::size_t, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::ReverseBlockCipher<tBlockSize>>();
      },
      static_cast<int>(tBlockSize), "ReverseBlockCipher", &aError);
  return peanutbutter::test_brew::ReportBlockResult("ReverseBlockCipher",
                                             static_cast<int>(tBlockSize),
                                             aError);
}

}  // namespace

int main() {
  if (RunCase<8>() != 0) return 1;
  if (RunCase<12>() != 0) return 1;
  if (RunCase<16>() != 0) return 1;
  if (RunCase<24>() != 0) return 1;
  if (RunCase<32>() != 0) return 1;
  if (RunCase<48>() != 0) return 1;
  return 0;
}
