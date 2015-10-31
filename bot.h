#include <stdint.h>

void readMatrix(uint32_t matrix[32][32], int&n, int&m);

void calculateChainReaction(uint32_t matrix[32][32], int n, int m);

void calculateFlameTimers(uint32_t matrix[32][32], int n, int m, char flameTimer[32][32]);

void initializeExistRoutes(uint32_t matrix[32][32], int n, int m, char exitRoutes[32][32]);
