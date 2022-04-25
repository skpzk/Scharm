#include "utils.h"

#include <algorithm>

using namespace::std;

string replace(string base, string oldValue, string newValue){
    size_t pos = base.find(oldValue);
    if (pos == string::npos) return base;
    return base.replace(pos, oldValue.length(), newValue);
}
string replaceAll(string base, string oldValue, string newValue){
    string returnvalue = "";
    while(returnvalue.compare(base)){
        returnvalue = base;
        base = replace(base, oldValue, newValue);
    }
    return returnvalue;
}
vector<string> split(string stringToBeSplitted, string delimeter)
{
    vector<string> splittedString;
    int startIndex = 0;
    int endIndex = 0;
    while( (endIndex = stringToBeSplitted.find(delimeter, startIndex)) < stringToBeSplitted.size() )
    {
        string val = stringToBeSplitted.substr(startIndex, endIndex - startIndex);
        splittedString.push_back(val);
        startIndex = endIndex + delimeter.size();
    }
    if(startIndex < stringToBeSplitted.size())
    {
        string val = stringToBeSplitted.substr(startIndex);
        splittedString.push_back(val);
    }
    return splittedString;
}
void lowerWithoutSpaces(string* s){
    transform(s->begin(), s->end(), s->begin(), ::tolower);
    *s = replaceAll(*s, " ", "");
}

void removeBrackets(std::string* s){
    s->erase(remove(s->begin(),s->end(),'['),s->end());
    s->erase(remove(s->begin(),s->end(),']'),s->end());
    s->erase(remove(s->begin(),s->end(),'{'),s->end());
    s->erase(remove(s->begin(),s->end(),'}'),s->end());
}

void removeChar(std::string* s, const char * ch){
    s->erase(remove(s->begin(),s->end(),*ch),s->end());
}