#include <iostream>
#include <cstdlib>
#include "PhoneBook.hpp"
#include "Contact.hpp"

static std::string promptField(const std::string &label)
{
	std::string value;

	while (true)
	{
		std::cout << label << ": ";
		std::getline(std::cin, value);
		if (!value.empty())
			return value;
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
			std::string firstName     = promptField("First name");
			std::string lastName      = promptField("Last name");
			std::string nickname      = promptField("Nickname");
			std::string phoneNumber   = promptField("Phone number");
			std::string darkestSecret = promptField("Darkest secret");

			contact.setContact(firstName, lastName, nickname, phoneNumber, darkestSecret);
			phoneBook.addContact(contact);
		}
		else if (command == "SEARCH")
		{
			std::cout << phoneBook.getSummarizedAllContact();

			std::cout << "Enter index to view details: ";
			std::string indexStr;
			std::getline(std::cin, indexStr);

			int index = std::atoi(indexStr.c_str());
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
