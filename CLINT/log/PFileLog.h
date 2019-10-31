///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
//                     内容描述：日志文件操作类头文件                                    //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////
#ifndef __FILELOGS__
#define __FILELOGS__
#include <iostream>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
using namespace std;

class CFileLogs
{
//
public:
	CFileLogs();
	CFileLogs(const char *,const char *);//设定日志文件名

	virtual ~CFileLogs();
//
private:
	char* GetCurrentPath();

//
protected:
	CRITICAL_SECTION _csLock; //临界区
	char *_szPath;			  //日志文件路径
	char * _szFileName;		  //日志文件名称
	HANDLE _hFile;            //文件句柄

	DWORD Write(LPCVOID lpBuffer, DWORD dwLength);   //无格式写日志
	void WriteLog( LPCVOID lpBuffer, DWORD dwLength);//有格式写日志, 可以扩展修改
	void Lock();                                     //锁定文件资源
	void Unlock();									 //释放文件资源
//Impliment
public:
	void SetPath(const char* szPath);			//设置日志文件路径
	void SetFileName(const char* szName);		//修改文件名， 同时关闭上一个日志文件
	const char* GetPathe;						//得到日志文件路径
	const char* GetFileName();					//得到日志文件名称
	bool OpenFile();							//打开文件， 指针到文件尾
	bool IsOpen();								//判断日志文件是否打开
	void Close();								//关闭日志文件
	void AddLog(LPCVOID lpBuffer, DWORD dwLength);	//追加日志内容
	void AddLog(const char* szText);				//同上，重载函数
	void CreatFilebox(void);
};
#endif