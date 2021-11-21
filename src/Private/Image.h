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

#ifndef CBRAIN__PRIVATE__IMAGE_H_
#define CBRAIN__PRIVATE__IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "CBrain/Image.h"
#include "CBrain/Types.h"

/**
 * @brief Sets image metadata.
 *
 * @param img source image.
 * @param width new width of image.
 * @param height new height of image.
 * @param channels new number of channels of image.
 * @return
 * - `true` on success, `false` otherwise.
 */
bool CBSetMetaInfo_Image(CBImage *img, i32 width, i32 height, i32 channels);

/**
 * @brief Mutator for `CBImage`.
 *
 * @param img source image.
 * @param width new width of image.
 * @param height new height of image.
 * @param channels new number of channels of image.
 * @param data pointer to new buffer.
 * @return
 * - `true` on success, `false` otherwise.
 */
bool CBSet_Image(CBImage *img, i32 width, i32 height, i32 channels, u8 *data);

/**
 * @brief Frees all memory buffers.
 *
 * @param img source image.
 * @return
 * - `img` on success, `nil` otherwise.
 */
CBImage *CBNullify_Image(CBImage *img);

#ifdef __cplusplus
}
#endif

#endif
