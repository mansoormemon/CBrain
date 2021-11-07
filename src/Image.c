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

#include <string.h>

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

CBImage *CBImageFrom(i32 width, i32 height, i32 channels) {
  if (width <= 0 || height <= 0 || channels <= 0) { return nil; }

  CBImage *img = CBImageNew();

  if (CBImageFromInto(img, width, height, channels) == nil) {
    CBImageDelete(&img);
    return nil;
  }

  return img;
}

CBImage *CBImageFromInto(CBImage *img, i32 width, i32 height, i32 channels) {
  if (img == nil || width <= 0 || height <= 0 || channels <= 0) { return nil; }

  i32 newBufSize = width * height * channels;
  if (!reallocateMemory__(CastTo(&img->data, void *), newBufSize)) {
    return nil;
  }

  img->width = width;
  img->height = height;
  img->channels = channels;

  memset(img->data, 0, newBufSize);

  return img;
}

CBImage *CBImageClone(CBImage *src) {
  if (src == nil) { return nil; }

  CBImage *img = CBImageNew();

  if (CBImageCloneInto(img, src) == nil) {
    CBImageDelete(&img);
    return nil;
  }

  return img;
}

CBImage *CBImageCloneInto(CBImage *dest, CBImage *src) {
  if (dest == nil || src == nil || dest == src) { return nil; }

  i32 bufSize = src->width * src->height * src->channels;
  if (!reallocateMemory__(CastTo(&dest->data, void *), bufSize)) {
    return nil;
  }

  dest->width = src->width;
  dest->height = src->height;
  dest->channels = src->channels;

  memcpy(dest->data, src->data, bufSize);

  return dest;
}

CBImage *CBImageSubImage(CBImage *src, i32 startY, i32 startX, i32 width, i32 height) {
  if (src == nil || startY < 0 || startX < 0 || width <= 0 || height <= 0) {
    return nil;
  }

  CBImage *dest = CBImageNew();

  if (CBImageSubImageInto(dest, src, startY, startX, width, height) == nil) {
    CBImageDelete(&dest);
    return nil;
  }

  return dest;
}

CBImage *CBImageSubImageInto(CBImage *dest, CBImage *src, i32 startY, i32 startX, i32 width, i32 height) {
  if (dest == nil || src == nil || startY < 0 || startX < 0 || width <= 0 || height <= 0) {
    return nil;
  }

  i32 endY = startY + height, endX = startX + width;
  if (endY > src->height || endX > src->width) { return nil; }

  i32 destWidth = endX - startX, destHeight = endY - startY, destChannels = src->channels;
  i32 destBufSize = destWidth * destHeight * destChannels;

  u8 *tempBuf = nil;
  if (!reallocateMemory__(CastTo(&tempBuf, void *), destBufSize)) {
    return nil;
  }

  i32 i = 0, y = startY;
  while (y < endY) {
    i32 srcIndex = (y * src->width * src->channels) + (startX * src->channels);
    i32 destIndex = i * destWidth * destChannels;
    memcpy(tempBuf + destIndex, src->data + srcIndex, destWidth * destChannels);
    i += 1, y += 1;
  }

  CBImageNullify(dest);
  dest->width = destWidth;
  dest->height = destHeight;
  dest->channels = destChannels;
  dest->data = tempBuf;

  return dest;
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

u8 *CBImagePixelAt(CBImage *img, i32 y, i32 x) {
  if (img == nil || y < 0 || x < 0) { return nil; }

  if (img->data == nil || y >= img->height || x >= img->width) { return nil; }

  i32 i = (x * img->channels) + (y * img->width * img->channels);

  return img->data + i;
}

u8 CBImageGetValueAt(CBImage *img, i32 y, i32 x, i32 chan) {
  u8 *pix = CBImagePixelAt(img, y, x);

  assert(pix != nil);
  assert(chan < img->channels);

  return pix[chan];
}

void CBImageSetValueAt(CBImage *img, i32 y, i32 x, i32 chan, u8 val) {
  u8 *pix = CBImagePixelAt(img, y, x);

  assert(pix != nil);
  assert(chan < img->channels);

  pix[chan] = val;
}
