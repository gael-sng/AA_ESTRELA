#ifndef _ENCODE_HPP_
#define _ENCODE_HPP_

#include "Run_length.hpp"
#include "bwt.hpp"
#include "Huffman.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

#define BYTE_SIZE 8

typedef unsigned long long int _ull;
typedef unsigned char _uc;

using namespace std;

void encodeFile(string inputFile, string outputFile, bool bwt, bool runl, bool huffman, int blockSize);

void decodeFile(string inputFile, string outputFile);

#endif
