#include "node.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, uint64_t frequency) {
  Node *n = (Node *)malloc(sizeof(Node));
  n->left = NULL;
  n->right = NULL;
  n->symbol = symbol;
  n->frequency = frequency;
  return n;
}

void node_delete(Node **n) {
  free(*n);
  *n = NULL;
}

Node *node_join(Node *left, Node *right) {
  uint64_t freq = left->frequency + right->frequency;
  Node *parent = node_create('$', freq);
  parent->left = left;
  parent->right = right;
  return parent;
}

void node_print(Node *n) {
  if (n != NULL) {
    printf("Symbol = %d\n", n->symbol);
    printf("Frequency = %" PRIu64 "\n", n->frequency);
    if (n->left != NULL) {
      printf("Left Child: %d\n", n->left->symbol);
    } else {
      printf("Left Child: NULL\n");
    }
    if (n->right != NULL) {
      printf("Right Child: %d\n", n->right->symbol);
    } else {
      printf("Right Child: NULL\n");
    }
  } else {
    printf("Node is NULL\n");
  }
}
