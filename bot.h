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

void calculateChainReaction();// uint32_t matrix[32][32], int n, int m, char flameTimer[32][32]

void calculateFlameTimers(); //uint32_t matrix[32][32], int n, int m, char flameTimer[32][32]

void initializeRoutes(); //uint32_t matrix[32][32], int n, int m,  node* corr[32][32], node rootNode

void playNormal();

void playAggresive(); //okay nu stiu inca diferenta prea tare dar ceva imi spune ca o sa avem nevoie
