#pragma once
#include <string>
#include "../ocilib-4.4.1-windows/include/ocilib.hpp"
using namespace std;
using namespace ocilib;
class Database
{
public:
	Database(string orclSrv,string usr,string pwd);
	
	~Database();

	int GetIndexFromName(string name);


	inline int ExecuteSql(string sqlstr)
	{
		st.Execute(sqlstr);
		int result=st.GetAffectedRows();
		con.Commit();
		return result;
	}
private:
	Connection con;
	Statement st;
};

