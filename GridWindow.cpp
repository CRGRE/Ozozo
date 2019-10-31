#include "GridWindow.h"

bool GridWindow::chooseFile() {
	
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	
	ofn.lStructSize = sizeof(ofn);
	char filePath[150];
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filePath;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(filePath);
	ofn.lpstrFilter = "Xml Files\0*.XML\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
	
	if (GetOpenFileName(&ofn)) {
		this->filePath = ofn.lpstrFile;
		logXML = new LogXMLParser(this->filePath);
		//MessageBox(NULL, "lel", filePath, MB_OK);
		return true;
	} else return false;
}

void GridWindow::createDefWindow() {
	PPMm = float(GetDeviceCaps(GetWindowDC(GetActiveWindow()), HORZRES)) / float(GetDeviceCaps(GetWindowDC(GetActiveWindow()), HORZSIZE));
	int rad = int(PPMm * 20.0 + 0.5);
	window = new RenderWindow(VideoMode(rad * 2 + 10, rad * 2 + 10), "OzoBots Grid", Style::Default);
	CircleShape round(rad, 64);
	round.setPosition(Vector2f(5, 5));
	round.setFillColor(Color::White);
	round.setOutlineThickness(2);
	round.setOutlineColor(Color(63, 63, 63));
	window->clear(Color::White);
	window->draw(round);
	window->display();
}

void GridWindow::createGridWindow() {
	//char mes[2]; mes[0] = '0' + logXML->cellSide % 10; mes[1] = '\0';
	//MessageBox(NULL, "lol", mes, MB_OK);
	PPMm = float(GetDeviceCaps(GetWindowDC(GetActiveWindow()), HORZRES)) / float(GetDeviceCaps(GetWindowDC(GetActiveWindow()), HORZSIZE)); //GetDesktopWindow
	if (logXML->cellSide) cellSide = int(PPMm * logXML->cellSide + 0.5); else {
		int w = int((float(GetDeviceCaps(GetWindowDC(GetActiveWindow()), HORZRES)) - 100.0) / logXML->gridWidth + 0.5);
		int h = int((float(GetDeviceCaps(GetWindowDC(GetActiveWindow()), VERTRES)) - 100.0) / logXML->gridHeight + 0.5);
		if (w < h) cellSide = w; else cellSide = h;
		logXML->cellSide = cellSide;
	}
	logXML->compileAgents();
	width = logXML->gridWidth * cellSide + 1;
	height = logXML->gridHeight * cellSide + 1;
	window = new RenderWindow(VideoMode(width, height), "OzoBots Grid [O = Open File  Enter = Flash  Space = Start  Esc = Escape]", Style::Default);
	window->setFramerateLimit(20);
	
	for (int w = 0; w < logXML->gridWidth; w++) for (int h = 0; h < logXML->gridHeight; h++) {
		if (!logXML->grid[w][h]) {
			RectangleShape obst(Vector2f(cellSide, cellSide));
			obst.setPosition(Vector2f(0 + cellSide * w, 0 + cellSide * h));
			obst.setFillColor(Color(31, 31, 31));
			gridObstacles.push_back(obst);
		}
	}
	for (int w = 0; w <= logXML->gridWidth; w++) {
		RectangleShape rectline(Vector2f(1, height));
		rectline.setPosition(Vector2f(0 + cellSide * w, 0));
		rectline.setFillColor(Color::Black);
		gridLines.push_back(rectline);
	}
	for (int h = 0; h <= logXML->gridHeight; h++) {
		RectangleShape rectline(Vector2f(width, 1));
		rectline.setPosition(Vector2f(0, 0 + cellSide * h));
		rectline.setFillColor(Color::Black);
		gridLines.push_back(rectline);
	}
	for (int i = 0; i < logXML->ozobots.size(); i++) {
		//char mes[2]; mes[0] = '0' + i % 10; mes[1] = '\0';
		//MessageBox(NULL, "lol", mes, MB_OK);
		CircleShape dir(float(cellSide) / 10.0, 16);
		dir.setOrigin(Vector2f(-float(cellSide) * (3.0 / 8.0 - 1.0 / 10.0), float(cellSide) / 10.0));
		dir.setPosition(Vector2f((float(logXML->ozobots[i].Sx) + 1.0 / 2.0) * float(cellSide) + 1.0, (float(logXML->ozobots[i].Sy) + 1.0 / 2.0) * float(cellSide) + 1.0));
		dir.setFillColor(Color::White);
		dir.setOutlineThickness(4);
		dir.setOutlineColor(Color(logXML->ozobots[i].color.r * 2, logXML->ozobots[i].color.g * 2, logXML->ozobots[i].color.b * 2));
		dir.rotate(-logXML->ozobots[i].Shead);
		ozoShapes.push_back(dir);
		
		CircleShape round(float(cellSide) / 8.0 * 3.0, 32);
		round.setPosition(Vector2f((float(logXML->ozobots[i].Sx) + 1.0 / 8.0) * float(cellSide) + 1.0, (float(logXML->ozobots[i].Sy) + 1.0 / 8.0) * float(cellSide) + 1.0));
		round.setFillColor(Color::White);
		round.setOutlineThickness(4);
		round.setOutlineColor(Color(logXML->ozobots[i].color.r * 2, logXML->ozobots[i].color.g * 2, logXML->ozobots[i].color.b * 2));
		ozoShapes.push_back(round);
		
		RectangleShape place(Vector2f(cellSide - 1, cellSide - 1));
		place.setPosition(Vector2f(1 + cellSide * logXML->ozobots[i].Sx, 1 + cellSide * logXML->ozobots[i].Sy));
		place.setOutlineThickness(1);
		place.setOutlineColor(Color(logXML->ozobots[i].color.r * 2, logXML->ozobots[i].color.g * 2, logXML->ozobots[i].color.b * 2));
		ozoPlaces.push_back(place);
	}
}

