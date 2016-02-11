#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "Shell.h"

using namespace std;

//Composite class for Connectors(&& and ||)
class Connector : public Shell {
    protected:
        Shell* left;
        Shell* right;

    public:
        Connector() {};
        bool execute() {return true;}
};

#endif
