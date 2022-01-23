#include "CBrain/NeuralNet.h"

#include "Private/Layer.h"
#include "Private/Memory.h"
#include "Private/NeuralNet.h"

#include <stdio.h>

CBNeuralNet *CBNeuralNetNew() {
  return CBAllocate_Memory(sizeof(CBNeuralNet));
}

CBNeuralNet *CBNeuralNetReset(CBNeuralNet *net) {
  if (net != nil) {
    CBLayerDelete(&net->begin);
    CBSetMetaInfo_NeuralNet(net, 0);
  }

  return net;
}

void CBNeuralNetDelete(CBNeuralNet **netRef) {
  if (netRef != nil) {
    CBNeuralNetReset(*netRef);
    CBFree_Memory(CastTo(netRef, void **));
  }
}

CBNeuralNet *CBNeuralNetFrom(i32 inputSize) {
  CBNeuralNet *net = CBNeuralNetNew();
  net->inputSize = inputSize;
  return net;
}

CBLayer *CBNeuralNetBegin(CBNeuralNet *net) {
  if (net == nil) { return nil; }
  return net->begin;
}

CBLayer *CBNeuralNetEnd(CBNeuralNet *net) {
  if (net == nil) { return nil; }

  CBLayer *end = net->begin;
  if (end == nil) { return nil; }

  CBLayer *layer = net->begin->next;
  while (layer != nil) {
    end = layer;
    layer = layer->next;
  };

  return end;
}

bool CBNeuralNetIsEmpty(CBNeuralNet *net) {
  if (net == nil) { return true; }
  return net->begin == nil;
}

CBLayer *CBNeuralNetAddLayer(CBNeuralNet *net, i32 neurons, CBActFunc actFunc, i32 seed) {
  if (net == nil || neurons < 0 || CastTo(actFunc, i32) < 0 || CastTo(actFunc, i32) >= CBLAF_MAX__) {
    return nil;
  }

  CBLayer *layer = CBLayerNew();
  if (layer == nil) { return nil; }

  CBSetMetaInfo_Layer(layer, neurons, actFunc);

  i32 prevNeurons = 0;

  // If neural network is empty then set `net->begin` otherwise append the layer at the end of the list.
  if (CBNeuralNetIsEmpty(net)) {
    prevNeurons = net->inputSize;
    net->begin = layer;
  } else {
    CBLayer *end = CBNeuralNetEnd(net);
    prevNeurons = end->neurons;
    end->next = layer;
    layer->previous = end;
  }

  // Initialize random weights based on given seed.
  if (!CBInitializeWeights_Layer(layer, prevNeurons, seed)) {
    CBLayerDelete(&layer);
    return nil;
  }

  if (!CBInitializeBiases_Layer(layer, seed)) {
    CBLayerDelete(&layer);
    return nil;
  }

  return layer;
}

CBTensor *CBNeuralNetPredict(CBNeuralNet *net, CBTensor *input, bool softmax) {
  if (net == nil || input == nil) { return nil; }

  CBTensor *output = CBTensorClone(input);

  CBLayer *layer = CBNeuralNetBegin(net);

  while (layer != nil) {
    // Multiply weights with input.
    CBTensor *temp = CBTensorMatMultiply(layer->weights, output);
    CBTensorDelete(&output);
    output = temp;

    // Apply activation function.
    for (i32 i = 0; i < output->shape[0]; i += 1) {
      f32 *x = CBTensorElemAt(output, f32, i, 0);
      f32 *b = CBTensorElemAt(layer->biases, f32, i);

      switch (layer->actFunc) {
        case CBLAF_Linear: {
          *x = CBActFuncLinear(*x + *b);
          break;
        }
        case CBLAF_Sigmoid: {
          *x = CBActFuncSigmoid(*x + *b);
          break;
        }
        case CBLAF_TanH: {
          *x = CBActFuncTanH(*x + *b);
          break;
        }
        case CBLAF_ArcTan: {
          *x = CBActFuncArcTan(*x + *b);
          break;
        }
        case CBLAF_ReLU: {
          *x = CBActFuncReLU(*x + *b);
          break;
        }
        case CBLAF_LeakyReLU: {
          *x = CBActFuncLeakyReLU(*x + *b);
          break;
        }
        case CBLAF_ELU: {
          *x = CBActFuncELU(*x + *b, 1);
          break;
        }
        default: {
          *x = *x + *b;
          break;
        }
      }
    }
    layer = layer->next;
  }

  if (softmax) {
    CBActFuncSoftmax(output);
  }

  return output;
}

