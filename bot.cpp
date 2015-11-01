#include "bot.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <queue>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>




int dirx[] = {0, -1,  0, 1},
	diry[] = {1,  0, -1, 0};

node* rootNode;

int fd;
int n, m, id, id_enemy;
int currentx, currenty, enemyx, enemyy, currentmovement;
int start_mod_agresiv, mutare_maxima;
uint32_t matrix[32][32];
char flameTimer[32][32];
node* corr[32][32];

int init()
{
	printf("1 ");
	struct sockaddr_in to_station = {0};
	char *buf = (char*)calloc (4, sizeof(char));
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	to_station.sin_family = AF_INET;

	to_station.sin_port = htons(SERVER_PORT);

	(inet_aton(SERVER_IP, &(to_station.sin_addr)));

	int connected = connect(fd, (struct sockaddr *)&to_station, sizeof(to_station));
	
	int receive;
	char * name = (char *)malloc(4);


	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	id = (int) atoi(name);
/*	if (receive < 0)
	{
		printf("Cannot receive id");
	}
	*/
	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	currentmovement = (int) atoi(name);
/*
	if (receive < 0)
	{
		printf("Cannot receive curr_mov");
	}
	*/
	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	start_mod_agresiv = (int) atoi(name);
/*
	if (receive < 0)
	{
		printf("Cannot receive aggr");
	}
	*/
	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	mutare_maxima = (int) atoi(name);

	/*if (receive < 0)
	{
		printf("Cannot receive max");
	}
	*/
	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	n = (int) atoi(name);
/*
	if (receive < 0)
	{
		printf("Cannot receive n");
	}
	*/
	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	m = (int) atoi(name);
/*
	if (receive < 0)
	{
		printf("Cannot receive m");
	}*/
	int i,j;
	for (i = 0; i < n ;i++)
	{
		for (j = 0; j < m; j++)
		{
			receive = recv(fd, &buf, 4, 0);
		/*	if (receive < 0)
			{
				printf("Cannot receive data");
			}*/	
			sprintf(name, "%d", buf);
			matrix[i][j] = (uint32_t) atoi(name);

			int aux = matrix[i][j];
			int k;
			for (k = 1; k <= 8 ;k++)
			{
				if (aux % 2 == 1)
				{
					if (k == id)
					{
						currentx = i;
						currenty = j;
					}
					else
					{
						id_enemy = k;
						enemyx = i;
						enemyy = j;
					}
				}
				aux = aux >> 1;
			}
		}
	}
	/*printf("%d %d %d \n", id, n, m);
	for (i = 0; i < n;i++)
	{
		for (j = 0; j < m; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
*/	
	return start_mod_agresiv;
//	return 2;
}

void close_connection()
{
	shutdown(fd, 2);
	close(fd);
}

void readMatrix()
{
	int receive;
	char * name = (char *)malloc(4);
	char *buf = (char*)calloc (4, 1);

	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	currentmovement = (int) atoi(name);

	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	start_mod_agresiv = (int) atoi(name);

	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	mutare_maxima = (int) atoi(name);


	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	n = (int) atoi(name);

	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	m = (int) atoi(name);

	int i,j;
	for (i = 0; i < n ;i++)
	{
		for (j = 0; j < m; j++)
		{
			receive = recv(fd, &buf, 4, 0);
			sprintf(name, "%d", buf);
			matrix[i][j] = (uint32_t) atoi(name);

			if (matrix[i][j] & 1 << id)
			{
				currentx = i;
				currenty = j;
			}
			if (matrix[i][j] & 1 << id_enemy)
			{
				enemyx = i;
				enemyy = j;
			}
		}
	}
}

void sendMove(bool place, int movedir)
{
	int move = movedir | (place << 31);
	char *buf = (char*)calloc (4, 1);
	sprintf(buf, "%d", move);
	write(fd, buf, sizeof(buf));
}

bool operator<(const queueNode& L, const queueNode& R)
{
	return (matrix[L.x][L.y]&(11111111<<24))<(matrix[R.x][R.y]&(11111111<<24));
}

void calculateChainReaction()
{
	int i,j;
	std::priority_queue<queueNode,vector<queueNode> > Q;
	queueNode current;
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<m;++j)
		{
			if(matrix[i][j]&(11111111<<24))
			{
				current.x=i;
				current.y=j;
				Q.push(current);
			}
		}
	}
	
	while(!Q.empty())
	{
		current=Q.top();
		Q.pop();
		for(i=1;i<=6;++i)
		{
			if((matrix[current.x+i][current.y]&(1<<15))==1||(current.x+i > n)) break;

			if(matrix[current.x+i][current.y]&(11111111<<24))
				matrix[current.x+i][current.y] = (matrix[current.x][current.y]&(11111111<<24))+(1<<24);
		}
		for(i=-6;i<0;++i)
		{
			if((matrix[current.x+i][current.y]&(1<<15))==1||(current.x-i<0)) break;
			if(matrix[current.x+i][current.y]&(11111111<<24))
				matrix[current.x+i][current.y] = (matrix[current.x][current.y]&(11111111<<24))+(1<<24);
		}
		for(j=1;j<=6;++j)
		{
			if((matrix[current.x][current.y+j]&(1<<15))==1||(current.y+j>m)) break;
			if(matrix[current.x][current.y+j]&(11111111<<24))
				matrix[current.x][current.y+j] = (matrix[current.x][current.y]&(11111111<<24))+(1<<24);
		}
		for(j=-6;j<0;++j)
		{
			if((matrix[current.x][current.y+j]&(1<<15))==1||(current.y+j<0)) break;
			if(matrix[current.x][current.y+j]&(11111111<<24))
				matrix[current.x][current.y+j] = (matrix[current.x][current.y]&(11111111<<24))+(1<<24);
		}
	}
}

