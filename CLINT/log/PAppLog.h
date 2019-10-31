
#ifndef __APPLOG__
#define __APPLOG__

#include <vector>
#include <list>
#include <set>
#include <map>
#include <deque>
#include <string>
#include <iostream>
#include "PFileLog.h"


//��־���Ͷ���
typedef enum eLogType              
{
    LOG_EVENT_DEBUG = 0,
    LOG_EVENT_WARNNING = 1,
    LOG_EVENT_ERROR = 2
}enumLogType;

//ģ�����Ͷ���
typedef enum eModuleType              
{
    OTHER = 0,
    TASK = 1,
    MYSQL = 2,
}enumModuleType;
//��־�������
typedef enum eLogOutPut
{
	LOG_OUTPUT_UNDEFINE = 0,	//δ�����������
	LOG_OUTPUT_SCREEN	= 1,	//�������Ļ
	LOG_OUTPUT_FILE		= 2,	//������ļ�
	LOG_OUTPUT_BOTH		= 3		//�������Ļ���ļ�
}enumLogOutPut;

const float EPSINON = 0.00001f;

//#define MAX_VIDEO_DSPS          24  //ͨ����
//#define NUM_RECORD_DSPS         20  //¼��ͨ����
//#define NUM_EVENTS              9   //�߳�ʹ�õĿ����¼�����

//�ؼ�����ͬ���࣬���ڱ������ڲ��Ĵ���ͬ��
class SynClass
{
private:
	bool     m_Init;
	CRITICAL_SECTION  m_Cs;
public:
	SynClass();
	~SynClass();
	void Lock();//����	
	void UnLock();//�˳�
};

///<summary>
///     ������־��
///<summary>
class CAppLog
{
public:
    CAppLog(void);
    virtual ~CAppLog(void);  
//
private:
    bool mOutputToFile;
    eLogType mLogType;  //��־����
	enumLogOutPut mLogOutPut;
	enumLogOutPut mOldLogOutPut; //�������ļ���ʼ��������
    void GetLogPath(eModuleType Module,char* strLogPath,char* strModule); //ͨ�������ĵõ�·������ȡ����Ӧģ����־�ļ�·��
//����ӿ�
public:

	eLogType GetLogType(void){return mLogType;};						//�õ���־������
	bool SetLogType(eLogType LogType){mLogType=LogType;return true;};   //������־������
	void ManualSetOutType(enumLogOutPut LogOutPut){mLogOutPut = LogOutPut;};	//�ֶ��ı���־�����ʽ
	void CloseManualSet(){mLogOutPut = mOldLogOutPut;};							//�ر��ֶ��ı�������ʽ����ԭΪĬ��

    void SetOpenKey(bool IsOpen);    
   // bool WriteToLog(eModuleType Module,eLogType LogType,char* EventLog); //��һ���ĸ�ʽд��־
    bool WriteToLog(eModuleType Module,eLogType LogType,string& EventLog); 
	bool WriteLog(eModuleType Module,eLogType LogType,std::string& EventLog,enumLogOutPut LogOutPut = LOG_OUTPUT_UNDEFINE);

	
    static CAppLog* GetInstance();
	static CAppLog* _Instance; 
    static SynClass* _Syn;
};



#endif