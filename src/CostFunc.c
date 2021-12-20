#include "CBrain/CostFunc.h"

#include <math.h>

f32 CBCostFuncSqError(CBTensor *output, CBTensor *expected) {
  // Apply root mean square error to every class.

  f32 cost = 0.0;
  i32 n = output->shape[0];
  for (i32 i = 0; i < n; i += 1) {
    f32 pX = *CBTensorElemAt(expected, f32, i, 0);
    f32 qX = *CBTensorElemAt(output, f32, i, 0);
    cost += pow(pX - qX, 2);
  }

  return cost;
}

f32 CBCostFuncCrossEntropy(f32 oProbofEClass) {
  // Only consider output probability expected class probability
  // https://towardsdatascience.com/cross-entropy-for-classification-d98e7f974451
  return -log(oProbofEClass);
}

f32 CBCostFuncSqErrorD() {
  // Todo: implement
  return 0;
}
