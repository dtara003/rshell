#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Shell.h"
#include "Prompt.h"

#include <iostream>

using namespace std;

//Composite class for Connectors(&& and || and ;)
//following same format as Lab3
class Connector : public Shell {
    protected:
        Shell* left;
        Shell* right;
    
    public:
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
        Semi(Shell* l, Shell* r) {
            left = l;
            right = r;
        };

        bool execute() {
            // semicolon works so that regardless of the left
            // side of the connector, the right side will always
            // pass through the execute function
            
            // the returned value of the right child command will
            // set the returned value of the semicolon itself
            // for a parent node it it itself is not the root

            //FIXME: account for 'exit' command

            if (left->execute()) {
                return right->execute();
            }

            return right->execute();
        };
};

class And : public Connector {
	public:
		And() {};
		
		// can't use initializaion lists when setting pointers for
        // some reason -- a limitation of inheritance?
        // probably because left and right aren't directly members of
        // the classes that inherit from the Connector class
		And(Shell* l, Shell* r) { //: left(l), right(r) {};
            left = l;
            right = r;
        };

		//set the left pointer to whatever the argument is pointing at
		//void setLeft(Shell* s) {
			//left = s;
		//};

		//execute function that will perform as &&
		bool execute() {
		    if (left->execute()) {
                return right->execute();
            }

            return false;
		};
};

class Or : public Connector {
	public:
        // constructors
        Or() {};
		Or(Shell* l, Shell* r) {
            left = l;
            right = r;
        };

		bool execute() {
			/*assuming execute returns the command name for testing
			cout << "Executed: " << left->execute() << " or " << "Executed: "
                 << right->execute();*/
            
            if (left->execute()) {
                return true;
            }

            return right->execute();
        };
};

#endif
