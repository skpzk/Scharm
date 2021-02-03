#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

using namespace::std;

string replace(string base, string oldValue, string newValue);
string replaceAll(string base, string oldValue, string newValue);
vector<string> split(string stringToBeSplitted, string delimeter);
void lowerWithoutSpaces(string*);

#endif // UTILS_H