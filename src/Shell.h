#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

//used for the execute function returns: EXECUTED = command
//executed properly, FAILED = command did not executed
//EXIT = EXIT to the prompt, all allocated memory and call
//exit(0)
enum status {EXECUTED, FAILED, EXIT};

using namespace std;

class Shell {
	public:
		// constructor
        // literally only needed for the pointer that points to root
        // at all times
        Shell() {};

		//a virtual destructor
		virtual ~Shell() {};

		//execute function that returns a bool based on conditions(i.e.
		//calling execute on an And composite that contains commands
		//that executed as executed  && failed to execute
		//will return a false)
		virtual status execute() = 0;

        // www.cprogramming.com/tutorial/lesson18.html
        // reference link for function that deconstructs tree alongside
        // the destructor
        virtual void freeMem() = 0;
};

#endif
