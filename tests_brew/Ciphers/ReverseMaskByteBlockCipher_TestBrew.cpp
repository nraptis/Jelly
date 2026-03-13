#include <string>

#include "../../src/Ciphers/Reverse/ReverseMaskByteBlockCipher.hpp"
#include "../../src/Jelly.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {

template <std::size_t tBlockSize>
int RunCase() {
  std::string aError;
  jelly::ExecuteTestBrew_Block(
      [](int, std::size_t pCaseIndex, jelly::CryptMode) {
        return std::make_unique<jelly::ReverseMaskByteBlockCipher<tBlockSize>>(
            jelly::test_brew::CaseMask(pCaseIndex, 0x403u));
      },
      static_cast<int>(tBlockSize), "ReverseMaskByteBlockCipher", &aError);
  return jelly::test_brew::ReportBlockResult("ReverseMaskByteBlockCipher",
                                             static_cast<int>(tBlockSize),
                                             aError);
}

}  // namespace

int main() {
  if (RunCase<jelly::EB_BLOCK_SIZE_08>() != 0) return 1;
  if (RunCase<jelly::EB_BLOCK_SIZE_12>() != 0) return 1;
  if (RunCase<jelly::EB_BLOCK_SIZE_16>() != 0) return 1;
  if (RunCase<jelly::EB_BLOCK_SIZE_24>() != 0) return 1;
  if (RunCase<jelly::EB_BLOCK_SIZE_32>() != 0) return 1;
  if (RunCase<jelly::EB_BLOCK_SIZE_48>() != 0) return 1;
  return 0;
}
