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

CBTensor *CBFromDimsIntoV_Tensor(CBTensor *dest, i32 size, i32 dims, va_list list);

bool CBSet_Tensor(CBTensor *tensor, i32 size, i32 dims, i32 *shape, void *data);

i32 *CBGetShapeV_Tensor(i32 dims, va_list list);

bool CBSetShape_Tensor(CBTensor *tensor, i32 dims, i32 *shape);

CBTensor *CBNullify_Tensor(CBTensor *tensor);

bool CBIsIndexValidV_Tensor(CBTensor *tensor, va_list list);

bool CBIsShapeCompatible_Tensor(CBTensor *tensor, i32 dims, i32 *shape);

#ifdef __cplusplus
}
#endif

#endif
