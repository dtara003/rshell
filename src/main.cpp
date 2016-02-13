#include "Prompt.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>

//comment

using namespace std;

int main() {
    //infinite loop until exit command called
    while(1) {
        Prompt pr;

        pr.promptUser();
        pr.parse();
        pr.run();
    }

    return 0;
}
