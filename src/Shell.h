#ifndef SHELL_H
#define SHELL_H
#include <iostream>

using namespace std;

class Shell {
	public:
		Shell() {};
		virtual void execute() = 0;
};
#endif

