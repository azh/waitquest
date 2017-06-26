#include <iostream>
#include <thread>
#include <vector>
#include <math.h>
#include <cstdlib>
#include <curses.h>
#include "Game_Time.h"
#include "General.h"

void printTime(int y, int x, std::vector<int> time_vect, std::string pstr)
{
	if (time_vect.size() > 0) {
		for (size_t i = time_vect.size() - 1; i > 0; i--) {
			switch (i) {
				default:
					pstr = "WTF???";
					break;
				case 1:
					pstr += std::to_string(time_vect.at(i)) + " minute(s), ";
					break;
				case 2:
					pstr += std::to_string(time_vect.at(i)) + " hour(s), ";
					break;
				case 3:
					pstr += std::to_string(time_vect.at(i)) + " day(s), ";
					break;
				case 4:
					pstr += std::to_string(time_vect.at(i)) + " weeks(s), ";
					break;
				case 5:
					pstr += std::to_string(time_vect.at(i)) + " months(s), ";
					break;
			} // TODO: change default so after eons the game outputs time in scientific notation
		}
		if (time_vect.size() > 0) {
			pstr += std::to_string(time_vect.at(0)) + " second(s)";
		}
		mvprintwc(y, x, pstr);
	}
}

void sortTime(std::vector<int> &time_vect) {
	bool leap_year = false;
	if (time_vect.size() > 2) { // TODO: implement this leap year algo
		if (time_vect.at(2) % 4 == 0 && time_vect.at(2) % 100 != 0) {
			leap_year = true;
		} else if (time_vect.at(2) % 100 == 0 && time_vect.at(2) % 400 != 0) {
			leap_year = true;
		}
	}
	try { // TODO: rewrite this into a loop and container of pairs
		if (time_vect.at(0) > 59) {
			if (time_vect.size() < 2) {
				time_vect.push_back(time_vect.at(0) / 60);
			} else {
				time_vect.at(1) += time_vect.at(0) / 60;
			}
			time_vect.at(0) = time_vect.at(0) % 60;
		}
		if (time_vect.at(1) > 59) {
			if (time_vect.size() < 3) {
				time_vect.push_back(time_vect.at(1) / 60);
			} else {
				time_vect.at(2) += time_vect.at(1) / 60;
			}
			time_vect.at(1) = time_vect.at(1) % 60;
		}
		if (time_vect.at(2) > 23) {
			if (time_vect.size() < 4) {
				time_vect.push_back(time_vect.at(2) / 24);
			} else {
				time_vect.at(3) += time_vect.at(2) / 24;
			}
			time_vect.at(2) = time_vect.at(2) % 24;
		}
		if (time_vect.at(3) > 7) {
			if (time_vect.size() < 5) {
				time_vect.push_back(time_vect.at(3) / 7);
			} else {
				time_vect.at(4) += time_vect.at(3) / 7;
			}
			time_vect.at(3) = time_vect.at(3) % 7;
		}
	}
	catch (const std::out_of_range& error) {
		return;
	}
}

void addTime(std::vector<int> &time_vect) {
	if (time_vect.size() == 0) {
		time_vect.push_back(0);
	}
	time_vect.at(0)++;
}

auto addTimeVectors(std::vector<int> t1, std::vector<int> t2)
{
	std::vector<int> rt = (t1.size() > t2.size()) ? t1 : t2;
	std::vector<int> ot = (rt == t1) ? t2 : t1;
	for (size_t i = 0; i < ot.size(); i++) {
		rt.at(i) += ot.at(i);
	}
	sortTime(rt);
	return rt;
}

// auto subtractTimeVectors

/*auto divideTimeVector(std::vector<int> time_vect, int divisor) {
	std::vector<double> temp_vect;
	for (size_t i = 0; i < time_vect.size(); i++) {
		temp_vect.push_back(time_vect.at(i));
	}
	for (size_t i = temp_vect.size() - 1; i > 0; i++) {
		temp_vect.at(i) = temp_vect.at(i) / divisor;
		if (temp_vect.at(i) - trunc(temp_vect.at(i)) > 0) {
			switch (i) {
				case second:
					time_vect.at(second) = round(temp_vect.at(second));
					break;
				case minute:
					time_vect.at(second) += round((temp_vect.at(minute) - trunc(temp_vect.at(minute))) * 60);
					time_vect.at(minute) = trunc(temp_vect.at(minute));
					break;
				case hour:
					time_vect.at(minute) += round((temp_vect.at(hour) - trunc(temp_vect.at(hour)) * 60));
					time_vect.at(hour) = trunc(temp_vect.at(hour));
					break;
				case day:
					time_vect.at(hour) += round((temp_vect.at(day) - trunc(temp_vect.at(day)) * 24));
					time_vect.at(day) = trunc(temp_vect.at(day));
					break;
				case week:
					time_vect.at(day) += round((temp_vect.at(week) - trunc(temp_vect.at(week)) * 7));
					time_vect.at(week) = trunc(temp_vect.at(week));
					break;
				case month:
					time_vect.at(week) += round((temp_vect.at(month) - trunc(temp_vect.at(month)) * 4.34812));
					time_vect.at(month) = trunc(temp_vect.at(month));
					break;
				case year:
					time_vect.at(month) += round((temp_vect.at(year) - trunc(temp_vect.at(year)) * 12));
					time_vect.at(year) = trunc(temp_vect.at(year));
					break;
				case decade:
					time_vect.at(year) += round((temp_vect.at(decade) - trunc(temp_vect.at(decade)) * 10));
					time_vect.at(decade) = trunc(temp_vect.at(decade));
					break;
				case century:
					time_vect.at(decade) += round((temp_vect.at(century) - trunc(temp_vect.at(century)) * 10));
					time_vect.at(century) = trunc(temp_vect.at(century));
					break;
			}
		}
	}
	std::vector<bool> useless;
	sortTime(time_vect, useless);
	return time_vect;
}*/

/*std::vector<int> calculateAverageTime(std::vector<std::vector<int>> time_vects)
{
	std::vector<int> time_vect = time_vects.at(0);
	for (size_t i = 1; i < time_vects.size(); i++) {
		time_vect = addTimeVectors(time_vect, time_vects.at(i));
	}
	time_vect = divideTimeVector(time_vect, time_vects.size());
	return time_vect;
}*/

