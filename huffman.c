#include "huffman.h"
#include "code.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

Node *build_tree(uint64_t hist[static ALPHABET]) {
  PriorityQueue *pq = pq_create(ALPHABET);
  for (int i = 0; i < 256; i += 1) { // for every ascii character
    if (hist[i] > 0) {               // if character appears one or more times
      Node *n = node_create(i, hist[i]); // create node for character
      enqueue(pq, n);                    // enqueue node
    }
  } // after this loop we should have a completed priority queue
  Node *left;
  Node *right;
  Node *root;
  while (pq_size(pq) > 1) {
    dequeue(pq, &left);
    dequeue(pq, &right); // dequeue first two nodes
    Node *par = node_join(left, right);
    enqueue(pq, par); // enqueue parent node
  }
  dequeue(pq, &root);
  pq_delete(&pq);
  return root;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
  Code code;
  static int first_pass;
  first_pass += 1;
  if (first_pass == 1) {
    code = code_init();
  } // this ensures the code is only initialized once
  uint8_t bit;
  if (root != NULL) {
    if (root->left == NULL && root->right == NULL) { // if leaf
      table[root->symbol] = code; // assign current code to symbol
    } else {
      code_push_bit(&code, 0);
      build_codes(root->left, table);
      code_pop_bit(&code, &bit);
      code_push_bit(&code, 1);
      build_codes(root->right, table);
      code_pop_bit(&code, &bit);
    }
  }
}

void dump_tree(int outfile, Node *root) {
  uint8_t leaf = 'L';
  uint8_t inter = 'I';
  uint8_t sym;
  if (root != NULL) {
    dump_tree(outfile, root->left);
    dump_tree(outfile, root->right);                 // postorder traversal
    if (root->left == NULL && root->right == NULL) { // if node is leaf
      sym = root->symbol;
      write(outfile, &leaf, 1);
      write(outfile, &sym, 1);
    } else { // if node is not leaf
      write(outfile, &inter, 1);
    }
  }
}

Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
  Stack *s = stack_create(nbytes);
  Node *right;
  Node *left;
  Node *root;
  int i = 0;
  while (stack_size(s) > 1 || i < 3) { // i < 3 to make sure it enters the loop
    if (tree_dump[i] == 'L') {
      i += 1; // account for advance in array
      Node *n = node_create(tree_dump[i], 0);
      stack_push(s, n);
    } else { // if node not leaf via treedump
      if (tree_dump[i] != 'I') {
        printf("error, indicator not I or L");
      } // error checker
      stack_pop(s, &right);
      stack_pop(s, &left); // pop and join two nodes
      Node *par = node_join(left, right);
      stack_push(s, par); // push parent onto stack
    }
    i += 1;
  }
  stack_pop(s, &root);
  stack_delete(&s);
  return root;
}

void delete_tree(Node **root) {
  if ((*root)->left) {
    delete_tree(&(*root)->left);
  }
  if ((*root)->right) {
    delete_tree(&(*root)->right);
  }
  node_delete(root);
  root = NULL;
}
