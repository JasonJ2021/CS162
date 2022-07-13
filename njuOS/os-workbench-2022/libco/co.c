#include "co.h"
#include <stdlib.h>
#include <setjmp.h>


struct co {
};

struct co *co_start(const char *name, void (*func)(void *), void *arg) {
  return NULL;
}

void co_wait(struct co *co) {
}

void co_yield() {
}
