#include "OzoBot.h"

OzoColor::OzoColor(int r, int g, int b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

OzoColor::OzoColor() {
	//srand(time(NULL));
	this->r = rand() % 128;
	this->g = rand() % 128;
	this->b = rand() % 128;
}

OzoBot::Section::Section(int type, int parameter) {
	this->type = type;
	this->parameter = parameter;
}

void OzoBot::compSequence(vector<unsigned char>* bytes) {
	for (auto section : sequence) {
		switch (section.type) {
			case OzoStay: {    //0.01s
				for (int duration = section.parameter; duration > 0; duration -= 127) {
					if (duration > 127) bytes->push_back(127); else bytes->push_back(duration);
					bytes->push_back(0x9b);
				}
				break;
			}
			case OzoMove: {
				for (int distance = section.parameter; distance > 0; distance -= 127) {
					if (distance > 127) bytes->push_back(127); else bytes->push_back(distance);
					bytes->push_back(moveSpeed);
					bytes->push_back(0x9e);
				}
				break;
			}
			case OzoRotate: {
				bool neg = false;
				int angle = section.parameter;
				if (angle < 0) {
					neg = true;
					angle *= -1;
				}
				for (; angle > 0; angle -= 127) {
					if (angle > 127) bytes->push_back(127); else bytes->push_back(angle);
					if (neg) bytes->push_back(0x8b);
					bytes->push_back(rotateSpeed);
					bytes->push_back(0x98);
				}
				break;
			}
			case OzoChangeColor: {
				bytes->push_back(0x00);
				bytes->push_back(0x1c);
				switch(section.parameter) {
					case OzoColorRed: {
						bytes->push_back(0x7f);
						bytes->push_back(0x00);
						bytes->push_back(0x00);
						break;
					}
					case OzoColorYellow: {
						bytes->push_back(0x7f);
						bytes->push_back(0x7f);
						bytes->push_back(0x00);
						break;
					}
					case OzoColorGreen: {
						bytes->push_back(0x00);
						bytes->push_back(0x7f);
						bytes->push_back(0x00);
						break;
					}
				}
				bytes->push_back(0xc9);
				break;
			}
		}
	}
}

void OzoBot::calcLengthBytes(vector<unsigned char>* bytes) {
	int first = 7;
	int second = 219 - bytes->size();
	while (second < 0) {
		first--;
		second += 256;
	}
	//if (first < 0) MEMORY ERROR
	unsigned char unsChar;
	bytes->insert(bytes->begin(), (unsChar = bytes->size() % 256));
	bytes->insert(bytes->begin(), (unsChar = bytes->size() / 256));
	bytes->insert(bytes->begin(), (unsChar = second));
	bytes->insert(bytes->begin(), (unsChar = first));
	bytes->insert(bytes->begin(), (unsChar = 1));
}

void OzoBot::calcCheckSum(vector<unsigned char>* bytes) {
	int res = 0;
	for (auto byte : *bytes) {
		res -= byte;
		if (res < 0) res += 256;
	}
	bytes->push_back(res);
}

string OzoBot::compColorCode() {
	//vector<unsigned char> bytesBegin = {127, 0, 0, 184, 1, 155, 14, 146, 0, 164, 128, 6, 151, 144, 0, 20, 186, 244, 0, 174, 127, 127, 0, 184};
	vector<unsigned char> bytesBegin = {0, 35, 0, 0, 0, 201, 0, 28, 127, 0, 0, 201, 1, 155, 14, 146, 0, 164, 128, 6, 151, 144, 0, 20, 186, 244, 0, 174, 0, 28, 127, 127, 0, 201};
	//vector<unsigned char> bytesEnd = {0, 127, 0, 184, 2, 174, 145};
	vector<unsigned char> bytesEnd = {0, 28, 0, 127, 0, 201, 2, 174, 145};
	vector<unsigned char> bytes;
	compSequence(&bytes);
	bytes.insert(bytes.begin(), bytesBegin.begin(), bytesBegin.end());
	bytes.insert(bytes.end(), bytesEnd.begin(), bytesEnd.end());
	bytes[2] = color.r;
	bytes[3] = color.g;
	bytes[4] = color.b;
	calcLengthBytes(&bytes);
	calcCheckSum(&bytes);
	
	string ColorBegin = "CRYCYMCRW";
	string ColorEnd = "CMW";
	string ColorAlphabet = "KRGYBMC";
	string colorString = "";
	compiledColorCode = "";
	for (auto byte : bytes) {
		string byteColor = "";
   		do {
   			byteColor = ColorAlphabet[byte % 7] + byteColor;
   			byte /= 7;
   		} while (byte != 0);
   		if (byteColor.length() < 3) {
   			if (byteColor.length() < 2) byteColor = ColorAlphabet[0] + byteColor;
   			byteColor = ColorAlphabet[0] + byteColor;
   		}		
		colorString += byteColor;
	}
	colorString = ColorBegin + colorString + ColorEnd;
	for (int i = 0; i < colorString.length(); i++) {
		if (!i || compiledColorCode[i - 1] != colorString[i]) compiledColorCode += colorString[i]; else compiledColorCode += "W";
	}
	return compiledColorCode;
}

OzoBot::OzoBot(int Sx, int Sy, int Shead, int Gx, int Gy, int Ghead, int moveSpeed, int rotateSpeed, OzoColor color) {
	this->Sx = Sx;
	this->Sy = Sy;
	this->Shead = Shead;
	this->Gx = Gx;
	this->Gy = Gy;
	this->Ghead = Ghead;
	this->moveSpeed = moveSpeed;
	this->rotateSpeed = rotateSpeed;
	this->color = color;
}

void OzoBot::appendSection(int type, int parameter) {
	sequence.push_back(Section(type, parameter));
}
