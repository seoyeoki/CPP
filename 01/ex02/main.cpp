#include <string>
#include <iostream>

int main(void)
{
    std::string string = "HI THIS IS BRAIN";

    std::string *stringPTR = &string;
    std::string &stringREF = string;

    std::cout << "========== MEMORY ADDRESS ==========" << std::endl;
    std::cout << "string: " << &string << std::endl;
    std::cout << "string pointer: " << stringPTR << std::endl;
    std::cout << "string reference: " << &stringREF << std::endl;
    std::cout << "====================================" << std::endl;

    std::cout << "=============== VALUE ===============" << std::endl;
    std::cout << "string: " << string << std::endl;
    std::cout << "string pointer: " << *stringPTR << std::endl;
    std::cout << "string reference: " << stringREF << std::endl;
    std::cout << "=====================================" << std::endl;
}