// Copyright 2021 CBrain
// Project URL: https://github.com/mansoormemon/CBrain
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

#ifndef CBRAIN__IMAGE_H_
#define CBRAIN__IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"

/**
 * @file Image.h
 *
 * @brief Contains all the basic functionality for working with images.
 *
 * @details
 * **Available functionality:**
 * 1. Create and delete images.
 * 2. IO functions to read and write image.
 * 3. Utilities:
 *  - Clone image.
 *  - Extracting a sub-image from an image.
 *  - Accessors and Mutators.
 *  - Query functions.
 *
 * @since v0.0.1.
 */

/**
 * @brief A simple 2D image.
 *
 * @details
 * ### Limitations:
 *
 * 1. All channels will have the same depth i.e. u8.
 *
 * 2. Color model will be inferred based on the number of channels as follows:
 *  | Channels | Associated Color Model |
 *  | :------: | ---------------------- |
 *  |     1    | Gray                   |
 *  |     2    | Gray Alpha             |
 *  |     3    | RGB                    |
 *  |     4    | RGBA                   |
 * **Note:** Anything besides these will render the image incompatible with
 * most of the library functions.
 *
 * @see `CBIPColorModel`
 */
typedef struct CBImage {
  /**
   * @brief Width of the image.
   */
  i32 width;

  /**
   * @brief Height of the image.
   */
  i32 height;

  /**
   * @brief Number of channels in the image.
   */
  i32 channels;

  /**
   * @brief Pointer to data buffer.
   */
  u8 *data;
} CBImage;

/**
 * @brief Creates a new dynamic `CBImage` object.
 *
 * @return
 * - a pointer to `CBImage` object allocated on heap, `nil` on failure.
 */
CBImage *CBImageNew();

/**
 * @brief Resets the image.
 *
 * @details
 * This includes:
 * 1. Setting all the fields to default values.
 * 2. Freeing the data buffer.
 *
 * @param img the image that is to be reset.
 * @return
 * - `img` on success, `nil` otherwise.
 */
CBImage *CBImageReset(CBImage *img);

/**
 * @brief Deletes the image.
 *
 * @note The inputted variable is set to `nil` after completion.
 *
 * @param imgRef reference to the variable that is to be deleted.
 */
void CBImageDelete(CBImage **imgRef);

/**
 * @brief Creates a new `CBImage` object from the given parameters.
 *
 * @param width width of image.
 * @param height height of image.
 * @param channels number of channels in the image.
 * @return
 * - a pointer to `CBImage` object allocated on heap, `nil` on failure.
 */
CBImage *CBImageFrom(i32 width, i32 height, i32 channels);

/**
 * @brief Recreates an existing `CBImage` object from the given parameters.
 *
 * @param dest existing `CBImage` object.
 * @param width width of image.
 * @param height height of image.
 * @param channels number of channels in the image.
 * @return
 * - `dest` on success, `nil` otherwise.
 *
 * @note In case of failure, the inputted image i.e. `dest` will be left unaltered.
 *
 * @see `CBImageFrom`
 */
CBImage *CBImageFromInto(CBImage *dest, i32 width, i32 height, i32 channels);

/**
 * @brief Creates a new `CBImage` object by cloning the given image.
 *
 * @param src the image to be cloned.
 * @return
 * - a pointer to `CBImage` object allocated on heap, `nil` on failure.
 */
CBImage *CBImageClone(CBImage *src);

/**
 * @brief Recreates an existing `CBImage` object by cloning the given image.
 *
 * @param dest existing `CBImage` object.
 * @param src the image to be cloned.
 * @return
 * - `dest` on success, `nil` otherwise.
 *
 * @note In case of failure, the inputted image i.e.`dest` will be left unaltered.
 */
CBImage *CBImageCloneInto(CBImage *dest, CBImage *src);

/**
 * @brief Creates a new `CBImage` object that is a sub-image of the given image.
 *
 * @param src the image from which the sub-image is to be extracted.
 * @param startY the vertical start index of the sub-image.
 * @param startX the horizontal start index of the sub-image.
 * @param width the width of the sub-image.
 * @param height the height of the sub-image.
 * @return
 * - a pointer to `CBImage` object allocated on heap, `nil` on failure.
 */
CBImage *CBImageSubImage(CBImage *src, i32 startY, i32 startX, i32 width, i32 height);

