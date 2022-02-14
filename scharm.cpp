#include "scharm.h"

#include "state/state.h"

#include "audioLib/patch.h"

// #include "audioLib/utils/quantize.h"

using namespace::std;

void nonMemberCallback(void* voidPtr, float value){
    cout << "this is a callback to a non-member function, value = " << value << endl;
}

void setGuiThing(float value_){
    cout << "setting gui thing, value = " << value_ << endl;
}

void setGuiThingVoid(){
    cout << "setting gui thing\n";
}

Scharm::Scharm(){
    cout << "creating Scharm class \n";


    State::read();

/*
    State::print();

    State::params("Test")->setValue(3);
    float test = *State::params("test");
    float test2 = *State::params("Other test");
    float test3 = State::params("Other test")->getValue();

    // this doesn't work :
    // State::params("test", 42);
    
    State::params("Other test")->setValue(10);

    cout << "State test, direct access = " << *State::params("test") << endl;
    cout << "State test, store value in a float = " << test << endl;
    cout << "State test2, store value in a float = " << test2 << endl;
    cout << "State test3, store value in a float with getValue() = " << test3 << endl;

    cout << "get value of non existing param : " << *State::params("non-existing")<< endl;
    cout << "get value of empty param : " << *State::params("")<< endl;

    State::params("new one")->setValue(42);

    State::print();

    State::params("test")->setValue(2);
    State::print();

    State::params("test")->attachCallback((void*) this, this->callback);
    State::params("test")->attachCallback((void*) nullptr, nonMemberCallback);
    State::params("Test")->call(10.);

    this->test = 30;
    State::params("test")->call(10.);

    State::params("vco1.freq", 127); */

    Patch* patch = new Patch();
    
    // float tmp = 12;
    // quantizeFloat(&tmp, 1);

    this->audio = new Audio;
    this->audio->setInput(patch);

    this->audio->start();

}

void Scharm::close(){
    cout << "closing Scharm class \n";
    State::save();
}

void Scharm::callback(void* voidPtr, float v){
    Scharm *p = (Scharm*) voidPtr;
    cout << "Scharm callback, scharm test = " << p->test << ", value = " << v << endl;
}