#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED
#include <vector>

#define MAXWEIGHTKIDS 16

using namespace std;

struct node{
	int x, y, weight;
	node* parent; //don't forget to allocate
	vector<node*> kids; //don't forget to allocate, or I will find you and free you
};

struct queueNode{
	int x, y;
};


int dirx[] = {-1, 0, 1,  0},
	diry[] = { 0, 1, 0, -1};

#endif // DEFINES_H_INCLUDED
