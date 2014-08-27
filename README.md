Module Demo
===========

A simple example application demonstrating a C++ application with self-registering modules, to be queried and instantiated by name using a factory.

It works by abusing the way static variables in C++ work. Using a macro, you create a static variable of a type whose constructor registers the module with a singleton registry class. As static variables are initialized at startup, by the time `main()` gets called, you will already have a complete index of your modules.
