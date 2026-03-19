#!/bin/sh
set -eu

aScriptDir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
cd "$aScriptDir"

aBuildDir="build"
aSwiftBuildDir="EncryptionTools-main/tests_swift/.build"
aPassCount=0
aFailCount=0

aTargets="
RotateCipher_TestBrew
RotateBlockCipher_TestBrew
RotateBlockByteCipher_TestBrew
RotateMaskCipher_TestBrew
RotateMaskBlockCipher_TestBrew
RotateMaskByteBlockCipher_TestBrew
SplintCipher_TestBrew
SplintBlockCipher_TestBrew
SplintByteBlockCipher_TestBrew
SplintMaskCipher_TestBrew
SplintMaskBlockCipher_TestBrew
SplintMaskByteBlockCipher_TestBrew
WeaveCipher_TestBrew
WeaveBlockCipher_TestBrew
WeaveByteBlockCipher_TestBrew
WeaveMaskCipher_TestBrew
WeaveMaskBlockCipher_TestBrew
WeaveMaskByteBlockCipher_TestBrew
ReverseCipher_TestBrew
ReverseBlockCipher_TestBrew
ReverseBlockByteCipher_TestBrew
ReverseMaskCipher_TestBrew
ReverseMaskBlockCipher_TestBrew
ReverseMaskByteBlockCipher_TestBrew
PasswordAddCipher_TestBrew
PasswordSubtractCipher_TestBrew
PasswordXORCipher_TestBrew
PasswordJumpAddCipher_TestBrew
PasswordJumpSubtractCipher_TestBrew
PasswordJumpXORCipher_TestBrew
RippleCipher_TestBrew
RippleBlockCipher_TestBrew
RippleMaskCipher_TestBrew
RippleMaskBlockCipher_TestBrew
InvertCipher_TestBrew
InvertMaskCipher_TestBrew
PepperAddCipher_TestBrew
PepperSubtractCipher_TestBrew
PepperXORCipher_TestBrew
PepperJumpXORCipher_TestBrew
PepperNoiseXORCipher_TestBrew
PepperJumpNoiseXORCipher_TestBrew
PepperDualJumpNoiseXORCipher_TestBrew
CascadeXORCipher_TestBrew
CascadeJumpXORCipher_TestBrew
SwapGridVV16_TestBrew
SwapGridVH16_TestBrew
SwapGridHV16_TestBrew
SwapGridHH16_TestBrew
SwapGridVV64_TestBrew
SwapGridVH64_TestBrew
SwapGridHV64_TestBrew
SwapGridHH64_TestBrew
SwapGridMaskVV16_TestBrew
SwapGridMaskVH16_TestBrew
SwapGridMaskHV16_TestBrew
SwapGridMaskHH16_TestBrew
SwapGridMaskVV64_TestBrew
SwapGridMaskVH64_TestBrew
SwapGridMaskHV64_TestBrew
SwapGridMaskHH64_TestBrew
SpiralGridH16_TestBrew
SpiralGridV16_TestBrew
SpiralGridH64_TestBrew
SpiralGridV64_TestBrew
SpiralGridMaskH16_TestBrew
SpiralGridMaskV16_TestBrew
SpiralGridMaskH64_TestBrew
SpiralGridMaskV64_TestBrew
"

rm -rf "$aBuildDir"
rm -rf "$aSwiftBuildDir"

cmake -S . -B "$aBuildDir"
cmake --build "$aBuildDir"

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

echo "Test brew passed: $aPassCount"
echo "Test brew failed: $aFailCount"

if [ "$aFailCount" -ne 0 ]; then
  exit 1
fi
