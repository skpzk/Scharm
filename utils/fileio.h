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

using namespace::std;
namespace fs = std::filesystem;

void getConfigurationFromFile(fs::path);
void writeConfigurationToFile(fs::path);

bool isAComment(string, bool);
bool isNumeric(string);
bool hasEnding (std::string const &fullString, std::string const &ending);

#endif