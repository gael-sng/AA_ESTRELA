#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BYTE_SIZE 8

#define MAX_N_CHAR 256

typedef unsigned char u_char;


class No {

public:
	u_char name;
	long long int freq;
	bool na_arvore;
	bool eh_folha;
	No* filho[2];
	No* pai;

	No(u_char name, long long int freq, bool eh_folha);
	~No();
};

class Huffman_tree {
	No* raiz;

	No** no_list;
	int n_nos;
	
	u_char** legenda;

private:
	
	void add(u_char letra);

	void Free_arvore(No* n);
	
	void print_tree(No* raiz);

	void Genarate_tree();

	u_char* get_tree_code_aux(No* n, u_char letra, u_char num);

	u_char* get_tree_code(u_char letra);

	void Genarate_legenda();

	void print_legenda();

	void write_tree(const char* outputFile, u_char encodeType);
public:

	void Genarate_tree_from_text(const char* inputFile);

	void compact(const char* inputFile, const char* outputFile, u_char encodeType);

	void descompact(const char* inputFile, const char* outputFile);

	Huffman_tree();

	~Huffman_tree();
};

#endif /* HUFFMAN_H */