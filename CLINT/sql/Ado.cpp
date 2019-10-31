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
	// ��ʼ��OLE/COM�⻷�� 
	::CoInitialize(NULL);

	int count = 0;
	HRESULT hr;
	try
	{
		// ����Connection����,����ͨ�������ļ���ȡ������Ϣ
		hr = m_pConnection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			//m_pConnection->ConnectionTimeout = 600;//�������ӳ�ʱʱ��
			//m_pConnection->CommandTimeout = 120;//����ִ�����ʱʱ��
			/*HRESULT Open (
				_bstr_t ConnectionString,
				_bstr_t UserID,
				_bstr_t Password,
				long Options );*/
			//std::string path = "Provider=SQLOLEDB.1;Persist Security Info=False;User ID=sa;Password=;Initial Catalog=vsdvb;Data Source=localhost";
			while(!CADOConn::IsOpen() && count<5)
			{
				string strmsg = "------ ���ݿ������У���ȴ�������";
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				m_pConnection->Open("Provider=SQLOLEDB.1;Data Source=localhost;Initial Catalog=vsdvb", "sa", "", adModeUnknown);
				count++;
			}
			if(CADOConn::IsOpen())
			{
				string strmsg = "------ ���ݿ��Ѿ����Ӵ�";
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				return true;
			}
			return true;
		}
	}
	// ��׽�쳣
	catch (_com_error e)
	{
		// ��ʾ������Ϣ
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
		// �������ݿ⣬���Connection����Ϊ�գ��������������ݿ�
		if (m_pConnection == NULL)
			OnInitADOConn();
		// ������¼������
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// ȡ�ñ��еļ�¼
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);
	}
	// ��׽�쳣
	catch (_com_error e)
	{
	}
	// ���ؼ�¼��
	return m_pRecordset;
}


bool CADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		// �Ƿ��Ѿ��������ݿ�
		if (m_pConnection == NULL)
			OnInitADOConn();
		// Connection�����Execute����:(_bstr_t CommandText, 
		// VARIANT * RecordsAffected, long Options) 
		// ����CommandText�������ִ���ͨ����SQL���
		// ����RecordsAffected�ǲ�����ɺ���Ӱ�������, 
		// ����Options��ʾCommandText�����ͣ�adCmdText-�ı����adCmdTable-����
		// adCmdProc-�洢���̣�adCmdUnknown-δ֪
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
	// �رռ�¼��������
	if (m_pRecordset != NULL)
	{
		m_pRecordset->Close();
		//m_pRecordset->Release();
	}
	m_pConnection->Close();
	//m_pConnection->Release();
	// �ͷŻ���
	::CoUninitialize();
}
