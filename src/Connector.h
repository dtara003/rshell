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
		~Connector() {
			delete left;
			delete right;
		};
        status execute() {
            return EXECUTED;
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
        ~Semi() {};

        status execute() {
            // semicolon works so that regardless of the left
            // side of the connector, the right side will always
            // pass through the execute function

			//execute the left child and determine its status
            status check = left->execute();

            if (check == EXECUTED) {
                return right->execute();
            }
			else if (check == EXIT) {
				return EXIT;
			}
			else
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
		And(Shell* l, Shell* r) { 
            left = l;
            right = r;
        };

        ~And() {}

		//execute function that will perform as &&
		status execute() {
			
			//check the status of the left->execute()
			status check = left->execute();

		    if (check == EXECUTED) {
                return right->execute();
            }
			else if (check == EXIT) {
				return EXIT;
			}
			else
				return FAILED;
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

        ~Or() {};

		status execute() {
            
            //check the what left->execute() returns
            status check = left->execute();

            if (check == EXECUTED) {
                return EXECUTED;
            }
			else if (check == EXIT) {
				return EXIT;
			}
			else
				return right->execute();
        };
};

#endif
