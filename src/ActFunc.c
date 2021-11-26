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
