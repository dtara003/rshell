#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include "Shell.h"

using namespace std;

class Command : public Shell {
   private:
       string str;

   public:
       Command(){};
       Command(string s) : str(s) {};

       void execute() {
       	   // execute this command
       	   // check

       	   // cout << "Executed!" << endl;

           return str;
       };
};

#endif
