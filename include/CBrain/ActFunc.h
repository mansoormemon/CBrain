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

#ifndef CBRAIN__ACT_FUNC_H_
#define CBRAIN__ACT_FUNC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Tensor.h"
#include "Types.h"

/**
 * @file ActFunc.h
 *
 * @brief Contains functionality for activation functions for neurons in a layer.
 *
 * @since v0.0.1.
 */

/**
 * @brief Activation functions.
 */
typedef enum CBActivationFunction {
  CBLAF_Linear = 0,
  CBLAF_Sigmoid = 1,
  CBLAF_TanH = 2,
  CBLAF_ArcTan = 3,
  CBLAF_ReLU = 4,
  CBLAF_LeakyReLU = 5,
  CBLAF_ELU = 6,
  CBLAF_MAX__
} CBActFunc;

/**
 * @brief Linear activation function.
 *
 * @param x independent variable.
 * @return
 * - y
 */
f32 CBActFuncLinear(f32 x);

/**
 * @brief Sigmoid activation function.
 *
 * @param x independent variable.
 * @return
 * - y
 */
f32 CBActFuncSigmoid(f32 x);

/**
 * @brief TanH activation function.
 *
 * @param x independent variable.
 * @return
 * - y
 */
f32 CBActFuncTanH(f32 x);

/**
 * @brief ArcTan activation function.
 *
 * @param x independent variable.
 * @return
 * - y
 */
f32 CBActFuncArcTan(f32 x);

/**
 * @brief ReLU (Rectified Linear Unit) activation function.
 *
 * @param x independent variable.
 * @return
 * - y
 */
f32 CBActFuncReLU(f32 x);

/**
 * @brief Leaky ReLU (Leaky Rectified Linear Unit) activation function.
 *
 * @param x independent variable.
 * @return
 * - y
 */
f32 CBActFuncLeakyReLU(f32 x);

/**
 * @brief ELU (Exponential Linear Unit) activation function.
 *
 * @param x independent variable.
 * @param alpha constant (must be positive)
 * @return
 * - y
 */
f32 CBActFuncELU(f32 x, f32 alpha);

/**
 * @brief Softmax function.
 *
 * @param tensor
 * - target tensor.
 */
void CBActFuncSoftmax(CBTensor *tensor);

/**
 * @brief Converts `actFunc` to string.
 *
 * @param actFunc target activation function.
 * @return
 * - Name of `actFunc` as string.
 */
const char *CBActFuncToString(CBActFunc actFunc);

f32 CBActFuncLinearD(f32 x);

f32 CBActFuncSigmoidD(f32 x);

f32 CBActFuncTanHD(f32 x);

f32 CBActFuncArcTanD(f32 x);

f32 CBActFuncReLUD(f32 x);

f32 CBActFuncLeakyReLUD(f32 x);

f32 CBActFuncELUD(f32 x, f32 alpha);

#ifdef __cplusplus
}
#endif

#endif
