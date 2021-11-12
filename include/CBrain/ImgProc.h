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

#define CB_IMG_CHANNEL_SIZE 256
#define CB_IMG_CHANNEL_MIN 0
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

typedef enum CBIPColorModel {
  CBIPCM_GRAY = 0x1,
  CBIPCM_GRAY_ALPHA = 0x2,
  CBIPCM_RGB = 0x4,
  CBIPCM_RGBA = 0x8
} CBIPColorModel;

typedef enum CBIPTransformation {
  CBIPT_RGB_TO_GRAY = 0,
  CBIPT_RGB_TO_RED = 1,
  CBIPT_RGB_TO_GREEN = 2,
  CBIPT_RGB_TO_BLUE = 3,
  CBIPT_MAX__
} CBIPTransformation;

CBImage *CBImgProcCrop(CBImage *img, i32 startY, i32 startX, i32 width, i32 height);

bool CBImgProcResize(CBImage *img, i32 oWidth, i32 oHeight);

bool CBImgProcRescale(CBImage *img, f32 factor);

bool CBImgProcIsCompatible(CBImage *img, CBIPColorModel model);

CBImage *CBImgProcTransform(CBImage *img, CBIPTransformation transformation);

CBImage *CBImgProcInvert(CBImage *img);

CBImage *CBImgProcReduceCS(CBImage *img, i32 factor);

CBImage *CBImgProcBinarize(CBImage *img);

CBImage *CBImgProcConv2D(CBImage *img, const f32 *kernel, i32 kernelWidth, i32 kernelHeight);

#ifdef __cplusplus
}
#endif

#endif
