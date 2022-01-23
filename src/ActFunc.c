#include "CBrain/ActFunc.h"

#include "CBrain/Algorithm.h"

#include <math.h>

f32 CBActFuncLinear(f32 x) { return x; }

f32 CBActFuncSigmoid(f32 x) { return 1.0F / (1.0F + pow(M_E, -x)); }

f32 CBActFuncTanH(f32 x) { return tanh(x); }

f32 CBActFuncArcTan(f32 x) { return atan(x); }

f32 CBActFuncReLU(f32 x) { return CBAlgoMax(0.0, x); }

f32 CBActFuncLeakyReLU(f32 x) { return CBAlgoMax(0.1F * x, x); }

f32 CBActFuncELU(f32 x, f32 alpha) { return x > 0 ? x : alpha * (pow(M_E, x) - 1); }

#include <stdio.h>

void CBActFuncSoftmax(CBTensor *tensor) {
  f32 *data = CBTensorElemAt(tensor, f32, 0, 0);


  i32 i = 0;
  f32 acc = 0.0F;
  while (i < tensor->shape[0]) {
    acc += pow(M_E, data[i]);
    i += 1;
  }

  i = 0;
  while (i < tensor->shape[0]) {
    data[i] = pow(M_E, data[i]) / acc;
    i += 1;
  }
}

const char *CBActFuncToString(CBActFunc actFunc) {
  switch (actFunc) {
    case CBLAF_Linear:return "Linear";
    case CBLAF_Sigmoid:return "Sigmoid";
    case CBLAF_TanH:return "TanH";
    case CBLAF_ArcTan:return "ArcTan";
    case CBLAF_ReLU:return "ReLU";
    case CBLAF_LeakyReLU:return "LeakyReLU";
    case CBLAF_ELU:return "ELU";
    default: return "Unknown";
  }
}

f32 CBActFuncLinearD(f32 x) { return 1.0F; }

f32 CBActFuncSigmoidD(f32 x) { return CBActFuncSigmoid(x) * (1 - CBActFuncSigmoid(x)); }

f32 CBActFuncTanHD(f32 x) { return 1.0F - (tanh(x) * tanh(x)); }

f32 CBActFuncArcTanD(f32 x) { return 1.0F / (1 + (x * x)); }

f32 CBActFuncReLUD(f32 x) { return CBAlgoMax(0, x); }

f32 CBActFuncLeakyReLUD(f32 x) { return CBAlgoMax(x * 0.1F, x); }

f32 CBActFuncELUD(f32 x, f32 alpha) { return x > 0 ? 1 : alpha * pow(M_E, x); }
