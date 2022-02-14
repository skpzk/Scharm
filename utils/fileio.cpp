#include "fileio.h"

void getConfigurationFromFile(fs::path loadPath){

	ifstream conf;

	conf.open(loadPath);

	if(conf.is_open()){
		int i=-1, j=0;
		string line;
		

		do{ // while(!conf.eof());
			getline(conf, line);
			if(!isAComment(line, false)){

				// remove spaces and tabs
				line.erase(remove(line.begin(),line.end(),' '),line.end());
				line.erase(remove(line.begin(),line.end(),'\t'),line.end());

				string paramName = line.substr(0, line.find("="));
				string paramValue = line.substr(line.find("=") + 1);
				string midi = paramName.substr(0, paramName.find("."));

				if(midi.compare("midi") == 0){
					// paramName = paramName.substr(paramName.find(".") + 1);
					// //midi configuration:
					// StateParameter* p = new StateParameter;
					// // TODO : check if paramName is not empty
					// p->name = paramName;
					// state->addParameter(p, false);
					// state->setCC(paramName, stof(paramValue));
				}
				else{
					// TODO : check if paramName is not empty
					try {
						if(!paramName.empty()){
							// convert value from string to float
							float value = stof(paramValue);
							State::params(paramName)->setValue(value);
						}
					}
					catch (const std::invalid_argument& e) {
						cerr << e.what();
					}
				}

			}
			// getline(conf, line);
		}while(!conf.eof());
		conf.close();
	}else{
		cout << "Error opening configuration file : no file\n";
		conf.close();
	}  
}

void writeConfigurationToFile(fs::path savePath){
	ofstream f;
	// cout << "opening file\n";
	f.open(savePath);
	if(f.is_open()){
		cout << "Saving data to " << savePath.filename() << "... ";
		State::print(&f);
		cout << "done.\n";
	}else{
		cout <<"error opening file "<< savePath <<"\n";
	}
	f.close();
}


bool isAComment(string s, bool print){
	if(print) cout << "line : " << s << "\n";
	int i=0;
	while(s[i] == ' ' || s[i] == '\t'){
		// if(s[i] == ' ')  cout<<"space ";
		// if(s[i] == '\t') cout<<"tab ";

		i++;
		if(i>=s.size()){
			if(print) cout <<"not a comment, blank line\n";
			return true;
		}
	}
	if(s[i] == '#'){
		if(print) cout <<"comment\n";
		return true;
	}else{
		if(print) cout << "Not a comment\n";
		return false;
	}
}

bool isNumeric(string str) {
	for (int i = 0; i < str.length(); i++)
	if (isdigit(str[i]) == false)
	return false; //when one non numeric value is found, return false
	return true;
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
	} else {
		return false;
	}
}