#include "HumanB.hpp"

HumanB::HumanB(const std::string &name) : _name(name), _weapon(NULL)
{}

HumanB::~HumanB(void)
{}

void HumanB::attack()
{
    if (_weapon)
        std::cout << getName() << " attacks with their " << _weapon->getType() << std::endl;
    else
        std::cout << getName() << " attacks with their hands" << std::endl;
}

void HumanB::setWeapon(Weapon &weapon)
{
    _weapon = &weapon;
}

const std::string &HumanB::getName() const
{
    return _name;
}