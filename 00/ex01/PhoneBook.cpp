#include "PhoneBook.hpp"

PhoneBook::PhoneBook(void)
{
    _updateIndex = 0;
}

void PhoneBook::addContact(Contact const &contact)
{
    _contacts[_updateIndex] = contact;
    _updateIndex = (_updateIndex + 1) % 8;
}

std::string PhoneBook::getSummarizedAllContact(void) const
{
    std::ostringstream oss;

    for (int i = 0; i < 8; i++)
    {
        oss << std::setw(10) << i << '|' << _contacts[i].getSummarizedContact();
    }

    return oss.str();
}

std::string PhoneBook::getDetailedIndexedContact(int index) const
{
    if (index < 0 || index > 7)
        return ("Out of proper index range, please input index between 0 and 7\n");

    return _contacts[index].getDetailedContact();
}