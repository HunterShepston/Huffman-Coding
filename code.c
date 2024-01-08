#include "code.h"
#include "defines.h"
#include <stdio.h>

Code code_init(void) {
  Code code;
  code.top = 0;
  for (int i = 0; i < MAX_CODE_SIZE; i += 1) {
    code.bits[i] = 0;
  }
  return code;
}

uint32_t code_size(Code *c) { return c->top; }

bool code_empty(Code *c) {
  if (c->top == 0) {
    return true;
  }
  return false;
}

bool code_full(Code *c) {
  if (c->top == MAX_CODE_SIZE) {
    return true;
  }
  return false;
}

bool code_set_bit(Code *c, uint32_t i) {
  if (i < 0 || i > MAX_CODE_SIZE) {
    return false;
  }
  c->bits[i] = 1;
  return true;
}

bool code_clr_bit(Code *c, uint32_t i) {
  if (i < 0 || i > MAX_CODE_SIZE) {
    return false;
  }
  c->bits[i] = 0;
  return true;
}

bool code_get_bit(Code *c, uint32_t i) {
  if (i < 0 || i > MAX_CODE_SIZE) {
    return false;
  }
  if (c->bits[i] == 1) {
    return true;
  }
  return false;
}

bool code_push_bit(Code *c, uint8_t bit) {
  if (code_full(c)) {
    return false;
  }
  c->bits[c->top] = bit;
  c->top += 1;
  return true;
}

bool code_pop_bit(Code *c, uint8_t *bit) {
  if (code_empty(c)) {
    return false;
  }
  c->top -= 1;
  uint32_t h = c->top;
  *bit = c->bits[h];
  c->bits[h] = 0;
  return true;
}

void code_print(Code *c) {
  printf("[ ");
  for (int i = 0; i < MAX_CODE_SIZE; i += 1) {
    printf("%u", c->bits[i]);
  }
  printf("]\n");
}
