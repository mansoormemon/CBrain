#include "CBrain/Image.h"

#include "Private/Memory.h"

#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#define STBI_ONLY_BMP

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb/stb_image.h"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include "stb/stb_image_write.h"

#define IMG_QUALITY 100

CBImage *CBImageNew() {
  CBImage *img = nil;

  if (!reallocateMemory__(CastTo(&img, void *), sizeof(CBImage))) {
    return nil;
  }

  img->width = 0;
  img->height = 0;
  img->channels = 0;
  img->data = nil;

  return img;
}

CBImage *CBImageNullify(CBImage *img) {
  if (img == nil) { return nil; }

  stbi_image_free(img->data);

  img->width = 0;
  img->height = 0;
  img->channels = 0;
  img->data = nil;

  return img;
}

void CBImageDelete(CBImage **imgRef) {
  if (imgRef != nil) {
    CBImageNullify(*imgRef);
    freeMemory__(CastTo(imgRef, void *));
  }
}

CBImage *CBImageRead(const char *pathToImg) {
  CBImage *img = CBImageNew();

  if (CBImageReadInto(img, pathToImg) == nil) {
    CBImageDelete(&img);
    return nil;
  }

  return img;
}

CBImage *CBImageReadInto(CBImage *img, const char *pathToImg) {
  if (img == nil) { return nil; }

  i32 width = 0, height = 0, channels = 0;
  u8 *tempPtr = stbi_load(pathToImg, &width, &height, &channels, 0);

  if (tempPtr == nil) { return nil; }

  CBImageNullify(img);

  img->width = width;
  img->height = height;
  img->channels = channels;
  img->data = tempPtr;

  return img;
}

bool CBImageWrite(CBImage *img, const char *pathToImg) {
  if (img == nil || pathToImg == nil) { return false; }

  i32 retVal = stbi_write_jpg(pathToImg,
                              img->width,
                              img->height,
                              img->channels,
                              img->data,
                              IMG_QUALITY);

  return retVal;
}
