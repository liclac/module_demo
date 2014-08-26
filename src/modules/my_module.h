#ifndef MY_MODULE_H
#define MY_MODULE_H

#include "../module.h"

class my_module : public module
{
public:
	int run(std::vector<std::string> params);
};

REGISTER_MODULE(my_module, "mymod", "My self-registering module");

#endif
