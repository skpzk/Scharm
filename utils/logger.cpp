#include "logger.h"
#include <iostream>

/*
default format :
(yellow or blue) [date] (color for level) [log level] (white) ([file:method, line]) : msg
*/

using namespace std;

const string CurrentDateTime()
{
  time_t     now = time(NULL);
  tm  tstruct;
  char       buf[80];
  localtime_r(&now, &tstruct);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  return buf;
}

LoggerInterface::LoggerInterface(){
  logger = Logger::getInstance();
}

void LoggerInterface::info(const char* msg){
  logger->info(msg);
}

Logger::Logger(){
  _printDate = true;
  _printFile = true;
  _printFunction = false;
  _printLine = true;

  _outputToFile = false;
  _outputToStd = true;
}

Logger* Logger::getInstance(){
  static Logger logger;
  return &logger;
}

void Logger::test(){
  cout<<"test"<<endl;
}

void Logger::info(std::string msg){
  cout<<CurrentDateTime()<<msg<<endl;
}

void Logger::info(const char* msg){
  info((string) msg);
}

void Logger::outputFormat(){
  cout << "print date     : " << _printDate << endl;
  cout << "print file     : " << _printFile << endl;
  cout << "print function : " << _printFunction << endl;
  cout << "print line     : " << _printLine << endl;
  cout << "output to std  : " << _outputToStd <<endl;
  cout << "output to file : " << _outputToFile <<endl;
}

void Logger::printDate(bool b){
  _printDate = b;
}

void Logger::printFile(bool b){
  _printFile = b;
}

void Logger::useOutputFile(bool b){
  _outputToFile = b;
}

void Logger::operator<<(const char* msg){
  // logger.info(msg);
  cout << "operator << used" << endl;
  cout << msg << endl;
}