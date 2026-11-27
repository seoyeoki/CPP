#ifndef FIXED_HPP
#define FIXED_HPP

#include <iostream>

class Fixed
{
public:
    Fixed();
    Fixed(const int &input);
    Fixed(const float &input);
    Fixed(const Fixed &other);
    Fixed &operator=(const Fixed &other);
    ~Fixed();

    int getRawBits( void ) const;
    void setRawBits( int const raw );

    float toFloat( void ) const;
    int toInt( void ) const;

private:
    int _value;
    static const int _fractionalBits = 8;

};

std::ostream &operator<<(std::ostream &out, const Fixed &fixed);

#endif