#include "SwiftTestData.hpp"

#include <fstream>

namespace swift_test_data {

namespace {

std::uint32_t ReadU32(std::ifstream& pStream) {
  std::uint32_t aValue = 0;
  pStream.read(reinterpret_cast<char*>(&aValue), sizeof(aValue));
  return aValue;
}

std::uint16_t ReadU16(std::ifstream& pStream) {
  std::uint16_t aValue = 0;
  pStream.read(reinterpret_cast<char*>(&aValue), sizeof(aValue));
  return aValue;
}

std::vector<unsigned char> ReadBytes(std::ifstream& pStream,
                                     std::size_t pCount) {
  std::vector<unsigned char> aBytes(pCount);
  if (pCount > 0) {
    pStream.read(reinterpret_cast<char*>(aBytes.data()),
                 static_cast<std::streamsize>(pCount));
  }
  return aBytes;
}

bool ReadExact(std::ifstream& pStream, char* pBytes, std::size_t pCount) {
  pStream.read(pBytes, static_cast<std::streamsize>(pCount));
  return static_cast<std::size_t>(pStream.gcount()) == pCount;
}

bool HasSize(const Param& pParam, std::size_t pExpected) {
  return pParam.mData.size() == pExpected;
}

}  // namespace

bool LoadFile(const std::string& pPath,
              TestFile& pFile,
              std::string& pError) {
  std::ifstream aStream(pPath, std::ios::binary);
  if (!aStream) {
    pError = "Could not open file";
    return false;
  }

  char aMagic[4] = {};
  if (!ReadExact(aStream, aMagic, sizeof(aMagic)) ||
      aMagic[0] != 'J' || aMagic[1] != 'L' ||
      aMagic[2] != 'Y' || aMagic[3] != 'T') {
    pError = "Bad magic";
    return false;
  }

  const std::uint32_t aVersion = ReadU32(aStream);
  if (aVersion != 2) {
    pError = "Bad version";
    return false;
  }

  const std::uint32_t aCipherNameLength = ReadU32(aStream);
  const std::vector<unsigned char> aCipherNameBytes =
      ReadBytes(aStream, aCipherNameLength);
  pFile.mCipherName.assign(aCipherNameBytes.begin(), aCipherNameBytes.end());

  const std::uint32_t aCaseCount = ReadU32(aStream);
  pFile.mCases.clear();
  pFile.mCases.reserve(aCaseCount);

  for (std::uint32_t aCaseIndex = 0; aCaseIndex < aCaseCount; ++aCaseIndex) {
    TestCase aCase;
    const std::uint32_t aParamCount = ReadU32(aStream);
    const std::uint32_t aInputCount = ReadU32(aStream);
    const std::uint32_t aExpectedCount = ReadU32(aStream);
    aCase.mParams.reserve(aParamCount);

    for (std::uint32_t aParamIndex = 0; aParamIndex < aParamCount; ++aParamIndex) {
      char aTypeByte = 0;
      if (!ReadExact(aStream, &aTypeByte, 1)) {
        pError = "Truncated param type";
        return false;
      }
      const std::uint16_t aNameLength = ReadU16(aStream);
      const std::uint32_t aDataLength = ReadU32(aStream);
      const std::vector<unsigned char> aNameBytes = ReadBytes(aStream, aNameLength);
      Param aParam;
      aParam.mType = static_cast<ParamType>(static_cast<std::uint8_t>(aTypeByte));
      aParam.mName.assign(aNameBytes.begin(), aNameBytes.end());
      aParam.mData = ReadBytes(aStream, aDataLength);
      aCase.mParams.push_back(std::move(aParam));
    }

    aCase.mInput = ReadBytes(aStream, aInputCount);
    aCase.mExpected = ReadBytes(aStream, aExpectedCount);
    pFile.mCases.push_back(std::move(aCase));
  }

  return true;
}

const Param* FindParam(const TestCase& pCase, const char* pName) {
  for (const Param& aParam : pCase.mParams) {
    if (aParam.mName == pName) {
      return &aParam;
    }
  }
  return nullptr;
}

bool ReadUInt8(const Param& pParam, std::uint8_t& pValue) {
  if (pParam.mType != ParamType::kUInt8 || !HasSize(pParam, 1)) {
    return false;
  }
  pValue = pParam.mData[0];
  return true;
}

bool ReadUInt16(const Param& pParam, std::uint16_t& pValue) {
  if (pParam.mType != ParamType::kUInt16 || !HasSize(pParam, 2)) {
    return false;
  }
  pValue = static_cast<std::uint16_t>(pParam.mData[0]) |
           (static_cast<std::uint16_t>(pParam.mData[1]) << 8);
  return true;
}

bool ReadInt16(const Param& pParam, std::int16_t& pValue) {
  if (pParam.mType != ParamType::kInt16 || !HasSize(pParam, 2)) {
    return false;
  }
  const std::uint16_t aValue = static_cast<std::uint16_t>(pParam.mData[0]) |
                               (static_cast<std::uint16_t>(pParam.mData[1]) << 8);
  pValue = static_cast<std::int16_t>(aValue);
  return true;
}

bool ReadUInt32(const Param& pParam, std::uint32_t& pValue) {
  if (pParam.mType != ParamType::kUInt32 || !HasSize(pParam, 4)) {
    return false;
  }
  pValue = static_cast<std::uint32_t>(pParam.mData[0]) |
           (static_cast<std::uint32_t>(pParam.mData[1]) << 8) |
           (static_cast<std::uint32_t>(pParam.mData[2]) << 16) |
           (static_cast<std::uint32_t>(pParam.mData[3]) << 24);
  return true;
}

bool ReadInt32(const Param& pParam, std::int32_t& pValue) {
  if (pParam.mType != ParamType::kInt32 || !HasSize(pParam, 4)) {
    return false;
  }
  const std::uint32_t aValue = static_cast<std::uint32_t>(pParam.mData[0]) |
                               (static_cast<std::uint32_t>(pParam.mData[1]) << 8) |
                               (static_cast<std::uint32_t>(pParam.mData[2]) << 16) |
                               (static_cast<std::uint32_t>(pParam.mData[3]) << 24);
  pValue = static_cast<std::int32_t>(aValue);
  return true;
}

}  // namespace swift_test_data
