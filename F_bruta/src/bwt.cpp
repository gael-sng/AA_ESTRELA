#include "bwt.hpp"

#define CHAR_MAX 256

/* Sorts suffixes based on its first 2 chars
 */
vector<int> suffixRadix(int N, vector<vector<_uc> > &W) {
    vector<int> V;
    vector<queue<int> > bucket(CHAR_MAX);

    for (int i = 0; i < N; ++i) {
        V.push_back(i); //makes a vector with indexes of suffixes
    }

    //sort based in chars 2 and 3 (first ones, as they are in reverse)
    for (int i = 2; i < 4; ++i) {
        //put indexes in buckets base on ith char
        for (int j = 0; j < N; ++j) {
            bucket[ W[V[j]][i] ].push(V[j]);
        }

        int posV = 0;
        //go through each bucket in order, removing all elements from bucket and putting in new sorted V of indexes
        for (int j = 0; j < CHAR_MAX; ++j) {
            while (!bucket[j].empty()) {
                V[posV++] = bucket[j].front();
                bucket[j].pop();
            }
        }
    }

    return V;
}

/* Compares 2 suffixes, given indexes (left right)
 * Returns 0 if equals, -1 if left < right, or 1 if left > right
 */
int compareSuffix(int left, int right, int N, vector<vector<_uc> > &W, vector<vector<char> > &cmp) {
    //convert 4 char in suffixes into integers
    unsigned int *ls = (unsigned int *) &W[ left ][0];
    unsigned int *rs = (unsigned int *) &W[ right ][0];

    if (cmp[left][right] != -2)
        return cmp[left][right];

    //if a simple comparison is needed
    if (*ls < *rs) {
        cmp[left][right] = -1;
        return -1;
    } else if (*ls > *rs) {
        cmp[left][right] = 1;
        return 1;
    //if more than a single comparison is needed
    } else {
        int maxCmp = (N/4)+1;   //max number of comparisons to compare entire string

        //keeps making comparisons until left != right or entire string is compared
        for (int i = 0; i < maxCmp; ++i) {
            //runs through left and right in a circular way (in the end got back to the begin)
            left = (left+4 < N) ? left+4 : (left+4) - N;
            right = (right+4 < N) ? right+4 : (right+4) - N;

            if (cmp[left][right] != -2)
                return cmp[left][right];

            //convert 4 char in suffixes into integers
            ls = (unsigned int *) &W[ left ][0];
            rs = (unsigned int *) &W[ right ][0];

            if (*ls < *rs) {
                cmp[left][right] = -1;
                return -1;
            } else if (*ls > *rs) {
                cmp[left][right] = 1;
                return 1;
            }
        }
    }
    //if equals
    cmp[left][right] = 0;
    return 0;
}

/* Switches 2 integers in a vector of integers
 */
void switchElements(int pos1, int pos2, vector<int> &V) {
    int aux = V[pos1];
    V[pos1] = V[pos2];
    V[pos2] = aux;
}

/* Sort indexes(V) that are between startPos ans endPos, each index represents a suffix in W.
 * The sort is based on quicksort and the vector of index V is modified.
 */
void suffixQuickSort(int startPos, int endPos, int N, vector<vector<_uc> > &W, vector<int> &V, vector<vector<char> > &cmp) {
    int pivot = endPos;
    int wall = startPos;

    while (wall != pivot) {
        //go through all indexes, comparing the suffixes, throwing then to leftside of the wall if lower than pivot
        for (int i = wall; i < pivot; ++i) {
            if (compareSuffix(V[i], V[pivot], N, W, cmp) <= 0) {
                switchElements(wall++, i, V);
            }
        }
        //switches wall and pivot positions
        switchElements(wall, pivot, V);

        //sorts leftside of wall
        if (startPos != wall)
            suffixQuickSort(startPos, wall-1, N, W, V, cmp);

        //sorts rightside of wall
        if (wall != pivot)
            suffixQuickSort(wall, pivot, N, W, V, cmp);
    }
}

/* Sort suffixes that are sorted by first 2 chars
 */
vector<int> suffixSort(int N, vector<vector<_uc> > &W, vector<int> &V, vector<vector<char> > &cmp) {
    _uc ch = W[ V[0] ][3];
    _uc ch1 = W[ V[0] ][2];
    int startPos = 0;
    int endPos = 0;

    //go through all indexes, making a quicksort of indexes that begin with the same char
    for (int i = 1; i < (int) V.size(); ++i) {
        if (W[ V[i] ][3] == ch) {
            if (W[ V[i] ][2] != ch1) {
                endPos = i-1;
                if (startPos != endPos)
                    suffixQuickSort(startPos, endPos, N, W, V, cmp);
                startPos = i;
                ch1 = W[ V[i] ][2];
            }
        } else if (W[ V[i] ][3] != ch) {  //if its a different char, sorts the previous indexes that begin with the same char
            endPos = i-1;
            if (startPos != endPos)
                suffixQuickSort(startPos, endPos, N, W, V, cmp);
            startPos = i;
            ch = W[ V[i] ][3];
            ch1 = W[ V[i] ][2];
        }
    }
    suffixQuickSort(startPos, V.size()-1, N, W, V, cmp);

    return V;
}

/* Encodes a block using BWT
 */
