#pragma once
#include <exception>
#include <string>
using namespace std;
class ParseException :public exception
{
private:
	string info;
public:
	ParseException();
	ParseException(string _info);
	~ParseException();
public:
	virtual char const * what()  const throw() override;
	
};

