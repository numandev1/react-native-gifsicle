#include "gifsicleutil.h"

#include <cstdlib>
#include <fstream>
#include <string.h>
#include <random>
#include <iomanip>
#include <sstream>

namespace gifsicle_util
{

  std::string testDirectoryPath()
  {
    char *value = std::getenv("TEST_DATA_DIR");
    return value ? std::string(value) : "";
  }

  std::vector<std::byte> loadFile(const std::string &file_path)
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
    if (!input_file_stream.read(reinterpret_cast<char *>(buffer.data()),
                                static_cast<std::streamsize>(buffer.size())))
    {
      return {};
    }

    return buffer;
  }

  bool saveFile(const std::byte *buffer, size_t size,
                const std::string &file_path)
  {
    std::ofstream output_file_stream(file_path,
                                     std::ios::binary | std::ios::trunc);

    if (!output_file_stream)
    {
      return false;
    }

    if (!output_file_stream.write(reinterpret_cast<const char *>(buffer),
                                  static_cast<std::streamsize>(size)))
    {
      return false;
    }

    return true;
  }

    std::string generate_uuid() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        std::stringstream ss;
        for (int i = 0; i < 32; ++i) {
            ss << std::hex << dis(gen);
        }
        return ss.str();
    }

  std::string getDestFilePath(string cacheDirStr)
  {
    string imageNameWithoutExtension = generate_uuid();
    string imageName = imageNameWithoutExtension + ".gif";
    string destFilePath = cacheDirStr+"/"+ imageName;
    return destFilePath;
  }

  GifOptions jsiObjectToGifOptions(jsi::Runtime &rt, const jsi::Object &object)
  {
    GifOptions options;

    // Set default values
    options.lossy = 200;
    options.optimize = 3;
    options.colors = 255;
    options.scale_x = 0;
    options.scale_y = 0;
    options.reduce_frames = 0;
    options.height = 0;
    options.width = 0;

    // Extract and convert fields from the jsi::Object
    try
    {
      if (object.hasProperty(rt, "lossy"))
      {
        options.lossy = static_cast<size_t>(object.getProperty(rt, "lossy").getNumber());
      }

      if (object.hasProperty(rt, "optimize"))
      {
        options.optimize = static_cast<size_t>(object.getProperty(rt, "optimize").getNumber());
      }

      if (object.hasProperty(rt, "colors"))
      {
        options.colors = static_cast<size_t>(object.getProperty(rt, "colors").getNumber());
      }

      if (object.hasProperty(rt, "scale_x"))
      {
        options.scale_x = static_cast<size_t>(object.getProperty(rt, "scale_x").getNumber());
      }

      if (object.hasProperty(rt, "scale_y"))
      {
        options.scale_y = static_cast<size_t>(object.getProperty(rt, "scale_y").getNumber());
      }

      if (object.hasProperty(rt, "reduce_frames"))
      {
        options.reduce_frames = static_cast<size_t>(object.getProperty(rt, "reduce_frames").getNumber());
      }

      if (object.hasProperty(rt, "height"))
      {
        options.height = static_cast<size_t>(object.getProperty(rt, "height").getNumber());
      }

      if (object.hasProperty(rt, "width"))
      {
        options.width = static_cast<size_t>(object.getProperty(rt, "width").getNumber());
      }
    }
    catch (const jsi::JSError &e)
    {
      //        std::cerr << "Error extracting fields from jsi::Object: " << e.what() << std::endl;
      // You may want to handle the error here.
    }

    return options;
  }

} // namespace gifsicle_util
