#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace {

int RunOne(const std::filesystem::path& pDirectory, const char* pName) {
  const std::filesystem::path aPath = pDirectory / pName;
  std::cout << "Running " << pName << "\n";
  const std::string aCommand = "\"" + aPath.string() + "\"";
  const int aCode = std::system(aCommand.c_str());
  if (aCode != 0) {
    std::cerr << "FAIL " << pName << "\n";
    return 1;
  }
  std::cout << "PASS " << pName << "\n";
  return 0;
}

}  // namespace

int main(int pArgc, char** pArgv) {
  const std::filesystem::path aSelf =
      pArgc > 0 ? std::filesystem::path(pArgv[0]).parent_path() : ".";
  const std::vector<const char*> aTargets = {
      "BenchmarkRun_RotateCipher",
      "BenchmarkRun_RotateBlockCipher",
      "BenchmarkRun_RotateBlockByteCipher",
      "BenchmarkRun_RotateMaskCipher",
      "BenchmarkRun_RotateMaskBlockCipher",
      "BenchmarkRun_RotateMaskByteBlockCipher",
      "BenchmarkRun_SplintCipher",
      "BenchmarkRun_SplintBlockCipher",
      "BenchmarkRun_SplintByteBlockCipher",
      "BenchmarkRun_SplintMaskCipher",
      "BenchmarkRun_SplintMaskBlockCipher",
      "BenchmarkRun_SplintMaskByteBlockCipher",
      "BenchmarkRun_WeaveCipher",
      "BenchmarkRun_WeaveBlockCipher",
      "BenchmarkRun_WeaveByteBlockCipher",
      "BenchmarkRun_WeaveMaskCipher",
      "BenchmarkRun_WeaveMaskBlockCipher",
      "BenchmarkRun_WeaveMaskByteBlockCipher",
      "BenchmarkRun_ReverseCipher",
      "BenchmarkRun_ReverseBlockCipher",
      "BenchmarkRun_ReverseBlockByteCipher",
      "BenchmarkRun_ReverseMaskCipher",
      "BenchmarkRun_ReverseMaskBlockCipher",
      "BenchmarkRun_ReverseMaskByteBlockCipher",
      "BenchmarkRun_PasswordCipher",
      "BenchmarkRun_RippleCipher",
      "BenchmarkRun_RippleBlockCipher",
      "BenchmarkRun_RippleMaskCipher",
      "BenchmarkRun_RippleMaskBlockCipher",
      "BenchmarkRun_InvertCipher",
      "BenchmarkRun_InvertMaskCipher",
      "BenchmarkRun_PepperCipherXOR",
      "BenchmarkRun_PepperCipherXORNoise",
      "BenchmarkRun_SwapGridVV16",
      "BenchmarkRun_SwapGridVH16",
      "BenchmarkRun_SwapGridHV16",
      "BenchmarkRun_SwapGridHH16",
      "BenchmarkRun_SwapGridVV64",
      "BenchmarkRun_SwapGridVH64",
      "BenchmarkRun_SwapGridHV64",
      "BenchmarkRun_SwapGridHH64",
      "BenchmarkRun_SwapGridMaskVV16",
      "BenchmarkRun_SwapGridMaskVH16",
      "BenchmarkRun_SwapGridMaskHV16",
      "BenchmarkRun_SwapGridMaskHH16",
      "BenchmarkRun_SwapGridMaskVV64",
      "BenchmarkRun_SwapGridMaskVH64",
      "BenchmarkRun_SwapGridMaskHV64",
      "BenchmarkRun_SwapGridMaskHH64",
      "BenchmarkRun_SpiralGridH16",
      "BenchmarkRun_SpiralGridV16",
      "BenchmarkRun_SpiralGridH64",
      "BenchmarkRun_SpiralGridV64",
      "BenchmarkRun_SpiralGridMaskH16",
      "BenchmarkRun_SpiralGridMaskV16",
      "BenchmarkRun_SpiralGridMaskH64",
      "BenchmarkRun_SpiralGridMaskV64",
  };

  for (const char* aTarget : aTargets) {
    if (RunOne(aSelf, aTarget) != 0) {
      return 1;
    }
  }
  return 0;
}
