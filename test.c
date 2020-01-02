#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

//#define hash_size 1009u
#define MAXVALUE INT_MAX

//----------------------------------------------------------
typedef struct file_data { 

// public data
long word_pos; // zero-based
long word_num; // zero-based
char word[64];

// private data
FILE *fp;
long current_pos; // zero-based
}
file_data_t;


typedef struct hash_data {
	
	struct hash_data *next;
	char word[64];
	int firstP;
	int IndexLastWord;
	int contador;
	int minD;
	int maxD;
	int totalD;
	
}hash_data;




unsigned int hash_function(const char *str,unsigned int s)
{
unsigned int h;
for(h = 0u;*str != '\0';str++)
h = 157u * h + (0xFFu & (unsigned int)*str); // arithmetic overflow may occur here (just ignore it!)
return h % s; // due to the unsigned int data type, it is guaranteed that 0 <= h % s < s
}

hash_data *find_data(const char *key, hash_data *hashT[], int hashSize) {

		unsigned int idx;
		hash_data *hd;
		idx = hash_function(key,hashSize);
		hd = hashT[idx];
		
		while(hd != NULL && strcmp(key,hd->word) != 0)
			hd = hd->next;
		
		return hd;
}

static hash_data *new_hash_data(int index, char * word) {

	hash_data *hd = (hash_data *)malloc(sizeof(hash_data));
	if(hd == NULL) {
		fprintf(stderr,"Out of memory\n");
		exit(1);
	}
	strcpy(hd->word, word);
	hd->firstP = index;
	hd->IndexLastWord = index;
	hd->contador = 1;
	hd->minD = MAXVALUE;
	hd->maxD = 0;
	hd->totalD = 0;
	hd->next = NULL;
	
	
	return hd;
}

void update(hash_data * node,int posicao){
	
	if((posicao - node->IndexLastWord) > node->maxD)
		node->maxD = (posicao - node->IndexLastWord);

	if((posicao - node->IndexLastWord) < node->minD)
		node->minD = (posicao - node->IndexLastWord);

	node->contador += 1;
	node->totalD += (posicao - node->IndexLastWord); 
	node->IndexLastWord = posicao;
}

hash_data** resize(int lastSize, hash_data**hashtable, int newSize){

	hash_data** newHashTable = malloc(newSize * sizeof(hash_data));
	for(int i=0;i<newSize;i++){
		newHashTable[i]=NULL;
	}

	int hc = 0;
	for(int i=0;i<lastSize;i++){
		for(hash_data* node = hashtable[i]; node!=NULL; node = node->next){

			hc = hash_function(node->word, newSize);
			hash_data *tempNode = node;
			tempNode->next=NULL;

			hash_data *head = newHashTable[hc];
			int count = 0;

		    while (head!=NULL) {
		        head = head->next;
		        count++;
		    }
		    if(count == 0)
		    	newHashTable[hc] = tempNode;
		    else
		    	head = tempNode;
		}
	}

	return newHashTable;
}



//------------------------------------------------------------------


int open_text_file(char *file_name,file_data_t *fd) {
	
	fd->fp = fopen(file_name,"rb");

	if(fd->fp == NULL)
		printf("Error opening file!\n");
		return -1;

	fd->word_pos = -1;
	fd->word_num = -1;
	fd->word[0] = '\0';
	fd->current_pos = -1;

	return 0;
}


void close_text_file(file_data_t *fd) {
	
	fclose(fd->fp);
	fd->fp = NULL;
}

int read_word(file_data_t *fd) {
	int i,c;
	// skip white spaces
	do{

		c = fgetc(fd->fp);

		if(c == EOF)
			return -1;

		fd->current_pos++;

	}while(c <= 32);
	
	// record word
	fd->word_pos = fd->current_pos;
	fd->word_num++;
	fd->word[0] = (char)c;


	for(i = 1;i < (int)sizeof(fd->word) - 1;i++){
		c = fgetc(fd->fp);

		if(c == EOF)
			break; // end of file

		fd->current_pos++;

		if(c <= 32)
			break; // terminate word

		fd->word[i] = (char)c;
	}

	fd->word[i] = '\0';
	return 0;
}

//-------------------------------------------------------

int main(int argc, char const *argv[])
{
	file_data_t *ficheiro = (file_data_t *)malloc(sizeof(file_data_t));
	open_text_file("SherlockHolmes.txt", ficheiro);

	int hash_size = 1009u;
	hash_data ** hash_table = malloc(hash_size * sizeof(hash_data));
	int wordContador = 0;
	int hash_code = 0;
	hash_data *temp = NULL;

	for(int i=0;i<hash_size;i++){
		hash_table[i]=NULL;
	}
	
	while(read_word(ficheiro) != -1){
		wordContador++;
		if(wordContador > (int)hash_size * 0.6){
			hash_table = resize((int)hash_size, hash_table, (int)hash_size*2);
			hash_size = (int)hash_size * 2;
		}
		
		if(find_data(ficheiro->word, hash_table, hash_size) == NULL){
			hash_code = hash_function(ficheiro->word, hash_size);
			hash_data *t = hash_table[hash_code];
			//hash_data *anterior = NULL;
			int count = 0;
			while(t != NULL){
				//anterior = t;
				t = t->next;
				count++;
			}

			hash_data *newNode = new_hash_data(ficheiro->word_pos, ficheiro->word);
			if(count == 0)
				hash_table[hash_code] = newNode;
			else
				t = newNode;
			
			
		}else{
				update(find_data(ficheiro->word, hash_table, hash_size), ficheiro->word_pos);
			}

	}

	for(int i=0;i<hash_size;i++){
		if(hash_table[i]!=NULL){
			hash_data *node = hash_table[i];
			while(node != NULL){
				printf("%s  ", node->word);
				printf("First position: %d  ||  ", node->firstP);
				printf("Last position: %d  ||  ", node->IndexLastWord);
				printf("Counter: %d  ||  ", node->contador);
				printf("Largest distance: %d  ||  ", node->maxD);
				if(node->minD == INT_MAX){
					printf("Smallest distance: 0  ||  ");
					printf("Average distance: 0\n\n");
				}else{
					printf("Smallest distance: %d  ||  ", node->minD);
					printf("Average distance: %d\n\n", (int)( (node->totalD)/(node->contador-1) ));
				}
				node = node->next;

			}
		}
	}

	close_text_file(ficheiro);
	free(ficheiro);

	return 0;
}