#include "header.h"
#include "huffman.h"
#include "io.h"
#include "node.h"
#include "pq.h"
#include "stack.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int opt;
  bool stats = false;
  int infile = STDIN_FILENO;
  int outfile = STDOUT_FILENO;
  while ((opt = getopt(argc, argv, "hi:o:v")) != -1) {
    switch (opt) {
    case 'h':
      printf("SYNOPSIS\n");
      printf("  A Huffman decoder.\n");
      printf("  Decompresses a file using the Huffman coding algorithm.\n\n");
      printf("USAGE\n");
      printf("  ./decode [-h] [-i infile] [-o outfile]\n\n");
      printf("OPTIONS\n");
      printf("  -h             Program usage and help.\n");
      printf("  -v             Print compression statistics.\n");
      printf("  -i infile      Input file to decompress.\n");
      printf("  -o outfile     Output of decompressed data.\n");
      break;
    case 'i':
      infile = open(optarg, O_RDONLY);
      break;
    case 'o':
      outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      break;
    case 'v':
      stats = true;
      break;
    }
  }
  Header h;
  read(infile, &h, sizeof(h));
  if (h.magic != MAGIC) {
    fprintf(stderr, "Error, incompatible file\n");
    return 1;
  }
  fchmod(outfile, h.permissions);

  uint8_t buffer[h.tree_size];
  memset(buffer, 0, h.tree_size);

  read_bytes(infile, buffer, h.tree_size);
  Node *root = rebuild_tree(h.tree_size, buffer);

  lseek(infile, 0, SEEK_SET);
  uint8_t bit;
  Node *n = root;
  uint8_t symb;
  while (read_bit(infile, &bit)) {
    bytes_read += 1;
    if (bit == 0) {
      n = n->left;
    } else {
      n = n->right;
    }
    if (n->left == NULL && n->right == NULL) {
      symb = n->symbol;
      // printf("symb %u\n", symb);
      write_bytes(outfile, &symb, 1);
      bytes_written += 1;
      n = root;
    }
  }
  bytes_read = bytes_read / 8;
  delete_tree(&root);
  close(infile);
  close(outfile);
  float space = 100 * (1 - bytes_read / h.file_size);
  if (stats) {
    fprintf(stderr, "Compressed file size: %lu bytes \n", bytes_read);
    fprintf(stderr, "Decompressed file size: %lu bytes \n", h.file_size);
    fprintf(stderr, "Space saving: %f%%\n", space);
  }
}
