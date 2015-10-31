#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED
#include <vector>
#include <stdint.h>

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

#define SERVER_IP "192.168.56.101"
#define SERVER_PORT 10000


#endif // DEFINES_H_INCLUDED
