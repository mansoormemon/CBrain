#include "NeuralNet.h"

bool CBSetMetaInfo_NeuralNet(CBNeuralNet *net, i32 inputSize) {
  if (net == nil || inputSize < 0) { return false; }

  net->inputSize = inputSize;
  return true;
}
