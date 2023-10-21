//
//  gifsicle_wrapper.h
//  Pods
//
//  Created by Numan on 16/10/2023.
//

#ifndef gifsicle_wrapper_h
#define gifsicle_wrapper_h

#include <stdio.h>
#include <string>
#include <utility.h>
using namespace std;

using namespace std;
class GifsicleWrapper {
public:
    GifsicleWrapper();
    string compressGifCpp(string sourceFile, string destFilePath,GifOptions options);
};

#endif /* gifsicle_wrapper_h */
