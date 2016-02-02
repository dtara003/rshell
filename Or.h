#ifndef OR_H
#define OR_H
#include <iostream>

using namespace std;

class Or : public Connector {
	public:
		Or(Shell* l, Shell* r) {left = l; right = r;};
		void execute() {

			//assuming execute returns the command name
			cout << "Executed: " << left->execute() << " or " << "Executed: " << right->execute();
		}
};
#endif
