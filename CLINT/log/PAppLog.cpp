
#include <direct.h>
#include "PAppLog.h"
#include "../TimeUtil.h"
CAppLog* CAppLog::_Instance = NULL;
SynClass* CAppLog::_Syn = new SynClass();

//ͬ������
SynClass::SynClass()
{
	::InitializeCriticalSection(&m_Cs);
	m_Init = true;	
}

SynClass::~SynClass()
{
	if (m_Init)
	{
		::DeleteCriticalSection(&m_Cs);
		m_Init = false;
	}
}

void SynClass::Lock()
{
	if (m_Init) ::EnterCriticalSection(&m_Cs);
}

void SynClass::UnLock()
{
	if (m_Init) ::LeaveCriticalSection(&m_Cs);
}

//��־��д
CAppLog::CAppLog(void)
{
    SetLogType(LOG_EVENT_DEBUG);
    mOutputToFile = false;
}

CAppLog::~CAppLog(void)
{

}

CAppLog* CAppLog::GetInstance()
{
    _Syn->Lock();
    if(_Instance==NULL)
    {
        _Instance = new CAppLog;
    }
    _Syn->UnLock();

    return _Instance;
}

/// <summary>�����Ƿ���־д���ļ�</summary>
/// <param name="IsOpen">�Ƿ�д���ļ�[true��д���ļ���false���ڿ���̨���]</param>
/// <retvalue>�޷���</retvalue>
void CAppLog::SetOpenKey(bool IsOpen)
{
    mOutputToFile = IsOpen;
}

/// <summary>������ݵ���־�ļ�</summary>
/// <param name="Module">ö�����ͣ�ֵΪOTHER��ANALYSER��VIDEO��RECORD</param>
/// <param name="LogType">ö�����ͣ�ֵΪLOG_EVENT_DEBUG��LOG_EVENT_WARNNING��LOG_EVENT_ERROR</param>
/// <param name="EventLog">����˵��</param>
/// <retvalue>����ֵ˵��</retvalue>
//bool CAppLog::WriteToLog(eModuleType Module,eLogType LogType,char* EventLog)
//{
//    return WriteToLog(Module,LogType,(const char*)EventLog);
//}

//
bool CAppLog::WriteToLog(eModuleType Module,eLogType LogType,string& EventLog)
{
    bool rst = false;
    time_t now;
    char temp[21];
    string strLogPath = "c:\\APP_logs\\logs";
    char strModule[20];
    eLogType LogLevel = GetLogType();
	SetOpenKey(true);
    //if(LogLevel <= LogType)
    //{
        if(mOutputToFile)
        {
            memset(strModule,0,20);
            memset(temp,0,21);
            time(&now);
            strftime(temp, 10, "%Y%m%d", localtime(&now));
            strcat(temp,".log");
            _mkdir(strLogPath.c_str());
            CFileLogs filelog(strLogPath.c_str(),(const char*)temp);
            if(filelog.OpenFile())
            {
                strftime(temp, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
                filelog.AddLog("\xd\xa#----------------------------- [");
                filelog.AddLog(temp);
                filelog.AddLog("] -----------------------------#\xd\xa");
                filelog.AddLog("��־���ݣ�");
                filelog.AddLog(EventLog.c_str());
				filelog.AddLog("\xd\xa");
                filelog.Close();
                rst = true;
            }
        }
      //  else 
      //  {
     //       rst = true;/
      // }
   // }
    return rst;
}
bool CAppLog::WriteLog(eModuleType Module,eLogType LogType,std::string& EventLog,enumLogOutPut LogOutPut)
{
	bool ret = false;
	enumLogOutPut tempLogOutPut;

	LogOutPut != LOG_OUTPUT_UNDEFINE ? tempLogOutPut = LogOutPut : tempLogOutPut = mLogOutPut;

	if (tempLogOutPut == LOG_OUTPUT_BOTH)
	{
		std::cout << TimeUtil::DateTimeToStr(time(0)) << ": " << EventLog << std::endl;
		ret = WriteToLog(Module,LogType,EventLog);
	}
	else if (tempLogOutPut == LOG_OUTPUT_SCREEN)
	{
		std::cout << TimeUtil::DateTimeToStr(time(0)) << ": " << EventLog << std::endl;
		ret = true;
	}
	else if (tempLogOutPut == LOG_OUTPUT_FILE)
	{
		ret = WriteToLog(Module,LogType,EventLog);
	} 
	else
	{
		std::cout << TimeUtil::DateTimeToStr(time(0)) << ": " << EventLog << std::endl;
		ret = WriteToLog(Module,LogType,EventLog);
	}
	return ret;
}
