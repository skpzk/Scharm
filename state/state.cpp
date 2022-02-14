#include "state.h"

#include <stdexcept>

// for string utils
#include "../utils/utils.h"
#include "../utils/fileio.h"


StateDict State::params;
fs::path State::configDir;
fs::path State::autosavePath;

void State::read(){
    cout << "reading state\n";

    createAutosavepath();

    getConfigurationFromFile(State::autosavePath);

    cout << "loaded\n" ;

    // cout << "State is loaded, state = \n";
    // State::print();
    // cout << "loading done\n";

    
}

void State::createAutosavepath(){

    //this works if the sharm executable is located in SHARM_ROOT/build

    fs::path tmpcwd = fs::current_path();

    cout << "CurrentPath = " << tmpcwd << endl;

    State::configDir = tmpcwd.parent_path()/"state"; 
    State::autosavePath = State::configDir/"state.sharm";
}

void State::save(){
    cout << "saving state\n";
    
    createAutosavepath();

    writeConfigurationToFile(State::autosavePath);
}

StateDict::StateDict(){
    nullParam = new Param("", 0);
}

void State::print(){
    params.print();
}

void State::print(ofstream * out){
    params.print(out);
}

Param* StateDict::operator()(string name_){
    // cout << "operator () called ";
    try{
        Param* p = findParam(name_);
        return p;
    }catch(const invalid_argument& e){
        if(name_.empty()) return nullParam;
        setParam(name_, 0);
        Param* p = findParam(name_);
        return p;
    }
}

void StateDict::operator()(string name_, float value){
    // cout << "operator () called ";
    try{
        Param* p = findParam(name_);
        p->setValue(value);
    }catch(const invalid_argument& e){
        if(name_.empty()) return;
        setParam(name_, value);
        Param* p = findParam(name_);
    }
}

int StateDict::size(){
    return params.size();
}

Param * StateDict::findParam(string name_){
    lowerWithoutSpaces(&name_);

    for(int i=0; i<params.size(); i++){
        if(params[i].getName() == name_){
            return &params[i];
        }
    }
    throw invalid_argument("Param name not found");
    return nullptr;
}

void StateDict::setParam(string name_, float value_){
    lowerWithoutSpaces(&name_);

    try{
        Param* p = findParam(name_);
        p->setValue(value_);
    }catch(const invalid_argument& e){
        params.push_back(Param(name_, value_));
    }
}

void StateDict::print(){
    // cout << "printing...\n";
    for(int i=0; i<params.size(); i++){
        cout << "params[" << params[i].getName() << "] = " << params[i].getValue() << endl;
    }
    // cout << "printed\n";
}

void StateDict::print(ofstream * out){
    // cout << "printing...\n";
    for(int i=0; i<params.size(); i++){
        *out << params[i].getName() << " = " << params[i].getValue() << endl;
    }
    // cout << "printed\n";
}

Param::Param(string name_, float value_){
    lowerWithoutSpaces(&name_);
    
    name = name_;
    value = value_;
}

Param::Param(string name_){
    lowerWithoutSpaces(&name_);
    
    name = name_;
    value = 0;
}

void Param::setValue(float value_){
    value = value_;
}

float Param::getValue(){
    return value;
}

string Param::getName(){
    return name;
}

Param::operator float() const {
    return value;
}

void Param::call(float v){
    for(int i=0; i<this->callbacks.size(); i++){
        callbacks[i].callback(callbacks[i].p, v);
    }
}

void Param::attachCallback(void* p_, function<void(void*, float)> f){
    CallbackFunction cf;
    cf.callback = f;
    cf.p = p_;
    this->callbacks.push_back(cf);
}