all: bot

bot: bot.cpp bot.h main.cpp
	gcc -o bot bot.cpp bot.h main.cpp

clean:
	rm bot