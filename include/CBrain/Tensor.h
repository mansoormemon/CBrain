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

#ifndef CBRAIN__TENSOR_H_
#define CBRAIN__TENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Image.h"
#include "Types.h"

/**
 * @file Tensor.h
 *
 * @brief Contains functionality to deal with n-dimensional generic vectors.
 *
 * @details
 * **Available functionality:**
 * 1. Create and delete tensor.
 * 2. Reshape tensor.
 * 3. Utilities:
 *  - Accessors and mutators.
 *  - Query functions.
 *
 * @since v0.0.1.
 */

/**
 * @brief Macro wrapper for @link Tensor.h::CBTensorFromDims CBTensorFromDims @endlink.
 *
 * @details
 * **Usage:**
 * ```c
 * // Create a 2D tensor of shape 3x3 of type i32.
 * CBTensor *tensor = CBTensorFrom(i32, 2, 3, 3);
 * ```
 *
 * @see `CBTensorFromDims`
 */
#define CBTensorFrom(type, ...) CBTensorFromDims((sizeof(type)), __VA_ARGS__)

/**
 * @brief Macro wrapper for @link Tensor.h::CBTensorFromDimsInto CBTensorFromDimsInto @endlink.
 *
 * @details
 * **Usage:**
 * ```c
 * // Create a blank tensor.
 * CBTensor *tensor = CBTensorNew();
 *
 * // Recreate a 2D tensor of shape 3x3 of type f64.
 * CBTensorFromInto(tensor, f64, 2, 3, 3);
 * ```
 *
 * @see `CBTensorFromDimsInto`
 */
#define CBTensorFromInto(dest, type, ...) CBTensorFromDimsInto(dest, (sizeof(type)), __VA_ARGS__)

/**
 * @brief Macro wrapper for @link Tensor.h::CBTensorAt CBTensorAt @endlink.
 *
 * @details
 * **Usage:**
 * ```c
 * // Create a 2D tensor of shape 3x4 of type i32.
 * CBTensor *tensor = CBTensorFrom(i32, 2, 3, 4);
 *
 * i32 *item = CBTensorElemAt(tensor, i32, 0, 0);
 * ```
 *
 * @see `CBTensorAt`
 */
#define CBTensorElemAt(tensor, type, ...) CastTo(CBTensorAt(tensor, __VA_ARGS__), type *)

/**
 * @brief A generic n-dimensional tensor.
 *
 * @details
 * `CBTensor` can:
 *
 * - hold any data type (as long as the user can keep track of it).
 * - work with any number of dimensions.
 */
typedef struct CBTensor {
  /**
   * @brief Size of the datatype the tensor will hold.
   */
  i32 size;

  /**
   * @brief Dimensions of tensor.
   */
  i32 dims;

  /**
   * @brief Shape of tensor i.e. size of each dimension as an array.
   */
  i32 *shape;

  /**
   * @brief Pointer to data buffer.
   */
  void *data;
} CBTensor;

/**
 * @brief Creates a new dynamic `CBTensor` object.
 *
 * @return
 * - a pointer to `CBTensor` object allocated on heap, `nil` on failure.
 */
CBTensor *CBTensorNew();

/**
 * @brief Resets the tensor.
 *
 * @details
 * This includes:
 * 1. Setting all the fields to default values.
 * 2. Freeing the shape array.
 * 3. Freeing the data buffer.
 *
 * @param tensor the tensor that is to be reset.
 * @return
 * - `tensor` on success, `nil` otherwise.
 */
CBTensor *CBTensorReset(CBTensor *tensor);

/**
 * @brief Deletes the tensor.
 *
 * @note The inputted variable is set to `nil` after completion.
 *
 * @param tensorRef reference to the variable that is to be deleted.
 */
void CBTensorDelete(CBTensor **tensorRef);

/**
 * @brief Creates a new `CBTensor` object from the given parameters.
 *
 * @param size size of the datatype the tensor will hold.
 * @param dims dimensions of tensor.
 * @param ... shape of tensor i.e. size of each dimension individually.
 * @return
 * - a pointer to `CBTensor` object allocated on heap, `nil` on failure.
 *
 * @see `CBTensorFrom` `CBTensorFromInto` `CBTensorFromDimsInto`
 */
