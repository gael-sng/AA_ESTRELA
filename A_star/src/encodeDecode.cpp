#include "encodeDecode.hpp"

string withComma(_ull value) {
    string num = to_string(value);
    int insertPosition = num.length() - 3;

    while (insertPosition > 0) {
        num.insert(insertPosition, ",");
        insertPosition -= 3;
    }

    return num;
}

void encodeFileBWTRunLenght(string inputFile, string outputFile, int blockSize, bool bwt, bool runl) {

    string source;
    FILE *fp = NULL, *fpw = NULL;
    _ull size, pos = 0;
    _uc filled, nBits;
    int read,  nChars;
    _uc encodeType = 2;
    _uc *buffer = NULL;
    bool both = false;
    vector<_uc> encoded;

    if ((bwt) && (runl))
        both = true;
    else if (bwt)
        encodeType = 0;
    else if (runl)
        encodeType = 1;
    else {
        cout << "Error! Invalid option" << endl;
        return;
    }

    if (blockSize <= 0) blockSize = 500;
    buffer = (_uc *) malloc(sizeof(_uc) * blockSize);

    //opens inputFile
    fp = fopen(inputFile.c_str(), "r");
    if (fp == NULL) {
        cout << "Error! Could not open input file: " << inputFile << endl;
        free(buffer);
        return;
    }

    //opens destination file
    fpw = fopen(outputFile.c_str(), "w+");
    if (fpw == NULL) {
        cout << "Error! Could not open output file: " << outputFile << endl;
        free(buffer);
        return;
    }

    //writes file header
    fwrite(&encodeType, sizeof(_uc), 1, fpw);
    fwrite(&blockSize, sizeof(int), 1, fpw);

    //gets file size
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    cout << "Compressing..." << endl << endl;
    cout << "0%" << flush;

    //reads file to source string
    while ((read = fread(buffer, sizeof(_uc), blockSize, fp)) > 0) {
        string source;
        vector<_uc> sourcev;

        if (bwt) {
            source.reserve(blockSize);
            source.append((char *)buffer, read);
        } else {
            sourcev.reserve(blockSize);
            sourcev.insert(sourcev.end(), buffer, buffer+read);
        }

        if (both) {
            encoded = encodeBWT(source, blockSize, filled);
            encoded = Run_length_compact(encoded, nBits, nChars);
        } else if (bwt) {
            encoded = encodeBWT(source, blockSize, filled);
        } else if (runl) {
            encoded = Run_length_compact(sourcev, nBits, nChars);
        }

        //writes file block header
        if (bwt) {
            fwrite(&filled, sizeof(_uc), 1, fpw);
        }
        if (runl) {
            fwrite(&nBits, sizeof(_uc), 1, fpw);
            fwrite(&nChars, sizeof(int), 1, fpw);
        }

        //writes compressed data
        fwrite(&encoded[0], sizeof(_uc), encoded.size(), fpw);
        pos += read;
        cout << "\r" << (int) ((pos/(double)size)*100) << "%" << flush;
    }

    cout << "\rDone!" << endl;

    cout << "Compressed file generated: " << outputFile << endl;
    cout << "Source file size: " << withComma(size) << " bytes" << endl;
    size = ftell(fpw);
    cout << "Compressed file size: " << withComma(size) << " bytes" << endl;

    fclose(fp);
    fclose(fpw);
    free(buffer);
}

