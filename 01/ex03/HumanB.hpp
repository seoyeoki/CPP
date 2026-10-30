#ifndef HUMAN_B_HPP
#define HUMAN_B_HPP

#include <iostream>
#include <string>
#include "Weapon.hpp"

class HumanB
{
public:
    HumanB(const std::string &name);
    ~HumanB(void);

    void attack();
    void setWeapon(Weapon &weapon);

private:
    std::string _name;
    Weapon *_weapon;

    const std::string &getName() const;
};

#endif