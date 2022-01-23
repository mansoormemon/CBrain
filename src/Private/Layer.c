#include "Layer.h"

#include "Memory.h"

#include "CBrain/Algorithm.h"
#include "CBrain/Tensor.h"

bool CBSetMetaInfo_Layer(CBLayer *layer, i32 neurons, CBActFunc actFunc) {
  if (layer == nil || neurons < 0 || CastTo(actFunc, i32) < 0 || CastTo(actFunc, i32) >= CBLAF_MAX__) {
    return false;
  }

  layer->neurons = neurons;
  layer->actFunc = actFunc;

  return true;
}

bool CBInitializeWeights_Layer(CBLayer *layer, i32 previousLayerNeurons, i32 seed) {

  //   L1       L2
  // [ A ]    [ N1 ]
  // [ B ]    [ N2 ]
  // [ C ]    [ N3 ]
  //           <b>
  //
  // For the above two layers:
  // L1 => input layer
  // L2 => target layer
  //
  // Formula: L2 = W * L1 + b
  // where,
  //  W => 2D tensor of weights.
  // L1 => 1D tensor of inputs.
  // L2 => 1D tensor of outputs.
  // b  => bias for layer L2.
  //
  // See `CBNeuralNet` documentation for more details.

  layer->weights = CBTensorFrom(f32, 2, layer->neurons, previousLayerNeurons);

  if (layer->weights == nil) { return false; }

  CBAlgoSeedRandom(seed);

  i32 i = 0, j = 0;
  for (i = 0; i < layer->neurons; i += 1) {
    for (j = 0; j < previousLayerNeurons; j += 1) {
      *CBTensorElemAt(layer->weights, f32, i, j) = CBAlgoRandomBetween(-0.1F, 0.1F);
    }
  }

  return true;
}

bool CBInitializeBiases_Layer(CBLayer *layer, i32 seed) {
  layer->biases = CBTensorFrom(f32, 1, layer->neurons);
  if (layer->biases == nil) {return false;}

  CBAlgoSeedRandom(seed);
  i32 i = 0;
  for (i = 0; i < layer->neurons; i += 1) {
    *CBTensorElemAt(layer->biases, f32, i) = CBAlgoRandomBetween(-0.1F, 0.1F);
  }

  return true;
}

CBLayer *CBNullify_Layer(CBLayer *layer) {
  // Delete weights tensor.
  if (layer != nil) {
    CBTensorDelete(&layer->weights);
    CBTensorDelete(&layer->biases);
  }

  return layer;
}

CBLayer *CBReset_Layer(CBLayer *layer) {
  if (layer != nil) {
    // Delete each consecutive layer recursively.
    // Base case: Last layer.
    CBLayerDelete(&layer->next);
    // Nullify layer after recursion ends.
    CBNullify_Layer(layer);
    // Reset all values.
    CBSetMetaInfo_Layer(layer, 0, CBLAF_Linear);
  }
  return layer;
}
