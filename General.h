#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>
#include <curses.h>

const char* cstr(std::string s);
void mvprintwc(int y, int x, std::string s);
bool fileExists(std::string file);


#endif // GENERAL_H
