#include "Tensor.h"

#include "Memory.h"

#include "CBrain/Algorithm.h"
#include "CBrain/Assert.h"

CBTensor *CBFromDimsIntoV_Tensor(CBTensor *dest, i32 size, i32 dims, va_list list) {
  if (dest == nil || size <= 0 || dims <= 0 || list == nil) { return nil; }

  i32 *shape = CBGetShapeV_Tensor(dims, list);
  if (shape == nil) { return nil; }

  i64 bufSize = CBAlgoMultiply(shape, dims) * size;
  void *tempBuf = CBAllocate_Memory(bufSize);
  if (tempBuf == nil) { return nil; }

  CBNullify_Tensor(dest);
  CBSet_Tensor(dest, size, dims, shape, tempBuf);

  return dest;
}

bool CBSet_Tensor(CBTensor *tensor, i32 size, i32 dims, i32 *shape, void *data) {
  if (tensor == nil || size < 0 || dims < 0) { return false; }

  tensor->size = size;
  tensor->dims = dims;
  tensor->shape = shape;
  tensor->data = data;

  return true;
}

bool CBSetShape_Tensor(CBTensor *tensor, i32 dims, i32 *shape) {
  if (tensor == nil || dims < 0) { return false; }

  CBFree_Memory(CastTo(&tensor->shape, void **));

  tensor->dims = dims;
  tensor->shape = shape;

  return true;
}

i32 *CBGetShapeV_Tensor(i32 dims, va_list list) {
  if (dims <= 0 || list == nil) { return nil; }

  i32 *shape = CBAllocate_Memory(dims * sizeof(i32));

  if (shape == nil) { return nil; }

  i32 i = 0;
  while (i < dims) {
    shape[i] = va_arg(list, i32);
    CBAssert(shape[i] > 0, "`shape[%d](=%d)` must be greater than 0!", i, shape[i]);
    i += 1;
  }

  return shape;
}

CBTensor *CBNullify_Tensor(CBTensor *tensor) {
  if (tensor != nil) {
    CBFree_Memory(CastTo(&(tensor->shape), void **));
    CBFree_Memory(CastTo(&(tensor->data), void **));
  }

  return tensor;
}

bool CBIsIndexValidV_Tensor(CBTensor *tensor, va_list list) {
  va_list copy = {};
  va_copy(copy, list);

  i32 i = 0;
  while (i < tensor->dims) {
    i32 index = va_arg(copy, i32);
    if (index >= tensor->shape[i]) { return false; }
    i += 1;
  }
  return true;
}

bool CBIsShapeCompatible_Tensor(CBTensor *tensor, i32 dims, i32 *shape) {
  if (tensor == nil) { return false; }

  i32 tgtBufSize = CBAlgoMultiply(shape, dims);

  return CBTensorTotal(tensor) == tgtBufSize;
}
