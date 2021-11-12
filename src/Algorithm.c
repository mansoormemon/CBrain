#include "CBrain/Algorithm.h"

u8 CBAlgoAvg_u8(u8 *arr, i32 size) {
  return CastTo(CBAlgoAccumulate(arr, size) / size, u8);
}

u32 CBAlgoAvg_u32(u32 *arr, i32 size) {
  return CastTo(CBAlgoAccumulate(arr, size) / size, u32);
}

i32 CBAlgoAvg_i32(i32 *arr, i32 size) {
  return CastTo(CBAlgoAccumulate(arr, size) / size, i32);
}

u32 CBAlgoAccumulate_u8(u8 *arr, i32 size) {
  i32 i = 0;
  u32 acc = 0;
  while (i < size) {
    acc += arr[i];
    i += 1;
  }

  return acc;
}

u64 CBAlgoAccumulate_u32(u32 *arr, i32 size) {
  i32 i = 0;
  u64 acc = 0;
  while (i < size) {
    acc += arr[i];
    i += 1;
  }

  return acc;
}

i64 CBAlgoAccumulate_i32(i32 *arr, i32 size) {
  i32 i = 0;
  i64 acc = 0;
  while (i < size) {
    acc += arr[i];
    i += 1;
  }

  return acc;
}
