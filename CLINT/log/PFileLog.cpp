
#include "PFileLog.h"
#include "shlwapi.h"

#pragma comment(lib,"shlwapi.lib")

//�޲����Ĺ��캯��
CFileLogs::CFileLogs()
{
	_szPath = NULL;
	_szFileName = NULL;
	_hFile = INVALID_HANDLE_VALUE;
	::InitializeCriticalSection(&_csLock);

	_szPath = GetCurrentPath();
    strcat(_szPath,"logs");
    SetPath(_szPath);
    SetFileName("sys.log");
}

//�вι��캯��
CFileLogs::CFileLogs(const char*szFilePath="", const char* szFileName="")
{
	_szPath = NULL;
	_szFileName = NULL;
	_hFile = INVALID_HANDLE_VALUE;
	::InitializeCriticalSection(&_csLock);

	if(0 == strlen(szFilePath))
	{
		_szPath = GetCurrentPath();
        strcat(_szPath,"logs");

        printf("nopath");
	}
    else
    {
        _szPath = (char*)szFilePath;
    }

    SetPath(_szPath);

	if(0==strlen(szFileName))
	{
		SetFileName("sys.log");
	}
    else
    {
        SetFileName(szFileName);
    }
}

//��������
CFileLogs::~CFileLogs()
{
	::DeleteCriticalSection(&_csLock);
	Close();

	if(_szFileName)
	{
		delete []_szFileName;
	}
	if(_szPath)
	{
		delete []_szPath;
	}

}

//д��־�������ڲ���̳�ʹ��
DWORD CFileLogs::Write(LPCVOID lpBuffer, DWORD dwLength)
{
	DWORD dwWriteLength = 0;
	
	if(IsOpen())
	{
		WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);
	}
	
	return dwWriteLength;
}

////////////////////////////////////////////////////////////////////////////

//
char* CFileLogs::GetCurrentPath()
{
	HINSTANCE hInst=NULL;
	static char szBuf[256];
	char *p; 

	//�õ�ȫ��·��
	GetModuleFileName(hInst,szBuf,sizeof(szBuf)); 

	//����·�����ļ���
	p = szBuf;
	while(strchr(p,'\\')) {
	p = strchr(p,'\\');
	p++;
	}
	*p = '\0';

	return szBuf;
}

