all: bot

bot: bot.cpp bot.h main.cpp
	g++ -o bot bot.cpp bot.h main.cpp

clean:
	rm bot