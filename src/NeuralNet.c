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

CBLayer *CBNeuralNetAddLayer(CBNeuralNet *net, i32 neurons, f32 bias, CBActFunc actFunc, i32 seed) {
  if (net == nil || neurons < 0 || CastTo(actFunc, i32) < 0 || CastTo(actFunc, i32) >= CBLAF_MAX__) {
    return nil;
  }

  CBLayer *layer = CBLayerNew();
  if (layer == nil) { return nil; }

  CBSetMetaInfo_Layer(layer, neurons, bias, actFunc);

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
      switch (layer->actFunc) {
        case CBLAF_Linear: {
          *x = CBActFuncLinear(*x + layer->bias);
          break;
        }
        case CBLAF_Sigmoid: {
          *x = CBActFuncSigmoid(*x + layer->bias);
          break;
        }
        case CBLAF_TanH: {
          *x = CBActFuncTanH(*x + layer->bias);
          break;
        }
        case CBLAF_ArcTan: {
          *x = CBActFuncArcTan(*x + layer->bias);
          break;
        }
        case CBLAF_ReLU: {
          *x = CBActFuncReLU(*x + layer->bias);
          break;
        }
        case CBLAF_LeakyReLU: {
          *x = CBActFuncLeakyReLU(*x + layer->bias);
          break;
        }
        case CBLAF_ELU: {
          *x = CBActFuncELU(*x + layer->bias, 1);
          break;
        }
        default: {
          *x = *x + layer->bias;
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
      "%-10s%-20s%-15s%-12s%-25s%-15s\n"
      "=================================================================================================\n",
      "Layer", "No. Of Neurons", "Type", "Bias", "Activation Function", "Shape (Weights)");

  i32 i = 0;
  printf("PL_%-7d%-20d%-15s%-12c%-25c%-15c\n"
         "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n",
         i, net->inputSize, "Input", '-', '-', '-');

  CBLayer *layer = net->begin;

  while (layer != nil) {
    i += 1;
    printf(
        "L_%-8d%-20d%-15s%-12.4f%-25s(%dx%d)\n"
        "-------------------------------------------------------------------------------------------------\n",
        i,
        layer->neurons,
        "Dense",
        layer->bias,
        CBActFuncToString(layer->actFunc),
        layer->weights->shape[0],
        layer->weights->shape[1]);

    layer = layer->next;
  }

  printf("Loss Function: N/A\n"
         "Optimizer: N/A\n");

  printf("=================================================================================================\n");
}
