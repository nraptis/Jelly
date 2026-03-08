#ifndef JELLY_TESTS_SWIFT_TEST_SWIFT_EXECUTOR_HPP_
#define JELLY_TESTS_SWIFT_TEST_SWIFT_EXECUTOR_HPP_

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../src/Core/CryptMode.hpp"
#include "../src/Jelly.hpp"
#include "SwiftTestData.hpp"

namespace test_swift_executor {

template <typename tCipher>
bool ExecuteRoundTrip(tCipher& pCipher,
                      const std::vector<unsigned char>& pInput,
                      const std::vector<unsigned char>& pExpected,
                      const char* pPath,
                      std::size_t pIndex) {
  if (pInput.size() != jelly::SB_CIPHER_LENGTH_GRANULARITY) {
    std::cerr << pPath << ": bad input length at case " << pIndex << "\n";
    return false;
  }
  if (pExpected.size() != jelly::SB_CIPHER_LENGTH_GRANULARITY) {
    std::cerr << pPath << ": bad expected length at case " << pIndex << "\n";
    return false;
  }

  for (jelly::CryptMode aMode : jelly::GetAvailableCryptModes()) {
    std::vector<unsigned char> aEncrypted(pInput.size());
    std::vector<unsigned char> aRoundTrip(pInput.size());
    std::vector<unsigned char> aWorker(pInput.size());

    if (!pCipher.SealData(pInput.data(), aWorker.data(), aEncrypted.data(),
                          pInput.size(), aMode)) {
      std::cerr << pPath << ": SealData failed at case " << pIndex
                << " mode=" << jelly::GetCryptModeName(aMode) << "\n";
      return false;
    }
    if (aEncrypted != pExpected) {
      std::cerr << pPath << ": encrypt mismatch at case " << pIndex
                << " mode=" << jelly::GetCryptModeName(aMode) << "\n";
      return false;
    }
    if (!pCipher.UnsealData(aEncrypted.data(), aWorker.data(),
                            aRoundTrip.data(), aEncrypted.size(), aMode)) {
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
int RunFiles(int pArgc,
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

}  // namespace test_swift_executor

#endif
