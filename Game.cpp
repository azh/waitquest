#ifdef _WIN32
	#define WQ_SAVE_LOCATION R"(save\wqsave)"
	#define TESTDATA R"(testdata\)"
#else
	#define WQ_SAVE_LOCATION R"(save/wqsave)"
	#define TESTDATA R"(testdata/)"
#endif

#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <time.h>
#include <cstdlib>
#include <chrono>
#include <math.h>
#include <limits.h>
#include <ctime>
#include <curses.h>
#include <gmpxx.h>
#include "Game_Time.h"
#include "General.h"
#include "Character.h"
#include "Game.h"

std::vector<int> time_played = {0};
std::vector<int> real_time_played = {0};

void Game::saveGame()
{
	nodelay(stdscr, false);
	nocbreak();
	echo();
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(max_y - 1, 0, "Enter save name: ");
	std::string savename;
	getline(std::cin, savename);
	std::ofstream save(WQ_SAVE_LOCATION);
	if (!save) {
		mvprintw(max_y - 1, 0, "Can't save to this file!");
		return;
	}
	std::string spstr = "Saving to \"" + savename + ".sav\"";
	mvprintw(max_y - 1, 0, spstr.c_str());
	save << time_played.size();
	for (size_t i = 0; i < time_played.size(); i++) {
		save << time_played.at(i) << std::endl;
	}
	save.close();
	// TODO: this function
}

void Game::loadGame()
{
	nodelay(stdscr, false);
	nocbreak();
	echo();
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	mvprintw(max_y - 1, 0, "Enter save name to load: ");
	std::string savename;
	getline(std::cin, savename);
	std::ifstream save(WQ_SAVE_LOCATION);
	// TODO: this function also
}


void Game::game()
{
	clear();
	srand(time(NULL));
	int l1 = 1100;
	int l2 = 200;
	int limit = (rand() % l1) + l2;
	int minlimit = limit;
	int maxlimit = limit;
	int real_limit = 1000; // 1000 ms per sec
	unsigned int frames = 0;
	int real_frames = 0;
	unsigned int age = 0; // real age: current effects are on time perception and dying
	unsigned int max_age = 0;
	unsigned int min_age = INT_MAX;
	uint64_t generation = 0;
	uint64_t totalage = 0;
	std::vector<int> lifespan_vect;
	std::vector<Upgrade> upgrades;
	std::vector<Character> ancestry;
	std::vector<std::vector<int>> lifespans;
	Character player = Character("Waitmaster #0", (rand() % l1) + l2, 0, true, upgrades, lifespan_vect, ancestry); // TODO: actually implement this

	if (state == 0) {
		for (;;) {
			// 1. PROCESS INPUT
			int c = wgetch(stdscr);
			switch(c) {
				case KEY_UP:
					time_played.at(0)++;
					break;
				case KEY_DOWN:
					time_played.at(0)--;
					break;
				case KEY_LEFT:
					time_played.at(0) -= 10;
					break;
				case KEY_RIGHT:
					time_played.at(0) += 10;
					break;
			}
			// 2. UPDATE GAME
			if (real_frames >= real_limit) {
				real_frames = 0;
				addTime(real_time_played);
				sortTime(real_time_played);
				refresh();
			}
			if (frames >= player.getLimit()) {
				frames = 0;
				addTime(time_played);
				addTime(lifespan_vect);
				sortTime(time_played);
				refresh();
				int rnd = rand();
				if (rnd % 7 == 0) {
					move(1, 0);
					clrtoeol();
					age++;
					if (rnd % 2) {
						limit = limit - (pow(log(age), 3));
					}
					mvprintw(1, 0, cstr("True Age: " + std::to_string(age)));
				}
			}
			if (limit <= 0) {
				lifespans.push_back(lifespan_vect);
				lifespan_vect.clear();
				max_age = (age > max_age) ? age : max_age;
				min_age = (age < min_age) ? age : min_age;
				limit = (rand() % l1) + l2;
				maxlimit = (limit > maxlimit) ? limit : maxlimit;
				minlimit = (limit < minlimit) ? limit : minlimit;
				generation++;
				totalage += age;
			}
			if (time_played.at(0) < 0 || real_time_played.at(0) < 0) {
				mvprintw(0, 0, "that's not actually possible you cheater");
				mvprintw(1, 0, "哎哟");
				getch();
				return;
			}
			if (state == 0) {
				mvprintwc(2, 0, "Limit (relative time): " + std::to_string(limit));
				mvprintwc(3, 0, "Max limit: " + std::to_string(maxlimit));
				mvprintwc(4, 0, "Min limit: " + std::to_string(minlimit));
				printTime(11, 0, real_time_played, "You have waited ");
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				real_frames++;
				frames++;
				printTime(0, 0, time_played, "Waitmaster #" + std::to_string(generation) + " has waited ");
			}
		}
	} else if (state == 1) {
		uint64_t gens = 0;
		mvprintwc(0, 0, "Enter # of generations to simulate: ");
		echo();
		curs_set(1);
		timeout(-1);
		nodelay(stdscr, false);
		while (gens == 0) {
			scanw("%d", &gens);
		}
		time_t t;
		struct tm * ti;
		char buf[256];
		time(&t);
		ti = localtime(&t);
		strftime(buf, 256, "wqsim%Y-%m-%d", ti);
		std::string outname(buf);
		char buf2[256];
		mvprintwc(0, 0, "Enter a filename (optional): ");
		wgetnstr(stdscr, buf2, 256);
		if (std::string(buf2).size() > 0) {
			outname = buf2;
		}
		if (fileExists(outname + ".csv")) {
			std::string non = outname + "-1.csv";
			for (int i = 1; fileExists(non); i++) {
				non = outname + "-" + std::to_string(i) + ".csv";
			}
			outname = non;
		} else {
			outname = outname + ".csv";
		}
		outname = TESTDATA + outname;
		std::ofstream out(outname);
		typedef std::chrono::high_resolution_clock Clock;
		typedef std::chrono::milliseconds msecs;
		typedef std::chrono::duration<float> fsec;
		auto bl = Clock::now();
		if (out.is_open()) {
			out << "# Limit,Age" << std::endl;
			out << limit << ",";
			while (generation < gens) { // game core
				if (rand() % 7 == 0) {
					move(1, 0);
					clrtoeol();
					age++;
					if (rand() % 2) {
						limit = limit - (pow(log(age), 3));
					}
				}
				if (limit <= 0) {
					out << age << "\n";
					generation++;
					if (generation == gens) {
						break;
					} else {
						limit = (rand() % l1) + l2;
						out << limit << ",";
						age = 0;
					}
				}
			}
		out.close();
		} else {
			mvprintwc(0, 0, "Cannot create new simulation file!");
			getch();
		}
		auto al = Clock::now();
		fsec fs = al - bl;
		mvprintwc(0, 0, "Completed " + std::to_string(gens) + " generation simulation in " + std::to_string(fs.count()) + "s");
		mvprintwc(1, 0, "Saved to " + outname);
		getch();
	}
}
