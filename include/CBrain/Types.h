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

#ifndef CBRAIN__TYPES_H_
#define CBRAIN__TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @file Types.h
 *
 * @brief Contains typedefs and macros that (re)define types.
 *
 * @since v0.0.1.
 */

/**
 * @brief Represent `NULL`.
 */
#define nil NULL

/**
 * @brief Macro for casting between data types.
 */
#define CastTo(target, type) ((type)(target))

/**
 * @brief Represents `unsigned char`.
 */
typedef uint8_t u8;

/**
 * @brief Represents `unsigned short`.
 */
typedef uint16_t u16;

/**
 * @brief Represents `unsigned int`.
 */
typedef uint32_t u32;

/**
 * @brief Represents `unsigned long`.
 */
typedef uint64_t u64;

/**
 * @brief Represents `char`.
 */
typedef int8_t i8;

/**
 * @brief Represents `short`.
 */
typedef int16_t i16;

/**
 * @brief Represents `int`.
 */
typedef int32_t i32;

/**
 * @brief Represents `long`.
 */
typedef int64_t i64;

/**
 * @brief Represents `float`.
 */
typedef float f32;

/**
 * @brief Represents `double`.
 */
typedef double f64;

#ifdef __cplusplus
}
#endif

#endif
