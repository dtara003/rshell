#ifndef AND_H
#define AND_H

#include "Connector.h"

using namespace std;

class And : public Connector {
	public:
		And() {}
		
		//a constructor that can have a null left pointer
		And(Shell* l, Shell* r) {left = l; right = r;}

		//set the left pointer to whatever the argument is pointing at
		void setLeft(Shell* s) {
			left = s;
		}

		//execute function that will perform as &&
		bool execute() {
			if (left->execute() == true) {
				right->execute();
				return true;
			}
			else
				return false;
		}

};

#endif
