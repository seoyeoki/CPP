#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>

class Weapon
{
public:
    Weapon();
    Weapon(const std::string &type);
    ~Weapon(void);

    void setType(const std::string &type);
    const std::string &getType() const;

private:
    std::string _type;
};

#endif