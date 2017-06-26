#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <vector>
#include <cstdlib>

enum Upgrade { research1, research2, research3, research4, research5, research6 };

class Character {
	private:
		std::string name;
		unsigned int limit; // both time perception as well as death counter
		unsigned int age;

		bool hasheir = false;
		bool sex;
		std::vector<Upgrade> upgrades;
		std::vector<int> lifespan;
		std::vector<Character> ancestry;
	public:
		Character(std::string name, int limit, int age, bool sex, std::vector<Upgrade> upgrades, std::vector<int> lifespan, std::vector<Character> ancestry): name(name), limit(limit),
		age(age), sex(sex), upgrades(upgrades), lifespan(lifespan), ancestry(ancestry) {}

		auto getLimit() { return limit; }
		auto getAge() { return age; }
		auto getLifespan() { return lifespan; }
		auto getAncestry() { return ancestry; }

		void setLimit(unsigned int l) { limit = l; }
		void setAge(unsigned int a) { age = a; }
		void setHeir(bool h) { hasheir = h; }
		void setSex(bool s) { sex = s; }
};

#endif // CHARACTER_H