void GridWindow::drawGrid() {
	//char mes[2]; mes[0] = '0' + logXML->ozobots.size() % 10; mes[1] = '\0';
	//if (window->isOpen()) MessageBox(NULL, "lol", mes, MB_OK);
	window->clear(Color::White);
	for (auto obst : gridObstacles) window->draw(obst);
	for (auto rectline : gridLines) window->draw(rectline);
	for (auto dir : ozoShapes) window->draw(dir);
	window->display();
}

void GridWindow::ozoFlash() {
	drawGrid();
	int ml = 0;
	for (int i = 0; i < logXML->ozobots.size(); i++) if (logXML->ozobots[i].compColorCode().size() > ml) ml = logXML->ozobots[i].compiledColorCode.size();
	for (int i = 0; i < ml; i++) {
		window->clear(Color::White);
		for (int z = 0; z < logXML->ozobots.size(); z++) {
			if (i < logXML->ozobots[z].compiledColorCode.size()) {
				switch (logXML->ozobots[z].compiledColorCode[i]) {//KRGYBMC
					case 'K':{
						ozoPlaces[z].setFillColor(Color::Black);
						break;
					}
					case 'R':{
						ozoPlaces[z].setFillColor(Color::Red);
						break;
					}
					case 'G':{
						ozoPlaces[z].setFillColor(Color::Green);
						break;
					}
					case 'Y':{
						ozoPlaces[z].setFillColor(Color::Yellow);
						break;
					}
					case 'B':{
						ozoPlaces[z].setFillColor(Color::Blue);
						break;
					}
					case 'M':{
						ozoPlaces[z].setFillColor(Color::Magenta);
						break;
					}
					case 'C':{
						ozoPlaces[z].setFillColor(Color::Cyan);
						break;
					}
					case 'W':{
						ozoPlaces[z].setFillColor(Color::White);
						break;
					}
				}
				window->draw(ozoPlaces[z]);
			}
		}
		window->display();
	}
}

GridWindow::GridWindow() {
	opened = false;
	createDefWindow();
	while (window->isOpen()) {
		Event event;
		while (window->pollEvent(event)) {
			if (event.type == Event::Closed) window->close(); else
	        if (event.type == Event::KeyPressed) {
	           	switch (event.key.code) {
	           		case 36: {
	           			window->close();
						break;
					}
					case 57: {
						if (opened) {
							drawGrid();
							window->clear(Color::Black);
							window->display();
							drawGrid();
						}
						break;
					}
	           		case 14: {
	           			if (chooseFile()) {
	           				opened = true;
	           				window->close();
	           				createGridWindow();
	           				drawGrid();
						}
						break;
					}
	           		case 58: {
	           			if (opened) {
	           				ozoFlash();
	           				drawGrid();
						}
						break;
					}
					default: {
	           			/*char num[4] = { '0' + event.key.code / 100,
										'0' + event.key.code / 10 % 10,
										'0' + event.key.code % 10, '\0'};
	           			MessageBox(NULL, num, "Key pressed", MB_OK);*/
						break;
					}
				}
			}
		}
	}
}
