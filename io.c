#include "io.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

uint64_t bytes_read;
uint64_t bytes_written;

static uint8_t statbuf[BLOCK];
static int ind = 0;
static int end = 0;

void print_buf(uint8_t *buf) {
  printf("[");
  for (int i = 0; i < 1; i += 1) {
    printf("%s, ", &(buf[i]));
  }
  printf("]");
}

int read_bytes(int infile, uint8_t *buf, int nbytes) {
  int total_bread = 0; // keeps track of total bytes read
  ssize_t bread = 1;   // keeps track of bytes read per iteration
  while (total_bread < nbytes && bread > 0) {
    bread = read(infile, buf + total_bread, nbytes - total_bread);
    total_bread += bread; // increments by amount of bits read
  }
  return total_bread;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  int total_bwrit = 0;
  ssize_t bwrit = 1;
  while (total_bwrit < nbytes && bwrit > 0) {
    bwrit = write(outfile, buf + total_bwrit, nbytes);
    total_bwrit += bwrit;
  }
  return total_bwrit;
}

bool read_bit(int infile, uint8_t *bit) {
  if (ind == BLOCK * 8) { // if buffer is full
    ind = 0;              // reset index to beginning
  }
  if (ind == 0) { // fill buffer if empty
    bytes_read = read_bytes(infile, statbuf, BLOCK);
    if (bytes_read == 0) {
      return false;
    }                         // if nothing read into buffer return false
    end = bytes_read * 8 + 1; // set end as remaining bits
  }
  if (ind == end) {
    return false;
  }
  uint8_t bitvalue = statbuf[ind / 8] & (1 << (ind % 8));
  *bit = bitvalue >> ind; // get bit at index
  ind += 1;
  return true;
}

void write_code(int outfile, Code *c) {
  for (uint32_t i = 0; i < c->top; i += 1) {
    statbuf[ind / 8] |= c->bits[i] & (1 << (ind % 8));
    ind += 1;
    if (ind == BLOCK * 8) { // checks if buffer is full
      write_bytes(outfile, statbuf, BLOCK);
      ind = 0; // if full, write whole buffer out and reset index
    }
  }
}

void flush_codes(int outfile) {
  if (ind != 0) {
    int bytes = (ind + 7) / 8;
    write_bytes(outfile, statbuf, bytes);
    memset(statbuf, 0, sizeof(statbuf));
    ind = 0;
  }
}
