#include "CBrain/Tensor.h"

#include "CBrain/Algorithm.h"

#include "Private/Memory.h"
#include "Private/Tensor.h"

#include <stdarg.h>
#include <string.h>

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
    // Delete `tensor` in case of failure to prevent memory leaks.
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

CBTensor *CBTensorClone(CBTensor *src) {
  if (src == nil) { return nil; }

  CBTensor *tensor = CBTensorNew();

  if (CBTensorCloneInto(tensor, src) == nil) {
    // Delete `tensor` in case of failure to prevent memory leaks.
    CBTensorDelete(&tensor);
    return nil;
  }

  return tensor;
}

CBTensor *CBTensorCloneInto(CBTensor *dest, CBTensor *src) {
  if (dest == nil || src == nil || dest == src) { return nil; }

  // Allocate memory for new shape.
  i32 shapeBufSize = src->dims * sizeof(i32);
  i32 *shape = CBAllocate_Memory(shapeBufSize);
  if (shape == nil) { return nil; }

  // Allocate a new buffer instead of reallocating the old one to prevent loss in case of failure.
  i64 bufSize = CBTensorBufferSize(src);
  u8 *tempBuf = CBAllocate_Memory(bufSize);
  if (tempBuf == nil) {
    // Free shape in case of failure.
    CBFree_Memory(CastTo(&shape, void **));
    return nil;
  }

  // Free old buffer.
  CBNullify_Tensor(dest);

  // Update metadata.
  CBSet_Tensor(dest, src->size, src->dims, shape, tempBuf);

  // Copy shape from source to destination.
  memcpy(dest->shape, src->shape, shapeBufSize);
  // Copy buffer from source to destination.
  memcpy(dest->data, src->data, bufSize);

  return dest;
}

bool CBTensorIsNull(CBTensor *tensor) {
  if (tensor == nil) { return true; }
  // If tensor->shape is `nil`, `tensor->data` will always be `nil`.
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

  // Check if given index(es) are valid.
  if (!CBIsIndexValidV_Tensor(tensor, list)) { return nil; }

  i64 index = 0;

  // Calculate linear index based on given index(es) for an n-dimensional tensor.
  // This works by narrowing down the dimensions from higher to lower.
  // This is derived from `(y * width) + x` which works for 2-dimensions only.
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

  // Convert variadic list to an integer array.
  va_list list = {};
  va_start(list, dims);
  i32 *shape = CBGetShapeV_Tensor(dims, list);
  va_end(list);

  if (shape == nil) { return nil; }

  // Reshape only if new shape is compatible with the old one.
  // i.e. the total number of elements remain the same.
  if (!CBIsShapeCompatible_Tensor(tensor, dims, shape)) { return nil; }
  CBSetShape_Tensor(tensor, dims, shape);

  return tensor;
}

CBTensor *CBTensorFromImage(CBImage *img, i32 dims, ...) {
  if (img == nil) { return nil; }

  CBTensor *tensor = CBTensorNew();
  if (tensor == nil) { return nil; }

  va_list list = {};
  va_start(list, dims);
  if (CBFromDimsIntoV_Tensor(tensor, sizeof(f32), dims, list) == nil) {
    // Delete `tensor` in case of failure to prevent memory leaks.
    CBTensorDelete(&tensor);
    return nil;
  }
  va_end(list);

  i32 bufSize = CBImagePixCount(img);

  i32 i = 0;
  while (i < bufSize) {
    *CBTensorElemAt(tensor, f32, i, 0) = CastTo(*CBImageLinearAt(img, i), f32);
    i += 1;
  }

  return tensor;
}

CBTensor *CBTensorFlatten(CBImage *img) {
  return CBTensorFromImage(img, 2, CBImagePixCount(img), 1);
}

CBTensor *CBTensorMatMultiply(CBTensor *a, CBTensor *b) {
  // a->shape[0] => r1
  // a->shape[1] => c1
  // b->shape[0] => r2
  // b->shape[1] => c2

  // Cols of first matrix must be equal to rows of second matrix.
  if (a->shape[1] != b->shape[0]) { return nil; }

  // Dimensions of new matrix will be (r1, c2).
  CBTensor *product = CBTensorFromDims(a->size, 2, a->shape[0], b->shape[1]);

  // c1 == r2
  i32 commonIndex = a->shape[1];

  for (i32 i = 0; i < a->shape[0]; i += 1) {
    for (i32 j = 0; j < b->shape[1]; j += 1) {
      for (i32 k = 0; k < commonIndex; k += 1) {
        // i represents row, j represents column, k is an accumulator index.
        *CBTensorElemAt(product, f32, i, j) += (*CBTensorElemAt(a, f32, i, k)) * (*CBTensorElemAt(b, f32, k, j));
      }
    }
  }

  return product;
}
