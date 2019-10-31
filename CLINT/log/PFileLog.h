///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
//                     ������������־�ļ�������ͷ�ļ�                                    //
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
	CFileLogs(const char *,const char *);//�趨��־�ļ���

	virtual ~CFileLogs();
//
private:
	char* GetCurrentPath();

//
protected:
	CRITICAL_SECTION _csLock; //�ٽ���
	char *_szPath;			  //��־�ļ�·��
	char * _szFileName;		  //��־�ļ�����
	HANDLE _hFile;            //�ļ����

	DWORD Write(LPCVOID lpBuffer, DWORD dwLength);   //�޸�ʽд��־
	void WriteLog( LPCVOID lpBuffer, DWORD dwLength);//�и�ʽд��־, ������չ�޸�
	void Lock();                                     //�����ļ���Դ
	void Unlock();									 //�ͷ��ļ���Դ
//Impliment
public:
	void SetPath(const char* szPath);			//������־�ļ�·��
	void SetFileName(const char* szName);		//�޸��ļ����� ͬʱ�ر���һ����־�ļ�
	const char* GetPathe;						//�õ���־�ļ�·��
	const char* GetFileName();					//�õ���־�ļ�����
	bool OpenFile();							//���ļ��� ָ�뵽�ļ�β
	bool IsOpen();								//�ж���־�ļ��Ƿ��
	void Close();								//�ر���־�ļ�
	void AddLog(LPCVOID lpBuffer, DWORD dwLength);	//׷����־����
	void AddLog(const char* szText);				//ͬ�ϣ����غ���
	void CreatFilebox(void);
};
#endif