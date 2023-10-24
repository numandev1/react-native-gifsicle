

#include <string>
#include <regex>
#include <gifsicle.h>
#include "gifsicleutil.h"

#include <iostream>
#include <inttypes.h>
#include "gifsicle_wrapper.h"
#include <filesystem>
#include "filesystem_wrapper.h"

using namespace std;

GifsicleWrapper::GifsicleWrapper()
{
}

string pathWithoutFileUri(string filePath)
{
    regex pattern("^file://*");
    filePath = regex_replace(filePath, pattern, "/");
    return filePath;
}

string slashifyFilePath(string filePath)
{
    if (!filePath.empty())
    {
        std::regex pattern("^/+");

        filePath = std::regex_replace(filePath, pattern, "file:///");

        if (filePath.find("file:///") != 0)
        {
            // If not, prepend "file:///" to the path
            filePath = "file:///" + filePath;
        }

        return filePath;
    }

    return filePath;
}

static void gifread_error(Gif_Stream *gfs, Gif_Image *gfi, int is_error, const char *error_text)
{
    if (is_error >= 0)
    {
        cerr << "Error reading GIF: " << error_text << endl;
    }
}

string GifsicleWrapper::compressGifCpp(string sourceFile, string destFilePath, GifOptions gifOptions)
{
    string filePathStr = pathWithoutFileUri(sourceFile);
    string destFilePathStr = pathWithoutFileUri(destFilePath);

    fs::path gif_file{filePathStr};

    const auto gif_bytes{
        gifsicle_util::loadFile(gif_file.string())};

    auto *gif{Gif_FullReadBuffer(reinterpret_cast<const uint8_t *>(gif_bytes.data()),
                                 gif_bytes.size(), GIF_READ_UNCOMPRESSED,
                                 "", gifread_error)};

    GifBuffer buffer;
    COptions coptions;
    parse_options(gifOptions, coptions);

    gifsicle_main(coptions.count(), coptions.options(), gif, &buffer.data, &buffer.size);

    bool isSaved = gifsicle_util::saveFile(reinterpret_cast<const byte *>(buffer.data), buffer.size, destFilePathStr);
    if (isSaved)
    {
        string destSlashifyFilePath = slashifyFilePath(destFilePathStr);
        return destSlashifyFilePath;
    }
    else
    {
        string actualSlashifyFilePath = slashifyFilePath(filePathStr);
        return actualSlashifyFilePath;
    }
}
