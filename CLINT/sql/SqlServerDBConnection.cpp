
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
		//�������ݿ�
		ado.OnInitADOConn();
		//��ѯ���
		_bstr_t sql="select taskinfo from Task";
		//ִ�в�ѯ���
		ado.GetRecordSet(sql);
		string vXml;
		while (!ado.m_pRecordset->adoEOF)
		{
			vXml = (_bstr_t)ado.m_pRecordset->GetCollect("TaskInfo");
			
			vecXml.push_back(vXml);

			ado.m_pRecordset->MoveNext();
		}
		//�Ͽ�����
		ado.ExitConnect();
		ret=true;
	}
	catch (...)
	{
		return false;
	}
	return ret;
}


