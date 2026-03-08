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
RotateMaskCipher_TestBrew
RotateMaskBlockCipher_TestBrew
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
