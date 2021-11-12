#include "Image.h"

#include "stb/stb_image.h"

bool CBSetMetaInfo_Image(CBImage *img, i32 width, i32 height, i32 channels) {
  if (img == nil) { return false; }
  if (width < 0 || height < 0 || channels < 0) { return false; }

  img->width = width;
  img->height = height;
  img->channels = channels;

  return true;
}

bool CBSet_Image(CBImage *img, i32 width, i32 height, i32 channels, u8 *data) {
  if (CBSetMetaInfo_Image(img, width, height, channels)) {
    img->data = data;
    return true;
  }

  return false;
}

CBImage *CBNullify_Image(CBImage *img) {
  if (img != nil) {
    stbi_image_free(img->data);
  }

  return img;
}