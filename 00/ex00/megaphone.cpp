#include <iostream>
#include <cctype>
#include <string>

std::string toUpper(const std::string &s)
{
	std::string result;

	for (size_t i = 0; i < s.length(); i++)
            result += std::toupper(static_cast<unsigned char>(s[i]));
	return result;
}

std::string toUpperStr(char *argv[])   // argv 전체(포인터 배열)를 통째로 받음
{
	std::string result;

    for (size_t i = 0; argv[i]; i++)
            result += toUpper(argv[i]);
	return result;
}

int main(int argc, char *argv[])
{
	if (argc == 1)
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
	else
		std::cout << toUpperStr(argv + 1) << std::endl;   // argv[0](프로그램명) 건너뛰고 넘김
	return 0;
}
