#include "bot.h"
#include <queue>

int dirx[] = {-1, 0, 1,  0},
	diry[] = { 0, 1, 0, -1};

node* rootNode;

int n, m;
int currentx, currenty, currentmovement;
uint32_t matrix[32][32];
char flameTimer[32][32];
node* corr[32][32];

void readMatrix()
{

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

				corr[current.x][current.y]->weight *= 2;
                corr[current.x][current.y]->kids.push_back(newNode);
			}
		}
	}
}

bool is_walkable(int time, node* currentNode) {
	return (matrix[currentNode->x][currentNode->y] &(11111111<<24)) == 0 &&
			flameTimer[currentNode->x][currentNode->y] >= time;
}

void constructRoutes(node* currentNode, node* parent, char &maxweight, char &x, char &y, int recursionlevel)
{
	char weight = -120, i=-1, j=-1;
	if(rootNode == NULL || rootNode->kids.empty())
	{
		initializeRoutes();
		return;
	}

	currentNode->parent = parent;

	for(i=0; i<currentNode->kids.size(); ++i)
	{
		if(is_walkable(recursionlevel+1, currentNode->kids[i]) &&
		   currentNode->kids[i] != currentNode->parent)
		{
			constructRoutes(currentNode->kids[i], currentNode, weight, i, j, recursionlevel+1);
			if(weight > maxweight)
			{
				maxweight = weight;
				x = i;
				y = j;
			}
		}
	}
}
