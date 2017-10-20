#include "Huffman.hpp"

typedef unsigned char u_char;


class bit_vector{
private:

	int bit_pos;
	u_char set(int i){
		return 0x01 << i;
	}

public:
	long long int i;
	u_char* vet = NULL;
	void add(u_char num){
		//se o byte estiver cheio
		if(this->bit_pos == BYTE_SIZE){
			this->bit_pos = 0;
			this->i++;
			vet = (u_char*)realloc( vet , (i+1) * sizeof(u_char));
			vet[this->i] &= 0x00;//zerando o byte
		}
		if(num == '1')vet[this->i] |= set(bit_pos);//setando a posição do bit para 1
		this->bit_pos++;
	}

	u_char* get_vet(){
		u_char* aux = (u_char*)malloc( (this->i+2) * sizeof(u_char));
		long long int n = this->i + 1;
		aux[n] = '\0';
		while(n--){
			aux[n] = vet[n];
		}
		return aux;
	}

	bit_vector(){
		this->i = -1;
		this->bit_pos = BYTE_SIZE;
		vet = NULL;
	}

	~bit_vector(){
		if(vet != NULL)free(vet);
	}
};

No::No(u_char name, long long int freq, bool eh_folha){
	this->na_arvore = false;
	this->name = name;
	this->freq = freq;
	this->eh_folha = eh_folha;
	this->filho[0] = NULL;
	this->filho[1] = NULL;
}
No::~No(){}

void Huffman_tree::add(u_char letra){
	
	long long int i, save_i = -1;
	
	//procurando se ja existe o caracter
	for(i=0; i< this->n_nos ;i++){
		if(this->no_list[i]->name == letra) save_i = i;
	}

	//se existir incrementar a frequencia dele
	if(save_i >= 0){
		this->no_list[save_i]->freq++;

	}else{//se nao existir, criar o nó do caracter
		this->n_nos++;
		this->no_list = (No**)realloc( this->no_list , this->n_nos * sizeof(No*));
		this->no_list[this->n_nos - 1] = new No(letra, 1, true);
	}
}

void Huffman_tree::Free_arvore(No* n){
	if(n->filho[0] != NULL)Free_arvore(n->filho[0]);
	if(n->filho[1] != NULL)Free_arvore(n->filho[1]);
	delete n;
}

void Huffman_tree::print_tree(No* raiz){
	printf("%c {", raiz->name);
	if(raiz->filho[0] != NULL)print_tree(raiz->filho[0]);
	printf(" , ");
	if(raiz->filho[1] != NULL)print_tree(raiz->filho[1]);
	printf("}");
}

void Huffman_tree::Genarate_tree(){
	if(this->no_list == NULL or this->n_nos == 0){
		printf("ERRO! nao foi possivel gerar a arvore sem a lista de NOs\n");
		return ;
	}
	if(this->raiz != NULL)Free_arvore(this->raiz);

	printf("Gerando a Arvore ...\n");

	No* pai = NULL;

	long long int i, save_1, save_2;
	do{
		save_1 = -1;
		save_2 = -1;
		//procurando os 2 Nós com as menores frequencias
		for(i=0;i< this->n_nos ;i++){
			if(not this->no_list[i]->na_arvore){
				if(save_1 == -1)save_1 = i;
				else if(save_2 == -1)save_2 = i;
				else if(this->no_list[i]->freq < this->no_list[save_1]->freq){
					save_1 = i;
				}else if(this->no_list[i]->freq < this->no_list[save_2]->freq){
					save_2 = i;
				}
			}
		}

		if(save_1 != -1 and save_2 != -1){
			//criando o nó pai com os dois Nós filhos com as menores frequencias
			//o filho com a menor freqencia sera o filho 0
			if(this->no_list[save_1]->freq < this->no_list[save_2]->freq){
				pai = new No('p', this->no_list[save_1]->freq + this->no_list[save_2]->freq, false);
				pai->filho[0] = this->no_list[save_1];
				pai->filho[1] = this->no_list[save_2];
				if(not this->no_list[save_1]->eh_folha)this->no_list[save_1]->name = '0';
				if(not this->no_list[save_2]->eh_folha)this->no_list[save_2]->name = '1';
			}else{
				pai = new No('p', this->no_list[save_1]->freq + this->no_list[save_2]->freq, false);
				pai->filho[0] = this->no_list[save_2];
				pai->filho[1] = this->no_list[save_1];
				if(not this->no_list[save_2]->eh_folha)this->no_list[save_2]->name = '0';
				if(not this->no_list[save_1]->eh_folha)this->no_list[save_1]->name = '1';
			}

			//criando o Nó pai 
			this->n_nos++;
			this->no_list = (No**)realloc( this->no_list , this->n_nos * sizeof(No*));
			this->no_list[this->n_nos - 1] = pai;

			//colocando so filhos na arvore, para nao serem comparados com a menor frequencia 
			this->no_list[save_1]->na_arvore = true;
			this->no_list[save_2]->na_arvore = true;
		}
	}while(save_1 != -1 and save_2 != -1);

	this->raiz = pai;

	printf("\tConcluido!\n");
}

u_char* Huffman_tree::get_tree_code_aux(No* n, u_char letra, u_char num){
	u_char* aux = NULL;
	if(n->eh_folha and n->name == letra){
		aux = (u_char*)malloc( 2 * sizeof(u_char));
		aux[0] = num;
		aux[1] = '\0';
		return aux;
	}
	if(n->filho[1] != NULL)aux = get_tree_code_aux(n->filho[1], letra, '1');
	if(n->filho[0] != NULL and aux == NULL)aux = get_tree_code_aux(n->filho[0], letra, '0');
	if(aux != NULL and num != 2){
		long long int i = strlen((const char*)aux);
		aux = (u_char*)realloc( aux , (i + 2) * sizeof(u_char));
		//shifitando os bytes para isnerir no começo
		while(i--){
			aux[i + 2] = aux[i + 1];
		}
		aux[1] = aux[0];
		aux[0] = num;
	}
	return aux;
}

