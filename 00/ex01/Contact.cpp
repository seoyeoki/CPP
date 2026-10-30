#include "Contact.hpp"

Contact::Contact(void) {}

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

    oss << std::right << std::setfill(' ');
    oss << std::setw(10) << truncate(_firstName) << "|"
        << std::setw(10) << truncate(_lastName) << "|"
        << std::setw(10) << truncate(_nickname) << "\n";

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

    if (str.length() > 10)
    {
        result = str.substr(0, 9);
        result += '.';
    }
    else
        result = str;

    return result;
}