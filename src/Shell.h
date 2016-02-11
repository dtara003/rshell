#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

using namespace std;

class Shell {
	public:
		Shell() {};
		//a function used to set the left pointer of a connector
		//that has two commands(will only be implemented by Connector
		//composite classes)
		//virtual void setLeft(Shell* s) {return;}
		
		//a function that will set the right pointer of a Connector Class
		//virtual void setRight() {return;}
		//execute function that returns a bool based on conditions(i.e.
		//calling execute on an And composite that contains commands
		//that executed as executed  && failed to execute
		//will return a false)
		virtual bool execute() = 0; 
};

#endif
