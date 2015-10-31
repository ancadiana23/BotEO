#include "bot.h"
#include <queue>

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
