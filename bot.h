#include <stdint.h>
#include <vector>

typedef struct {
	int x, y, weight;
	node* parent; //don't forget to allocate
	std::vector node* kids; //don't forget to allocate, or I will find you and free you
} node;

node rootNode;

int n, m;
uint32_t matrix[32][32];
char flameTimer[32][32];
node* corr[32][32];

void readMatrix();//uint32_t matrix[32][32], int& n, int& m
	// Hai ca o fac acum repede pana termina Anca cu scripturile
	// ar trebui sa arate ca in enunt

void calculateChainReaction();// uint32_t matrix[32][32], int n, int m, char flameTimer[32][32]
	//Anca can Dijkstra? I can't Dijkstra

void calculateFlameTimers(); //uint32_t matrix[32][32], int n, int m, char flameTimer[32][32]
	//astea ar trebui sa fie cateva foruri, can Lexi please do it

void initializeRoutes(); //uint32_t matrix[32][32], int n, int m,  node* corr[32][32], node rootNode

void playNormal();

void playAggresive(); //okay nu stiu inca diferenta prea tare dar ceva imi spune ca o sa avem nevoie
