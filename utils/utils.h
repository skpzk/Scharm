#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// using namespace::std;

std::string replace(std::string base, std::string oldValue, std::string newValue);
std::string replaceAll(std::string base, std::string oldValue, std::string newValue);
std::vector<std::string> split(std::string stringToBeSplitted, std::string delimeter);
void lowerWithoutSpaces(std::string*);
void removeBrackets(std::string*);
void removeChar(std::string*, const char*);

#endif // UTILS_H