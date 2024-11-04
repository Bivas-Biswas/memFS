#include <iostream>
#include <string>
#include <vector>
#include "./lib/utils/split.h"
#include "./lib/utils/printLine.h"
using namespace std;

int main(){

    std::string in ="write -n 2 todo1.txt \"Wake up at 7 AM\" todo2.txt \"Have Breakfast\"";
    
    SplitVect inputs = split(in, ' ');

    for(auto &a: inputs){
        printLine(a);
    }

    return 1;
}