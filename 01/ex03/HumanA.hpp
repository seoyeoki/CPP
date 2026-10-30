#ifndef HUMAN_A_HPP
#define HUMAN_A_HPP

#include <iostream>
#include <string>
#include "Weapon.hpp"

class HumanA
{
public:
    HumanA(const std::string &name, Weapon &weapon);
    ~HumanA(void);

    void attack();

private:
    std::string _name;
    Weapon &_weapon;

    const std::string &getName() const;
};

#endif