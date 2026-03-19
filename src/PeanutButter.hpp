#ifndef PEANUTBUTTER_PEANUT_BUTTER_HPP_
#define PEANUTBUTTER_PEANUT_BUTTER_HPP_

#include <cstddef>
#include <cstdint>

namespace peanutbutter {

// !IMPORTANT: Every cipher entry point must reject any pLength that is not a
// multiple of BLOCK_GRANULARITY.
#define BLOCK_GRANULARITY 192u
#define ARCHIVE_HEADER_LENGTH 48u
#define RECOVERY_HEADER_LENGTH 48u
#define BLOCK_SIZE_L1 261120u
#define BLOCK_SIZE_L2 522240u
#define BLOCK_SIZE_L3 1044480u

inline constexpr std::size_t kBlockGranularity = BLOCK_GRANULARITY;
inline constexpr std::size_t kArchiveHeaderLength = ARCHIVE_HEADER_LENGTH;
inline constexpr std::size_t kRecoveryHeaderLength = RECOVERY_HEADER_LENGTH;
inline constexpr std::size_t kBlockSizeL1 = BLOCK_SIZE_L1;
inline constexpr std::size_t kBlockSizeL2 = BLOCK_SIZE_L2;
inline constexpr std::size_t kBlockSizeL3 = BLOCK_SIZE_L3;

inline constexpr std::uint32_t kMagicHeaderBytes = 0xDECAFBADu;
inline constexpr std::uint32_t kMagicFooterBytes = 0xF01DAB1Eu;
inline constexpr std::uint32_t kMajorVersion = 1u;
inline constexpr std::uint32_t kMinorVersion = 6u;

#ifndef ENCRYPTION_FORCE_SOFTWARE_MODE
#define ENCRYPTION_FORCE_SOFTWARE_MODE 1
#endif

inline constexpr bool PEANUTBUTTER_SOFTWARE_ONLY_MODE =
    (ENCRYPTION_FORCE_SOFTWARE_MODE != 0);

struct ArchiveHeader {
  std::uint32_t mMagic = kMagicHeaderBytes;
  std::uint16_t mVersionMajor =
      static_cast<std::uint16_t>(kMajorVersion & 0xFFFFu);
  std::uint16_t mVersionMinor =
      static_cast<std::uint16_t>(kMinorVersion & 0xFFFFu);
  std::uint32_t mArchiveIndex = 0;
  std::uint32_t mArchiveCount = 0;
  std::uint32_t mPayloadLength = 0;
  std::uint8_t mRecordCountMod256 = 0;
  std::uint8_t mFolderCountMod256 = 0;
  std::uint16_t mReserved16 = 0;
  std::uint64_t mReservedA = 0;
  std::uint64_t mReservedB = 0;
  std::uint64_t mArchiveFamilyId = 0;
};

struct Checksum {
  std::uint64_t mWord1 = 0;
  std::uint64_t mWord2 = 0;
  std::uint64_t mWord3 = 0;
  std::uint64_t mWord4 = 0;
  std::uint64_t mWord5 = 0;
};

struct SkipRecord {
  std::uint16_t mArchiveDistance = 0;
  std::uint16_t mBlockDistance = 0;
  std::uint32_t mByteDistance = 0;
};

struct RecoveryHeader {
  Checksum mChecksum{};
  SkipRecord mSkip{};
};

static_assert((BLOCK_SIZE_L1 % 48u) == 0u,
              "BLOCK_SIZE_L1 must be divisible by 48");
static_assert((BLOCK_SIZE_L1 % 8u) == 0u,
              "BLOCK_SIZE_L1 must be divisible by 8");
static_assert((BLOCK_SIZE_L1 % 12u) == 0u,
              "BLOCK_SIZE_L1 must be divisible by 12");
static_assert((BLOCK_SIZE_L1 % 16u) == 0u,
              "BLOCK_SIZE_L1 must be divisible by 16");
static_assert((BLOCK_SIZE_L1 % 24u) == 0u,
              "BLOCK_SIZE_L1 must be divisible by 24");
static_assert((BLOCK_SIZE_L1 % 32u) == 0u,
              "BLOCK_SIZE_L1 must be divisible by 32");
static_assert((BLOCK_SIZE_L1 % BLOCK_GRANULARITY) == 0u,
              "BLOCK_SIZE_L1 must be divisible by BLOCK_GRANULARITY");

}  // namespace peanutbutter

#endif  // PEANUTBUTTER_PEANUT_BUTTER_HPP_
