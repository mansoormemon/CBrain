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

#ifndef CBRAIN__ALGORITHM_H_
#define CBRAIN__ALGORITHM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"

/**
 * @file Algorithm.h
 *
 * @brief Contains functions for a variety of algorithms, however, most of them revolve around arrays.
 *
 * @since v0.0.1.
*/

/**
 * @brief Euler's constant.
 */
#define M_E 2.71828

/**
 * @brief Generic macro for CBAlgoAccumulate_[data_type: u8, u32, i32]
 */
#define CBAlgoAccumulate(arr, size) \
  _Generic((arr),                   \
    u8 *: CBAlgoAccumulate_u8,      \
    u32 *: CBAlgoAccumulate_u32,    \
    i32 *: CBAlgoAccumulate_i32)(arr, size)

/**
* @brief Generic macro for CBAlgoAvg_[data_type: u8, u32, i32]
*/
#define CBAlgoAvg(arr, size) \
  _Generic((arr),            \
    u8 *: CBAlgoAvg_u8,      \
    u32 *: CBAlgoAvg_u32,    \
    i32 *: CBAlgoAvg_i32)(arr, size)

/**
 * @brief Generic macro for CBAlgoMultiply_[data_type: i32]
 */
#define CBAlgoMultiply(arr, size) \
  _Generic((arr),                 \
    i32 *: CBAlgoMultiply_i32)(arr, size)

/**
 * @brief Rounds down `x` to a multiple of `factor`.
 */
#define CBAlgoRoundDown(x, factor) ((x) / (factor) * (factor))

/**
 * @brief Checks if `x` is even.
 */
#define CBAlgoIsEven(x) ((x) % 2 == 0)

/**
 * @brief Checks if `x` is odd.
 */
#define CBAlgoIsOdd(x) ((x) % 2 == 1)

/**
 * @brief Returns maximum.
 */
#define CBAlgoMax(x, y) ((x) > (y) ? (x) : (y))

/**
 * @brief Returns minimum.
 */
#define CBAlgoMin(x, y) ((x) < (y) ? (x) : (y))

/**
 * @brief Accumulates all elements in the given array.
 *
 * @param arr array to be accumulated.
 * @param size size of array.
 * @return
 * - accumulated result.
 */
u32 CBAlgoAccumulate_u8(u8 *arr, i32 size);

/**
 * @brief Accumulates all elements in the given array.
 *
 * @param arr array to be accumulated.
 * @param size size of array.
 * @return
 * - accumulated result.
 */
u64 CBAlgoAccumulate_u32(u32 *arr, i32 size);

/**
 * @brief Accumulates all elements in the given array.
 *
 * @param arr array to be accumulated.
 * @param size size of array.
 * @return
 * - accumulated result.
 */
i64 CBAlgoAccumulate_i32(i32 *arr, i32 size);

/**
 * @brief Finds the average of the given array.
 *
 * @param arr array whose average is to be found.
 * @param size size of array.
 * @return
 * - average value.
 */
u8 CBAlgoAvg_u8(u8 *arr, i32 size);

/**
 * @brief Finds the average of the given array.
 *
 * @param arr array whose average is to be found.
 * @param size size of array.
 * @return
 * - average value.
 */
u32 CBAlgoAvg_u32(u32 *arr, i32 size);

/**
 * @brief Finds the average of the given array.
 *
 * @param arr array whose average is to be found.
 * @param size size of array.
 * @return
 * - average value.
 */
i32 CBAlgoAvg_i32(i32 *arr, i32 size);

/**
 * @brief Multiplies all the array elements into one another.
 *
 * @param arr array to be multiplied.
 * @param size size of array.
 * @return
 * - multiplied result.
 */
i64 CBAlgoMultiply_i32(i32 *arr, i32 size);

/**
 * @brief Seeds the given seed for a random number generator.
 *
 * @param seed seed to be seeded.
 */
void CBAlgoSeedRandom(i32 seed);

/**
 * @brief Generates a random number in the given range inclusively.
 *
 * @param min minimum number.
 * @param max maximum number.
 * @return
 * - Generated random number.
 */
f32 CBAlgoRandomBetween(f32 min, f32 max);

#ifdef __cplusplus
}
#endif

#endif
