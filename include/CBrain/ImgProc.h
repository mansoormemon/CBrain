// Copyright 2021 CBrain
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Copyright (c) 2021 Mansoor Memon <mansoorahmed.one@gmail.com>

#ifndef CBRAIN__IMGPROC_H_
#define CBRAIN__IMGPROC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Image.h"
#include "Types.h"

/**
 * @file ImgProc.h
 *
 * @brief Contains functionality for image processing.
 *
 * @since v0.0.1.
 */

/**
 * @brief Number of possible values for a single channel.
 */
#define CB_IMG_CHANNEL_SIZE 256

/**
 * @brief Minimum possible value for a channel.
 */
#define CB_IMG_CHANNEL_MIN 0

/**
 * @brief Maximum possibe value for a channel.
 */
#define CB_IMG_CHANNEL_MAX 255

static const f32 CBIPCK_IDENTITY_3x3[] = {0.0F, 0.0F, 0.0F,
                                          0.0F, 1.0F, 0.0F,
                                          0.0F, 0.0F, 0.0F};

static const f32 CBIPCK_EDGE_DET_0_3x3[] = {1.0F, 0.0F, -1.0F,
                                            0.0F, 0.0F, 0.0F,
                                            -1.0F, 0.0F, 1.0F};

static const f32 CBIPCK_EDGE_DET_4_3x3[] = {0.0F, -1.0F, 0.0F,
                                            -1.0F, 4.0F, -1.0F,
                                            0.0F, -1.0F, 0.0F};

static const f32 CBIPCK_EDGE_DET_8_3x3[] = {-1.0F, -1.0F, -1.0F,
                                            -1.0F, 8.0F, -1.0F,
                                            -1.0F, -1.0F, -1.0F};

static const f32 CBIPCK_EDGE_DET_24_5x5[] = {-1.0F, -1.0F, -1.0F, -1.0F, -1.0F,
                                             -1.0F, -1.0F, -1.0F, -1.0F, -1.0F,
                                             -1.0F, -1.0F, 24.0F, -1.0F, -1.0F,
                                             -1.0F, -1.0F, -1.0F, -1.0F, -1.0F,
                                             -1.0F, -1.0F, -1.0F, -1.0F, -1.0F};

static const f32 CBIPCK_SHARPEN_3x3[] = {0.0F, -1.0F, 0.0F,
                                         -1.0F, 5.0F, -1.0F,
                                         0.0F, -1.0F, 0.0F};

static const f32 CBIPCK_GBLUR_3x3[] = {0.075F, 0.124F, 0.075F,
                                       0.124F, 0.204F, 0.124F,
                                       0.075F, 0.124F, 0.075F};

/**
 * @brief Color models recognized and supported by `CBrain` library.
 *
 * @see `CBImage`
 */
typedef enum CBIPColorModel {
  /**
   * @brief Grayscale color model i.e. `img->channels` equals `1`.
   */
  CBIPCM_GRAY = 0x1,

  /**
   * @brief Grayscale color model with an alpha channel i.e. `img->channels` equals `2`.
   */
  CBIPCM_GRAY_ALPHA = 0x2,

  /**
   * @brief RGB color model i.e. `img->channels` equals `3`.
   */
  CBIPCM_RGB = 0x4,

  /**
   * @brief RGBA color model i.e. `img->channels` equals `4`.
   */
  CBIPCM_RGBA = 0x8,
  CBIPCM_MAX__ = 0x10
} CBIPColorModel;

/**
 * @brief Supported transformations that can be applied to an image.
 */
typedef enum CBIPTransformation {
  /**
   * @brief Transform from RGB to grayscale.
   */
  CBIPT_RGB_TO_GRAY = 0,

  /**
   * @brief Transform from RGB to red channel.
   */
  CBIPT_RGB_TO_RED = 1,

  /**
   * @brief Transform from RGB to green channel.
   */
  CBIPT_RGB_TO_GREEN = 2,

  /**
   * @brief Transform from RGB to blue channel.
   */
  CBIPT_RGB_TO_BLUE = 3,
  CBIPT_MAX__ = 4
} CBIPTransformation;

/**
 * @brief Crops an image.
 *
 * @param img the image to be cropped.
 * @param startY the vertical start index of the sub-image.
 * @param startX the horizontal start index of the sub-image.
 * @param width the width of the sub-image.
 * @param height the height of the sub-image.
 * @return
 * - `img` on success, `nil` otherwise.
 */
CBImage *CBImgProcCrop(CBImage *img, i32 startY, i32 startX, i32 width, i32 height);

/**
 * @brief Resizes an image.
 *
 * @param img the image to be resized.
 * @param oWidth output width.
 * @param oHeight output height.
 * @return
 * - `true` on success, `false` otherwise.
 */
bool CBImgProcResize(CBImage *img, i32 oWidth, i32 oHeight);

/**
 * @brief Rescales an image.
 *
 * @param img the image to be rescaled.
 * @param factor the factor by which the image is to be rescaled.
 * @return
 * - `true` on success, `false` otherwise.
 *
 * @details
 * Consider the following table:
 * | Factor | Output                       |
 * | :----: | ---------------------------- |
 * |   <1   | Image will shrink.           |
 * |   =1   | Image will remain unchanged. |
 * |   >1   | Image will expand.           |
 *
 */
bool CBImgProcRescale(CBImage *img, f32 factor);

/**
 * @brief Checks if the given image is compatible with the said color model.
 *
 * @param img the image to be checked.
 * @param model the model with which the compatibility is to be checked.
 * @return
 * - `true` if the image is compatible with the given model, `false` otherwise.
 *
 * @details
 * `model` can also be a bit-field.
 *
 * **For example:**
 *
 * ```c
 * // Checks if the image is compatible with any of the two models.
 * bool isCompatible = CBImgProcIsCompatible(img, CBIPCM_GRAY | CBIPCM_RGB);
 * ```
 */
bool CBImgProcIsCompatible(CBImage *img, CBIPColorModel model);

/**
 * @brief Applies given transformation on the given image.
 *
 * @param img the image to be transformed.
 * @param transformation the transformation to be applied.
 * @return
 * - `img` on success, `nil` otherwise.
 */
CBImage *CBImgProcTransform(CBImage *img, CBIPTransformation transformation);

/**
 * @brief Inverts the given image.
 *
 * @param img the image to be inverted.
 * @return
 * - `img` on success, `nil` otherwise.
 */
CBImage *CBImgProcInvert(CBImage *img);

/**
 * @brief Reduces the color space of the given image.
 *
 * @param img the image to be reduced.
 * @param factor the factor by which the image is to be reduced.
 * @return
 * - `img` on success, `nil` otherwise.
 */
CBImage *CBImgProcReduceCS(CBImage *img, i32 factor);

/**
 * @brief Binarizes the given image.
 *
 * @param img the image to be binarized.
 * @return
 * - `img` on success, `nil` otherwise.
 *
 * @details
 *
 * This function uses [Otsu's method](http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html)
 * for finding the optimal threshold.
 *
 * @see
 * `http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html`
 */
CBImage *CBImgProcBinarize(CBImage *img);

/**
 * @brief Applies the given convolution filter to the given image.
 *
 * @param img the image on which the convolution is to be applied.
 * @param kernel the kernel that will be applied.
 * @param kernelWidth width of the kernel.
 * @param kernelHeight height of the kernel.
 * @return
 * - `img` on success, `nil` otherwise.
 */
CBImage *CBImgProcConv2D(CBImage *img, const f32 *kernel, i32 kernelWidth, i32 kernelHeight);

#ifdef __cplusplus
}
#endif

#endif
