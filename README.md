# Overview

This program contains multiple sorting algorithms with a test harness.
The four sorting algorithms are:
-Insertion sort
-Batcher's odd even merge sort
-Heap sort
-Quick sort

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
