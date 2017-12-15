#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;
class ParseCSV
{
public:
	string strPath;
	ifstream infile;
	const int MaxColumn = 100;
	//ifstream lastPos;
public:
	ParseCSV(string strPath);
	void GoToStart();
	void GoToLine(int num);
	string GetLine();
	string GetValue(string lineStr,int iColumn);
	int Lookup(string str, int row);//return 0 if faild
	static  string ConvertTime(string time);//from 10-Jan-15 00:12:23 to 2015-01-10 00:12:23
	~ParseCSV();
};

