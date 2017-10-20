#ifndef _RUN_LENGHT_H_
#define _RUN_LENGHT_H_

#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <math.h>


typedef  unsigned long long int _ull;
typedef unsigned char _uc;

#define BYTE_SIZE 8

#define INT_SIZE 32

using namespace std;

vector<_uc> Run_length_compact(vector<_uc> txt, _uc &nBits, int &nChars);

vector<_uc> Run_length_descompact(vector<_uc> zip, _uc nBits, int nChars);


#endif /* _RUN_LENGHT_H_ */
