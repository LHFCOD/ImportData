#include "ParseException.h"
#include <string>
#include <iostream>
using namespace std;
ParseException::ParseException()
{
}


ParseException::~ParseException()
{
	
}
ParseException::ParseException(string _info)
{
	info = _info;
}
const char * ParseException::what() const throw()
{
	return info.data();
}