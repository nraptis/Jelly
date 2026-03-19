#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../../src/Encryption/Ciphers/Password/PasswordSubtractCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {

std::vector<unsigned char> BuildPassword(std::size_t pCaseIndex) {
  const std::size_t aLength = 16u * (1u + (pCaseIndex % 4u));
  std::vector<unsigned char> aPassword(aLength);
  std::uint32_t aBits = peanutbutter::test_brew::CaseBits(pCaseIndex, 0x601u);
  for (std::size_t aIndex = 0; aIndex < aLength; ++aIndex) {
    aBits = (aBits * 1103515245u) + 12345u;
    aPassword[aIndex] = static_cast<unsigned char>((aBits >> 16) & 0xffu);
  }
  return aPassword;
}

}  // namespace

int main() {
  std::string aError;
  peanutbutter::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, peanutbutter::CryptMode)
          -> std::unique_ptr<peanutbutter::Crypt> {
        const std::vector<unsigned char> aPassword = BuildPassword(pCaseIndex);
        return std::make_unique<peanutbutter::PasswordSubtractCipher>(aPassword);
      },
      "PasswordSubtractCipher", &aError);
  return peanutbutter::test_brew::ReportFlatResult("PasswordSubtractCipher", aError);
}
