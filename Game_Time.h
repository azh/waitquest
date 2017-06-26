#ifndef GAME_TIME_H
#define GAME_TIME_H

#include <iostream>
#include <cstdlib>
#include <vector>

enum TimeUnit { second, minute, hour, day, week, month, year, decade, century, millenium, epoch, cosmic_year, eon }; // just for readability

void printTime(int y, int x, std::vector<int> time_vect, std::string pstr);
void sortTime(std::vector<int> &time_vect);
void addTime(std::vector<int> &time_vect);
auto addTimeVectors(std::vector<int> t1, std::vector<int> t2);
//auto divideTimeVector(std::vector<int> time_vect, int divisor);
//std::vector<int> calculateAverageTime(std::vector<std::vector<int>> time_vects);

#endif // GAME_TIME_H
