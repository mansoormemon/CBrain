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

#include <CBrain/CBrain.h>
#include <stdio.h>

#define debug(tensor) debugPrint(tensor, #tensor)

void debugPrint(CBTensor *tensor, const char *name) {
  printf("%s: [size=%d, dims=%d, total=%ld, bufferSize=%ld, shape=(",
         name,
         tensor->size,
         tensor->dims,
         CBTensorTotal(tensor),
         CBTensorBufferSize(tensor));

  i32 i = 0;
  for (i = 0; i < tensor->dims; i += 1) {
    printf("%d", tensor->shape[i]);
    if (i != tensor->dims - 1) {
      printf("x");
    }
  }
  printf(")]\n");
}

int main() {
  printf("Creating tensor...\n");
  CBTensor *tensor = CBTensorFrom(i32, 3, 28, 28, 1);

  debug(tensor);

  printf("Reshaping tensor...\n");
  CBTensorReshape(tensor, 5, 784, 1, 1, 1, 1);
  debug(tensor);

  printf("Morphing tensor...\n");
  CBTensorFromInto(tensor, f64, 1, 20);
  debug(tensor);

  CBTensorDelete(&tensor);
  return 0;
}
