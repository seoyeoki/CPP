#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <string>
#include <sstream>
#include <iomanip>

class Contact
{
    public:
        Contact(void);
        void setContact(
                const std::string &firstName,
                const std::string &lastName,
                const std::string &nickname,
                const std::string &phoneNumber,
                const std::string &darkestSecret
        );
        std::string getSummarizedContact(void) const;
        std::string getDetailedContact(void) const;

    private:
        std::string _firstName;
        std::string _lastName;
        std::string _nickname;
        std::string _phoneNumber;
        std::string _darkestSecret;

        std::string truncate(std::string str) const;
};

#endif
