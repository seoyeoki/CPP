#include "Zombie.hpp"
#include <iostream>

Zombie* newZombie(std::string name);
void randomChump(std::string name);

int main(void)
{
    std::cout << "========== A ZOMBIE ANNOUNCES ==========" << std::endl;
    randomChump("seoykim");
    randomChump("seoykim");
    randomChump("seoykim");
    std::cout << "========================================" << std::endl;

    std::cout << "========= THE ZOMBIE ANNOUNCES =========" << std::endl;
    Zombie *zombie = newZombie("seoykim");
    zombie->announce();
    zombie->announce();
    zombie->announce();
    delete zombie;
    std::cout << "========================================" << std::endl;

    return 0;
}