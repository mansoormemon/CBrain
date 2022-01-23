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

#ifndef CBRAIN__LAYER_H_
#define CBRAIN__LAYER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ActFunc.h"
#include "Tensor.h"
#include "Types.h"

/**
 * @file Layer.h
 *
 * @brief Contains functionality for layers of a neural network.
 *
 * @since v0.0.1.
 */

/**
 * @brief A dense layer for a neural network.
 *
 * @details
 *
 * Its basically a doubly-linked list.
 */
typedef struct CBLayer {
  /**
   * @brief Number of neurons in the layer.
   */
  i32 neurons;

  /**
   * @brief bias of that layer.
   */
  CBTensor *biases;

  /**
   * @brief Activation function for neurons of that layer.
   */
  CBActFunc actFunc;

  /**
   * @brief Weights of all connections between this layer and the previous one.
   *
   * @details
   *
   * Its dimensions are 2D:(neurons is this layer, neurons in previous layer).
   */
  CBTensor *weights;

  /**
   * @brief Pointer to previous layer.
   */
  struct CBLayer *previous;

  /**
   * @brief Pointer to next layer.
   */
  struct CBLayer *next;
} CBLayer;

/**
 * @brief Creates a new `CBLayer` object.
 *
 * @return
 * - a pointer to `CBLayer` object allocated on heap, `nil` on failure.
 */
CBLayer *CBLayerNew();

/**
 * @brief Deletes the given layer including all consecutive layers.
 *
 * @details
 *
 * Layer 1 ====> Layer 2 ====> Layer 3
 *
 * In this scenario, deleting `Layer 2` will also delete `Layer 3`.
 *
 * @param layerRef reference to the variable that is to be deleted.
 */
void CBLayerDelete(CBLayer **layerRef);

#ifdef __cplusplus
}
#endif

#endif
