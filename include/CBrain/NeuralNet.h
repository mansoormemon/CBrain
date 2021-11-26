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

#ifndef CBRAIN__NEURAL_NET_H_
#define CBRAIN__NEURAL_NET_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ActFunc.h"
#include "Layer.h"
#include "Tensor.h"
#include "Types.h"

/**
 * @file NeuralNet.h
 *
 * @brief Contains functionality to work with a neural network.
 *
 * @since v0.0.1.
 */

/**
 * @brief A simple feed forward neural network.
 *
 * ## Neural Network's Architecture
 * A neural network consists of three types of layers:
 *
 *  1. Input layer - the first layer of the neural network.
 *  2. Hidden layer - the non-observable layer(s) of a neural network.
 *  3. Output layer - the last layer of the neural network.
 *
 * Given below is a bird's-eye view of the architecture of a neural network.
 *
 * @image html "NeuralNetArchitecture.svg"
 *
 * ### Diving In
 *
 * By zooming in between the layers, we can observe that the layers are connected deeply, and every
 * individual connection has some weight incorporated with it. See the given figure.
 *
 * @image html "Layers.svg"
 *
 * ### Anatomy of a neuron
 *
 * A neuron is a composite function that consists of two parts:
 * 1. A summation function
 * 2. An activation function
 *
 * @image html "Neuron.svg"
 *
 * It is time to explore the underlying mathematics of how neural networks work.
 *
 * @image html "Formula_g(x).svg"
 *
 * @image html "Summation.svg" "Summation"
 *
 * The output of the summation function `g(x)` is passed to the activation function as `f(g(x))`.
 *
 * @image html "ActFunc.svg" "Activation Function"
 *
 * Now, the output of `f(g(x))` becomes the input for the next layer and the process repeats.
 *
 * @cond
 * Note: All the images can be found in the `res` directory.
 * @endcond
 *
 * @see `CBActFunc`
 */
typedef struct CBNeuralNetwork {
  /**
   * @brief Number of neurons in the input layer.
   *
   * @note Input layer is a `Pseudo Layer` i.e. it acts as a placeholder for the input data.
   * It is not a part of the actual architecture.
   */
  i32 inputSize;

  /**
   * @brief First layer of the network.
   *
   * @note This is not the input layer.
   */
  CBLayer *begin;
} CBNeuralNet;

/**
 * @brief Creates a new `CBNeuralNet` object.
 *
 * @return
 * - a pointer to `CBNeuralNet` object allocated on heap, `nil` on failure.
 */
CBNeuralNet *CBNeuralNetNew();

/**
 * @brief Deletes the given neural net.
 *
 * @param netRef reference to the variable that is to be deleted.
 */
void CBNeuralNetDelete(CBNeuralNet **netRef);

/**
 * @brief Creates a new `CBNeuralNet` object from the given params.
 *
 * @param inputSize  number of neurons in the input layer.
 * @return
 * - a pointer to `CBNeuralNet` object allocated on heap, `nil` on failure.
 */
CBNeuralNet *CBNeuralNetFrom(i32 inputSize);

/**
 * @brief Returns first layer of the neural network.
 *
 * @param net target network.
 * @return
 * - `net->begin` on success, `nil` otherwise.
 */
CBLayer *CBNeuralNetBegin(CBNeuralNet *net);

/**
 * @brief Returns last layer of the neural network.
 *
 * @param net target network.
 * @return
 * - last layer on success, `nil` otherwise.
 */
CBLayer *CBNeuralNetEnd(CBNeuralNet *net);

/**
 * @brief Checks if neural network is empty.
 *
 * @param net target network.
 * @return
 * - `true` if:
 *  - `net` is `nil`.
 *  - `net->begin` is `nil`.
 * - `false` otherwise.
 */
bool CBNeuralNetIsEmpty(CBNeuralNet *net);

/**
 * @brief Adds a layer at the end of a neural network.
 *
 * @param net target network.
 * @param neurons number of neurons in the layer.
 * @param bias bias for the layer.
 * @param actFunc activation function that will be applied to all neurons of that layer.
 * @param seed seed for initializing random weights.
 * @return
 * - the layer appended to the neural network, `nil` on failure.
 */
CBLayer *CBNeuralNetAddLayer(CBNeuralNet *net, i32 neurons, f32 bias, CBActFunc actFunc, i32 seed);

/**
 * @brief Makes predictions based on the given input data.
 *
 * @param net target network.
 * @param input input data.
 * @return
 * - Output layer on success, `nil` otherwise.
 */
CBTensor *CBNeuralNetPredict(CBNeuralNet *net, CBTensor *input);

/**
 * @brief Prints network summary.
 *
 * @param net target network.
 */
void CBNeuralNetSummary(CBNeuralNet *net);

#ifdef __cplusplus
}
#endif

#endif
