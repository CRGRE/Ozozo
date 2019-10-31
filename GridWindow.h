#pragma once

#include <vector>
#include <windows.h>
#include <stdlib.h>
#include "main.h"
#include "LogXMLParser.h"

using namespace sf;

class GridWindow {
	private:
		bool opened;
		float PPMm;
		int cellSide;
		int width;
		int height;
		char* filePath;
		vector<RectangleShape> ozoPlaces;
		vector<RectangleShape> gridObstacles;
		vector<RectangleShape> gridLines;
		vector<CircleShape> ozoShapes;
		LogXMLParser* logXML;
		RenderWindow* window;
		bool chooseFile();
		void createDefWindow();
		void createGridWindow();
		void drawGrid();
		void ozoFlash();
	public:
		GridWindow();
};
