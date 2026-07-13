#include "demo.hpp"

#include <iostream>

Demo::Demo(const char* msg) : _msg(msg) {}
void Demo::echo() { std::cout << _msg << "\n"; }
