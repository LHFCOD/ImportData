#pragma once
#include <iostream>
#include <list>
#include <map>
#include "ParseException.h"
using namespace std;
enum class keyWords
{
	orclSrv,
	path,
	start,
	end,
	title,
	field,
	titleLine,
	timePos
};

class FileInfo
{
public:
	string filePath;
	int titleLine;
	list<string> title;
	list<string> field;
	int start=-1;
	int end=-1;
	//the column index of the time
	int timePos = -1;
};
class ParseFile
{
public:
	map<string,keyWords> keyWordsMap;
	//the name of database server
	string orclSrv;
public:
	//the list of file info
	list<FileInfo*> list_info;
public:
	bool ReadFile(string strPath);
	bool initial();
	ParseFile();
	~ParseFile();
	bool CreateInfo(keyWords keyword, string info);
};
