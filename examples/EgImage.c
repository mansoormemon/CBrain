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

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage:\n"
           "\t%s [in_image] [out_img]\n", argv[0]);
    return -1;
  }

  const char *in = argv[1];
  const char *out = argv[2];

  printf("Attempting to read image: %s\n", in);
  CBImage *img = CBImageRead(in);

  if (img == nil) {
    printf("Unable to read file: %s\n", in);
    return -1;
  }

  printf("Read successful!\n");

  printf("Width: %d, Height: %d, Channels: %d\n", img->width, img->height, img->channels);

  printf("Extracting sub-image...\n");
  CBImage *sub = CBImageSubImage(img, 0, 0, img->width / 2, img->height / 2);

  if (sub == nil) {
    printf("Error: Failed to extract sub-image!\n");
    return -1;
  }

  printf("New dimensions => Width: %d, Height: %d, Channels: %d\n", sub->width, sub->height, sub->channels);

  if (CBImageWrite(sub, out)) {
    printf("Write successful: %s\n", out);
  } else {
    printf("Unable to write file: %s\n", out);
  }

  CBImageDelete(&img);
  CBImageDelete(&sub);
  return 0;
}
