#ifndef fileio_h_
#define fileio_h_

#include <iostream>
#include <fstream>
#include <string>  
#include <cstring>

#include <unistd.h> // pwd
#include <algorithm>
#include <filesystem>

#include "../state/state.h"

// using namespace::std;
// namespace fs = std::filesystem;

void getConfigurationFromFile(std::filesystem::path);
void writeConfigurationToFile(std::filesystem::path);

bool isAComment(std::string, bool);
bool isNumeric(std::string);
bool hasEnding (std::string const &fullString, std::string const &ending);

#endif