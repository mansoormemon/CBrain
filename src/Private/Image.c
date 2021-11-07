#include "Image.h"

bool CBsetMetaInfoImage__(CBImage *img, i32 width, i32 height, i32 channels) {
  if (img == nil) { return false; }
  if (width < 0 || height < 0 || channels < 0) { return false; }

  img->width = width;
  img->height = height;
  img->channels = channels;

  return true;
}
