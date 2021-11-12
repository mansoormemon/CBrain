#include "CBrain/ImgProc.h"

#include "CBrain/Algorithm.h"

#include "Private/Memory.h"
#include "Private/Image.h"

#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#endif
#include "stb/stb_image_resize.h"

#include <assert.h>

CBImage *CBImgProcCrop(CBImage *img, i32 startY, i32 startX, i32 width, i32 height) {
  return CBImageSubImageInto(img, img, startY, startX, width, height);
}

bool CBImgProcResize(CBImage *img, i32 oWidth, i32 oHeight) {
  if (img == nil || oWidth < 0 || oHeight < 0) { return false; }

  i32 channels = img->channels;

  i32 newBufSize = oWidth * oHeight * channels;
  u8 *tempBuf = nil;
  if (!CBreallocateMemory__(CastTo(&tempBuf, void *), newBufSize)) {
    return nil;
  }

  i32 retVal = stbir_resize(img->data,
                            img->width,
                            img->height,
                            0,
                            tempBuf,
                            oWidth,
                            oHeight,
                            0,
                            STBIR_TYPE_UINT8,
                            img->channels,
                            STBIR_ALPHA_CHANNEL_NONE,
                            0,
                            STBIR_EDGE_CLAMP,
                            STBIR_EDGE_CLAMP,
                            STBIR_FILTER_BOX,
                            STBIR_FILTER_BOX,
                            STBIR_COLORSPACE_SRGB,
                            nil
  );

  if (retVal == 0) { return false; }

  CBImageNullify(img);

  CBsetMetaInfoImage__(img, oWidth, oHeight, channels);
  img->data = tempBuf;

  return retVal;
}

bool CBImgProcRescale(CBImage *img, f32 factor) {
  i32 oWidth = CastTo(img->width * factor, f32);
  i32 oHeight = CastTo(img->height * factor, f32);

  return CBImgProcResize(img, oWidth, oHeight);
}

bool CBImgProcIsCompatible(CBImage *img, CBIPColorModel model) {
  if (img == nil || CastTo(model, i32) < 0) { return false; }

  switch (img->channels) {
    case 1: {
      return model & CBIPCM_GRAY;
    }
    case 2: {
      return model & (CBIPCM_GRAY | CBIPCM_GRAY_ALPHA);
    }
    case 3: {
      return model & CBIPCM_RGB;
    }
    case 4: {
      return model & (CBIPCM_RGB | CBIPCM_RGBA);
    }
    default: {
      return false;
    }
  }
}

CBImage *CBImgProcTransform(CBImage *img, CBIPTransformation transformation) {
  if (img == nil || CastTo(transformation, i32) < 0 || CastTo(transformation, i32) >= CBIPT_MAX__) {
    return nil;
  }

  if (!CBImgProcIsCompatible(img, CBIPCM_RGB)) { return nil; }

  const i32 O_CHANNELS = 1;
  i32 width = img->width, height = img->height;

  i32 bufSize = img->width * img->height;
  u8 *tempBuf = nil;

  if (!CBreallocateMemory__(CastTo(&tempBuf, void *), bufSize)) {
    return nil;
  }

  const i8 R = 0, G = 1, B = 2;

  i32 totalPixels = CBImagePixCount(img);
  i32 i = 0, j = 0;
  u8 *pix = nil;
  for (i = 0, j = 0; i < totalPixels; i += 1, j += img->channels) {
    pix = CBImageLinearAt(img, j);
    switch (transformation) {
      case CBIPT_RGB_TO_GRAY: {
        tempBuf[i] = CastTo((0.299F * pix[R]) + (0.587F * pix[G]) + (0.114F * pix[B]), i32);
        break;
      }
      case CBIPT_RGB_TO_RED: {
        tempBuf[i] = pix[R];
        break;
      }
      case CBIPT_RGB_TO_GREEN: {
        tempBuf[i] = pix[G];
        break;
      }
      case CBIPT_RGB_TO_BLUE: {
        tempBuf[i] = pix[B];
        break;
      }
      default: {
        break;
      }
    }
  }

  CBImageNullify(img);

  CBsetMetaInfoImage__(img, width, height, O_CHANNELS);
  img->data = tempBuf;

  return img;
}

