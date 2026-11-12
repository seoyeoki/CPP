#include "Contact.hpp"

// std::setw and std::string::length() count bytes, but a UTF-8 character
// (Korean, emoji, ...) takes 2 to 4 bytes. These helpers count characters instead.
static std::string::size_type countChars(const std::string &str)
{
    std::string::size_type count = 0;

    for (std::string::size_type i = 0; i < str.length(); i++)
    {
        if ((static_cast<unsigned char>(str[i]) & 0xC0) != 0x80)   // not a continuation byte
            count++;
    }
    return count;
}

// byte position where the n-th (0-based) character starts
static std::string::size_type byteOffsetOfChar(const std::string &str, std::string::size_type n)
{
    std::string::size_type count = 0;

    for (std::string::size_type i = 0; i < str.length(); i++)
    {
        if ((static_cast<unsigned char>(str[i]) & 0xC0) != 0x80)
        {
            if (count == n)
                return i;
            count++;
        }
    }
    return str.length();
}

Contact::Contact(void) : _firstName(""), _lastName(""), _nickname(""), _phoneNumber(""), _darkestSecret("")
{}

void Contact::setContact(
    const std::string &firstName,
    const std::string &lastName,
    const std::string &nickname,
    const std::string &phoneNumber,
    const std::string &darkestSecret)
{
    _firstName = firstName;
    _lastName = lastName;
    _nickname = nickname;
    _phoneNumber = phoneNumber;
    _darkestSecret = darkestSecret;
}

std::string Contact::getSummarizedContact(void) const
{
    std::ostringstream oss;

    const std::string cells[3] = { truncate(_firstName), truncate(_lastName), truncate(_nickname) };

    oss << std::right << std::setfill(' ');
    for (int i = 0; i < 3; i++)
    {
        if (i > 0)
            oss << "|";
        // setw counts bytes: widen it by the extra bytes of multi-byte characters
        oss << std::setw(static_cast<int>(10 + cells[i].length() - countChars(cells[i]))) << cells[i];
    }
    oss << "\n";

    return oss.str();
}

std::string Contact::getDetailedContact(void) const
{
    std::ostringstream oss;

    oss << "First name: " << _firstName << "\n"
        << "Last name: " << _lastName << "\n"
        << "Nickname: " << _nickname << "\n"
        << "Phone number: " << _phoneNumber << "\n"
        << "Darkest secret: " << _darkestSecret << "\n";

    return oss.str();
}

std::string Contact::truncate(std::string str) const
{
    std::string result;

    if (countChars(str) > 10)
    {
        result = str.substr(0, byteOffsetOfChar(str, 9));
        result += '.';
    }
    else
        result = str;

    return result;
}