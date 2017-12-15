#include "ParseCSV.h"
#include "ParseException.h"
#include <regex>
#include <cassert>
#include <sstream>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace boost::posix_time;
ParseCSV::ParseCSV(string _strPath)
{
	strPath = _strPath;
	infile.open(strPath.data());
	if (!infile.is_open())
	{
		ParseException *excep = new ParseException("Can't open the csv file:" + strPath);
		throw excep;
	}
}
int ParseCSV::Lookup(string str, int row)
{
	streampos pos = infile.tellg();
	infile.seekg(0, ios::beg);
	string line;
	int index = 0;
	int iColumn = 0;
	while (getline(infile, line))
	{
		index++;
		if (index == row)
		{
			//stream of string
			stringstream stream(line);
			string title;
			while (getline(stream, title, ','))
			{
				iColumn++;
				if (str == title)
				{
					break;
				}
			}
			break;
		}
	}
	//redirect to the original position
	infile.seekg(pos);
	if (iColumn == 0)
	{
		ParseException *ex = new ParseException("can't find the title in the csv file:" +strPath);
		throw ex;
	}
	return iColumn;

}

ParseCSV::~ParseCSV()
{
	infile.close();
}

void ParseCSV::GoToStart()
{
	infile.seekg(0, ios::beg);
}
void ParseCSV::GoToLine(int num)
{
	if (num <= 0)
	{
		ParseException *excep = new ParseException("the input row number error!");
		throw excep;
	}
	infile.seekg(0, ios::beg);
	int index = 0;
	string temp;
	do
	{
		index++;
		if (index == num)
			break;
	}
	while (getline(infile, temp));
}

string ParseCSV::GetLine()
{
	string str;
	getline(infile, str);
	return str;
}

string ParseCSV::GetValue(string lineStr, int iColumn)
{
	stringstream stream(lineStr);
	string temp_str;
	int index = 0;
	while (getline(stream, temp_str, ','))
	{
		index++;
		if (index == iColumn)
			break;
	}
	return temp_str;
}

 string ParseCSV::ConvertTime(string time)
{
	 time_input_facet *inFormat = new time_input_facet();
	 inFormat->format("%d-%b-%y %H:%M:%S");
	stringstream stream(time);
	stream.imbue(std::locale(stream.getloc(), inFormat));
	ptime pt;
	stream >> pt;
	stream.clear();
	time_facet *outFormat = new time_facet();
	outFormat->format("%Y-%m-%d %H:%M:%S");
	stream.imbue(std::locale(stream.getloc(), outFormat));
	stream << pt;
	string result=stream.str();
	return result;
}