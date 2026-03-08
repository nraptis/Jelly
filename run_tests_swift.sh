#!/bin/sh
set -eu

aScriptDir="$(CDPATH= cd -- "$(dirname "$0")" && pwd)"
cd "$aScriptDir"

aSwiftTestDir="EncryptionTools-main/tests_swift"
aSupportFile="$aSwiftTestDir/TestSupport.swift"
aBuildDir="$aSwiftTestDir/.build"
aGeneratorFile="$aSwiftTestDir/GenerateAllCipherTests.swift"
aBinaryPath="$aBuildDir/GenerateAllCipherTests"

rm -rf "$aBuildDir"
find tests_swift -mindepth 2 -type f -name '*.dat' -delete
mkdir -p "$aBuildDir"

echo "Running $(basename "$aGeneratorFile")"
swiftc \
  "$aSupportFile" \
  EncryptionTools-main/EncryptionTools/Cipher.swift \
  EncryptionTools-main/EncryptionTools/Helpers/BlockHelper.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Rotation/RotateBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Rotation/RotateCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Rotation/RotateMaskBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Rotation/RotateMaskCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Splint/SplintBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Splint/SplintByteBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Splint/SplintCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Splint/SplintMaskBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Splint/SplintMaskByteBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Splint/SplintMaskCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Weave/WeaveBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Weave/WeaveByteBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Weave/WeaveCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Weave/WeaveMaskBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Weave/WeaveMaskByteBlockCipher.swift \
  EncryptionTools-main/EncryptionTools/Ciphers/Weave/WeaveMaskCipher.swift \
  "$aGeneratorFile" \
  -o "$aBinaryPath"
"$aBinaryPath"

echo "Swift generators passed: 1"
echo "Swift generators failed: 0"
