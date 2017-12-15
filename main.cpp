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
			//����ļ�����
			ofile << fileinfo->filePath << endl;
			cout << fileinfo->filePath << endl;
			ParseCSV parsecsv(fileinfo->filePath);
			//�������
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
			//�ֶεĴ洢����
			vector<int> saveindex;
			for (auto field : fileinfo->field)
			{
				int temp_saveindex = database.GetIndexFromName(field);
				saveindex.push_back(temp_saveindex);
			}
			//ʹ�ļ�ָ�뵽��ָ��λ��
			parsecsv.GoToLine(fileinfo->start);
			//��Ÿ������
			stringstream stream;
			//��ȡcsv�����ݽ��е���
			for (int index = fileinfo->start; index < fileinfo->end; index++)
			{
				//��ȡһ��
				string lineStr = parsecsv.GetLine();
				//�����Ϊ��
				if (!lineStr.empty())
				{
					//��ո����ַ�����
					stream.str("");
					stream<<"update tb_nx_his_run set ";
					for (int i=0;i<saveindex.size();i++)
					{
						//��ȡcsv�е�i�е�ֵ
						//iColumn�д������ֶεĵ���λ��
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
					//���ִ��sql��䷵�ز�Ϊ1������ʧ��
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
					//���뵽Ӳ��
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


