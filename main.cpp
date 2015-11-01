#include "bot.h"

int main(int argc, char * argv[])
{
	int i;
	bool place;
	int movedir;
	int start_mod_agresiv = init();
	printf("%d mod agresiv\n", start_mod_agresiv);
	for (i = 0; i < start_mod_agresiv; i++)
	{
		calculateChainReaction();
		calculateFlameTimers();
		playNormal(place, movedir);
		sendMove(place, movedir);
		readMatrix();
	}
	close_connection();
	printf("%s\n", "SUCCESS");
}
