#include "CBrain/Assert.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

void CBAssert_M(bool eval,
                const char *evalExpr,
                i32 line,
                const char *filename,
                const char *func,
                const char *fmt, ...) {
  if (!eval) {
    fprintf(stderr, "%s:%d: %s: Assertion `%s` failed!\n", filename, line, func, evalExpr);
    if (fmt != nil) {
      fprintf(stderr, "Description: ");
      va_list list = {};
      va_start(list, fmt);
      vfprintf(stderr, fmt, list);
      va_end(list);
      fprintf(stderr, "\n");
    }
    abort();
  }
}
