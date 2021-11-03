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

#ifndef CBRAIN__IMAGE_H_
#define CBRAIN__IMAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"

typedef struct CBImage {
  i32 width;
  i32 height;
  i32 channels;
  u8 *data;
} CBImage;

CBImage *CBImageNew();

CBImage *CBImageNullify(CBImage *img);
void CBImageDelete(CBImage **imgRef);

CBImage *CBImageRead(const char *pathToImg);
CBImage *CBImageReadInto(CBImage *img, const char *pathToImg);

bool CBImageWrite(CBImage *img, const char *pathToImg);

#ifdef __cplusplus
}
#endif

#endif
