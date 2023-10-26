#include "state.h"

#include <stdexcept>

// for string utils
#include "../utils/utils.h"
#include "../utils/fileio.h"

using namespace::std;
namespace fs = std::filesystem;


StateDict State::params;
Connections State::connections;
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

    //this works if the sharm executable is located in SHARM_ROOT/**

    fs::path tmpcwd = fs::current_path();

    cout << "CurrentPath = " << tmpcwd << endl;

    while(tmpcwd.filename() != "Scharm"){
        tmpcwd = tmpcwd.parent_path();
    }

    State::configDir = tmpcwd/"state"; 
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
    connections.print();
}

void State::print(ofstream * out){
    params.print(out);
    connections.print(out);
}

Param* StateDict::operator()(string name_){
    // cout << "operator () called ";
    try{
        Param* p = findParam(&name_);
        return p;
    }catch(const invalid_argument& e){
        if(name_.empty()) return nullParam;
        setParam(name_, 0);
        Param* p = findParam(&name_);
        return p;
    }
}

void StateDict::operator()(string name_, float value){
    // cout << "operator () called ";
    try{
        Param* p = findParam(&name_);
        p->setValue(value);
    }catch(const invalid_argument& e){
        if(name_.empty()) return;
        setParam(name_, value);
        Param* p = findParam(&name_);
    }
}

int StateDict::size(){
    return params.size();
}

Param * StateDict::findParam(string * name_){
    lowerWithoutSpaces(name_);

    if(*name_ == "connections"){
        name_->clear();
        throw invalid_argument("Param name not found");
    }

    for(int i=0; i<params.size(); i++){
        if(params[i].getName() == *name_){
            return &params[i];
        }
    }
    throw invalid_argument("Param name not found");
    return nullptr;
}

void StateDict::setParam(string name_, float value_){
    lowerWithoutSpaces(&name_);

    try{
        Param* p = findParam(&name_);
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
    call(value);
}

Param::Param(string name_){
    lowerWithoutSpaces(&name_);
    
    name = name_;
    value = 0;

    callbacks.clear();
}

void Param::setValue(float value_){
    value = value_;
    call(value);
}

void Param::setValue(vector<float> value_){
    // printf("Set value for oscillo\n");
    value = value_.at(0);
    call(value_);
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

void Param::call(vector<float> v){
    for(int i=0; i<this->callbacksV.size(); i++){
        callbacksV[i].callback(callbacksV[i].p, v);
    }
}

void Param::attachCallback(void* p_, function<void(void*, float)> f){
    CallbackFunction cf;
    cf.callback = f;
    cf.p = p_;
    this->callbacks.push_back(cf);
}
void Param::attachCallback(void* p_, function<void(void*, vector<float>)> f){
    CallbackFunctionVector cf;
    cf.callback = f;
    cf.p = p_;
    this->callbacksV.push_back(cf);
}

Connections::Connections(){
    patchbay = nullptr;
}

void Connections::setPatchbay(Patchbay* pb){
    patchbay = pb;
}

void Connections::connect(string inPp, string outPp){
    // find if inPp exists
    if(outPp.empty())
        return;
    try{
        ConnectedPpOuts* p = findInPp(&inPp);
        for(auto ppOut: p->PpOuts){
            if(ppOut == outPp)
                return;
        }
        p->PpOuts.push_back(outPp);
        // call patchbay
        if(patchbay!=nullptr)
            patchbay->connect(inPp, outPp);
    }catch(const invalid_argument& e){
        if(inPp.empty()) return;
        // create pp
        ConnectedPpOuts newInPp;
        newInPp.PpIn = inPp;
        ppIns.push_back(newInPp);
        // connect
        connect(inPp, outPp);
        
    }
}

void Connections::disconnect(string inPp, string outPp){
    // find if inPp exists
    if(outPp.empty())
        return;
    try{
        ConnectedPpOuts* p = findInPp(&inPp);
        for(int i=0;i<p->PpOuts.size();i++){
            if(p->PpOuts[i] == outPp)
                p->PpOuts.erase(p->PpOuts.begin() + i);
        }
        // call patchbay
        if(patchbay!=nullptr)
            patchbay->disconnect(inPp, outPp);
    }catch(const invalid_argument& e){
        return;
    }
}

ConnectedPpOuts* Connections::findInPp(string * name){
    lowerWithoutSpaces(name);

    // cout << "find outPp connected to " << *name <<endl;

    for(int i=0; i<ppIns.size(); i++){
        if(ppIns[i].PpIn == *name){
            return &ppIns[i];
        }
    }
    throw invalid_argument("patchpoint in name not found");
    return nullptr;
}

void Connections::print(){
    cout << "connections = {";
    if(!ppIns.empty())
        cout << "\n";
    for(auto p: ppIns){
        cout << "  " << p.PpIn << ": [";
        for(int i=0;i<p.PpOuts.size();i++){
            if(i!=0)
                cout << ", ";
            cout << p.PpOuts[i];
        }
        cout << "]\n";
    }
    cout << "}\n";
}

void Connections::print(ofstream * out){
    *out << "connections = {";
    for(auto p: ppIns){
        *out << "  " << p.PpIn << ": [";
        for(int i=0;i<p.PpOuts.size();i++){
            if(i!=0)
                *out << ", ";
            *out << p.PpOuts[i];
        }
        *out << "]";
    }
    *out << "}\n";
}

vector<string> Connections::getConnectionsToPpIn(string inPp){
    //try/catch return empty vector
    try{
        ConnectedPpOuts* p = findInPp(&inPp);
        return p->PpOuts;
    }catch(const invalid_argument& e){
        vector<string> emptyVector;
        return emptyVector;
    }
}