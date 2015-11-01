#ifndef BOT_H_INCLUDED
#define BOT_H_INCLUDED

#include "defines.h"
#include <stdio.h>
#include <stdlib.h>

int init();
void close_connection();
void sendMove(bool place, uint32_t movedir);
void readMatrix();//uint32_t matrix[32][32], int& n, int& m
	// Hai ca o fac acum repede pana termina Anca cu scripturile
	// ar trebui sa arate ca in enunt

void calculateChainReaction();// uint32_t matrix[32][32], int n, int m, char flameTimer[32][32]
	//Anca can Dijkstra? I can't Dijkstra

void calculateFlameTimers(); //uint32_t matrix[32][32], int n, int m, char flameTimer[32][32]
	//astea ar trebui sa fie cateva foruri, can Lexi please do it

void initializeRoutes(); //apelat la prima mutare

void constructRoutes(node* currentNode, node* parent, int &maxweight, uint32_t &dir, int recursionlevel, int &depth); //uint32_t matrix[32][32], int n, int m,  node* corr[32][32], node rootNode

void playNormal(bool &place, uint32_t &movedir);

void playAggresive(); //okay nu stiu inca diferenta prea tare dar ceva imi spune ca o sa avem nevoie

#define SERVER_PORT 10000
#define SERVER_IP   "192.168.56.101"

#endif
