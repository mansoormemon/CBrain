#include "Memory.h"

#include <stdlib.h>

bool CBreallocateMemory__(void **tgtPtr, u64 size) {
  if (tgtPtr == nil) { return false; }

  void *tempPtr = realloc(*tgtPtr, size);
  if (tempPtr == nil) { return false; }

  *tgtPtr = tempPtr;

  return true;
}

void CBfreeMemory__(void **tgtPtr) {
  if (tgtPtr != nil) {
    free(*tgtPtr);
    *tgtPtr = nil;
  }
}
