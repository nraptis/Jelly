#!/bin/sh
set -eu

aScriptDir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
cd "$aScriptDir"

aBuildDir="build"
aSwiftBuildDir="EncryptionTools-main/tests_swift/.build"
aPassCount=0
aFailCount=0

aTargets="
BenchmarkRun_RotateCipher
BenchmarkRun_RotateBlockCipher
BenchmarkRun_RotateBlockByteCipher
BenchmarkRun_RotateMaskCipher
BenchmarkRun_RotateMaskBlockCipher
BenchmarkRun_RotateMaskByteBlockCipher
BenchmarkRun_SplintCipher
BenchmarkRun_SplintBlockCipher
BenchmarkRun_SplintByteBlockCipher
BenchmarkRun_SplintMaskCipher
BenchmarkRun_SplintMaskBlockCipher
BenchmarkRun_SplintMaskByteBlockCipher
BenchmarkRun_WeaveCipher
BenchmarkRun_WeaveBlockCipher
BenchmarkRun_WeaveByteBlockCipher
BenchmarkRun_WeaveMaskCipher
BenchmarkRun_WeaveMaskBlockCipher
BenchmarkRun_WeaveMaskByteBlockCipher
BenchmarkRun_ReverseCipher
BenchmarkRun_ReverseBlockCipher
BenchmarkRun_ReverseBlockByteCipher
BenchmarkRun_ReverseMaskCipher
BenchmarkRun_ReverseMaskBlockCipher
BenchmarkRun_ReverseMaskByteBlockCipher
BenchmarkRun_PasswordCipher
BenchmarkRun_RippleCipher
BenchmarkRun_RippleBlockCipher
BenchmarkRun_RippleMaskCipher
BenchmarkRun_RippleMaskBlockCipher
BenchmarkRun_InvertCipher
BenchmarkRun_InvertMaskCipher
BenchmarkRun_SwapGridVV16
BenchmarkRun_SwapGridVH16
BenchmarkRun_SwapGridHV16
BenchmarkRun_SwapGridHH16
BenchmarkRun_SwapGridVV64
BenchmarkRun_SwapGridVH64
BenchmarkRun_SwapGridHV64
BenchmarkRun_SwapGridHH64
BenchmarkRun_SwapGridMaskVV16
BenchmarkRun_SwapGridMaskVH16
BenchmarkRun_SwapGridMaskHV16
BenchmarkRun_SwapGridMaskHH16
BenchmarkRun_SwapGridMaskVV64
BenchmarkRun_SwapGridMaskVH64
BenchmarkRun_SwapGridMaskHV64
BenchmarkRun_SwapGridMaskHH64
BenchmarkRun_SpiralGridH16
BenchmarkRun_SpiralGridV16
BenchmarkRun_SpiralGridH64
BenchmarkRun_SpiralGridV64
BenchmarkRun_SpiralGridMaskH16
BenchmarkRun_SpiralGridMaskV16
BenchmarkRun_SpiralGridMaskH64
BenchmarkRun_SpiralGridMaskV64
"

rm -rf "$aBuildDir"
rm -rf "$aSwiftBuildDir"
rm -rf BENCHMARK_LOGS
mkdir -p BENCHMARK_LOGS

cmake -S . -B "$aBuildDir" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -DNDEBUG"
cmake --build "$aBuildDir" --config Release -j4

for aTarget in $aTargets; do
  echo "Running $aTarget"
  if "$aBuildDir/$aTarget"; then
    echo "PASS $aTarget"
    aPassCount=$((aPassCount + 1))
  else
    echo "FAIL $aTarget"
    aFailCount=$((aFailCount + 1))
  fi
done

echo "Benchmark runs passed: $aPassCount"
echo "Benchmark runs failed: $aFailCount"

if [ "$aFailCount" -ne 0 ]; then
  exit 1
fi
