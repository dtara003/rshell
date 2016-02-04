#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <unistd.h>

using namespace std;

class Shell {
	public:
		Shell() {};
		
        void prompt() {
            string user = getlogin();
            char hostname[100];
            gethostname(hostname, 100);
    
            cout << "[" << user << "@" << hostname << "]$ ";


        };   

        //virtual void execute() = 0;
};

#endif

