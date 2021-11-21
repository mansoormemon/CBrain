#include "CBrain/Image.h"

#include "CBrain/Assert.h"

#include "Private/Image.h"
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

#include <stdarg.h>
#include <string.h>

#define CB_JPEG_IMG_QUALITY 96

CBImage *CBImageNew() {
  return CBAllocate_Memory(sizeof(CBImage));
}

CBImage *CBImageReset(CBImage *img) {
  if (img != nil) {
    // Free allocated buffers.
    CBNullify_Image(img);

    // Reset all fields.
    CBSet_Image(img, 0, 0, 0, nil);
  }

  return img;
}

void CBImageDelete(CBImage **imgRef) {
  if (imgRef != nil) {
    CBImageReset(*imgRef);
    CBFree_Memory(CastTo(imgRef,
    void **));
  }
}

CBImage *CBImageFrom(i32 width, i32 height, i32 channels) {
  if (width <= 0 || height <= 0 || channels <= 0) { return nil; }

  CBImage *img = CBImageNew();

  if (CBImageFromInto(img, width, height, channels) == nil) {
    // Delete `img` in case of failure to prevent memory leaks.
    CBImageDelete(&img);
    return nil;
  }

  return img;
}

CBImage *CBImageFromInto(CBImage *dest, i32 width, i32 height, i32 channels) {
  if (dest == nil || width <= 0 || height <= 0 || channels <= 0) { return nil; }

  // Allocate a new buffer instead of reallocating the old one to prevent loss in case of failure.
  u8 *tempBuf = CBAllocate_Memory(width * height * channels);
  if (tempBuf == nil) { return nil; }

  // Free old buffer.
  CBNullify_Image(dest);
  // Update metadata.
  CBSet_Image(dest, width, height, channels, tempBuf);

  return dest;
}

CBImage *CBImageClone(CBImage *src) {
  if (src == nil) { return nil; }

  CBImage *img = CBImageNew();

  if (CBImageCloneInto(img, src) == nil) {
    // Delete `img` in case of failure to prevent memory leaks.
    CBImageDelete(&img);
    return nil;
  }

  return img;
}

CBImage *CBImageCloneInto(CBImage *dest, CBImage *src) {
  if (dest == nil || src == nil || dest == src) { return nil; }

  // Allocate a new buffer instead of reallocating the old one to prevent loss in case of failure.
  i32 bufSize = CBImageBufferSize(src);
  u8 *tempBuf = CBAllocate_Memory(bufSize);
  if (tempBuf == nil) { return nil; }

  // Free old buffer.
  CBNullify_Image(dest);
  // Update metadata.
  CBSet_Image(dest, src->width, src->height, src->channels, tempBuf);

  // Copy buffer from source to destination.
  memcpy(dest->data, src->data, bufSize);

  return dest;
}

CBImage *CBImageSubImage(CBImage *src, i32 startY, i32 startX, i32 width, i32 height) {
  if (src == nil || startY < 0 || startX < 0 || width <= 0 || height <= 0) {
    return nil;
  }

  CBImage *dest = CBImageNew();

  if (CBImageSubImageInto(dest, src, startY, startX, width, height) == nil) {
    // Delete `img` in case of failure to prevent memory leaks.
    CBImageDelete(&dest);
    return nil;
  }

  return dest;
}

CBImage *CBImageSubImageInto(CBImage *dest, CBImage *src, i32 startY, i32 startX, i32 width, i32 height) {
  if (dest == nil || src == nil || startY < 0 || startX < 0 || width <= 0 || height <= 0) {
    return nil;
  }

  // Calculate the end-point of the sub-image i.e. the diagonal opposite of the given start-point.
  i32 endY = startY + height, endX = startX + width;
  // Check if end-point is in range i.e. falls inside or on the image boundary.
  if (endY > src->height || endX > src->width) { return nil; }

  // Attributes of sub-image.
  i32 destWidth = endX - startX, destHeight = endY - startY, destChannels = src->channels;
  i32 destBufSize = destWidth * destHeight * destChannels;

  // Allocate a new buffer instead of reallocating the old one to prevent loss in case of failure.
  u8 *tempBuf = CBAllocate_Memory(destBufSize);
  if (tempBuf == nil) { return nil; }

  // Copy buffer from source to destination row-by-row.
  i32 destIndex = 0;
  u8 *srcPtr = nil;
  i32 i = 0, y = startY;
  while (y < endY) {
    srcPtr = CBImageGetPixAt(src, y, startX);
    destIndex = i * destWidth * destChannels;
    memcpy(tempBuf + destIndex, srcPtr, destWidth * destChannels);
    i += 1, y += 1;
  }

  // Free old buffer.
  CBNullify_Image(dest);
  // Update metadata.
  CBSet_Image(dest, destWidth, destHeight, destChannels, tempBuf);

  return dest;
}

