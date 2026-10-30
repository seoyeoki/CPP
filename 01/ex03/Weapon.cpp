#include "Weapon.hpp"

Weapon::Weapon() : _type("")
{}

Weapon::Weapon(const std::string &type) : _type(type)
{}

Weapon::~Weapon(void)
{}

void Weapon::setType(const std::string &type)
{
    _type = type;
}

const std::string &Weapon::getType() const
{
    return _type;
}