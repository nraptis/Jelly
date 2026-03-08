#ifndef JELLY_TESTS_SWIFT_TEST_FAMILY_SUPPORT_HPP_
#define JELLY_TESTS_SWIFT_TEST_FAMILY_SUPPORT_HPP_

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../src/Jelly.hpp"
#include "../src/Core/CryptMode.hpp"
#include "SwiftTestData.hpp"

namespace swift_test_family {

template <typename tCipher>
bool ExecuteRoundTripAllModes(tCipher& pCipher,
                              const std::vector<unsigned char>& pInput,
                              const std::vector<unsigned char>& pExpected,
                              const char* pPath,
                              std::size_t pIndex) {
  if ((pInput.size() % jelly::SB_CIPHER_LENGTH_GRANULARITY) != 0) {
    std::cerr << pPath << ": bad input length at case " << pIndex << "\n";
    return false;
  }
  for (jelly::CryptMode aMode : jelly::GetAvailableCryptModes()) {
    std::vector<unsigned char> aEncrypted(pInput.size());
    unsigned char aInputDummy = 0;
    unsigned char aEncryptedDummy = 0;
    const unsigned char* aInputPtr = pInput.empty() ? &aInputDummy : pInput.data();
    unsigned char* aEncryptedPtr =
        aEncrypted.empty() ? &aEncryptedDummy : aEncrypted.data();

    if (!pCipher.SealData(aInputPtr, aEncryptedPtr, pInput.size(), aMode)) {
      std::cerr << pPath << ": SealData failed at case " << pIndex
                << " mode=" << jelly::GetCryptModeName(aMode) << "\n";
      return false;
    }
    if (aEncrypted != pExpected) {
      std::cerr << pPath << ": encrypt mismatch at case " << pIndex
                << " mode=" << jelly::GetCryptModeName(aMode) << "\n";
      return false;
    }

    std::vector<unsigned char> aRoundTrip(aEncrypted.size());
    unsigned char aRoundTripDummy = 0;
    const unsigned char* aEncryptedReadPtr =
        aEncrypted.empty() ? &aEncryptedDummy : aEncrypted.data();
    unsigned char* aRoundTripPtr =
        aRoundTrip.empty() ? &aRoundTripDummy : aRoundTrip.data();
    if (!pCipher.UnsealData(aEncryptedReadPtr, aRoundTripPtr, aEncrypted.size(),
                            aMode)) {
      std::cerr << pPath << ": UnsealData failed at case " << pIndex
                << " mode=" << jelly::GetCryptModeName(aMode) << "\n";
      return false;
    }
    if (aRoundTrip != pInput) {
      std::cerr << pPath << ": round-trip mismatch at case " << pIndex
                << " mode=" << jelly::GetCryptModeName(aMode) << "\n";
      return false;
    }
  }
  return true;
}

template <typename tRunner>
int RunFamilyMain(int pArgc,
                  char** pArgv,
                  const char* pExpectedCipherName,
                  tRunner pRunner) {
  if (pArgc < 2) {
    std::cerr << "Expected one or more .dat files\n";
    return 1;
  }

  for (int aArgIndex = 1; aArgIndex < pArgc; ++aArgIndex) {
    swift_test_data::TestFile aFile;
    std::string aError;
    if (!swift_test_data::LoadFile(pArgv[aArgIndex], aFile, aError)) {
      std::cerr << pArgv[aArgIndex] << ": " << aError << "\n";
      return 1;
    }
    if (aFile.mCipherName != pExpectedCipherName) {
      std::cerr << pArgv[aArgIndex] << ": unexpected cipher family\n";
      return 1;
    }
    for (std::size_t aCaseIndex = 0; aCaseIndex < aFile.mCases.size();
         ++aCaseIndex) {
      if (!pRunner(aFile.mCases[aCaseIndex], aCaseIndex, pArgv[aArgIndex])) {
        return 1;
      }
    }
  }

  return 0;
}

}  // namespace swift_test_family

#endif