vector<_uc> encodeBlockBWT(string &S, int &a) {
    int n = S.length();
    vector<char> cmpLine(n, -2);
    vector<vector<char> > cmp(n, cmpLine);
    vector<vector<_uc> > W;
    vector<_uc> L;

    //generates a matrix W, each line v contains a suffix of 4 chars (S[i] and 3 more), one v for each char S
    for (int i = 0; i < n; ++i) {
        vector<_uc> v;
        for (int j = 3; j >= 0; --j) {
            v.push_back(S[(i+j < n) ? i+j : i+j-n]);    //if it reachs end, pick starting chars
        }
        W.push_back(v);
    }

    vector<int> V = suffixRadix(n, W);  //pre-sort suffixes by the first 2 char
    V = suffixSort(n, W, V, cmp);    //sort all the suffixes alphabetically

    for (int i = 0; i < (int) V.size(); ++i) {
        if (V[i] == 0)
            a = i;  //get line in bwt matrix that contains original string
        _uc c = S[V[i]-1 < 0 ? n-1 : V[i]-1];   //get the char that comes before the ith char
        L.push_back(c); //insert in encoded string
    }

    return L;
}

/* Encodes text using BWT, separating it in blocks of blockSize.
 */
vector<_uc> encodeBWT(string text, int blockSize, _uc &toFill) {
    int a;
    vector<_uc> encoded;
    toFill = 0;
    /*toFill = blockSize - text.length()%blockSize;
    if (toFill == blockSize) toFill = 0;

    if (toFill > 0) {   //if needs filling
        for (int i = 0; i < toFill; ++i) {
            text += rand()%CHAR_MAX;    //appends chars to fill last block
        }
    }*/

    int i = 0;
    //cout << "0%" << flush;
    while (i < (int) text.length()) {
        //generates substring of blockSize
        string sub = text.substr(i, blockSize);
        for (int j = sub.length(); j < 4; ++j, ++toFill) {
            sub.push_back('*');
        }
        vector<_uc> aux = encodeBlockBWT(sub, a);  //encode substring
        encoded.push_back(a & 0xFF);
        encoded.push_back((a >> 8) & 0xFF);
        encoded.push_back((a >> 16) & 0xFF);
        encoded.push_back((a >> 24) & 0xFF);
        //++nBlocks;
        i += blockSize;
        //cout << "\r" << i << flush;
        //cout << "\r" << (int) ((i/(double)text.length())*100) << "%" << flush;
        encoded.insert(encoded.end(), aux.begin(), aux.end());  //appends to the encoded vector
    }

    //cout << "\rDone!" << endl;
    return encoded;
}

/* Decode a block of text encoded by BWT
 */
vector<_uc> decodeBlockBWT(vector<_uc> L, int a) {
    vector<int> K(CHAR_MAX, 0); //initialize with 0
    vector<int> M(CHAR_MAX);
    vector<int> C(L.size());
    vector<_uc> Q(L.size());
    int sum = 0;

    //generates vector C, C[i] represents the occurence of char in L[i] (if its the first 'a' C[i] = 0, in the second 'a' C[i] = 1, etc)
    //K represents the total number of occurences of a char in the string (if there is 2 'a' in a string, k['a'] = 2)
    for (int i = 0; i < (int) L.size(); ++i) {
        C[i] = K[L[i]]++;
    }

    //generates the vector M, M[i] represents the position of the first char i in the first column of the BWT matrix (the sorted column)
    for (int i = 0; i < CHAR_MAX; ++i) {
        M[i] = sum;
        sum += K[i];
    }

    //generate the line a in the matrix (line that contains the decoded string), from end to begin
    for (int i = L.size()-1; i >= 0; --i) {
        Q[i] = L[a];    //next char is in positon a
        a = C[a] + M[L[a]]; //new positon is the position L[a] is in the firts column, which is a = occurence of a + position of the first char L[a]
    }

    return Q;
}

/* Decode a text encoded with BWT, by decoding its blocks
 */
vector<_uc> decodeBWT(vector<_uc> &encoded, int blockSize, _uc filled) {
    int i = 0;
    vector<_uc> decoded;

    while (i < (int) encoded.size()) {
        vector<_uc> auxEncoded;

        int *a = (int *) &encoded[i];
        i += (sizeof(int)/sizeof(_uc)) * sizeof(_uc);

        //create block of encoded text
        for (int j = 0; (j < blockSize) && (i < (int)encoded.size()); ++i, ++j) { //+1 for \0 (\0 not included in blockSize)
            auxEncoded.push_back(encoded[i]);
        }
        //decodes block
        vector<_uc> auxDecoded = decodeBlockBWT(auxEncoded, (int)*a);
        //insert decoded block in decoded vector
        decoded.insert(decoded.end(), auxDecoded.begin(), auxDecoded.end());
    }

    for (int j = 0; j < filled; ++j) {
        decoded.pop_back(); //removes chars used for filling
    }

    return decoded;
}

/*
int main () {
    string S;
    int n;
    int I;

    cin >> S;
    cin >> I;
    vector<_uc> L = encodeBWT(S, I);

    int filled = I - S.length()%I;
    if (filled == S.length()) filled = 0;

    vector<_uc> Q = decodeBWT(L, I, filled);

    cout << "tranformada reversa: " << endl;

    for (int i = 0; i < Q.size(); ++i) {
        cout << Q[i];
    }
    cout << endl;

    return 0;
}*/
