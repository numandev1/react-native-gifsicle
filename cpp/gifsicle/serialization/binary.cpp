#include "binary.h"

#include <iostream>

#include "gif_generated.h"

static void gifread_error(Gif_Stream* gfs, Gif_Image* gfi, int is_error, const char* error_text)
{
  if (is_error >= 0)
    std::cerr << "Error reading GIF: " << error_text << std::endl;
}

Gif_Stream* deserialize(const uint8_t* buffer, size_t size)
{
  if (flatbuffers::Verifier verifier{buffer, size};
      animately::flat::VerifyGifBuffer(verifier))
  {
    return from_flatbuffer(reinterpret_cast<const void*>(buffer));
  }

  return Gif_FullReadBuffer(buffer, size, GIF_READ_COMPRESSED, "", gifread_error);
}

Gif_Stream* from_flatbuffer(const void* buffer)
{
  using flat_size_t = flatbuffers::Vector<uint8_t>::size_type;

  auto* stream{Gif_NewStream()};

  auto fgif{animately::flat::GetGif(buffer)};

  stream->background = fgif->background();
  stream->screen_width = fgif->width();
  stream->screen_height = fgif->height();
  stream->loopcount = fgif->loopcount();

  stream->images = Gif_NewArray(Gif_Image*, fgif->images()->size());

  for (const auto& fimage : *fgif->images())
  {
    auto* image{Gif_NewImage()};

    const auto fsize{*fimage->size()};
    image->left = fsize.left();
    image->top = fsize.top();
    image->width = fsize.width();
    image->height = fsize.height();
    image->delay = fimage->delay();
    image->transparent = fimage->transparent().value_or(-1);
    image->disposal = fimage->disposal();
    image->interlace = fimage->interlace();

    if (fimage->local())
    {
      const auto& local{*fimage->local()};
      const auto local_size{local.size() / 3};

      auto* palette{Gif_NewFullColormap(local_size, local_size)};

      for (size_t i{0}; i < local_size; ++i)
      {
        palette->col[i].gfc_red = local[static_cast<flat_size_t>(i * 3)];
        palette->col[i].gfc_green = local[static_cast<flat_size_t>(i * 3 + 1)];
        palette->col[i].gfc_blue = local[static_cast<flat_size_t>(i * 3 + 2)];
        palette->col[i].haspixel = 0;
      }

      image->local = palette;
      image->local->refcount = 1;
    }

    if (fimage->bitmap())
    {
      Gif_CreateUncompressedImage(image, image->interlace);
      const auto& bitmap{*fimage->bitmap()};
      std::copy(std::begin(bitmap), std::end(bitmap), image->image_data);
    }

    stream->images[stream->nimages] = image;
    stream->nimages++;
    image->refcount++;
  }

  if (fgif->global())
  {
    const auto& global{*fgif->global()};
    const auto global_size{global.size() / 3};

    auto* palette{Gif_NewFullColormap(global_size, global_size)};

    for (size_t i{0}; i < global_size; ++i)
    {
      palette->col[i].gfc_red = global[static_cast<flat_size_t>(i * 3)];
      palette->col[i].gfc_green = global[static_cast<flat_size_t>(i * 3 + 1)];
      palette->col[i].gfc_blue = global[static_cast<flat_size_t>(i * 3 + 2)];
      palette->col[i].haspixel = 0;
    }

    stream->global = palette;
    stream->global->refcount = 1;
  }
  else
  {
    stream->background = 256;
  }

  return stream;
}
