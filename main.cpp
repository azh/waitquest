#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <math.h>
#include <limits.h>
#include <curses.h>
#include <gmpxx.h>
#include "Game.h"
#include "General.h"

// 4.8*exp((x*log(100/4.8)/(140*log(exp(1))))) - natural equation that hits y: 140 at x: 100

void init()
{
	initscr();
	clear();
	nodelay(stdscr, true);
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	curs_set(0);
}


int main()
{
	init();
	int r, c;
	int pr, pc;
	getmaxyx(stdscr, pr, pc);
	std::string msg = "Welcome to WaitQuest v. -1!";
	std::string menu1 = "1. Start Game";
	std::string menu2 = "2. Simulate Game";
	Game g(0);
	bool b = 0;
	while (!b) {
		getmaxyx(stdscr, r, c);
		if (pr != r || c != pc) {
			clear();
			pr = r;
			pc = c;
		}
		mvprintwc(trunc(r/2/1.5), (c - msg.size())/2, msg.c_str());
		mvprintwc(trunc(r/2*1.5) + 1, (c - menu1.size())/2, menu1.c_str());
		mvprintwc(trunc(r/2*1.5) + 2, (c - menu2.size())/2, menu2.c_str());
		int ch = wgetch(stdscr);
		switch (ch) {
			case 49:
				b = 1;
				break;
			case 50:
				g.State(1);
				b = 1;
				break;
		}
	}
	std::thread maingame(&Game::game, g);
	maingame.join();
	clear();
	endwin();
	return 0;
}
