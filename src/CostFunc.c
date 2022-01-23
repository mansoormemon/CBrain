#include "CBrain/CostFunc.h"

#include <math.h>

f32 CBCostFuncSqError(CBTensor *output, CBTensor *expected) {
  // Apply root mean square error to every class.

  f32 cost = 0.0F;
  i32 n = output->shape[0];
  for (i32 i = 0; i < n; i += 1) {
    f32 pX = *CBTensorElemAt(expected, f32, i, 0);
    f32 qX = *CBTensorElemAt(output, f32, i, 0);
    cost += (pX - qX) * (pX - qX);
  }

  return cost;
}

f32 CBCostFuncCategoricalCrossEntropy(CBTensor *output, CBTensor *expected) {
  f32 cost = 0.0F;
  i32 n = output->shape[0];
  for (i32 i = 0; i < n; i += 1) {
    f32 yi = *CBTensorElemAt(expected, f32, i, 0); // y
    f32 si = *CBTensorElemAt(output, f32, i, 0); // y hat
    cost -= yi * log(si);
  }

  return cost;
}

f32 CBCostFuncSqErrorD(CBTensor *output, CBTensor *expected) {
  return 0.0F;
}

f32 CBCostFuncCategoricalCrossEntropyD(CBTensor *output, CBTensor *expected) {

  return 0.0F;
}
