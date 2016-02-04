#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

class Shell {
    //private:
        //string input; 
    public:
		Shell() {};
		
        void prompt() {
            string user = getlogin();
            char hostname[100];
            gethostname(hostname, 100);
    
            cout << "[" << user << "@" << hostname << "]$ ";
            
            char input[1000];
            
            cin.getline(input, 1000, '\n');

            if (strcmp(input, "exit") == 0) {
                exit(0);
            }

            for (int i = 0; i < strlen(input); ++i) {
                if (input[i] == '#') {
                    input[i] = '\0';
                    i = strlen(input);
                }
            }

            for (int i = 0; i < strlen(input); ++i) {
                cout << input[i];
            }

            cout << endl;
        };   

        //virtual void execute() = 0;
};

#endif

