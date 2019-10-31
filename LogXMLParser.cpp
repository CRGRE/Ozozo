#include "LogXMLParser.h"

void LogXMLParser::compileAgents() {
	ozobots.clear();
	for (auto child : doc->child("root").child("log").children()) {
		if (!strcmp(child.name(), "agent")) {
			int Sx = child.attribute("start.x").as_int();
			int Sy = child.attribute("start.y").as_int();
			int Shead = int(child.attribute("start.heading").as_float() + 0.5);
			int Gx = child.attribute("start.x").as_int();
			int Gy = child.attribute("start.y").as_int();
			int Ghead = int(child.attribute("start.heading").as_float() + 0.5);
			int moveSpeed = int(child.attribute("movespeed").as_float() * cellSide + 0.5);
			int rotateSpeed = int(child.attribute("rotationspeed").as_float() * 31.5196 + 0.5); //3.1416 * 20.066 / 2.0
			OzoBot ozobot(Sx, Sy, Shead, Gx, Gy, Ghead, moveSpeed, rotateSpeed, OzoColor());
			for (auto section : child.child("path").children()) {
				int Shead = int(section.attribute("start.heading").as_float() + 0.5);
				int Ghead = int(section.attribute("goal.heading").as_float() + 0.5);
				if (Shead == Ghead) {
					int Sx = section.attribute("start.x").as_int();
					int Sy = section.attribute("start.y").as_int();
					int Gx = section.attribute("goal.x").as_int();
					int Gy = section.attribute("goal.y").as_int();
					if (Sx == Gx && Sy == Gy) {
						int duration = int(section.attribute("duration").as_float() * 100 + 0.5);
						ozobot.appendSection(OzoStay, duration);
					} else {
						int distance = int(sqrt((Sx - Gx) * (Sx - Gx) + (Sy - Gy) * (Sy - Gy)) * cellSide + 0.5);
						ozobot.appendSection(OzoMove, distance);
					}
				} else {
					int angle = Ghead - Shead;
					if (Ghead - Shead > 180) {
						angle -= 360;
					}
					if (Shead - Ghead > 180) {
						angle += 360;
					}
					ozobot.appendSection(OzoRotate, angle);
				}
			}
			ozobots.push_back(ozobot);
		}
	}
	for (auto ozobot : ozobots) ozobot.compColorCode();
}

void LogXMLParser::parseXML() {
	gridWidth = doc->child("root").child("map").child("grid").attribute("width").as_int();
	gridHeight = doc->child("root").child("map").child("grid").attribute("height").as_int();
	grid.resize(gridWidth, vector<bool>(gridHeight));
	int h = 0;
	for (auto row : doc->child("root").child("map").child("grid").children()) {
		string rowString = row.text().as_string();
		for (int w = 0; w < gridWidth; w++) if (rowString[w * 2] == '0') grid[w][h] = true; else grid[w][h] = false;
		h++;
	}
	cellSide = doc->child("root").child("map").child("grid").attribute("cell").as_int();
}

LogXMLParser::LogXMLParser(const char* xmlFileName) {
	doc = new xml_document();
	doc->load_file(xmlFileName);
	parseXML();
}
