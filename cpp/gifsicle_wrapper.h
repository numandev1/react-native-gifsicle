

#ifndef GifsicleWrapper_H
#define GifsicleWrapper_H

#include <stdio.h>
#include <string>
#include <utility.h>
#include <gifsicle.h>
using namespace std;

class GifsicleWrapper
{
public:
  GifsicleWrapper();
    string compressGifCpp(string sourceFile, string destFilePath,GifOptions gifOptions);
};
#endif


