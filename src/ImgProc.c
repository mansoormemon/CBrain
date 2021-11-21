#include "CBrain/ImgProc.h"

#include "CBrain/Algorithm.h"

#include "Private/Memory.h"
#include "Private/Image.h"

#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#endif
#include "stb/stb_image_resize.h"

CBImage *CBImgProcCrop(CBImage *img, i32 startY, i32 startX, i32 width, i32 height) {
  return CBImageSubImageInto(img, img, startY, startX, width, height);
}

bool CBImgProcResize(CBImage *img, i32 oWidth, i32 oHeight) {
  if (img == nil || oWidth < 0 || oHeight < 0) { return false; }

  i32 channels = img->channels;

  // Allocate a new buffer instead of reallocating the old one to prevent loss in case of failure.
  i32 newBufSize = oWidth * oHeight * channels;
  u8 *tempBuf = CBAllocate_Memory(newBufSize);
  if (tempBuf == nil) { return false; }

  i32 retVal = stbir_resize(img->data,
                            img->width,
                            img->height,
                            0,
                            tempBuf,
                            oWidth,
                            oHeight,
                            0,
                            STBIR_TYPE_UINT8,
                            channels,
                            STBIR_ALPHA_CHANNEL_NONE,
                            0,
                            STBIR_EDGE_CLAMP,
                            STBIR_EDGE_CLAMP,
                            STBIR_FILTER_BOX,
                            STBIR_FILTER_BOX,
                            STBIR_COLORSPACE_SRGB,
                            nil);

  if (retVal == 0) { return false; }

  // Free old buffer.
  CBNullify_Image(img);
  // Update metadata.
  CBSet_Image(img, oWidth, oHeight, channels, tempBuf);

  return true;
}

bool CBImgProcRescale(CBImage *img, f32 factor) {
  i32 oWidth = CastTo(img->width * factor, f32);
  i32 oHeight = CastTo(img->height * factor, f32);

  return CBImgProcResize(img, oWidth, oHeight);
}

