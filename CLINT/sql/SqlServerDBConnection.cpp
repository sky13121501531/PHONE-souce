
#include "SqlServerDBConnection.h"

MySql::MySql(void)
{
}

MySql::~MySql(void)
{
}
bool MySql::QueryTask(vector<string>& vecXml)
{
	bool ret=false;
	try
	{
		CADOConn ado;
		//连接数据库
		ado.OnInitADOConn();
		//查询语句
		_bstr_t sql="select taskinfo from Task";
		//执行查询语句
		ado.GetRecordSet(sql);
		string vXml;
		while (!ado.m_pRecordset->adoEOF)
		{
			vXml = (_bstr_t)ado.m_pRecordset->GetCollect("TaskInfo");
			
			vecXml.push_back(vXml);

			ado.m_pRecordset->MoveNext();
		}
		//断开连接
		ado.ExitConnect();
		ret=true;
	}
	catch (...)
	{
		return false;
	}
	return ret;
}


