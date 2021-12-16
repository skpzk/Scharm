#ifndef UTILS_LOGGER_H
#define UTILS_LOGGER_H

#include <filesystem>
#include <string>
#include <time.h>

// define colors for terminal

const std::string CurrentDateTime();

class Logger{
  public:

    static Logger* getInstance();

    void operator<<(const char*);

    static void test();

    void info(const char* msg);
    void warning(char* msg);
    void debug(char* msg);
    void error(char* msg);

    void info(std::string);
    void warning(std::string);
    void debug(std::string);
    void error(std::string);
    
    void log(int level, char*msg);
    void log(int level, std::string);
    
    void setFormat(bool printDate, bool printFile, bool printLine, bool printFunction);

    void printDate(bool);
    void printFile(bool);
    void printLine(bool);
    void printFunction(bool); 

    void setOutputFile(std::filesystem::path);
    void useOutputFile(bool);
    void outputToStd(bool);

    // debug functions
    void outputFormat();

  private:
    // singleton class
    Logger();
    
    // copy constructor (left undefined)
    Logger(const Logger&);

    // copy-assignment constructor (left undefined)
    Logger& operator=(const Logger&);

    std::filesystem::path path;
    bool _outputToFile;
    bool _outputToStd;
    bool _printDate, _printFile, _printLine, _printFunction;

};

class LoggerInterface{
  public:
    LoggerInterface();
    void setFormat();

    void info(const char*);
  private:
    Logger* logger;
};

#endif /* UTILS_LOGGER_H */
