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




int dirx[] = {-1, 0, 1,  0},
	diry[] = { 0, 1, 0, -1};

node* rootNode;

int fd;
int n, m, id, id_enemy;
int currentx, currenty, enemyx, enemyy, currentmovement;
int start_mod_agresiv, mutare_maxima;
uint32_t matrix[32][32];
char flameTimer[32][32];
node* corr[32][32];

void init()
{
	struct sockaddr_in to_station = {0};
	char *buf = (char*)calloc (4, 1);
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	if(fd < 0) {
		return;
	}
	
	to_station.sin_family = AF_INET;

	to_station.sin_port = htons(SERVER_PORT);

	(inet_aton(SERVER_IP, &(to_station.sin_addr)));

	int connected = connect(fd, (struct sockaddr *)&to_station, sizeof(to_station));

	int receive;
	char * name = (char *)malloc(4);
	
	receive = recv(fd, &buf, 4, 0);
	sprintf(name, "%d", buf);
	id = (int) atoi(name);

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
}

void close_connection()
{
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
void calculateFlameTimers()
{
	int i, j, ic, jc;
	for(i=0;i<m;++i)
	{
		for(j=0;j<n;++j)
		{
			if(matrix[i][j]&(11111111<<24!=0))
			{
				flameTimer[i][j]= -(11111111<<24)&matrix[i][j]; //Posibil tre schimbat cu 8 in lil endian
				for(ic=0;ic<=6;++ic)
				{
					if((matrix[ic][j]&(1<<15))||((ic+i)>n)) break;
					if(flameTimer[i][j]<flameTimer[ic][j])
						flameTimer[ic][j]=flameTimer[i][j];
				}
				for(ic=0;ic>=-6;--ic)
				{
					if((matrix[ic][j]&(1<<15))||((i-ic)<0)) break;
					if(flameTimer[i][j]<flameTimer[ic][j])
						flameTimer[ic][j]=flameTimer[i][j];
				}
				for(jc=0;jc<=6;++jc)
				{
					if((matrix[i][jc]&(1<<15))||((jc+j)>m)) break;
					if(flameTimer[i][j]<flameTimer[i][jc])
						flameTimer[i][jc]=flameTimer[i][j];
				}
				for(jc=0;jc>=-6;--jc)
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
	rootNode->weight = MAXWEIGHTKIDS;
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
				newNode->weight = MAXWEIGHTKIDS;
				newNode->parent = corr[current.x][current.y];

				corr[current.x][current.y]->weight /= 2;
                corr[current.x][current.y]->kids.push_back(newNode);
			}
		}
	}
}

void constructRoutes()
{
	if(rootNode->kids.empty()) {
		initializeRoutes();
		return;
	}

	rootNode = corr[currentx][currenty];
	rootNode->kids.push_back(rootNode->parent);
	rootNode->parent = NULL;
}
