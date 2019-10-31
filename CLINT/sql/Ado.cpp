#include "Ado.h"
#include <string>
#include <iostream>
#include "../log/PAppLog.h"

using namespace std;
CADOConn::CADOConn(void)
{
}

CADOConn::~CADOConn(void)
{
}

bool CADOConn::OnInitADOConn()
{
	// 初始化OLE/COM库环境 
	::CoInitialize(NULL);

	int count = 0;
	HRESULT hr;
	try
	{
		// 创建Connection对象,可以通过配置文件获取连接信息
		hr = m_pConnection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			//m_pConnection->ConnectionTimeout = 600;//设置连接超时时间
			//m_pConnection->CommandTimeout = 120;//设置执行命令超时时间
			/*HRESULT Open (
				_bstr_t ConnectionString,
				_bstr_t UserID,
				_bstr_t Password,
				long Options );*/
			//std::string path = "Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Password=;Initial Catalog=vsdvb;Data Source=localhost";
			while(!CADOConn::IsOpen() && count<5)
			{
				string strmsg = "------ 数据库链接中，请等待。。。";
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				m_pConnection->Open("Provider=SQLOLEDB.1;Data Source=localhost;Initial Catalog=vsdvb", "sa", "", adModeUnknown);
				count++;
			}
			if(CADOConn::IsOpen())
			{
				string strmsg = "------ 数据库已经连接打开";
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				return true;
			}
			return true;
		}
	}
	// 捕捉异常
	catch (_com_error e)
	{
		// 显示错误信息
		return false;
	}
}

bool CADOConn::IsOpen()
{
	if(m_pConnection!=NULL)
		return m_pConnection->GetState() != adStateClosed;
	return false;
}

_RecordsetPtr& CADOConn::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		// 连接数据库，如果Connection对象为空，则重新连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		// 创建记录集对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// 取得表中的记录
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	// 捕捉异常
	catch (_com_error e)
	{
	}
	// 返回记录集
	return m_pRecordset;
}


bool CADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		// 是否已经连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		// Connection对象的Execute方法:(_bstr_t CommandText, 
		// VARIANT * RecordsAffected, long Options) 
		// 其中CommandText是命令字串，通常是SQL命令。
		// 参数RecordsAffected是操作完成后所影响的行数, 
		// 参数Options表示CommandText的类型：adCmdText-文本命令；adCmdTable-表名
		// adCmdProc-存储过程；adCmdUnknown-未知
		m_pConnection->Execute(bstrSQL, NULL, adCmdText);
		
		return true;
	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
		return false;
	}
}

void CADOConn::ExitConnect()
{
	// 关闭记录集和连接
	if (m_pRecordset != NULL)
	{
		m_pRecordset->Close();
		//m_pRecordset->Release();
	}
	m_pConnection->Close();
	//m_pConnection->Release();
	// 释放环境
	::CoUninitialize();
}
