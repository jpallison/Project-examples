/*
Program should read in the lines of code of give program


*/

#include <cstdlib>
#ifndef LINEREADER_H
#define	LINEREADER_H

#include <iostream>
#include <fstream>

using namespace std;

string filename;

class Linereader
{
public:
	Linereader();
	int getLines();
private:
	int counter;
};

Linereader::Linereader()
{
	cout << "What is your file name: ";
	cin >> filename;
	ifstream input;
	input.open(filename.c_str());
	while(!input.eof())
	{
		string line;
		getline(input, line);
		counter++;
	}

input.close();
}

int Linereader::getLines()
{
	return counter;
}

#endif


