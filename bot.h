#include "defines.h"
#include <stdint.h>

node* rootNode;

int n, m;
int currentx, currenty, currentmovement;
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

void initializeRoutes(); //apelat la prima mutare

void constructRoutes(); //uint32_t matrix[32][32], int n, int m,  node* corr[32][32], node rootNode

void playNormal();

void playAggresive(); //okay nu stiu inca diferenta prea tare dar ceva imi spune ca o sa avem nevoie
