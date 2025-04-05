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

inline size_t read(binary_data_writer& dataWriter, std::fstream& outputFile)
{
  size_t maxBytesRead = dataWriter.size - dataWriter.position;
  outputFile.read(dataWriter.data, maxBytesRead);
  size_t bytesRead = outputFile ? maxBytesRead : outputFile.gcount();
  dataWriter.position += bytesRead;
  return bytesRead;
}

inline size_t write(std::fstream& outputFile, const binary_data& data)
{
  outputFile.write(data.data, data.size);
  return data.size;
}
