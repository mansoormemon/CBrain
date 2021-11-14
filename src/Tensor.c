#include "CBrain/Tensor.h"

#include "CBrain/Algorithm.h"

#include "Private/Memory.h"
#include "Private/Tensor.h"

#include <stdarg.h>

CBTensor *CBTensorNew() {
  return CBAllocate_Memory(sizeof(CBTensor));
}

CBTensor *CBTensorReset(CBTensor *tensor) {
  if (tensor != nil) {
    CBNullify_Tensor(tensor);
    CBSet_Tensor(tensor, 0, 0, nil, nil);
  }

  return tensor;
}

void CBTensorDelete(CBTensor **tensorRef) {
  if (tensorRef != nil) {
    CBTensorReset(*tensorRef);
    CBFree_Memory(CastTo(tensorRef, void **));
  }
}

CBTensor *CBTensorFromDims(i32 size, i32 dims, ...) {
  if (size <= 0 || dims <= 0) { return nil; }

  CBTensor *tensor = CBTensorNew();

  va_list list = {};
  va_start(list, dims);
  if (CBFromDimsIntoV_Tensor(tensor, size, dims, list) == nil) {
    CBTensorDelete(&tensor);
    return nil;
  }
  va_end(list);

  return tensor;
}

CBTensor *CBTensorFromDimsInto(CBTensor *dest, i32 size, i32 dims, ...) {
  va_list list = {};
  va_start(list, dims);
  CBFromDimsIntoV_Tensor(dest, size, dims, list);
  va_end(list);

  return dest;
}

bool CBTensorIsNull(CBTensor *tensor) {
  if (tensor == nil) { return true; }
  return tensor->shape == nil || tensor->data == nil;
}

i64 CBTensorTotal(CBTensor *tensor) {
  if (tensor == nil) { return -1; }
  if (CBTensorIsNull(tensor)) { return -1; }

  return CBAlgoMultiply(tensor->shape, tensor->dims);
}

i64 CBTensorBufferSize(CBTensor *tensor) {
  if (tensor == nil) { return -1; }
  if (CBTensorIsNull(tensor)) { return -1; }

  return CBTensorTotal(tensor) * tensor->size;
}

void *CBTensorAt(CBTensor *tensor, ...) {
  if (tensor == nil) { return nil; }

  va_list list = {};
  va_start(list, tensor);

  if (!CBIsIndexValidV_Tensor(tensor, list)) { return nil; }

  i64 index = 0;

  i32 i = 1, higherDI = 0;
  while (i < tensor->dims) {
    higherDI = va_arg(list, i32);
    i64 lowerDSProduct = CBAlgoMultiply(tensor->shape + i, tensor->dims - i);
    index += higherDI * lowerDSProduct;
    i += 1;
  }
  i32 lastDI = va_arg(list, i32);
  index += lastDI;
  va_end(list);

  return tensor->data + (index * tensor->size);
}

CBTensor *CBTensorReshape(CBTensor *tensor, i32 dims, ...) {
  if (tensor == nil || dims <= 0) { return nil; }

  va_list list = {};
  va_start(list, dims);
  i32 *shape = CBGetShapeV_Tensor(dims, list);
  va_end(list);

  if (shape == nil) { return nil; }

  if (!CBIsShapeCompatible_Tensor(tensor, dims, shape)) { return nil; }

  CBSetShape_Tensor(tensor, dims, shape);

  return tensor;
}
