#include "ParseFile.h"
#include "ParseCSV.h"
#include <sstream>
#include <vector>
#include "Database.h"
#include <boost/date_time/gregorian/gregorian.hpp> 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iomanip>
int main(int argc,char** argv)
{
	ofstream ofile;
	ofile.open("output.log");
	try
	{
		if (argc != 2)  
		{
			ParseException *ex = new ParseException("command line parameter error!");
			throw ex;
		}

		ParseFile parse;
		parse.ReadFile(argv[1]);

		Database database(parse.orclSrv, "imsoft", "imsoft");
		int num_file = 0;
		int count_file = parse.list_info.size();
		for (auto iter = parse.list_info.begin(); iter != parse.list_info.end(); iter++)
		{
			num_file++;
			FileInfo *fileinfo = *iter;
			//输出文件名字
			ofile << fileinfo->filePath << endl;
			cout << fileinfo->filePath << endl;
			ParseCSV parsecsv(fileinfo->filePath);
			//输出标题
			ofile <<"file\tline\tprocess\tstate"<< endl;
			cout << "file\tline\tprocess\tstate" << endl;
			//used to store the position of titile
			vector<int> iColumn;
			for (auto title : fileinfo->title)
			{
				//look up the position of title
				int pos = parsecsv.Lookup(title, fileinfo->titleLine);
				iColumn.push_back(pos);
			}
			//字段的存储索引
			vector<int> saveindex;
			for (auto field : fileinfo->field)
			{
				int temp_saveindex = database.GetIndexFromName(field);
				saveindex.push_back(temp_saveindex);
			}
			//使文件指针到达指定位置
			parsecsv.GoToLine(fileinfo->start);
			//存放更新语句
			stringstream stream;
			//读取csv的数据进行导入
			for (int index = fileinfo->start; index < fileinfo->end; index++)
			{
				//获取一行
				string lineStr = parsecsv.GetLine();
				//如果不为空
				if (!lineStr.empty())
				{
					//清空更新字符串流
					stream.str("");
					stream<<"update tb_nx_his_run set ";
					for (int i=0;i<saveindex.size();i++)
					{
						//获取csv中第i列的值
						//iColumn中代表了字段的的列位置
						string value=parsecsv.GetValue(lineStr, iColumn[i]);
						stream << "v" << saveindex[i] << "=" << value;
						if (i != saveindex.size() - 1)
							stream << ",";
						else
							stream << " ";
					}
					stream << "where cytime ='";
					string cytime = ParseCSV::ConvertTime(parsecsv.GetValue(lineStr, fileinfo->timePos));
					stream << cytime<<"'";	
					string sqlstr = stream.str();
					//如果执行sql语句返回不为1，表明失败
					float process = (index - fileinfo->start + 1.0) / (fileinfo->end - fileinfo->start + 1)*100.0;
					if (database.ExecuteSql(sqlstr) != 1)
					{
						ofile <<num_file<<'/'<<count_file<<'\t'<< index<<'\t' <<setprecision(2)<<process<<'%'<< '\t' << "faild"<<endl;
						cout << num_file << '/' << count_file << '\t' << index << '\t' << setprecision(2) << process << '%' << '\t' << "faild" << endl;
					}
					else
					{
						ofile << num_file << '/' << count_file << '\t' << index << '\t' << setprecision(2) << process << '%' << '\t' << "success" << endl;
						cout << num_file << '/' << count_file << '\t' << index << '\t' << setprecision(2) << process << '%' << '\t' << "success" << endl;
					}
					//推入到硬盘
					ofile.flush();

				}
			}
		}	
		ofile << "Done!" << endl;
		cout << "Done!" << endl;
	}
	catch (exception *ex)
	{
		ofile<<"Exception:" << ex->what() << endl;
		cout << "Exception:" << ex->what()<<endl;
		delete ex;
	}
	ofile.close();
}


