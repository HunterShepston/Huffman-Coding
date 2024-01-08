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
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int opt;
  bool help = false;
  bool stats = false;
  int infile = STDIN_FILENO;
  int outfile = STDOUT_FILENO;
  while ((opt = getopt(argc, argv, "hi:o:v")) != -1) {
    switch (opt) {
    case 'h':
      help = true;
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
  if (help) {
    printf("SYNOPSIS\n");
    printf("  A Huffman encoder.\n");
    printf("  Compresses a file using the Huffman coding algorithm.\n");
    printf("\n");
    printf("USAGE\n");
    printf("  ./encode [-h] [-i infile] [-o outfile]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("  -h             Program usage and help.\n");
    printf("  -v             Print compression statistics.\n");
    printf("  -i infile      Input file to compress.\n");
    printf("  -o outfile     Output of compressed data.\n");
  }
  uint64_t hist[ALPHABET];
  uint8_t buffer[BLOCK];
  uint16_t uniq_symbols = 2; // two to account for the two added symbols
  int bread;
  Code table[ALPHABET];
  for (int i = 0; i < 256; i++) {
    hist[i] = 0;
  }
  while ((bread = read_bytes(infile, buffer, BLOCK)) !=
         0) { // while bytes are still being read
    for (int i = 0; i < bread; i += 1) {
      hist[buffer[i]] += 1;
      if (hist[buffer[i]] == 1) {
        uniq_symbols += 1; // only add unique symbol when symbol
                           // is added for the first time
      }
    }
  }
  hist[0] += 1;
  hist[255] += 1;
  Node *root = build_tree(hist);
  build_codes(root, table);

  struct stat filebuf;
  if (fstat(infile, &filebuf) == -1) {
    return 1;
  }
  fchmod(outfile, filebuf.st_mode);

  Header h;
  h.magic = MAGIC;
  h.permissions = filebuf.st_mode;
  h.tree_size = (3 * uniq_symbols) - 1;
  h.file_size = filebuf.st_size;
  bytes_written += write(outfile, &h, sizeof(h));

  dump_tree(outfile, root);

  lseek(infile, 0, SEEK_SET); // reset to beginning of file

  while ((bread = read_bytes(infile, buffer, 1)) !=
         0) { // read until there are no more bytes

    write_code(outfile, &table[buffer[0]]);
  }
  delete_tree(&root);
  flush_codes(outfile);
  close(infile);
  close(outfile);

  float space = 100 * (1 - bytes_read / bytes_written);
  if (stats) {
    fprintf(stderr, "Uncompressed file size: %lu bytes \n", bytes_read);
    fprintf(stderr, "Compressed file size: %lu bytes \n", bytes_written);
    fprintf(stderr, "Space saving: %f%%\n", space);
  }
}
