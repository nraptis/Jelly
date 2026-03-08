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
      "BenchmarkRun_RotateMaskCipher",
      "BenchmarkRun_RotateMaskBlockCipher",
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
  };

  for (const char* aTarget : aTargets) {
    if (RunOne(aSelf, aTarget) != 0) {
      return 1;
    }
  }
  return 0;
}