//
void CFileLogs::WriteLog( LPCVOID lpBuffer, DWORD dwLength)
{
	time_t now;
	char temp[21];
	DWORD dwWriteLength;

	if(IsOpen())
	{
		time(&now);
		strftime(temp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
		WriteFile(_hFile, "\xd\xa#-----------------------------", 32, &dwWriteLength, NULL);
		WriteFile(_hFile, temp, 19, &dwWriteLength, NULL);
		WriteFile(_hFile, "-----------------------------#\xd\xa", 32, &dwWriteLength, NULL);
		WriteFile(_hFile, lpBuffer, dwLength, &dwWriteLength, NULL);
		WriteFile(_hFile, "\xd\xa", 2, &dwWriteLength, NULL);
		WriteFile(_hFile, "-------------------------------------------------------------------------------\xd\xa\xd\xa", 81, &dwWriteLength, NULL);
	}
}

//
void CFileLogs::Lock()
{
	::EnterCriticalSection(&_csLock);
}

//
void CFileLogs::Unlock()
{
	::LeaveCriticalSection(&_csLock);
}

/////////////////////////////////////////////////////////////////////////////////////////////

//
bool CFileLogs::IsOpen()
{
	return _hFile != INVALID_HANDLE_VALUE;
}

//
void CFileLogs::SetFileName(const char *szName)
{
	assert(szName);

	if(_szFileName)
	{
		delete []_szFileName;
	}
	
	Close();
	_szFileName = new char[strlen(szName) + 1];
	assert(_szFileName);
	strcpy(_szFileName, szName);
}

//
void CFileLogs::SetPath(const char *szPath)
{
	assert(szPath);
    string strPath = string(szPath);
    int i = 0;
    string strSub = "";
    string strFind = "/";
    WIN32_FIND_DATA wfd;
    char temp[MAX_PATH + 1] = {0};
    
    i = (int)strPath.find_first_of(strFind);
    strSub += strPath.substr(0,i+1);
    strPath = strPath.substr(i+1,strPath.length());
    i = (int)strPath.find_first_of(strFind);
    for(;i>=0;)
    {
        strSub += strPath.substr(0,i+1);

        int j = (int)strSub.find_last_of(strFind);
        string strValue = strSub.substr(0,j);
		HANDLE hndFile = FindFirstFile(strValue.c_str(), &wfd);
        if( hndFile == INVALID_HANDLE_VALUE && CreateDirectory(strValue.c_str(), NULL) == 0)
        {
            strcat(strcpy(temp, strValue.c_str()), " Create Fail. Exit Now! Error ID :");
            ltoa(GetLastError(), temp + strlen(temp), 10);
            printf(temp);
        } 		
		if (hndFile != INVALID_HANDLE_VALUE)
			FindClose(hndFile);
		
        strPath = strPath.substr(i+1,strPath.length());
        i = (int)strPath.find_first_of(strFind);
    }

    GetFullPathName(szPath, MAX_PATH, temp, NULL);
    _szPath = new char[strlen(temp) + 1];
    assert(_szPath);
    strcpy(_szPath, temp);
}

//
const char* CFileLogs::GetFileName()
{
	return _szFileName;
}

//
bool CFileLogs::OpenFile()
{
	char szFullPath[1024];
	if(IsOpen())
	{
		return true;
	}

	if(!_szPath)
	{
		return false;
	}
	if(!_szFileName)
	{
		return false;
	}
	//�����־�ļ�·�����ļ��У������ڣ��ʹ�����
	string str = "c:\\APP_logs";
	if (!PathIsDirectory(str.c_str()))
	{
		CreateDirectory(str.c_str(), NULL);
	}
	str += "\\logs";
	if (!PathIsDirectory(str.c_str()))
	{
		CreateDirectory(str.c_str(), NULL);
	}
	//��־�ļ������ڣ�������־�ļ�
	memset(szFullPath,0,1024);
	strcat(szFullPath,_szPath);
	strcat(szFullPath,"\\");
	strcat(szFullPath,_szFileName);
	_hFile =  CreateFile(
		szFullPath, 
		GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,//�ļ������ڣ��ʹ����ļ�
		FILE_ATTRIBUTE_NORMAL,
		NULL 
		);
	if(IsOpen())
	{
		SetFilePointer(_hFile, 0, NULL, FILE_END);
	}
	return IsOpen();
}

//
void CFileLogs::Close()
{
	if(IsOpen())
	{
		CloseHandle(_hFile);
		_hFile = INVALID_HANDLE_VALUE;
	}
}

//
void CFileLogs::AddLog(LPCVOID lpBuffer, DWORD dwLength)
{
	assert(lpBuffer);
	__try
	{
		Lock();

		if(!OpenFile())
		{
			return;
		}

		WriteLog(lpBuffer, dwLength);
	}
	__finally
	{
		Unlock();
	} 
}

//
void CFileLogs::AddLog(const char* szText)
{
	//AddLog(szText, strlen(szText));
    assert(szText);
	__try
	{
		Lock();

		if(!OpenFile())
		{
			return;
		}
        size_t len = strlen(szText);
        Write(szText, DWORD(len));
	}
	__finally
	{
		Unlock();
	} 
}

void CFileLogs::CreatFilebox(void)
{
	vector<string> vFile;
	vFile.push_back( "C:/ProgramList" );
	vFile.push_back( "C:/RecordList" );
	vFile.push_back( "C:/RecordList/CTTB" );
	vFile.push_back( "C:/RecordList/DVBC" );
	vFile.push_back( "C:/RecordList/RADIO" );
	vFile.push_back( "C:/RecordList/AM" );
	vFile.push_back( "C:/RecordList/DVBS" );

	/* �����Щ�ļ����Ƿ��Ѿ���������û�У��ʹ����� */
	for (int dirIndex=0; dirIndex<vFile.size(); dirIndex++)
	{
		if(!PathIsDirectory(vFile[dirIndex].c_str()))
		{
			//std::cout<<"�ļ��� ["<<vecVscttbDir[dirIndex]<<"] �Ѿ�����,���ô���"<<std::endl;
			CreateDirectory(vFile[dirIndex].c_str(), NULL);
			cout<<"�ļ��� ["<<vFile[dirIndex]<<"] �����ɹ�"<<endl;
		}
		else
			cout<<"�ļ��� ["<<vFile[dirIndex]<<"] �Ѿ�����"<<endl;
	}
}
