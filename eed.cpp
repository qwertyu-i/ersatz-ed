#include <iostream>
// maybe making our own fstream could be a
// fun project 
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <functional>

using namespace std;

// pseudocode
// void execcommand(command table, argc, char *argv[]) {
// if command exists
// run with args
// else
// cout "?"
// }
// how would one interpret a line number? - if argv[n].type == int {special case, else execcommand}
// flip that to be if not int
void execcmd(unordered_map<string, function<void()>> cmds, int argc, vector<string> argv) {
    cmds[argv[0]]();
}

int main(int argc, char *argv[]) {
    vector<string> docbuf;
    // last arg is file 
    // also wtf is this way of creating objects
    fstream File(argv[argc-1]);

    if (!File.is_open())
	cerr << "no";

    unordered_map<string, function<void()>> cmds = {
	{"a", [&File](){File << "Ran a";}}
    };
    
    int i = 0;
    string line;
    while (getline(File, line)) {
	docbuf.push_back(line);
	i++;
    }

    File.clear();
    File.seekg(0, ios::beg);

    vector<string> test = { "a" };
    execcmd(cmds, 1, test);

    File.close();

    return 0;
}
