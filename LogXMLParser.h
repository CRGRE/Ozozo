#pragma once

#include "PUG/pugixml.hpp"
#include "OzoBot.h"
#include <string>
#include <string.h>
#include <cmath>

using namespace pugi;
using namespace std;

class LogXMLParser {
	private:
		void parseXML();
	public:
		int gridWidth;
		int gridHeight;
		int cellSide;
		vector<vector<bool>> grid;
		vector<OzoBot> ozobots;
		xml_document* doc;
		LogXMLParser(const char* xmlFileName);
		void compileAgents();
};
