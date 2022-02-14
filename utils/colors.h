#ifndef UTILS_COLORS_H
#define UTILS_COLORS_H
#include <string>

class Colors{
  public:
    Colors(){};
    std::string reset = "\033[0m";
    std::string black = "\033[30m";
    std::string red = "\033[31m";
    std::string green = "\033[32m";
    std::string yellow = "\033[33m";
    std::string blue = "\033[34m";
    std::string magenta = "\033[35m";
    std::string cyan = "\033[36m";
    std::string white = "\033[37m";
};

static const Colors colors = Colors();

#endif /* UTILS_COLORS_H */
