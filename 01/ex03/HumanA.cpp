#include "HumanA.hpp"

HumanA::HumanA(const std::string &name, Weapon &weapon) : _name(name), _weapon(weapon)
{}

HumanA::~HumanA(void)
{}

void HumanA::attack()
{
    std::cout << getName() << " attacks with their " << _weapon.getType() << std::endl;
}

const std::string &HumanA::getName() const
{
    return _name;
}