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

#include "CBrain/Types.h"

void *CBAllocate_Memory(u64 size);
bool CBReallocate_Memory(void **tgtPtr, u64 size);
void CBFree_Memory(void **tgtPtr);

#ifdef __cplusplus
}
#endif

#endif
