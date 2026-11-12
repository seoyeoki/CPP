#include "Zombie.hpp"
#include <iostream>

Zombie *zombieHorde(int N, std::string name);

int main(void)
{
	int const N = 5;

	std::cout << "========== ZOMBIE HORDE ==========" << std::endl;
	Zombie *horde = zombieHorde(N, "seoykim");

	if (horde)
	{
		for (int i = 0; i < N; i++)
			horde[i].announce();
		delete[] horde;
	}
	std::cout << "===================================" << std::endl;

	std::cout << "====== INVALID SIZES (no zombies) ======" << std::endl;
	std::cout << "N = 0  : " << (zombieHorde(0, "none") == NULL ? "NULL" : "not NULL") << std::endl;
	std::cout << "N = -1 : " << (zombieHorde(-1, "none") == NULL ? "NULL" : "not NULL") << std::endl;
	std::cout << "========================================" << std::endl;

	return 0;
}