void decodeFileBWTRunLenght(string inputFile, string outputFile) {

    FILE *fp = NULL, *fpw = NULL;
    _ull size, total, pos = 0;
    _uc filled, nBits;
    int read, blockSize, nChars;
    //int sizeHeader = 4*sizeof(int) + sizeof(char);
    _uc encodeType = 1;
    _uc *buffer = NULL;
    bool bwt = true, runl = true, both = true;
    vector<_uc> decoded;

    //opens inputFile
    fp = fopen(inputFile.c_str(), "r");
    if (fp == NULL) {
        cout << "Error! Could not open input file: " << inputFile << endl;
        return;
    }

    //opens destination file
    fpw = fopen(outputFile.c_str(), "w+");
    if (fpw == NULL) {
        cout << "Error! Could not open output file: " << outputFile << endl;
        return;
    }

    //gets file size
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    rewind(fp);

    //reads file header
    fread(&encodeType, sizeof(_uc), 1, fp);
    fread(&blockSize, sizeof(int), 1, fp);

    cout << "Compression method detected: ";
    if (encodeType == 1) {
        bwt = false;
        both = false;
        cout << "Run Length" << endl;
    } else if (encodeType == 0) {
        runl = false;
        both = false;
        cout << "BWT" << endl;
    } else {
        cout << "BWT + Run Length" << endl;
    }

    //reads file block header
    if (bwt)
        fread(&filled, sizeof(_uc), 1, fp);
    if (runl) {
        fread(&nBits, sizeof(_uc), 1, fp);
        fread(&nChars, sizeof(int), 1, fp);
    }

    int totalBWT = blockSize+(sizeof(int)/sizeof(_uc)) * sizeof(_uc);
    if (runl) {
        buffer = (_uc *) malloc(sizeof(_uc) * blockSize * 10);
        total = (_ull) nBits*nChars + (_ull) BYTE_SIZE*nChars;
    	if (total%BYTE_SIZE != 0)
    		total += BYTE_SIZE - total%BYTE_SIZE;
    	total /= BYTE_SIZE;
    } else {
        buffer = (_uc *) malloc(sizeof(_uc) * totalBWT);
        total = totalBWT;
    }
    
    cout << "Decompressing..." << endl << endl;
    cout << "0%" << flush;

    //reads file to source vector
    while ((read = fread(buffer, sizeof(_uc), total, fp)) > 0) {
        vector<_uc> source;
        source.reserve(read);
        source.insert(source.end(), buffer, buffer+read);

        if (both) {
            decoded = Run_length_descompact(source, nBits, nChars);
            decoded = decodeBWT(decoded, blockSize, filled);
        } else if (runl) {
            decoded = Run_length_descompact(source, nBits, nChars);
        } else if (bwt) {
            decoded = decodeBWT(source, blockSize, filled);
        }


        //writes decompressed data
        fwrite(&decoded[0], sizeof(_uc), decoded.size(), fpw);

        //get next block header
        if (bwt) {
            fread(&filled, sizeof(_uc), 1, fp);
            pos += sizeof(_uc);
        } if (runl) {
            fread(&nBits, sizeof(_uc), 1, fp);
            fread(&nChars, sizeof(int), 1, fp);
            pos += sizeof(_uc) + sizeof(int);
        }

        if (runl) {
            total = (_ull) nBits*nChars + (_ull) BYTE_SIZE*nChars;
        	if (total%BYTE_SIZE != 0)
        		total += BYTE_SIZE - total%BYTE_SIZE;
        	total /= BYTE_SIZE;
        }

        pos += read;
        cout << "\r" << (int) ((pos/(double)size)*100) << "%" << flush;
    }

    cout << "\rDone!" << endl;

    cout << "Decompressed file generated: " << outputFile << endl;
    cout << "Source file size: " << withComma(size) << " bytes" << endl;
    size = ftell(fpw);
    cout << "Decompressed file size: " << withComma(size) << " bytes" << endl;

    fclose(fp);
    fclose(fpw);
    free(buffer);
}

void encodeFileHuffman(const char* inputFile, const char *outputFile, _uc encodeType){
    printf("olaaaaa comapctando com huff huff\n");
    Huffman_tree* tree = new Huffman_tree();
    tree->Genarate_tree_from_text(inputFile);
    tree->compact(inputFile, outputFile, encodeType);

    delete tree;
}

void encodeFile(string inputFile, string outputFile, bool bwt, bool runl, bool huffman, int blockSize) {
    if (!huffman) {
        encodeFileBWTRunLenght(inputFile, outputFile, blockSize, bwt, runl);
    }else{
        encodeFileHuffman(inputFile.c_str(), outputFile.c_str(), 3);
    }
}

void decodeFileHuffman(const char* inputFile, const char* outputFile){
    Huffman_tree* tree = new Huffman_tree();
    tree->descompact(inputFile, outputFile);
    delete tree;
}

void decodeFile(string inputFile, string outputFile) {

    /*bwt apenas encode 0
    run l apenas encode 1
    bwt+run l encode 2*/
    FILE *fp = NULL;
    _uc encodeType;

    fp = fopen(inputFile.c_str(), "r");
    if (fp == NULL) {
        cout << "Error! Could not open input file: " << inputFile << endl;
        return;
    }

    fread(&encodeType, sizeof(_uc), 1, fp);
    fclose(fp);

    if (encodeType <= 2) {
        decodeFileBWTRunLenght(inputFile, outputFile);
    }else if(encodeType == 3){
        decodeFileHuffman(inputFile.c_str(), outputFile.c_str());
    }
}
