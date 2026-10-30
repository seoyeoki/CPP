#include "Harl.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: " << argv[0] << " <level>" << std::endl;
		return 1;
	}

	Harl harl;

	harl.complain(argv[1]);
	return 0;
}