void CBNeuralNetSummary(CBNeuralNet *net) {
  if (net == nil) {
    printf("(nil)\n");
    return;
  }

  printf("Model Summary:\n");

  printf(
      "=================================================================================================\n"
      "%-10s%-20s%-15s%-25s%-15s\n"
      "=================================================================================================\n",
      "Layer", "No. Of Neurons", "Type", "Activation Function", "Shape (Weights)");

  i32 i = 0;
  printf("PL_%-7d%-20d%-15s%-25c%-15c\n"
         "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",
         i, net->inputSize, "Input", '-', '-');

  CBLayer *layer = net->begin;

  while (layer != nil) {
    i += 1;
    printf(
        "L_%-8d%-20d%-15s%-25s(%dx%d)\n"
        "-------------------------------------------------------------------------------------------------\n",
        i,
        layer->neurons,
        "Dense",
        CBActFuncToString(layer->actFunc),
        layer->weights->shape[0],
        layer->weights->shape[1]);

    layer = layer->next;
  }

  printf("Total Parameters: N/A\n");
  printf("=================================================================================================\n");
}

i32 CBFindMax(CBTensor *tensor) {
  i32 index = 0;

  f32 max = *CBTensorElemAt(tensor, f32, 0, 0);

  for (i32 i = 1; i < tensor->shape[0]; i += 1) {
    f32 val = *CBTensorElemAt(tensor, f32, i, 0);
    if (val > max) {
      max = val;
      index = i;
    }
  }

  return index;
}

void CBNeuralNetTrain(CBNeuralNet *net,
                      f32 learningRate,
                      i32 batchSize,
                      CBCostFunc costFunc,
                      i32 epochs,
                      CBTensor *trainData,
                      CBTensor *trainLabels) {
  if (net == nil || trainData == nil || trainLabels == nil) { return; }

  i32 totalBatches = trainData->shape[0] / batchSize;
  printf("Batch Size: %d, Total Batches: %d, Dropped samples: %d\n",
         batchSize,
         totalBatches,
         trainData->shape[0] - (batchSize * totalBatches));

  // Shape of tensor.
  i32 arr[] = {trainData->shape[1], 1};

  CBTensor in = {};
  in.dims = 2;
  in.size = sizeof(f32);
  in.shape = arr;

  // epoch
  for (i32 e = 0; e < epochs; e += 1) {
    i32 index = 0;
    printf("Epoch %d of %d:\n", e + 1, epochs);
    // batch
    for (i32 b = 0; b < totalBatches; b += 1) {
      f32 costAcc = 0.0F;
      i32 correct = 0;
      // sample
      for (i32 s = 0; s < batchSize; s += 1) {
        in.data = CBTensorElemAt(trainData, f32, index, 0);

        // Forward Pass
        CBTensor *output = CBNeuralNetPredict(net, &in, true);
        i32 oClass = CBFindMax(output);

        // Fetch label/class for sample.
        i32 eClass = *CBTensorElemAt(trainLabels, u8, index, 0);

        // Increment counter for correct predictions.
        correct += eClass == oClass;

        // Generate one-hot true vector of probabilities i.e. expected class will
        // have a probability of '1.0' whereas others will be '0.0'.
        CBTensor *expected = CBTensorFrom(f32, 2, output->shape[0], output->shape[1]);
        *CBTensorElemAt(expected, f32, eClass, 0) = 1;

        // Calculate individual cost and accumulate result to find mean later on.
        switch (costFunc) {
          case CBCF_MeanSqError: {
            costAcc += CBCostFuncSqError(output, expected);
            break;
          }
          case CBCF_MeanCatCrossEntropy: {
            // Only pass the output probability of expected class.
            costAcc += CBCostFuncCategoricalCrossEntropy(output, expected);
            break;
          }
          default: { break; }
        }

        CBTensorDelete(&expected);

        index += 1;
      }

      // Calculate mean.
      f32 meanCost = costAcc / batchSize;

      // Accuracy (correct_guesses / total).
      f32 accuracy = (f32) correct / batchSize;
      printf("Batch # %d/%d => cost=%.4f, accuracy=%.4f (%d)\n",
             b + 1,
             totalBatches,
             meanCost,
             accuracy,
             correct);

      // Todo: Implement gradient descent
      // here after mini-batch cost calculation.

    } // batches
  } // epoch
} // function
