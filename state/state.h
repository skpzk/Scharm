#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include <fstream>
#include <iostream>

#include <filesystem>
namespace fs = std::filesystem;


using namespace::std;

class StateDict;
class Param;

void writeConfigurationToFile(fs::path savePath);

class State{
    public:
        // static vector<string> params;
        static StateDict params;
        // State();
        static void read();
        static void save();
        // static const int test=0;
        // static int test2;
        static void print();
        static void print(ofstream * out);

        // void saveConf();

        static fs::path configDir;
        static fs::path autosavePath;

        static void createAutosavepath();

        // void exportState(ofstream* f);
};

class StateDict{
    public:
        StateDict();
        // StateDict(string);
        void print();
        void print(ofstream * out);
        // void operator=();
        // operator float() const;

        Param *operator()(string);
        // void operator()(string, float);
        
        
        
        // Param *testReturnParam(string name_);

        // friend class Param;
        int size();

    private:
        vector<Param> params;
        void setParam(string, float);
        Param* findParam(string);

};

struct CallbackFunction
{
    void* p;
    function<void(void*, float)> callback;
};


class Param{
    public:
        Param(string, float);
        Param(string);
        void setValue(float);
        float getValue();

        string getName();

        operator float() const;
        void setCallback(void*, function<void(void*, float)>);
        
        // friend class StateDict;

        void call(float);
        
    private:
        string name;
        float value;
        
        vector<CallbackFunction> callbacks;

        

};

#endif // STATE_H