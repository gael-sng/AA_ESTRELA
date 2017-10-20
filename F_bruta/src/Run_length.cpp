#include "Run_length.hpp"


_uc bit(int i) {
	return 0x01 << i;
}

vector<_uc> Run_length_compact(vector<_uc> txt, _uc &nBits, int &nChars){
	if(txt.empty()){
		cout << "ERRO!! string vazia" << endl;
	}

	vector<unsigned short> zip;
	vector<_uc> zipc;
	int cont_rep = 0;//contador de repetiçoes
	_uc save = 0;
	int size = 0;
	int i = 0;
	int highest = 0;

	//zip.reserve(txt.size());
	/*zipc.reserve(txt.size());


	cout << "Tamanho " << txt.size() << endl << "zip " << zip.capacity() << endl << "zipc " << zipc.capacity() << endl;
	if ((zip.capacity() < txt.size()) || (zipc.capacity() < txt.size()))
		cout << "ERRO, NAO FOI POSSIVEL ALOCAR" << endl;*/

	while(i < (int) txt.size()){
		cont_rep = 0;
		save = txt[i];
		//contando repetiçoes
		while ((i < (int) txt.size()) && (txt[i] == save)) {
			cont_rep++;
			i++;
		}
		//salvando o numero de repetiçoes na string resposta
		//zip.push_back(cont_rep);
		//zipc.push_back(save);
		++size;
		if (cont_rep > highest)
			highest = cont_rep;
	}

	nBits = (int) log2(highest) + 1;
	_ull total = (_ull) nBits*size + (_ull) BYTE_SIZE*size;
	if (total%BYTE_SIZE != 0)
		total += BYTE_SIZE - total%BYTE_SIZE;

	nChars = size;
	total /= BYTE_SIZE;

	vector<_uc> aux(total, 0);
	//zip.reserve(size);
	//zipc.reserve(size);
	//cout << "reserved "<< total << " bytes"<< endl;
	i = 0;

	int writeChar = 0;
	int writePos = 7;
	while(i < (int) txt.size()){
		cont_rep = 0;
		save = txt[i];
		//contando repetiçoes
		while ((i < (int) txt.size()) && (txt[i] == save)) {
			cont_rep++;
			i++;
		}
		//salvando o numero de repetiçoes na string resposta
		bitset<INT_SIZE> auxbits1(cont_rep);
		//cout << zip[i] << " = " << auxbits1.to_string() << endl;
		for (int j = 0; j < nBits; ++j) {
			if (auxbits1.test(j))
				aux[writeChar] |= bit(writePos);
			--writePos;
			if (writePos < 0) {
				writeChar++;
				writePos = 7;
			}
		}
		bitset<BYTE_SIZE> auxbits2(save);
		//cout << zip[i] << " = " << auxbits2.to_string() << endl;
		for (int j = 0; j < BYTE_SIZE; ++j) {
			if (auxbits2.test(j))
				aux[writeChar] |= bit(writePos);
			--writePos;
			if (writePos < 0) {
				writeChar++;
				writePos = 7;
			}
		}
	}

	return aux;
}


vector<_uc> Run_length_descompact(vector<_uc> zip, _uc nBits, int nChars){
	vector<_uc> unzip;

	_uc n;
	int readChar = 0;
	int readPos = 7;
	n = zip[readChar];
	for (int j = 0; j < nChars; ++j) {
		int nRep = 0;
		int potencia = 1;
		for (int i = 0; i < nBits; ++i) {
			if (n & bit(readPos)) {
				nRep += potencia;
			}
			potencia *= 2;
			--readPos;
			if (readPos < 0) {
				n = zip[++readChar];
				readPos = 7;
			}
		}
		int ch = 0;
		potencia = 1;
		for (int i = 0; i < BYTE_SIZE; ++i) {
			if (n & bit(readPos)) {
				ch += potencia;
			}
			potencia *= 2;
			--readPos;
			if (readPos < 0) {
				if (++readChar < (int) zip.size())
					n = zip[readChar];
				readPos = 7;
			}
		}

		//copiando a mesma letra 'n' vezes
		while(nRep > 0){
			--nRep;
			unzip.push_back((_uc) ch);
		}
	}

	return unzip;
}



/*int main() {

	string s;
	cin >> s;
	int nBits, nChars;

	vector<_uc> ans = Run_length_compact(s, nBits, nChars);

	for (int i = 0; i < ans.size(); ++i) {
		bitset<BYTE_SIZE> a((int)ans[i]);
		cout << a.to_string() << " ";
	}
	cout << endl;

	vector<_uc> ans2 = Run_length_descompact(ans, nBits, nChars);

	for (int i = 0; i < ans2.size(); ++i) {
		cout << ans2[i];
	}
	cout << endl;

	return 0;
}*/
