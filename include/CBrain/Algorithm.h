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

#define CBAlgoAccumulate(arr, size) \
  _Generic((arr),                   \
  u8 *: CBAlgoAccumulate_u8,        \
  u32 *: CBAlgoAccumulate_u32,      \
  i32 *: CBAlgoAccumulate_i32)(arr, size)

#define CBAlgoAvg(arr, size) \
  _Generic((arr),            \
  u8 *: CBAlgoAvg_u8,        \
  u32 *: CBAlgoAvg_u32,      \
  i32 *: CBAlgoAvg_i32)(arr, size)

#define CBAlgoRoundDown(x, factor) ((x) / (factor) * (factor))

#define CBAlgoIsEven(x) ((x) % 2 == 0)
#define CBAlgoIsOdd(x) ((x) % 2 == 1)

u32 CBAlgoAccumulate_u8(u8 *arr, i32 size);
u64 CBAlgoAccumulate_u32(u32 *arr, i32 size);
i64 CBAlgoAccumulate_i32(i32 *arr, i32 size);

u8 CBAlgoAvg_u8(u8 *arr, i32 size);
u32 CBAlgoAvg_u32(u32 *arr, i32 size);
i32 CBAlgoAvg_i32(i32 *arr, i32 size);

#ifdef __cplusplus
}
#endif

#endif