bool CBImgProcIsCompatible(CBImage *img, CBIPColorModel model) {
  if (img == nil || CastTo(model, i32) < 0 || CastTo(model, i32) >= CBIPCM_MAX__) { return false; }

  // Compatibility Criteria:
  // `CBIPCM_GRAY` is compatible with `CBIPCM_GRAY` only.
  // `CBIPCM_GRAY_ALPHA` is compatible with `CBIPCM_GRAY` and `CBIPCM_GRAY_ALPHA` (alpha channel will be ignored during processing).
  // `CBIPCM_RGB` is compatible with `CBIPCM_RGB` only.
  // `CBIPCM_RGBA` is compatible with `CBIPCM_RGB` and `CBIPCM_RGBA` (alpha channel will be ignored during processing).
  //
  // Please refer to documentation to see how color models are inferred based on channels.
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

  // Image must be compatible with CBIPCM_RGB color model.
  if (!CBImgProcIsCompatible(img, CBIPCM_RGB)) { return nil; }

  // New attributes of image.
  const i32 O_CHANNELS = 1;
  i32 width = img->width, height = img->height;

  i32 bufSize = img->width * img->height;
  u8 *tempBuf = CBAllocate_Memory(bufSize);

  if (tempBuf == nil) { return nil; }

  const i8 R = 0, G = 1, B = 2;

  i32 totalPixels = CBImagePixCount(img);
  i32 i = 0, j = 0;
  u8 *pix = nil;
  // Apply transformation to each pixel by iterating linearly.
  for (i = 0, j = 0; i < totalPixels; i += 1, j += img->channels) {
    pix = CBImageLinearAt(img, j);
    switch (transformation) {
      case CBIPT_RGB_TO_GRAY: {
        // Formula:
        // Y = 0.299R + 0.587G + 0.114B
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

  CBNullify_Image(img);
  CBSet_Image(img, width, height, O_CHANNELS, tempBuf);

  return img;
}

CBImage *CBImgProcInvert(CBImage *img) {
  if (img == nil) { return nil; }

  i32 bufSize = CBImageBufferSize(img);
  i32 i = 0;
  u8 *pix = nil;
  // Invert values of each pixel channel by channel through iterating linearly without considering pixel boundaries.
  // Formula:
  // C = CB_IMG_CHANNEL_MAX - C
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
  // Reduce the quality of each pixel channel by channel through iterating linearly without considering pixel boundaries.
  // Thus, reduce the image quality by reducing the colorspace.
  while (i < bufSize) {
    pix = CBImageLinearAt(img, i);
    *pix = CBAlgoRoundDown(*pix, factor);
    i += 1;
  }

  return img;
}

CBImage *CBImgProcBinarize(CBImage *img) {
  // Algorithm: Otsu's method:
  // Explanation:
  // Otsu's thresholding method involves iterating through all the possible threshold values and calculating a
  // measure of spread for the pixel levels each side of the threshold, i.e. the pixels that either fall in
  // foreground or background. The aim is to find the threshold value where the sum of foreground and background
  // spreads is at its minimum. For this, we can calculate what is called the 'Between-Class Variance'.
  //
  // Formula:
  //  𝜎B² = Wb * Wf (μb - μf)²
  // where:
  //  𝜎B - Between Class Variance
  //  Wb = Weight (background)
  //  Wf = Weight (foreground)
  //  μb = Mean (background)
  //  μf = Mean (foreground)
  //
  // The desired threshold corresponds to the maximum 𝜎B².
  //
  // Reference:
  // 1. http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html#explained
  // 2. https://en.wikipedia.org/wiki/Otsu%27s_method


  if (img == nil) { return nil; }

  // Image must be compatible with CBIPCM_GRAY color model.
  if (!CBImgProcIsCompatible(img, CBIPCM_GRAY)) { return nil; }

  i32 totalPixels = CBImagePixCount(img);
  i32 bufferSize = CBImageBufferSize(img);
  u32 histogram[CB_IMG_CHANNEL_SIZE] = {};

  i32 i = 0, t = 0;
  u8 *pix = 0;

  // Generate a histogram of image intensity.
  // => let 'L' represent bins of histogram.
  for (i = 0; i < bufferSize; i += img->channels) {
    histogram[*CBImageLinearAt(img, i)] += 1;
  }

  // Calculate the sum of weights for all possible thresholds.
  f32 sumT = 0.0F;
  for (t = 0; t < CB_IMG_CHANNEL_SIZE; t += 1) {
    sumT += CastTo(t * histogram[t], f32);
  }

  f32 sumB = 0.0F, sumF = 0.0F; // accumulated weights for background and foreground, respectively.
  f32 wB = 0.0F, wF = 0.0F; // weight of threshold `t` for background and foreground, respectively.
  f32 mB = 0.0F, mF = 0.0F; // mean for background and foreground, respectively.
  f32 BCV = 0.0F, maxBCV = 0.0F; // Between class variance and maximum between class variance, respectively.
  i32 optThresh = 0; // optimal threshold.

  for (t = 0; t < CB_IMG_CHANNEL_SIZE; t += 1) {
    // Calculate background weight for threshold `t`.
    // Formula: WB = Σ[i = 0, t] histogram[i]
    wB += CastTo(histogram[t], f32);
    if (wB == 0) { continue; }

    // Calculate foreground weight for threshold `t`.
    // Formula: Wf = totalPixels - wB.
    wF = CastTo(totalPixels, f32) - wB;
    if (wF == 0) { break; }

    // Calculate mean (background) for threshold `t`.
    // mB = (Σ[i = 0, t] i * histogram[i]) / wB
    sumB += CastTo(t * histogram[t], f32);
    mB = sumB / wB;

    // Calculate mean (foreground) for threshold `t`.
    // mF = (sumT - sumB) / wF
    sumF = sumT - sumB;
    mF = sumF / wF;

    // Calculate 'Between Class Variance' using formula mentioned above.
    BCV = wB * wF * (mB - mF) * (mB - mF);

    // Update optimal threshold in order to find threshold where between class variance is maximum.
    if (BCV > maxBCV) {
      maxBCV = BCV;
      optThresh = t;
    }
  }

  // Apply thresholding by iterating linearly while respecting pixel boundaries.
  for (i = 0; i < bufferSize; i += img->channels) {
    pix = CBImageLinearAt(img, i);
    *pix = (*pix) > optThresh ? CB_IMG_CHANNEL_MAX : CB_IMG_CHANNEL_MIN;
  }

  return img;
}

CBImage *CBImgProcConv2D(CBImage *img, const f32 *kernel, i32 kernelWidth, i32 kernelHeight) {
  if (img == nil || kernel == nil || kernelWidth < 0 || kernelHeight < 0) { return nil; }

  // Image must be compatible with CBIPCM_GRAY color model.
  if (!CBImgProcIsCompatible(img, CBIPCM_GRAY)) { return nil; }

  // Kernel width and height must be odd, so it can be centered on a pixel.
  if (!CBAlgoIsOdd(kernelWidth) || !CBAlgoIsOdd(kernelHeight)) { return nil; }

  const i32 O_CHANNELS = 1;
  i32 bufSize = img->width * img->height;
  u8 *tempBuf = CBAllocate_Memory(bufSize);

  if (tempBuf == nil) { return nil; }

  i32 dispX = kernelWidth / 2;
  i32 dispY = kernelHeight / 2;

  i32 y = 0, x = 0;

  for (y = dispY; y < img->height - dispY; y += 1) {
    for (x = dispX; x < img->width - dispX; x += 1) {
      i32 index = (y * img->width) + x;
      i32 i = 0;
      i32 r = 0, c = 0;
      f32 acc = 0;

      // Consider the following:
      //    +----------------+-----+------------+-----+----------------+
      //    | (y - r, x - c) | ... | (y - r, x) | ... | (y - r, x + c) |
      //    +----------------+-----+------------+-----+----------------+
      //    ...
      //    +----------------+-----+------------+-----+----------------+
      //    |   (y, x - c)   | ... |   (y, x)   | ... |   (y, x + c)   |
      //    +----------------+-----+------------+-----+----------------+
      //    ...
      //    +----------------+-----+------------+-----+----------------+
      //    | (y + r, x - c) | ... | (y + r, x) | ... | (y + r, x + c) |
      //    +----------------+-----+------------+-----+----------------+
      // Kernel will be centered at (y, x).
      //
      // Kernel parameters:
      // => `r` will increase by one step on every iteration to move from top to bottom.
      // => `c` will increase by one step on every iteration to proceed from left to right for each row of the kernel.
      //
      // This applies to every pixel of the image (except the ones that make up the image boundary and its
      // thickness depends on the dimensions of the kernel).
      //
      // The convolution will be the sum of scalar product of pixels and kernel at corresponding positions.

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

  CBNullify_Image(img);
  CBSet_Image(img, img->width, img->height, O_CHANNELS, tempBuf);

  return img;
}
