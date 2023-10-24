#pragma once

#include <string>
#include <vector>
#include <gifsicle.h>
#import <jsi/jsi.h>

using namespace std;
using namespace facebook;

namespace gifsicle_util
{

std::string testDirectoryPath();

std::vector<std::byte> loadFile(const std::string& file_path);

bool saveFile(const std::byte* buffer, size_t size,
              const std::string& file_path);

std::string getDestFilePath(string cacheDirStr);

GifOptions jsiObjectToGifOptions(jsi::Runtime& rt, const jsi::Object& object);

}