u_char* Huffman_tree::get_tree_code(u_char letra){
	u_char* role = get_tree_code_aux(this->raiz, letra, 2);
	return role;
}

void Huffman_tree::Genarate_legenda(){
	if(this->legenda != NULL)return;
	printf("Gerando legenda...\n");

	long long int i;
	this->legenda = (u_char**)calloc(MAX_N_CHAR, sizeof(u_char*));
	for(i=0;i< MAX_N_CHAR ;i++){
		this->legenda[i] = get_tree_code(i);	
	}
	printf("\tConcluido\n");
}

void Huffman_tree::print_legenda(){
	long long int i;
	printf("Printando legenda..\n");
	for(i=0;i< MAX_N_CHAR ;i++){
		printf("%c = %s\n", i, this->legenda[i]);	
	}
}

void Huffman_tree::Genarate_tree_from_text(const char* inputFile){
	printf("Adicionando os caracteres a lista de NOs ...\n");

	FILE* fp = fopen(inputFile,"r");
	u_char aux;
	while(fread(&aux, sizeof(u_char), 1, fp)){
		this->add(aux);	
	}
	fclose(fp);
	printf("\n\tConcluido!\n");
	this->Genarate_tree();
}

void Huffman_tree::write_tree(const char* outputFile, u_char encodeType){
	printf("Salvando a estatistica no arquivo\n");
	FILE* fp = fopen(outputFile, "w+");
	int i = 0;
	fwrite(&encodeType, sizeof(u_char), 1, fp);//colocando o encode type no header
	
	int n_nos2 = 0;

	for(i=0;i< this->n_nos ;i++)if(this->no_list[i]->eh_folha)n_nos2++;

	fwrite(&n_nos2, sizeof(int), 1, fp);//colocando o numero de nos no arquivo

	for(i=0; i< this->n_nos; i++){
		if(this->no_list[i]->eh_folha){
			fwrite(&this->no_list[i]->name, sizeof(u_char), 1, fp);
			fwrite(&this->no_list[i]->freq, sizeof(long long int), 1, fp);
		}
	}
	fclose(fp);
}


void Huffman_tree::compact(const char* inputFile, const char* outputFile, u_char encodeType){
	this->Genarate_legenda();
	this->write_tree(outputFile, encodeType);


	printf("Compactando...\n");

	FILE* fp = fopen(inputFile, "r");
	bit_vector* zip = new bit_vector();

	long long int j=0;
	u_char* aux = NULL;
	u_char xar;
	long long int contador = 0;
	//adicionando os codigos dos caracteres no vetor de bits
	while(fread(&xar, sizeof(u_char), 1, fp)){
		contador++;
		aux = this->legenda[xar];
		j=0;
		if(aux != NULL)while(aux[j] != '\0')zip->add(aux[j++]);
	}
	fclose(fp);
	fp = fopen(outputFile, "a");
	if(fp != NULL){
		fwrite(&contador,sizeof(long long int), 1, fp );
		fwrite(zip->vet, sizeof(u_char), zip->i+1, fp);
	}
	else printf("ERRO! Nao foi possivel abrir o %s\n", outputFile);
	fclose(fp);

	delete zip;
}


void Huffman_tree::descompact(const char* inputFile, const char* outputFile){
	FILE* f_in = fopen(inputFile, "r");
	long long int i;
	fread(&this->n_nos, sizeof(u_char), 1, f_in);//lendo o byte encode type
	fread(&this->n_nos, sizeof(int), 1, f_in);//lendo o nuemro de nos

	char name;
	long long int freq;
	this->no_list = (No**)malloc( this->n_nos * sizeof(No*));
	//listando os nós
	for(i=0;i< this->n_nos ;i++){
		fread(&name, sizeof(u_char), 1, f_in);
		fread(&freq, sizeof(long long int), 1, f_in);
		this->no_list[i] = new No(name, freq, true);
	}

	this->Genarate_tree();


	No* current_node;
	long long int bit_pos = 0;
	current_node = this->raiz;
	
	FILE* f_out = fopen(outputFile, "w");
	u_char aux;
	long long int contador;
	fread(&contador, sizeof(long long int), 1, f_in);
	fread(&aux, sizeof(u_char), 1, f_in);
	while(contador){
		if(current_node->eh_folha){//se o nó atual for uma folha(vulgo char)
			fwrite(&current_node->name, sizeof(u_char), 1, f_out);
			contador--;
			current_node = this->raiz;
		}else{
			if(aux & (1 << bit_pos)){//se o bit for 1
				current_node = current_node->filho[1];
			}else{//se o bit for 0
				current_node = current_node->filho[0];
			}
			bit_pos++;
			if(bit_pos == BYTE_SIZE){
				bit_pos = 0;
				fread(&aux, sizeof(u_char), 1, f_in);
			}
		}
	}
	fclose(f_in);
	fclose(f_out);
}

Huffman_tree::Huffman_tree(){
	this->raiz = NULL;
	this->n_nos = 0;
	this->no_list = NULL;
	this->legenda = NULL;
}

Huffman_tree::~Huffman_tree(){
	if(raiz != NULL)Free_arvore(raiz);
	if(no_list != NULL)free(no_list);
	long long int i;
	if(this->legenda != NULL){
		for(i=0; i < MAX_N_CHAR; i++){
			free(this->legenda[i]);				
		}
		free(this->legenda);
	}
}
