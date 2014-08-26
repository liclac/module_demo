#include "module.h"
#include <iostream>

int main(int argc, char const *argv[])
{
	for(auto mod_pair : module::registry::get().all)
		std::cout << mod_pair.first << ": " << mod_pair.second.desc << std::endl;
	
	return 0;
}
