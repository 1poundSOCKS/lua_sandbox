#include "pch.h"

struct binary_data
{
  size_t size { 0 };
  char* data { nullptr };
};

struct binary_data_owner
{
  static constexpr size_t size = 0x100000;
  std::unique_ptr<char[]> data = std::make_unique<char[]>(size);
};

struct binary_data_writer
{
  size_t size { 0 };
  char* data { nullptr };
  size_t position { 0 };
};

inline size_t append(binary_data_writer& dest, const binary_data& source)
{
  size_t remainingBytes = dest.size - dest.position;
  size_t bytesToAppend = std::min(source.size, remainingBytes);
  ::memcpy(dest.data, source.data, bytesToAppend);
  dest.position += bytesToAppend;
  return bytesToAppend;
}
