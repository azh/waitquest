#include <iostream>
#include <curses.h>
#include "General.h"
#include <sys/stat.h>

const char* cstr(std::string s)
{
	return s.c_str();
}

void mvprintwc(int y, int x, std::string s)
{
	move(y, 0);
	clrtoeol();
	mvprintw(y, x, cstr(s));
}

bool fileExists(std::string file)
{
	struct stat st;
	return stat(file.c_str(), &st) != -1;
}
