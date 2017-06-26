#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>

extern std::vector<int> time_played;
extern std::vector<int> real_time_played;

class Game {
	public:
		Game(int state) : state(state) {}
		void saveGame();
		void loadGame();
		void game();
		auto State() { return state; }
		void State(int s) { state = s; }

	private:
		int state;
};

#endif // GAME_H
