#ifndef PHONEBOOK_HPP
#define PHONEBOOK_HPP

#include <string>
#include <sstream>
#include "Contact.hpp"

class PhoneBook
{
    public:
        PhoneBook(void);
        void addContact(Contact const &contact);
        std::string getSummarizedAllContact(void) const;
        std::string getDetailedIndexedContact(int index) const;

    private:
        Contact _contacts[8];
        int _updateIndex;
};

#endif