CBTensor *CBTensorFromDims(i32 size, i32 dims, ...);

/**
 * @brief Recreates an existing `CBTensor` object from the given parameters.
 *
 * @param dest existing `CBTensor` object.
 * @param size size of the datatype the tensor will hold.
 * @param dims dimensions of tensor.
 * @param ... shape of tensor i.e. size of each dimension individually.
 * @return
 * - `dest` on success, `nil` on failure.
 *
 * @see `CBTensorFromInto` `CBTensorFrom` `CBTensorFromDims`
 */
CBTensor *CBTensorFromDimsInto(CBTensor *dest, i32 size, i32 dims, ...);

/**
 * @brief Creates a new `CBTensor` object by cloning the given tensor.
 *
 * @param src the tensor to be cloned.
 * @return
 * - a pointer to `CBTensor` object allocated on heap, `nil` on failure.
 */
CBTensor *CBTensorClone(CBTensor *src);

/**
 * @brief Recreates an existing `CBTensor` object by cloning the given tensor.
 *
 * @param dest existing `CBTensor` object.
 * @param src the tensor to be cloned.
 * @return
 * - `dest` on success, `nil` otherwise.
 *
 * @note In case of failure, the inputted tensor i.e.`dest` will be left unaltered.
 */
CBTensor *CBTensorCloneInto(CBTensor *dest, CBTensor *src);

/**
 * @brief Checks if a tensor is `nil` or blank.
 *
 * @param tensor the tensor that is to be checked.
 * @return
 * - `true` if:
 *  - `tensor` is `nil`.
 *  - `tensor->shape` or `tensor->data` is `nil`.
 * - `false` otherwise.
 */
bool CBTensorIsNull(CBTensor *tensor);

/**
 * @brief Counts the total number of elements in the given tensor.
 *
 * @param tensor the tensor that is to be checked.
 * @return
 * - total number of elements in the tensor.
 */
i64 CBTensorTotal(CBTensor *tensor);

/**
 * @brief Calculates the buffer size of the given tensor.
 *
 * @param tensor
 * @return
 * - buffer size of the tensor.
 */
i64 CBTensorBufferSize(CBTensor *tensor);

/**
 * @brief Gets the element at the specified index for a given tensor.
 *
 * @param tensor the source tensor.
 * @param ... index(es) - number of index(es) must be equal to `tensor->dims`.
 * @return
 * - pointer to the specified element, `nil` on failure.
 *
 * @see `CBTensorElemAt`
 */
void *CBTensorAt(CBTensor *tensor, ...);

/**
 * @brief Reshape a given tensor based on the given parameters.
 *
 * @param tensor the tensor to be reshaped.
 * @param dims the new dimensions of the tensor.
 * @param ... new shape of tensor i.e. the size of each dimension individually.
 * @return
 * - `tensor` on success, `nil` on failure.
 *
 * @note No buffer (re)allocation will take place during reshaping. Reshaping will only be performed
 * if the new dimensions are compatible with the old ones i.e. total number of elements remain the same.
 *
 * @details
 * **Usage:**
 * ```c
 * // Create a 3D tensor of shape 3D:28x28x3 of type u8.
 * // Total elements = 2352.
 * CBTensor *tensor = CBTensorFrom(u8, 3, 28, 28, 3);
 *
 * // Reshaping tensor, so the new shape becomes 6D:2x2x3x4x7x7.
 * // Total elements = 2352.
 * CBTensorReshape(tensor, 6, 2, 2, 3, 4, 7, 7);
 * ```
 */
CBTensor *CBTensorReshape(CBTensor *tensor, i32 dims, ...);

/**
 * @brief Converts an image to a tensor.
 *
 * @param img
 * @param dims
 * @param ...
 * @return
 */
CBTensor *CBTensorFromImage(CBImage *img, i32 dims, ...);

/**
 * @brief Converts the given image to a tensor and flattens it to a 1D array.
 *
 * @param img
 * @return
 */
CBTensor *CBTensorFlatten(CBImage *img);

CBTensor *CBTensorMatMultiply(CBTensor *a, CBTensor *b);

#ifdef __cplusplus
}
#endif

#endif
