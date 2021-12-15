#include "CBrain/CostFunc.h"

#include <math.h>

f32 CBCostFuncSqError(CBTensor *output, CBTensor *expected) {
  f32 cost = 0.0;
  i32 n = output->shape[0];
  for (i32 i = 0; i < n; i += 1) {
    f32 pX = *CBTensorElemAt(expected, f32, i, 0);
    f32 qX = *CBTensorElemAt(output, f32, i, 0);
    cost += pow(pX - qX, 2);
  }

  return cost;
}

f32 CBCostFuncCrossEntropy(CBTensor *output, CBTensor *expected) {
  f32 cost = 0.0;
  i32 n = output->shape[0];
  for (i32 i = 0; i < n; i += 1) {
    f32 pX = *CBTensorElemAt(expected, f32, i, 0);
    f32 qX = *CBTensorElemAt(output, f32, i, 0);
    cost += pX * log(qX) + ((1 - pX) * log(1 - qX));
  }

  return -cost;
}

f32 CBCostFuncSqErrorD() {

}
