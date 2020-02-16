#include "test.h"
enum { Iterations = 2000000000 };

volatile int counter;

namespace standard {
  #undef __cplusplus
  #undef LIBCO_C
  #undef LIBCO_H
  #include "../libco.c"
  #define __cplusplus

  cothread_t x, y;
  void co_timingtest() {
    for(;;) {
      counter++;
      co_switch(x);
    }
  }
}
namespace interleave {
  #define LIBCO_INTERLEAVE
  #undef __cplusplus
  #undef LIBCO_C
  #undef LIBCO_H
  #include "../libco.c"
  #define __cplusplus
  #undef LIBCO_INTERLEAVE

  cothread_t x, y;
  void co_timingtest() {
    for(;;) {
      counter++;
      co_switch(x);
    }
  }
}
namespace sjlj {
  #undef __cplusplus
  #undef LIBCO_C
  #undef LIBCO_H
  #undef __amd64__
  #include "../libco.c"
  #define __amd64__
  #define __cplusplus

  cothread_t x, y;
  void co_timingtest() {
    for(;;) {
      counter++;
      co_switch(x);
    }
  }
}

void sub_timingtest() {
  counter++;
}

int main() {
  printf("context-switching timing test\n\n");
  time_t start, end;
  int i, t1;

  start = clock();
  for(counter = 0, i = 0; i < Iterations; i++) {
    sub_timingtest();
  }
  end = clock();

  t1 = (int)difftime(end, start);
  printf("%2.3f seconds per  50 million subroutine calls (%d iterations)\n", (float)t1 / CLOCKS_PER_SEC, counter);

  {
    printf("standard\n");
    using namespace standard;
    x = co_active();
    y = co_create(65536, co_timingtest);
    start = clock();
    for(counter = 0, i = 0; i < Iterations; i++) {
      co_switch(y);
    }
    end = clock();
    co_delete(y);
    int t3 = (int)difftime(end, start);
    printf("%2.3f seconds per 100 million co_switch  calls (%d iterations)\n", (float)t3 / CLOCKS_PER_SEC, counter);
    printf("co_switch skew = %fx\n\n", (double)t3 / (double)t1);
  }

  {
    printf("interleave\n");
    using namespace interleave;
    x = co_active();
    y = co_create(65536, co_timingtest);
    start = clock();
    for(counter = 0, i = 0; i < Iterations; i++) {
      co_switch(y);
    }
    end = clock();
    co_delete(y);
    int t2 = (int)difftime(end, start);
    printf("%2.3f seconds per 100 million co_switch  calls (%d iterations)\n", (float)t2 / CLOCKS_PER_SEC, counter);
    printf("co_switch skew = %fx\n\n", (double)t2 / (double)t1);
  }

  {
    printf("sjlj\n");
    using namespace sjlj;
    x = co_active();
    y = co_create(65536, co_timingtest);
    start = clock();
    for(counter = 0, i = 0; i < Iterations; i++) {
      co_switch(y);
    }
    end = clock();
    co_delete(y);
    int t3 = (int)difftime(end, start);
    printf("%2.3f seconds per 100 million co_switch  calls (%d iterations)\n", (float)t3 / CLOCKS_PER_SEC, counter);
    printf("co_switch skew = %fx\n\n", (double)t3 / (double)t1);
  }

  return 0;
}
