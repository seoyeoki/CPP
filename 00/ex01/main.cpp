#include <iostream>
#include "PhoneBook.hpp"
#include "Contact.hpp"

static bool promptField(const std::string &label, std::string &value)
{
	while (true)
	{
		std::cout << label << ": ";
		if (!std::getline(std::cin, value))
			return false;   // input closed (EOF): give up instead of looping forever
		if (value.find_first_not_of(" \t\n\v\f\r") != std::string::npos)
			return true;
		std::cout << "This field can't be empty. Try again." << std::endl;
	}
}

int main(void)
{
	PhoneBook phoneBook;
	std::string command;

	while (true)
	{
		std::cout << "Enter a command (ADD, SEARCH, EXIT): ";
		if (!std::getline(std::cin, command))
			break;   // 입력 끊기면(EOF) 종료

		if (command == "ADD")
		{
			Contact contact;
			std::string firstName;
			std::string lastName;
			std::string nickname;
			std::string phoneNumber;
			std::string darkestSecret;

			if (!(promptField("First name", firstName)
				&& promptField("Last name", lastName)
				&& promptField("Nickname", nickname)
				&& promptField("Phone number", phoneNumber)
				&& promptField("Darkest secret", darkestSecret)))
				break;   // EOF while adding: the half-filled contact is dropped, then exit

			contact.setContact(firstName, lastName, nickname, phoneNumber, darkestSecret);
			phoneBook.addContact(contact);
		}
		else if (command == "SEARCH")
		{
			std::cout << phoneBook.getSummarizedAllContact();

			std::cout << "Enter index to view details: ";
			std::string indexStr;
			std::getline(std::cin, indexStr);

			int index = -1;
			if (indexStr.length() == 1 && indexStr[0] >= '0' && indexStr[0] <= '7')
				index = indexStr[0] - '0';
			std::cout << phoneBook.getDetailedIndexedContact(index);
		}
		else if (command == "EXIT")
		{
			std::cout << "Good Bye!" << std::endl;
			return 0;
		}
		else
		{
			// Any other input is ignored — no message, no state change.
		}
	}
	return 0;
}