/**
 * @brief Recreates an existing `CBImage` object that is a sub-image of the given image.
 *
 * @param dest existing `CBImage` object.
 * @param src the image from which the sub-image is to be extracted.
 * @param startY the vertical start index of the sub-image.
 * @param startX the horizontal start index of the sub-image.
 * @param width the width of the sub-image.
 * @param height the height of the sub-image.
 * @return
 * - `dest` on success, `nil` otherwise.
 */
CBImage *CBImageSubImageInto(CBImage *dest, CBImage *src, i32 startY, i32 startX, i32 width, i32 height);

/**
 * @brief Creates a new `CBImage` object by reading an image from disk.
 *
 * @param pathToImg path to image on disk.
 * @return a pointer to `CBImage` object allocated on heap, `nil` on failure.
 *
 * @details
 * **Supported Formats:**
 * 1. BMP
 * 2. JPG/JPEG
 * 3. PNG
 *
 * @see `https://github.com/nothings/stb/blob/master/stb_image.h`
 */
CBImage *CBImageRead(const char *pathToImg);

/**
 * @brief Recreates an existing `CBImage` object by reading an image from disk.
 *
 * @param dest existing `CBImage` object.
 * @param pathToImg path to image on disk.
 * @return `dest` on success, `nil` otherwise.
 *
 * @details
 * **Supported Formats:**
 * 1. BMP
 * 2. JPG/JPEG
 * 3. PNG
 *
 * @see `https://github.com/nothings/stb/blob/master/stb_image.h`
 */
CBImage *CBImageReadInto(CBImage *dest, const char *pathToImg);

/**
 * @brief Writes the given image to disk.
 *
 * @param img image to be saved.
 * @param pathToImg path to image.
 * @return
 * - `true` on success, `false` otherwise.
 *
 * @note
 * - The output image will always be `jpg` irrespective of the file extension.
 * - This function cannot create new directories. If a directory does not exist,
 * the function will return `false` indicating failure.
 *
 * @see `https://github.com/nothings/stb/blob/master/stb_image_write.h`
 */
bool CBImageWrite(CBImage *img, const char *pathToImg);

/**
 * @brief Gets the pixel at specified index.
 *
 * @param img source image.
 * @param y Y co-ordinate of pixel.
 * @param x X co-ordinate of pixel.
 * @return
 * - pointer to first channel of the target pixel on success, `nil` otherwise.
 */
u8 *CBImageGetPixAt(CBImage *img, i32 y, i32 x);

/**
 * @brief Sets the pixel at specified index.
 *
 * @param img target image.
 * @param y Y co-ordinate of pixel.
 * @param x X co-ordinate of pixel.
 * @param ... values for each individual channel.
 *
 * @note All channels must be set at once. If the number of arguments passed as `...`
 * is less than `img->channels`, the result is undefined!
 */
void CBImageSetPixAt(CBImage *img, i32 y, i32 x, ...);

/**
 * @brief Gets the specified channel of the specified pixel in the given image.
 *
 * @param img source image.
 * @param y Y co-ordinate of pixel.
 * @param x X co-ordinate of pixel.
 * @param chan channel of pixel.
 * @return
 * - the specified channel.
 */
u8 CBImageGetPixChanAt(CBImage *img, i32 y, i32 x, i32 chan);

/**
 * @brief Sets the specified channel of the specified pixel in the given image.
 *
 * @param img source image.
 * @param y Y co-ordinate of pixel.
 * @param x X co-ordinate of pixel.
 * @param chan channel of pixel.
 * @param val value the is to be set.
 */
void CBImageSetPixChanAt(CBImage *img, i32 y, i32 x, i32 chan, u8 val);

/**
 * @brief Gets a pointer to the specified index.
 *
 * @param img source image.
 * @param index target index.
 * @return
 * - pointer to the specified index, `nil` in case of failure.
 *
 * @note This function does not respect pixel boundaries.
 */
u8 *CBImageLinearAt(CBImage *img, i32 index);

/**
 * @brief Calculates the total pixels in the given image.
 *
 * @param img source image.
 * @return
 * - total pixels in image.
 */
i32 CBImagePixCount(CBImage *img);

/**
 * @brief Calculates buffer size of the given image.
 *
 * @param img source image.
 * @return
 * - buffer size of image.
 */
i32 CBImageBufferSize(CBImage *img);

#ifdef __cplusplus
}
#endif

#endif
