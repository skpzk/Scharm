#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include <fstream>
#include <iostream>

#include <filesystem>

class StateDict;
class Param;

/**
 * State is a class containing a number of parameters for the audio engine and the gui
 * It can be accessed from anywhere, behaving like a global variable
 * 
 * it stores its contents in a file, which must be read when starting the app
 * 
 * an arbitrary number of callbacks can be attached to any parameter (see below)
 * 
 * - Structure :
 *   State is a class containing a StateDict, which stores Params;
 *   To get a param, you are forced to talk to the StateDict class, 
 *   which manages a vector of all the params and returns a pointer to a param.
 * 
 *   Param* p = State::params("name of the param");
 *   
 *   Although most of the time, you wouldn't have to store the pointer.
 *   
 * 
 *   If you try to access a param that doesn't exist, the param is created with a default value of 0, 
 *   unless you try to access a param with an empty name,
 *   in which case it returns a param with a 0 value which is not stored.
 * 
 *   The param names are stored without spaces and in lowercase, but you can access them with spaces and uppercase, 
 *   they are removed when searching for the param in the param vector.
 * 
 * 
 * 
 * - reading from a file
 *   // somewhere at the start of the program, this must be called to read from the default file:
 *    State::read();
 * 
 * - Writing a parameter
 *   // note that in both cases the parameter is created if not already existing
 *    State::params("name")->setValue( floatValue );
 *   or
 *    State::params("name", floatValue);
 * 
 * - Reading a parameter
 *   // note that in both cases the parameter is created if not already existing
 *    float value = State::params("name")->getValue();  
 *   or
 *    float value = *State::params("name");
 *    
 * - writing to a file
 *   // before closing the app entirely, this must be called to store the values in the default file :
 *    State::save();
 * 
 * - Callbacks :
 *   any param can have any number of callback functions.
 *   a callback must have this synopsis :
 *  
 *     function<void(void*, float)>;
 *   eg:
 *     void callback(void* pointerToTheInstance, float value);
 * 
 *   In practice, it would look like this :
 * 
 *     void CallingClass::callback(void* voidPtr, float value){
 *       CallingClass *p = (CallingClass*) voidPtr;
 *       p->someMemberFunction(value) ;
 *     }
 *   
 *   To attach a callback to a param :
 * 
 *     State::params("name")->attachCallback((void*) this, this->callback);
 * 
 *   You can also attach non member callbacks :
 *     State::params("name")->attachCallback((void*) nullptr, nonMemberCallback);
 *     
 *    where
 *     void nonMemberCallback(void* voidPtr, float value);
 *    ignores the voidPtr
 * 
 *   To execute callbacks :
 *     
 *     State::params("name")->call(floatValue);
 *     // all callbacks attached to this param are called sequentially
 *     
 *   
 * 
 */

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
		static void print(std::ofstream * out);

		// void saveConf();

		static std::filesystem::path configDir;
		static std::filesystem::path autosavePath;

		static void createAutosavepath();

		// void exportState(ofstream* f);
};

class StateDict{
	public:
		StateDict();
		// StateDict(string);
		void print();
		void print(std::ofstream * out);
		// void operator=();
		// operator float() const;

		Param *operator()(std::string);
		void operator()(std::string, float);
		// void operator()(string, float);
		
		
		
		// Param *testReturnParam(string name_);

		// friend class Param;
		int size();

	private:
	  // this pointer is returned when trying to access a param with empty name
		// it is not stored, and has a default value of 0
		Param * nullParam;

		// vector of all params
		std::vector<Param> params;

		// create param
		void setParam(std::string, float);

		// find param
		Param* findParam(std::string);

};

struct CallbackFunction
{
	void* p;
	std::function<void(void*, float)> callback;
};


class Param{
	public:
		Param(std::string, float);
		Param(std::string);
		void setValue(float);
		float getValue();

		std::string getName();

		operator float() const;
		void attachCallback(void*, std::function<void(void*, float)>);
		
		// friend class StateDict;

		void call(float);
			
	private:
		std::string name;
		float value;
		
		std::vector<CallbackFunction> callbacks;
};

#endif // STATE_H