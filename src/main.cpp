#include "Prompt.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>

//comment

using namespace std;

int main() {
    while(1) {
        Prompt p;
        p.promptUser();
    }

    return 0;
}

