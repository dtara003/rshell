#include "Command.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>

//comment

using namespace std;

int main() {
	//char* c[3];
	
	//string ls = "ls";
	//string a = "-a";
	
	//c[0] = (char *) ls.c_str();
	//c[1] = (char *) a.c_str();
	//c[2] = NULL;
	string s = "echo hello";
	Command e(s);
	
	if (e.execute() == false)
		cout << "Did not work" << endl;

    return 0;
}
