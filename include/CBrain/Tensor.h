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

#include "Types.h"

#define CBTensorFrom(type, ...) CBTensorFromDims((sizeof(type)), __VA_ARGS__)
#define CBTensorFromInto(dest, type, ...) CBTensorFromDimsInto(dest, (sizeof(type)), __VA_ARGS__)

#define CBTensorElemAt(tensor, type, ...) CastTo(CBTensorAt(tensor, __VA_ARGS__), type *)

typedef struct CBTensor {
  i32 size;
  i32 dims;
  i32 *shape;
  void *data;
} CBTensor;

CBTensor *CBTensorNew();

CBTensor *CBTensorReset(CBTensor *tensor);
void CBTensorDelete(CBTensor **tensorRef);

CBTensor *CBTensorFromDims(i32 size, i32 dims, ...);
CBTensor *CBTensorFromDimsInto(CBTensor *dest, i32 size, i32 dims, ...);

bool CBTensorIsNull(CBTensor *tensor);
i64 CBTensorTotal(CBTensor *tensor);
i64 CBTensorBufferSize(CBTensor *tensor);

void *CBTensorAt(CBTensor *tensor, ...);

CBTensor *CBTensorReshape(CBTensor *tensor, i32 dims, ...);

#ifdef __cplusplus
}
#endif

#endif