CBImage *CBImageRead(const char *pathToImg) {
  CBImage *img = CBImageNew();

  if (CBImageReadInto(img, pathToImg) == nil) {
    // Delete `img` in case of failure to prevent memory leaks.
    CBImageDelete(&img);
    return nil;
  }

  return img;
}

CBImage *CBImageReadInto(CBImage *dest, const char *pathToImg) {
  if (dest == nil) { return nil; }

  i32 width = 0, height = 0, channels = 0;
  u8 *tempBuf = stbi_load(pathToImg, &width, &height, &channels, 0);
  if (tempBuf == nil) { return nil; }

  // Free old buffer.
  CBNullify_Image(dest);
  // Update metadata.
  CBSet_Image(dest, width, height, channels, tempBuf);

  return dest;
}

bool CBImageWrite(CBImage *img, const char *pathToImg) {
  if (img == nil || pathToImg == nil) { return false; }

  return stbi_write_jpg(pathToImg,
                        img->width,
                        img->height,
                        img->channels,
                        img->data,
                        CB_JPEG_IMG_QUALITY);
}

u8 *CBImageGetPixAt(CBImage *img, i32 y, i32 x) {
  if (img == nil || y < 0 || x < 0) { return nil; }

  if (img->data == nil || y >= img->height || x >= img->width) { return nil; }

  // Calculate index of required pixel.
  // This is the 3D equivalent of `(y * width) + x` for 2D matrices.
  i32 i = (y * img->width * img->channels) + (x * img->channels);

  // Returns a pointer to the first channel of the required pixel.
  return img->data + i;
}

void CBImageSetPixAt(CBImage *img, i32 y, i32 x, ...) {
  // Assertion are used because there is no other way to notify the caller function of any failures.
  CBAssert(img != nil, nil);

  u8 *pix = CBImageGetPixAt(img, y, x);

  CBAssert(pix != nil,
           "Index out of bounds!\n"
           "=> `y(=%d)` >= `img->width(=%d)` || `x(=%d)` > `img->width(=%d)` is true.",
           y,
           img->width,
           x,
           img->height);

  // Set value of each channel iteratively.
  va_list args = {};
  va_start(args, x);
  i32 i = 0;
  while (i < img->channels) {
    pix[i] = CastTo(va_arg(args, i32), u8);
    i += 1;
  }
  va_end(args);
}

u8 CBImageGetPixChanAt(CBImage *img, i32 y, i32 x, i32 chan) {
  // Assertions are used because there is no other way to notify the caller function of any failures.
  CBAssert(img != nil, nil);

  u8 *pix = CBImageGetPixAt(img, y, x);

  CBAssert(pix != nil,
           "Index out of bounds!\n"
           "=> `y(=%d)` >= `img->width(=%d)` || `x(=%d)` > `img->width(=%d)` is true.",
           y,
           img->width,
           x,
           img->height);

  CBAssert(chan < img->channels,
           "Index out of bounds!\n"
           "=> `chan(=%d)` >= `img->channels(=%d)` is true.",
           chan,
           img->channels);

  return pix[chan];
}

void CBImageSetPixChanAt(CBImage *img, i32 y, i32 x, i32 chan, u8 val) {
  // Assertions are used  because there is no other way to notify the caller function of any failures.
  CBAssert(img != nil, nil);

  u8 *pix = CBImageGetPixAt(img, y, x);

  CBAssert(pix != nil,
           "Index out of bounds!\n"
           "=> `y(=%d)` >= `img->width(=%d)` || `x(=%d)` > `img->width(=%d)` is true.",
           y,
           img->width,
           x,
           img->height);

  CBAssert(chan < img->channels,
           "Index out of bounds!\n"
           "=> `chan(=%d)` >= `img->channels(=%d)` is true.",
           chan,
           img->channels);

  pix[chan] = val;
}

u8 *CBImageLinearAt(CBImage *img, i32 index) {
  if (img == nil || index < 0) { return nil; }

  i32 bufSize = CBImageBufferSize(img);
  if (index >= bufSize) { return nil; }

  return img->data + index;
}

i32 CBImagePixCount(CBImage *img) {
  if (img == nil) { return -1; }

  return img->width * img->height;
}

i32 CBImageBufferSize(CBImage *img) {
  if (img == nil) { return -1; }

  return CBImagePixCount(img) * img->channels;
}
