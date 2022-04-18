#ifndef AUDIOLIB_STATEKEYS_H
#define AUDIOLIB_STATEKEYS_H

#include <string>

typedef struct 
{
  std::string freq = "";
  std::string reso = "";
	std::string quant = "quantradio";
	std::string wave = "";
	std::string volume = "";
	std::string range = "rangeradio";
	std::string assignvco = "";
	// std::string assignsub = "";
	std::string step1 = "";
	std::string step2 = "";
	std::string step3 = "";
	std::string step4 = "";
	std::string clk1 = "";
	std::string clk2 = "";
	std::string clk3 = "";
	std::string clk4 = "";
	std::string a = "";
	std::string d = "";
	std::string div = "";
	std::string sub1level = "";
	std::string filterType = "filtersradio";
	std::string guiCallback = "";
}StateKeys;


#endif /* AUDIOLIB_STATEKEYS_H */
