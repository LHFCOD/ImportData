#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <regex>
#include <iostream>
#include <cassert>
#include "ParseFile.h"
using namespace std;

bool ParseFile::ReadFile(string strPath)
{
	ifstream infile;
	infile.open(strPath.data());
	if (!infile.is_open())
	{
		ParseException *excep = new ParseException("Can't opent configure file:" + strPath);
		throw excep;
	}

	string buffer;
	int line = 0;
	while (getline(infile, buffer))
	{
		line++;
		//remove the comment based on "##"
		string::size_type pos = buffer.find("##");
		string serchStr;
		if (pos != string::npos)
		{
			if (pos != 0)
				serchStr = buffer.substr(0, pos - 1);
			else
				serchStr = "";///if all are comments
		}
		else
		{
			serchStr = buffer;
		}
		regex reg("\\s*(\\S+)\\s*=\\s*(.+)\\s*");
		smatch m;
		if (regex_search(serchStr, m, reg))
		{
			//str(1) reprents the variable on the left of the equal sign
			//str(2) reprents the variable on the right of the equal sign
			//cout << m.str(1) << m.str(2) << endl;
			//analyze the keywords
			map<string, keyWords>::iterator iter;
			string strKey= m.str(1);
			iter = keyWordsMap.find(strKey);
			if (iter != keyWordsMap.end())
			{
				keyWords keyword = iter->second;
				if (keyword == keyWords::titleLine)
				{
					int a=2;
				}
				//if not successfully create the class info
				if (!CreateInfo(keyword, m.str(2)))
				{
					char msg[100];
					sprintf(msg, "line%d:error when creating infomation based on the keyword!", line);
					ParseException *excep = new ParseException(msg);
					throw excep;
				}
			}
			else
			{
				//if not find keyword in map
				char msg[100];
				sprintf(msg, "line%d:not find keyword!", line);
				ParseException *excep = new ParseException(msg);
				throw excep;
			}
		}
		else
		{
			///if not find a suitable expression and the string is not empty,an error is reported
			if (!serchStr.empty())
			{
				char *msg = new char[100];
				sprintf(msg, "line%d:syntax error!", line);
				ParseException *excep = new ParseException(msg);
				throw excep;
			}
		}
	}
	infile.close();
	return true;
}
bool ParseFile::CreateInfo(keyWords keyword, string info)
{
	switch (keyword)
	{
	case keyWords::orclSrv:
	{
		orclSrv = info;
		break;
	}
	case keyWords::path:
	{
		FileInfo *fileinfo = new FileInfo();
		fileinfo->filePath = info;
		list_info.push_back(fileinfo);
		break;
	}
	case keyWords::title:
	{
		regex reg("([^,]\\S[^,]+)");
		smatch m;
		//get the recent file path
		FileInfo *fileinfo = list_info.back();
		if (fileinfo == nullptr)
		{
			//if you don't specify the file path
			ParseException *excep = new ParseException("No file path specified!");
			throw excep;
		}
		else
		{
			while (regex_search(info, m, reg))
			{
				fileinfo->title.push_back(m.str(1));
				info = m.suffix();
			}
		}
		break;
	}
	case keyWords::field:
	{
		regex reg("([^,]\\S[^,]+)");
		smatch m;
		FileInfo *fileinfo = list_info.back();
		if (fileinfo == nullptr)
		{
			ParseException *excep = new ParseException("No file path specified!");
			throw excep;
		}
		else
		{
			while (regex_search(info, m, reg))
			{
				fileinfo->field.push_back(m.str(1));
				info = m.suffix();
			}
		}
		break;
	}
	case keyWords::start:
	{
		FileInfo *fileinfo = list_info.back();
		if (fileinfo == nullptr)
		{
			ParseException *excep = new ParseException("No file path specified!");
			throw excep;
		}
		else
		{
			stringstream stream;
			stream << info;
			stream >> fileinfo->start;
		}
		break;
	}
	case keyWords::end:
	{
		FileInfo *fileinfo = list_info.back();
		if (fileinfo == nullptr)
		{
			ParseException *excep = new ParseException("No file path specified!");
			throw excep;
		}
		else
		{
			stringstream stream;
			stream << info;
			stream >> fileinfo->end;
		}
		break;
	}
	case keyWords::titleLine:
	{
		FileInfo *fileinfo = list_info.back();
		if (fileinfo == nullptr)
		{
			ParseException *excep = new ParseException("No file path specified!");
			throw excep;
		}
		else
		{
			stringstream stream;
			stream << info;
			stream >> fileinfo->titleLine;
		}
		break;
	}
	case keyWords::timePos:
	{
		FileInfo *fileinfo = list_info.back();
		if (fileinfo == nullptr)
		{
			ParseException *excep = new ParseException("No file path specified!");
			throw excep;
		}
		else
		{
			stringstream stream;
			stream << info;
			stream >> fileinfo->timePos;
		}
		break;
	}
	default:
		return false;
		break;
	}
	return true;
}
ParseFile::ParseFile()
{
	initial();
}
ParseFile::~ParseFile()
{
	list<FileInfo*>::iterator iter;
	for (iter = list_info.begin(); iter != list_info.end(); iter++)
	{
		if (*iter != nullptr)
		{
			delete *iter;
		}
	}
}
bool ParseFile::initial()
{
	//create map of keywords
	keyWordsMap.insert(map<string, keyWords>::value_type("orclSrv", keyWords::orclSrv));
	keyWordsMap.insert(map<string, keyWords>::value_type("path", keyWords::path));
	keyWordsMap.insert(map<string, keyWords>::value_type("start", keyWords::start));
	keyWordsMap.insert(map<string, keyWords>::value_type("end", keyWords::end));
	keyWordsMap.insert(map<string, keyWords>::value_type("title", keyWords::title));
	keyWordsMap.insert(map<string, keyWords>::value_type("field", keyWords::field));
	keyWordsMap.insert(map<string, keyWords>::value_type("titleLine", keyWords::titleLine));
	keyWordsMap.insert(map<string, keyWords>::value_type("timePos", keyWords::timePos));
	return true;
}