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

#ifndef CBRAIN__PRIVATE__TENSOR_H_
#define CBRAIN__PRIVATE__TENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "CBrain/Tensor.h"
#include "CBrain/Types.h"

#include <stdarg.h>

/**
 * @brief Recreates an existing `CBTensor` object from the given dimensions.
 *
 * @param dest an existing `CBTensor` object.
 * @param size new size of the datatype the tensor will hold.
 * @param dims new dimensions of the tensor.
 * @param list a variadic list that represents the new shape of tensor.
 * @return
 * - `dest` on success, `nil` otherwise.
 */
CBTensor *CBFromDimsIntoV_Tensor(CBTensor *dest, i32 size, i32 dims, va_list list);

/**
 * @brief Mutator for `CBTensor`.
 *
 * @param tensor target tensor.
 * @param size new size of datatype that the tensor will hold.
 * @param dims new dimensions of the tensor.
 * @param shape new shape of tensor.
 * @param data pointer to new buffer.
 * @return
 * - `true` on success, `nil` otherwise.
 */
bool CBSet_Tensor(CBTensor *tensor, i32 size, i32 dims, i32 *shape, void *data);

/**
 * @brief Converts a variadic list into an integer array that represents shape of tensor.
 *
 * @param dims new dimensions of tensor.
 * @param list variadic list that represents new shape.
 * @return
 * - an integer array on success, `nil` otherwise.
 */
i32 *CBGetShapeV_Tensor(i32 dims, va_list list);

/**
 * @brief Mutate the shape of the given tensor.
 *
 * @param tensor target tensor.
 * @param dims new dimensions of tensor.
 * @param shape new shape of tensor.
 * @return
 * - `true` on success, `false` otherwise.
 */
bool CBSetShape_Tensor(CBTensor *tensor, i32 dims, i32 *shape);

/**
 * @brief Frees all memory buffers.
 *
 * @param tensor source tensor.
 * @return
 * - `tensor` on success, `nil` otherwise.
 */
CBTensor *CBNullify_Tensor(CBTensor *tensor);

/**
 * @brief Checks if the given index is valid, this also includes bounds checking for each dimension.
 *
 * @param tensor target tensor.
 * @param list variadic list that represents index(es).
 * @return
 * - `true` if index(es) are valid, `false` otherwise.
 */
bool CBIsIndexValidV_Tensor(CBTensor *tensor, va_list list);

/**
 * @brief Checks compatibility between old and new tensor shape.
 *
 * @param tensor target tensor.
 * @param dims new dimensions.
 * @param shape new shape.
 * @return
 * - `true` if shapes are compatible, `false` otherwise.
 */
bool CBIsShapeCompatible_Tensor(CBTensor *tensor, i32 dims, i32 *shape);

#ifdef __cplusplus
}
#endif

#endif
