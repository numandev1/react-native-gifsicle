#pragma once

#include <string>
#include <vector>

namespace animately
{

std::string testDirectoryPath();

std::vector<std::byte> loadFile(const std::string& file_path);

bool saveFile(const std::byte* buffer, size_t size,
              const std::string& file_path);

}
