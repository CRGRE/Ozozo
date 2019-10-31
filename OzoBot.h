#pragma once

#define OzoStay 0
#define OzoMove 1
#define OzoRotate 2
#define OzoChangeColor 3
#define OzoColorRed 4
#define OzoColorYellow 5
#define OzoColorGreen 6

#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h> 

using namespace std;

struct OzoColor{
	int r;
	int g;
	int b;
	OzoColor();
	OzoColor(int r, int g, int b);
};

class OzoBot {
	private:
		struct Section {
			int type;
			int parameter;
			Section(int type = 0, int parameter = 0);
		};
		vector<Section> sequence;
		void compSequence(vector<unsigned char>* bytes);
		void calcLengthBytes(vector<unsigned char>* bytes);
		void calcCheckSum(vector<unsigned char>* bytes);
	public:
		int Sx;
		int Sy;
		int Shead;
		int Gx;
		int Gy;
		int Ghead;
		int moveSpeed;
		int rotateSpeed;
		OzoColor color;
		string compiledColorCode;
		OzoBot(int Sx = 0, int Sy = 0, int Shead = 0, int Gx = 0, int Gy = 0, int Ghead = 0, int moveSpeed = 0, int rotateSpeed = 0, OzoColor color = OzoColor());
		void appendSection(int type, int parameter);
		string compColorCode();
};
