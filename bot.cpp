#include "bot.h"
#include <queue>

void calculateFlameTimers()
{
	for(int i=0;i<m;++i)
	{
		for(int j=0;j<n;++j)
		{
			if(matrix[i][j]&(11111111<<24!=0))
			{
				flameTimer[i][j]= -(11111111<<24)&matrix[i][j]; //Posibil tre schimbat cu 8 in lil endian
				for(int ic=0;ic<=6;++ic)
				{
					if((matrix[ic][j]&(1<<15))||((ic+i)>n)) break;
					if(flameTimer[i][j]<flameTimer[ic][j])
						flameTimer[ic][j]=flameTimer[i][j];
				}
				for(int ic=0;ic>=-6;--ic)
				{
					if((matrix[ic][j]&(1<<15))||((i-ic)<0)) break;
					if(flameTimer[i][j]<flameTimer[ic][j])
						flameTimer[ic][j]=flameTimer[i][j];
				}
				for(int jc=0;jc<=6;++jc)
				{
					if((matrix[i][jc]&(1<<15))||((jc+j)>m)) break;
					if(flameTimer[i][j]<flameTimer[i][jc])
						flameTimer[i][jc]=flameTimer[i][j];
				}
					for(int jc=0;jc>=-6;--jc)
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

	node * newNode = nullptr;
	rootNode = new node;
	rootNode->x = currentx;
	rootNode->y = currenty;
	rootNode->weight = MAXWEIGHTKIDS;
	rootNode->parent = nullptr;
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

}
