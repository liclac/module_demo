#include "module.h"
#include <iostream>

int main(int argc, char const *argv[])
{
	int retval = 0;
	
	// Check to make sure we have a module argument, at least
	if(argc < 2 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-?") == 0)
	{
		// If not, print usage information
		std::cerr << "Usage: " << argv[0] << " module [args...]" << std::endl;
		std::cerr << std::endl;
		
		// And list the available modules!
		std::cerr << "Available Modules:" << std::endl;
		for(auto mod_pair : module::registry::get().all)
			std::cerr << "  - " << mod_pair.first << ": " << mod_pair.second.desc << std::endl;
		
		return 0;
	}
	
	// Extract the module name and params
	std::string modname(argv[1]);
	std::vector<std::string> params(argv + 2, argv + argc);
	
	// Look up a module named that
	auto& modules = module::registry::get().all;
	auto it = modules.find(modname);
	if(it != modules.end())
	{
		module *mod = it->second.createfn();
		retval = mod->run(params);
		delete mod;
	}
	else
	{
		std::cerr << "ERROR: No such module: " << modname << std::endl;
		retval = 1;
	}
	
	return retval;
}
