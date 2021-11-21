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

/**
 * @brief Allocates new memory.
 *
 * @param size memory (in bytes) to allocate.
 * @return
 * - pointer to allocated memory, `nil` in case of failure.
 */
void *CBAllocate_Memory(u64 size);

/**
 * @brief Reallocates memory for a given location.
 *
 * @param tgtPtr reference to target pointer.
 * @param size new memory (in bytes) to reallocate.
 * @return
 * - pointer to reallocated memory, `nil` in case of failure.
 *
 * @note In case of failure, target pointer will remain unchanged.
 */
bool CBReallocate_Memory(void **tgtPtr, u64 size);

/**
 * @brief Frees memory.
 *
 * @param tgtPtr reference to target pointer.
 *
 * @note Upon successful completion, the target pointer is set to `nil`.
 */
void CBFree_Memory(void **tgtPtr);

#ifdef __cplusplus
}
#endif

#endif
