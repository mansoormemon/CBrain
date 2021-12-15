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

#ifndef CBRAIN__COST_FUNC_H_
#define CBRAIN__COST_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Tensor.h"
#include "Types.h"

/**
 * @brief Loss functions.
 */
typedef enum CBCostFunction {
  CBCF_MeanSqError = 0,
  CBCF_MeanCrossEntropy = 1,
  CBLF_MAX__
} CBCostFunc;

f32 CBCostFuncSqError(CBTensor *output, CBTensor *expected);

f32 CBCostFuncCrossEntropy(CBTensor *output, CBTensor *expected);

f32 CBCostFuncSqErrorD();

f32 CBCostFuncCrossEntropyD();

#ifdef __cplusplus
}
#endif

#endif
