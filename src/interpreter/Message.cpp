#include "Message.h"
#include <iostream>
void ErrorMsg(string msg) {
    cerr << "Error";
    cerr << ": ";
    cerr << msg << endl;
    exit(1);
}

void WarningMsg(string msg) {
    cerr << "Warning";
    cerr << ": ";
    cerr << msg << endl;
}
