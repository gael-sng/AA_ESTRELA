#ifndef _BWT_HPP_
#define _BWT_HPP_

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <stdlib.h>

typedef unsigned char _uc;

using namespace std;

/* Encodes text using BWT, separating it in blocks of blockSize.
 */
vector<_uc> encodeBWT(string text, int blockSize, _uc &toFill);

/* Decode a text encoded with BWT, by decoding its blocks
 */
vector<_uc> decodeBWT(vector<_uc> &encoded, int blockSize, _uc filled);

#endif
