#include "my_module.h"
#include <iostream>

int my_module::run(std::vector<std::string> params)
{
	std::cout << "This is printed from my_module" << std::endl;
	return 0;
}
