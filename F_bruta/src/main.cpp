#include <iostream>
#include <stdlib.h>
#include <string>
#include "encodeDecode.hpp"

int main (int argc, char *argv[]) {

	bool encode = true, bwt = false, runl = false, huffman = false;
	string inputFile, outputFile;
	int blockSize = 0;

	for (int i = 1; i < argc; ++i) {
		string arg(argv[i]);
		if (arg.compare("decode") == 0) {
			encode = false;
		} else if (arg.compare("-i") == 0) {
			++i;
			string file(argv[i]);
			inputFile = file;
		} else if (arg.compare("-o") == 0) {
			++i;
			string file(argv[i]);
			outputFile = file;
		} else if (arg.find("bwt") != string::npos) {
			if ((arg.find("true") != string::npos) || (arg.find("TRUE") != string::npos))
				bwt = true;
		} else if (arg.find("txtblck") != string::npos) {
			int k = 0;
			while ((arg[k] < '0') || (arg[k] > '9')) ++k;
			blockSize = atoi(arg.substr(k, arg.length()).c_str());
		} else if (arg.find("huffman") != string::npos) {
			if ((arg.find("true") != string::npos) || (arg.find("TRUE") != string::npos))
				huffman = true;
		} else if (arg.find("runl") != string::npos) {
			if ((arg.find("true") != string::npos) || (arg.find("TRUE") != string::npos))
				runl = true;
		}
	}

	if ((bwt == false) && (runl == false) && (huffman == false)) {
		bwt = true;
		runl = true;
		huffman = true;
	}

	if (encode)
		encodeFile(inputFile, outputFile, bwt, runl, huffman, blockSize);
	else
		decodeFile(inputFile, outputFile);

	return 0;
}
