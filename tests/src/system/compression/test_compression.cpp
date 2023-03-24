
#include "headers.hpp"

TEST(system_compression, easy__can_compress_to_smaller_size_and_decompress_to_original) {
  constexpr int k_buffSize = 256;
  constexpr int k_offset = 0;

  auto smartBufferPrimary = std::make_unique<char[]>(k_buffSize);
  char* rawBufferPrimary = smartBufferPrimary.get();

  for (int ii = 0; ii < k_buffSize; ++ii) {
    rawBufferPrimary[ii] = char(ii % 10);
  }

  auto smartBufferSecondary = std::make_unique<char[]>(k_buffSize);
  char* rawBufferSecondary = smartBufferSecondary.get();
  std::memcpy(rawBufferSecondary, rawBufferPrimary, k_buffSize);

  int tmpBuffSize = k_buffSize;

  const bool success = gero::compression::AdaptiveHuffman::compress(rawBufferSecondary, tmpBuffSize, k_offset);

  ASSERT_EQ(success, true);
  ASSERT_LT(tmpBuffSize, k_buffSize);

  gero::compression::AdaptiveHuffman::decompress(rawBufferSecondary, tmpBuffSize, k_buffSize, k_offset);

  ASSERT_EQ(tmpBuffSize, k_buffSize);

  for (int ii = 0; ii < tmpBuffSize; ++ii) {
    ASSERT_EQ(rawBufferSecondary[ii], rawBufferPrimary[ii]);
  }
}

TEST(system_compression, difficult__can_compress_to_smaller_size_and_decompress_to_original) {
  constexpr int k_buffSize = 256;
  constexpr int k_offset = 0;

  auto smartBufferPrimary = std::make_unique<char[]>(k_buffSize);
  char* rawBufferPrimary = smartBufferPrimary.get();

  for (int ii = 0; ii < k_buffSize; ++ii) {
    rawBufferPrimary[ii] = char(ii % 50);
  }

  auto smartBufferSecondary = std::make_unique<char[]>(k_buffSize);
  char* rawBufferSecondary = smartBufferSecondary.get();
  std::memcpy(rawBufferSecondary, rawBufferPrimary, k_buffSize);

  int tmpBuffSize = k_buffSize;

  const bool success = gero::compression::AdaptiveHuffman::compress(rawBufferSecondary, tmpBuffSize, k_offset);

  ASSERT_EQ(success, true);
  ASSERT_LT(tmpBuffSize, k_buffSize);

  gero::compression::AdaptiveHuffman::decompress(rawBufferSecondary, tmpBuffSize, k_buffSize, k_offset);

  ASSERT_EQ(tmpBuffSize, k_buffSize);

  for (int ii = 0; ii < tmpBuffSize; ++ii) {
    ASSERT_EQ(rawBufferSecondary[ii], rawBufferPrimary[ii]);
  }
}

TEST(system_compression, can_fail_to_compress_and_just_return_false) {
  constexpr int k_buffSize = 256;
  constexpr int k_offset = 0;

  auto smartBufferPrimary = std::make_unique<char[]>(k_buffSize);
  char* rawBufferPrimary = smartBufferPrimary.get();

  for (int ii = 0; ii < k_buffSize; ++ii) {
    rawBufferPrimary[ii] = char(ii % 100);
  }

  auto smartBufferSecondary = std::make_unique<char[]>(k_buffSize);
  char* rawBufferSecondary = smartBufferSecondary.get();
  std::memcpy(rawBufferSecondary, rawBufferPrimary, k_buffSize);

  int tmpBuffSize = k_buffSize;

  const bool success = gero::compression::AdaptiveHuffman::compress(rawBufferSecondary, tmpBuffSize, k_offset);

  ASSERT_EQ(success, false);
  ASSERT_EQ(tmpBuffSize, k_buffSize);

  for (int ii = 0; ii < tmpBuffSize; ++ii) {
    ASSERT_EQ(rawBufferSecondary[ii], rawBufferPrimary[ii]);
  }
}
