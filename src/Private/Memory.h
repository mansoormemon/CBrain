// Copyright 2021 CBrain
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// Copyright (c) 2021 Mansoor Memon <mansoorahmed.one@gmail.com>

#ifndef CBRAIN__PRIVATE__MEMORY_H_
#define CBRAIN__PRIVATE__MEMORY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "CBrain/Types.h"

static bool reallocateMemory__(void **tgtPtr, u64 size) {
  if (tgtPtr == nil) { return false; }

  void *tempPtr = realloc(*tgtPtr, size);
  if (tempPtr == nil) { return false; }

  *tgtPtr = tempPtr;

  return true;
}

static void freeMemory__(void **tgtPtr) {
  if (tgtPtr != nil) {
    free(*tgtPtr);
    *tgtPtr = nil;
  }
}

#ifdef __cplusplus
}
#endif

#endif
