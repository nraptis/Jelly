#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../../src/Ciphers/Password/PasswordCipher.hpp"
#include "../TestBrewExecutor.hpp"
#include "TestBrewCipherSupport.hpp"

namespace {

std::vector<unsigned char> BuildPassword(std::size_t pCaseIndex) {
  const std::size_t aLength = 1 + (pCaseIndex % 31);
  std::vector<unsigned char> aPassword(aLength);
  std::uint32_t aBits = jelly::test_brew::CaseBits(pCaseIndex, 0x501u);
  for (std::size_t aIndex = 0; aIndex < aLength; ++aIndex) {
    aBits = (aBits * 1103515245u) + 12345u;
    aPassword[aIndex] = static_cast<unsigned char>((aBits >> 16) & 0xffu);
  }
  return aPassword;
}

}  // namespace

int main() {
  std::string aError;
  jelly::ExecuteTestBrew_Flat(
      [](std::size_t pCaseIndex, jelly::CryptMode) -> std::unique_ptr<jelly::Crypt> {
        return std::make_unique<jelly::PasswordCipher>(BuildPassword(pCaseIndex));
      },
      "PasswordCipher", &aError);
  return jelly::test_brew::ReportFlatResult("PasswordCipher", aError);
}