void calculateFlameTimers()
{
	int i, j, ic, jc;
	for(i=0;i<n;++i)
	{
		for(j=0;j<m;++j)
		{
			if(matrix[i][j]&(11111111<<24!=0))
			{
				flameTimer[i][j]= -(11111111<<24)&matrix[i][j]; //Posibil tre schimbat cu 8 in lil endian
				for(ic=1;ic<=6;++ic)
				{
					if((matrix[ic][j]&(1<<15))||((ic+i)>n)) break;
					if(flameTimer[i][j]<flameTimer[ic][j])
						flameTimer[ic][j]=flameTimer[i][j];
				}
				for(ic=-1;ic>=-6;--ic)
				{
					if((matrix[ic][j]&(1<<15))||((i-ic)<0)) break;
					if(flameTimer[i][j]<flameTimer[ic][j])
						flameTimer[ic][j]=flameTimer[i][j];
				}
				for(jc=1;jc<=6;++jc)
				{
					if((matrix[i][jc]&(1<<15))||((jc+j)>m)) break;
					if(flameTimer[i][j]<flameTimer[i][jc])
						flameTimer[i][jc]=flameTimer[i][j];
				}
				for(jc=-1;jc>=-6;--jc)
				{
					if((matrix[i][jc]&(1<<15))||((j-jc)<0)) break;
					if(flameTimer[i][j]<flameTimer[i][jc])
						flameTimer[i][jc]=flameTimer[i][j];
				}
			}
		}
	}
}

void initializeRoutes() {
	int i;

	queueNode current, next;
	current.x = currentx;
	current.y = currenty;
	queue<queueNode> q;
	q.push(current);

	node * newNode = NULL;
	rootNode = new node;
	rootNode->x = currentx;
	rootNode->y = currenty;
	rootNode->weight = MINWEIGHTKIDS;
	rootNode->parent = NULL;
	corr[currentx][currenty] = rootNode;

	while(!q.empty())
	{
		current = q.front();
		q.pop();

		for(i=0; i<4; ++i)
		{
			next.x = current.x + dirx[i];
			next.y = current.y + diry[i];
			if((matrix[next.x][next.y]&(1<<15)) == 0)
			{
				q.push(next);

				newNode = new node;
				newNode->x = next.x;
				newNode->y = next.y;
				newNode->weight = MINWEIGHTKIDS;
				newNode->parent = corr[current.x][current.y];
				newNode->kids.push_back(newNode->parent);
				
				corr[next.x][next.y] = newNode;
				corr[current.x][current.y]->weight *= 2;
                corr[current.x][current.y]->kids.push_back(newNode);
			}
		}
	}
}

bool is_walkable(int time, node* currentNode) {
	return  (matrix[currentNode->x][currentNode->y] &(11111111<<24)) == 0 &&
			(matrix[currentNode->x][currentNode->y] &(11111111<<16)) == 0 &&
			(flameTimer[currentNode->x][currentNode->y] == 0 ||
			 flameTimer[currentNode->x][currentNode->y] + time <= 0);
}

void constructRoutes(node* currentNode, node* parent, int &maxweight, int &dir, int recursionlevel, int &depth)
{
	int weight = -120, i=-1, k;
	if(rootNode == NULL || rootNode->kids.empty())
	{
		initializeRoutes();
		return;
	}
/*
	currentNode->parent = parent;

	for(k=0; k<currentNode->kids.size(); ++k)
	{
		if(is_walkable(recursionlevel+1, currentNode->kids[k]) &&
		   currentNode->kids[k] != currentNode->parent)
		{
			constructRoutes(currentNode->kids[k], currentNode, weight, i, recursionlevel+1, depth);

			weight += currentNode->weight;
			if(flameTimer[currentNode->x][currentNode->y])
				weight += BOMBRANGE;
			if(currentNode->x == enemyx && currentNode->y == enemyy)
				weight += ENEMYVALUE;

			if(weight > maxweight)
			{
				maxweight = weight;
				dir = i;
				depth = recursionlevel + 1;
			}
		}
	}*/
}

void playNormal(bool &place, int &movedir)
{
	int length, weight;
	movedir = -1;

	constructRoutes(rootNode, NULL, weight, movedir, 1, length);
	/*if(length > 3)
	{
		place = length<=6;
		++movedir;
		return;
	}

	int maxweight = -1;
	node tempNode, currNode;
	place = 1;
	weight = 0;
	// euristica, n-as paria ca si mere dar fie...

	for(int i=0; i<4; ++i)
	{
		currNode.x = rootNode->x + dirx[i];
		currNode.y = rootNode->y + diry[i];
		if(is_walkable(1, &currNode));
			for(int j=0; j<4; ++j)
			{
				tempNode.x = currNode.x + dirx[j];
				tempNode.y = currNode.y + diry[j];
				if(is_walkable(2, &tempNode))
					++weight;
			}
		if(weight > maxweight)
		{
			maxweight = weight;
			movedir = i;
		}
	}
	++movedir;*/
}

void playAggresive(bool &place, int&movedir) {
	//nota: cand suntem aproape de adversar, tactica ar trebui sa fie aceeasi ca la play aggressive
	//care e tactica asta...yeah idk
}

void cleanup()
{
	int i, j;
	for(i=0; i<n; ++i)
		for(j=0; j<m; ++j)
			if(corr[i][j])
				free(corr[i][j]);
}
