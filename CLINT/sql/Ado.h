#pragma once
#include "Winsock2.h"
#include "windows.h"

//����ADO�⣬λ���ڼ����c��
#import "msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF") //32λϵͳ�������
//#import "c:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF") rename("BOF","adoBOF") //64λϵͳ�������

class CADOConn
{
public:
	CADOConn(void);
	~CADOConn(void);

public:
	//���һ��ָ��Connection�����ָ��:
	_ConnectionPtr m_pConnection;
	//���һ��ָ��Recordset�����ָ��:
	_RecordsetPtr m_pRecordset;
	// ���巽��
public:
	// ��ʼ�����������ݿ�
	bool OnInitADOConn();
	bool IsOpen();
	// ִ�в�ѯ
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	// ִ��SQL��䣬Insert Update _variant_t
	bool ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();
};
