#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Shell.h"
#include <iostream>

using namespace std;

//Composite class for Connectors(&& and ||)
class Connector : public Shell {
    public:
        Shell* left;
        Shell* right;

        // constructor
        Connector() {};

        bool execute() {
            return true;
        };
};

class Semi : public Connector {
    public:
        // constructor
        Semi() {};
        Semi(Shell* l, Shell* r) : left(l), right(r) {};

        bool execute() {
            //
        };
};

class And : public Connector {
	public:
		And() {};
		
		//a constructor that can have a null left pointer
		And(Shell* l, Shell* r) : left(l), right(r) {};

		//set the left pointer to whatever the argument is pointing at
		//void setLeft(Shell* s) {
			//left = s;
		//};

		//execute function that will perform as &&
		bool execute() {
			if (left->execute() == true) {
				right->execute();
				return true;
			}
			else {
				return false;
            }
		};
};

class Or : public Connector {
	public:
        // constructors
        Or() {};
		Or(Shell* l, Shell* r) : left(l), right(r) {};

		void execute() {
			//assuming execute returns the command name for testing
			cout << "Executed: " << left->execute() << " or " << "Executed: "
                 << right->execute();
		};
};

#endif
