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

bool operator<(const queueNode& L, const queueNode& R)
{
	return (matrix[L.x][L.y]&(11111111<<24))<(matrix[R.x][R.y]&(11111111<<24));
}

void calculateChainReaction()
{
	int i,j;
	std::priority_queue<queueNode,vector<queueNode> > Q;
	queueNode current;
	for(int i=0;i<m;++i)
	{
		for(int j=0;j<n;++j)
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
		for(i=1;i<=6;++i)
		{
			if((matrix[current.x+i][current.y]&((1<<15)==1))||(current.x+i>m)) break;
			if(matrix[current.x+i][current.y]&(11111111<<24)
				matrix[current.x+i][current.y] = matrix[current.x][current.y]&(11111111<<24)+1<<24;
		}
		for(i=-6;i<0;++i)
		{
			if((matrix[current.x+i][current.y]&((1<<15)==1))||(current.x-i<0)) break;
			if(matrix[current.x+i][current.y]&(11111111<<24)
				matrix[current.x+i][current.y] = matrix[current.x][current.y]&(11111111<<24)+1<<24;
		}
		for(j=1;j<=6;++j)
		{
			if((matrix[current.x][current.y+j]&(1<<15==1))||(current.y+j>n)) break;
			if(matrix[current.x][current.y+j]&(11111111<<24))
				matrix[current.x][current.y+j] = matrix[current.x][current.y]&(11111111<<24)+1<<24;
		}
		for(j=-6;j<0;++j)
		{
			if((matrix[current.x][current.y+j]&(1<<15)==1)||(current.y+j<0)) break;
			if(matrix[current.x][current.y+j]&(11111111<<24))
				matrix[current.x][current.y+j] = matrix[current.x][current.y]&(11111111<<24)+1<<24;
		}
		Q.pop();
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

				corr[current.x][current.y]->weight *= 2;
                corr[current.x][current.y]->kids.push_back(newNode);
			}
		}
	}
}

inline bool is_walkable(int time, node* currentNode) {
	return  (matrix[currentNode->x][currentNode->y] &(11111111<<24)) == 0 &&
			(matrix[currentNode->x][currentNode->y] &(11111111<<16)) == 0 &&
			(flameTimer[currentNode->x][currentNode->y] == 0 ||
			 flameTimer[currentNode->x][currentNode->y] + time <= 0);
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
