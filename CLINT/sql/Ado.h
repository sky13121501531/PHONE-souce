#pragma once
#include "Winsock2.h"
#include "windows.h"

//导入ADO库，位置在计算机c盘
#import "msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF") //32位系统加上这个
//#import "c:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF") rename("BOF","adoBOF") //64位系统加上这个

class CADOConn
{
public:
	CADOConn(void);
	~CADOConn(void);

public:
	//添加一个指向Connection对象的指针:
	_ConnectionPtr m_pConnection;
	//添加一个指向Recordset对象的指针:
	_RecordsetPtr m_pRecordset;
	// 定义方法
public:
	// 初始化—连接数据库
	bool OnInitADOConn();
	bool IsOpen();
	// 执行查询
	_RecordsetPtr& GetRecordSet(_bstr_t bstrSQL);
	// 执行SQL语句，Insert Update _variant_t
	bool ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();
};
