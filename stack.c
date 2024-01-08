#include "stack.h"
#include "node.h"

#include <stdio.h>
#include <stdlib.h>

struct Stack {
  uint32_t top;
  uint32_t capacity;
  Node **items;
};

Stack *stack_create(uint32_t capacity) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->top = 0;
  stack->capacity = capacity;
  stack->items = (Node **)calloc(capacity, sizeof(Node *));
  return stack;
}

void stack_delete(Stack **s) {
  free((*s)->items);
  (*s)->items = NULL;
  free(*s);
  *s = NULL;
}

bool stack_empty(Stack *s) {
  if (s->top == 0) {
    return true;
  }
  return false;
}

bool stack_full(Stack *s) {
  if (s->top == s->capacity) {
    return true;
  }
  return false;
}

uint32_t stack_size(Stack *s) { return s->top; }

bool stack_push(Stack *s, Node *n) {
  if (stack_full(s)) {
    return false;
  }
  s->items[s->top] = n;
  s->top += 1;
  return true;
}

bool stack_pop(Stack *s, Node **n) {
  if (stack_empty(s)) {
    return false;
  }
  s->top -= 1;
  *n = s->items[s->top];
  return true;
}

void stack_print(Stack *s) {
  printf("[");
  for (uint32_t i = 0; i < s->capacity; i += 1) {
    if (s->items[i] == NULL) {
      printf("-, ");
    } else {
      printf("%u, ", s->items[i]->symbol);
    }
  }
  printf("]\n");
}
