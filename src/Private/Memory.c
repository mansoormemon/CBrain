#include "Memory.h"

#include <stdlib.h>
#include <string.h>

void *CBAllocate_Memory(u64 size) {
  void *ptr = nil;
  if (!CBReallocate_Memory(&ptr, size)) { return nil; }
  memset(ptr, 0, size);
  return ptr;
}

bool CBReallocate_Memory(void **tgtPtr, u64 size) {
  if (tgtPtr == nil) { return false; }

  void *tempPtr = realloc(*tgtPtr, size);
  if (tempPtr == nil) { return false; }

  *tgtPtr = tempPtr;

  return true;
}

void CBFree_Memory(void **tgtPtr) {
  if (tgtPtr != nil) {
    free(*tgtPtr);
    *tgtPtr = nil;
  }
}
