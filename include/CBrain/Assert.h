// Copyright 2021 CBrain
// Project URL: https://github.com/mansoormemon/CBrain
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

#ifndef CBRAIN__ASSERT_H_
#define CBRAIN__ASSERT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"

#include <stdarg.h>

#define CBAssert(eval, ...) (CBAssert_M(eval, #eval, __LINE__, __FILE__, __func__, __VA_ARGS__))

void CBAssert_M(bool eval,
                const char *evalExpr,
                i32 line,
                const char *filename,
                const char *func,
                const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
