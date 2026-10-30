#include "Zombie.hpp"
#include <iostream>

Zombie::Zombie() : _name("")
{
}

Zombie::Zombie(const std::string &name) : _name(name)
{
}

Zombie::~Zombie(void)
{
	std::cout << _name << " has been destroyed. Zzz..." << std::endl;
}

void Zombie::setName(const std::string &name)
{
	_name = name;
}

void Zombie::announce(void) const
{
	std::cout << _name << ": BraiiiiiinnnzzzZ..." << std::endl;
}
