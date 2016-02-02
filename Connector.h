#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <iostream>
#include "Shell.h"

using namespace std;

class Connector : public Shell {
    public:
        Shell* left;
        Shell* right;

    private:
        Connector() {};
        Connector(Shell* l, Shell* r) : left(l), right(r) {};

        void execute() {
            // execute something
            return 0;
        };
};

#endif
