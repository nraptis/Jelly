#ifndef JELLY_TESTS_SWIFT_TEST_DATA_HPP_
#define JELLY_TESTS_SWIFT_TEST_DATA_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace swift_test_data {

enum class ParamType : std::uint8_t {
  kUInt8 = 1,
  kUInt16 = 2,
  kUInt32 = 3,
  kInt16 = 4,
  kInt32 = 5,
  kBytes = 6,
};

struct Param {
  std::string mName;
  ParamType mType;
  std::vector<unsigned char> mData;
};

struct TestCase {
  std::vector<Param> mParams;
  std::vector<unsigned char> mInput;
  std::vector<unsigned char> mExpected;
};

struct TestFile {
  std::string mCipherName;
  std::vector<TestCase> mCases;
};

bool LoadFile(const std::string& pPath,
              TestFile& pFile,
              std::string& pError);

const Param* FindParam(const TestCase& pCase, const char* pName);
bool ReadUInt8(const Param& pParam, std::uint8_t& pValue);
bool ReadUInt16(const Param& pParam, std::uint16_t& pValue);
bool ReadInt16(const Param& pParam, std::int16_t& pValue);
bool ReadUInt32(const Param& pParam, std::uint32_t& pValue);
bool ReadInt32(const Param& pParam, std::int32_t& pValue);

}  // namespace swift_test_data

#endif
