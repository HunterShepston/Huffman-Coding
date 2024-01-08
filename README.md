# Overview

This is a program for a lossless compression algorithm known as Huffman Coding. The basic
idea is that each character gets a unique bitstring , and characters  that appear more 
frequently will be represented with less bits. The encoder is responsible for compressing
data into arbitrary bytes, and the decoder will revert it to it's original state.

An integral part of this program is an abstract data type called a node. Inside every 
node is a pointer to it's left child, and it's right child (even if the child is NULL). 
It also contains the symbol it corresponds to, and the frequency of which said symbol 
occurs. This will be used to implement a priority queue of nodes which we will use to 
sort through our data.


## Using the program

To create the program you must first run the Makefile, you can do this by using the
commands; '$ make', '$make all', or '$make sorting'. You can use '$make clean' to 
get rid of the extra files.

After the encode executable has been created, it can be run via '$./encode [-hi:o:v]'
-h			prints help message for usage of program
-i	file	specifies input file for which to encode
-o	file	specifies output to send encoded file to
-v			enables statistics on encoding

Similarly decode can be ran after creation, '$./decode [-hi:o:v]'
-h			prints help message for usage of program
-i	file	specifies input file for which to decode
-o	file	specifies output to send decoded file to
-v			enables statistics on decoding
