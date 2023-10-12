#include "testutils.h"

#include <cstdlib>
#include <fstream>

namespace animately
{

std::string testDirectoryPath()
{
  char* value = std::getenv("TEST_DATA_DIR");
  return value ? std::string(value) : "";
}

std::vector<std::byte> loadFile(const std::string& file_path)
{
  std::ifstream input_file_stream(file_path, std::ios::binary | std::ios::ate);

  if (!input_file_stream)
  {
    return {};
  }

  auto end_position = input_file_stream.tellg();
  input_file_stream.seekg(0, std::ios::beg);

  auto buffer_size = std::size_t(end_position - input_file_stream.tellg());
  if (0 == buffer_size)
  {
    return {};
  }

  std::vector<std::byte> buffer(buffer_size);
  if (!input_file_stream.read(reinterpret_cast<char*>(buffer.data()),
                              static_cast<std::streamsize>(buffer.size())))
  {
    return {};
  }

  return buffer;
}

bool saveFile(const std::byte* buffer, size_t size,
              const std::string& file_path)
{
  std::ofstream output_file_stream(file_path,
                                   std::ios::binary | std::ios::trunc);

  if (!output_file_stream)
  {
    return false;
  }

  if (!output_file_stream.write(reinterpret_cast<const char*>(buffer),
                                static_cast<std::streamsize>(size)))
  {
    return false;
  }

  return true;
}

}  // namespace animately
