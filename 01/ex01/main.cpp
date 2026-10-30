#include "Zombie.hpp"
#include <iostream>

Zombie *zombieHorde(int N, std::string name);

int main(void)
{
	int const N = 5;

	std::cout << "========== ZOMBIE HORDE ==========" << std::endl;
	Zombie *horde = zombieHorde(N, "seoykim");

	for (int i = 0; i < N; i++)
		horde[i].announce();

	delete[] horde;
	std::cout << "===================================" << std::endl;

	return 0;
}
