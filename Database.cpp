#include "Database.h"
#include "ParseException.h"
Database::Database(string orclSrv, string usr, string pwd)
{
	Environment::Initialize();
	con.Open(orclSrv, usr, pwd);
	////////////////
	Statement temp_st(con);
	st = temp_st;

}
int Database::GetIndexFromName(string name)
{
	string sql = "select saveindex from tb_nx_measure where name ='" + name + "'";
	st.Execute(sql);
	Resultset rs = st.GetResultset();
	int saveindex = 0;
	while (rs.Next())
	{
		saveindex = rs.Get<int>(1);
	}
	if (saveindex <= 0)
	{
		ParseException *ex = new ParseException("Get saveindex error!");
		throw ex;
	}
	return saveindex;
}

Database::~Database()
{
	Environment::Cleanup();
}


