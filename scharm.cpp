#include "scharm.h"

#include "state/state.h"

void setGuiThing(float value_){
    cout << "setting gui thing, value = " << value_ << endl;
}

void setGuiThingVoid(){
    cout << "setting gui thing\n";
}

Scharm::Scharm(){
    cout << "creating Scharm class \n";
    State::params;

    State::read();

    State::print();

    State::params("Test")->setValue(3);
    float test = *State::params("test");
    // float test2 = *State::params.testReturnParam("test");
    
    
    State::params("Other test")->setValue(10);

    cout << "State test, direct access= " << State::params("test") << endl;
    cout << "State test, store value in a float = " << test << endl;
    // cout << "State test, conv from ptr to param = " << *State::params.testReturnParam("test") << endl;
    // cout << "State test, store value in a float from conv from ptr to param = " << test2 << endl;
    // cout << "State test, access from ptr to param = " << State::params.testReturnParam("test")->value << endl;
    State::print();

    State::params("test")->setValue(2);
    State::print();

    State::params("test")->setCallback((void*) this, this->callback);
    State::params("Test")->call(10.);

    this->test = 30;
    State::params("test")->call(10.);

    // State::read();
}

void Scharm::close(){
    cout << "closing Scharm class \n";
    State::save();
}

void Scharm::callback(void* voidPtr, float v){
    Scharm *p = (Scharm*) voidPtr;
    cout << "Scharm callback, scharm test = " << p->test << ", value = " << v << endl;
}