CBImage *CBImgProcInvert(CBImage *img) {
  if (img == nil) { return nil; }

  i32 bufSize = CBImageBufferSize(img);
  i32 i = 0;
  u8 *pix = nil;
  while (i < bufSize) {
    pix = CBImageLinearAt(img, i);
    *pix = CB_IMG_CHANNEL_MAX - (*pix);
    i += 1;
  }

  return img;
}

CBImage *CBImgProcReduceCS(CBImage *img, i32 factor) {
  if (img == nil) { return nil; }

  i32 bufSize = CBImageBufferSize(img);
  i32 i = 0;
  u8 *pix = nil;
  while (i < bufSize) {
    pix = CBImageLinearAt(img, i);
    *pix = CBAlgoRoundDown(*pix, factor);
    i += 1;
  }

  return img;
}

CBImage *CBImgProcBinarize(CBImage *img) {
  if (img == nil) { return nil; }

  if (!CBImgProcIsCompatible(img, CBIPCM_GRAY)) { return nil; }

  i32 totalPixels = CBImagePixCount(img);
  i32 bufferSize = CBImageBufferSize(img);
  u32 histogram[CB_IMG_CHANNEL_SIZE] = {};

  i32 i = 0, t = 0;
  u8 *pix = 0;

  for (i = 0; i < bufferSize; i += img->channels) {
    histogram[*CBImageLinearAt(img, i)] += 1;
  }

  f32 sumT = 0.0F;
  for (t = 0; t < CB_IMG_CHANNEL_SIZE; t += 1) {
    sumT += CastTo(t * histogram[t], f32);
  }

  f32 sumB = 0.0F, sumF = 0.0F;
  f32 wB = 0.0F, wF = 0.0F, mB = 0.0F, mF = 0.0F;
  f32 BCV = 0.0F, maxBCV = 0.0F;
  i32 optThresh = 0;

  for (t = 0; t < CB_IMG_CHANNEL_SIZE; t++) {
    wB += CastTo(histogram[t], f32);
    if (wB == 0) { continue; }

    wF = CastTo(totalPixels, f32) - wB;
    if (wF == 0) { break; }

    sumB += CastTo(t * histogram[t], f32);
    sumF = sumT - sumB;

    mB = sumB / wB;
    mF = sumF / wF;

    BCV = wB * wF * (mB - mF) * (mB - mF);

    if (BCV > maxBCV) {
      maxBCV = BCV;
      optThresh = t;
    }
  }

  for (i = 0; i < bufferSize; i += img->channels) {
    pix = CBImageLinearAt(img, i);
    *pix = (*pix) > optThresh ? CB_IMG_CHANNEL_MAX : CB_IMG_CHANNEL_MIN;
  }

  return img;
}

CBImage *CBImgProcConv2D(CBImage *img, const f32 *kernel, i32 kernelWidth, i32 kernelHeight) {
  if (img == nil || kernel == nil || kernelWidth < 0 || kernelHeight < 0) { return nil; }

  if (!CBImgProcIsCompatible(img, CBIPCM_GRAY)) { return nil; }

  assert(CBAlgoIsOdd(kernelWidth));
  assert(CBAlgoIsOdd(kernelHeight));

  u8 *tempBuf = nil;
  i32 width = img->width, height = img->height, channels = img->channels;
  i32 bufSize = CBImageBufferSize(img);

  if (!CBreallocateMemory__(CastTo(&tempBuf, void *), bufSize)) {
    return nil;
  }

  memset(tempBuf, 0, bufSize);

  i32 dispX = kernelWidth / 2;
  i32 dispY = kernelHeight / 2;

  i32 y = 0, x = 0;
  for (y = dispY; y < img->height - dispY; y += 1) {
    for (x = dispX; x < img->width - dispX; x += 1) {
      i32 index = (y * width * channels) + (x * channels);
      i32 i = 0;
      i32 r = 0, c = 0;
      f32 acc = 0;
      for (r = -dispY; r <= dispY; r += 1) {
        for (c = -dispX; c <= dispX; c += 1) {
          u8 *pix = CBImageGetPixAt(img, y + r, x + c);
          acc += CastTo((*pix) * kernel[i], f32);
          i += 1;
        }
      }
      tempBuf[index] = CastTo(acc, i32);
    }
  }

  CBImageNullify(img);

  CBsetMetaInfoImage__(img, width, height, channels);
  img->data = tempBuf;

  return img;
}
