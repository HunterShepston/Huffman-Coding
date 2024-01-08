#include "pq.h"
#include "node.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct PriorityQueue {
  uint32_t capacity;
  uint32_t size;
  Node **nodes;
};

PriorityQueue *pq_create(uint32_t capacity) {
  PriorityQueue *pq = (PriorityQueue *)malloc(sizeof(PriorityQueue));
  assert(pq);
  pq->size = 0;
  pq->capacity = capacity;
  pq->nodes = (Node **)calloc(pq->capacity, sizeof(Node *));
  return pq;
}

void pq_delete(PriorityQueue **q) {
  if (&pq_empty == false) {
    for (uint32_t i = 0; i < (*q)->size; i += 1) {
      node_delete(&(*q)->nodes[i]);
    }
  }
  free((*q)->nodes);
  free(*q);
  q = NULL;
}

bool pq_empty(PriorityQueue *q) {
  if (q->size == 0) {
    return true;
  }
  return false;
}

bool pq_full(PriorityQueue *q) {
  if (q->size == q->capacity) {
    return true;
  }
  return false;
}

uint32_t pq_size(PriorityQueue *q) { return q->size; }

bool enqueue(PriorityQueue *q, Node *n) {
  if (pq_full(q)) {
    return false;
  }
  if (pq_empty(q)) {
    q->nodes[0] = n;
    q->size += 1;
    return true;
  }
  uint32_t k = q->size;
  while (k > 0 && q->nodes[k - 1]->frequency < n->frequency) {
    q->nodes[k] = q->nodes[k - 1];
    k -= 1;
  }
  q->nodes[k] = n;
  q->size += 1;
  return true;
}

bool dequeue(PriorityQueue *q, Node **n) {
  if (pq_empty(q)) {
    return false;
  }
  uint32_t s = q->size - 1;
  *n = q->nodes[s];   // s is the index of the last
  q->nodes[s] = NULL; // node in the queue
  q->size -= 1;
  return true;
}

void pq_print(PriorityQueue *q) {
  for (uint32_t i = 0; i < q->size; i += 1) {
    if (q->nodes[i] != NULL) {
      printf("%d: (%d, %lu) ", i, q->nodes[i]->symbol, q->nodes[i]->frequency);
    }
  }
  printf("\n");
}
