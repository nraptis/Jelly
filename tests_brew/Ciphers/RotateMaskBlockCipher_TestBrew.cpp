#include <string>

#include "../../src/PeanutButter.hpp"
#include "../../src/Encryption/Ciphers/Rotation/RotateMaskBlockCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {
template <std::size_t tBlockSize>
int RunCase() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Block(
      [](int, std::size_t pCaseIndex, peanutbutter::CryptMode) {
        return std::make_unique<peanutbutter::RotateMaskBlockCipher<tBlockSize>>(
            peanutbutter::test_brew::CaseMask(pCaseIndex, 0x105u),
            peanutbutter::test_brew::CaseShift(pCaseIndex, 0x106u));
      },
      static_cast<int>(tBlockSize), "RotateMaskBlockCipher", &aError);
  return peanutbutter::test_brew::ReportBlockResult("RotateMaskBlockCipher",
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
