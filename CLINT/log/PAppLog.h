
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


//日志类型定义
typedef enum eLogType              
{
    LOG_EVENT_DEBUG = 0,
    LOG_EVENT_WARNNING = 1,
    LOG_EVENT_ERROR = 2
}enumLogType;

//模块类型定义
typedef enum eModuleType              
{
    OTHER = 0,
    TASK = 1,
    MYSQL = 2,
}enumModuleType;
//日志输出类型
typedef enum eLogOutPut
{
	LOG_OUTPUT_UNDEFINE = 0,	//未定义输出类型
	LOG_OUTPUT_SCREEN	= 1,	//输出到屏幕
	LOG_OUTPUT_FILE		= 2,	//输出到文件
	LOG_OUTPUT_BOTH		= 3		//输出到屏幕和文件
}enumLogOutPut;

const float EPSINON = 0.00001f;

//#define MAX_VIDEO_DSPS          24  //通道数
//#define NUM_RECORD_DSPS         20  //录像通道数
//#define NUM_EVENTS              9   //线程使用的控制事件数量

//关键代码同步类，用于本程序内部的代码同步
class SynClass
{
private:
	bool     m_Init;
	CRITICAL_SECTION  m_Cs;
public:
	SynClass();
	~SynClass();
	void Lock();//进入	
	void UnLock();//退出
};

///<summary>
///     生成日志类
///<summary>
class CAppLog
{
public:
    CAppLog(void);
    virtual ~CAppLog(void);  
//
private:
    bool mOutputToFile;
    eLogType mLogType;  //日志类型
	enumLogOutPut mLogOutPut;
	enumLogOutPut mOldLogOutPut; //从配置文件初始化的类型
    void GetLogPath(eModuleType Module,char* strLogPath,char* strModule); //通过给定的得到路径规则取得相应模块日志文件路径
//对外接口
public:

	eLogType GetLogType(void){return mLogType;};						//得到日志的类型
	bool SetLogType(eLogType LogType){mLogType=LogType;return true;};   //设置日志的类型
	void ManualSetOutType(enumLogOutPut LogOutPut){mLogOutPut = LogOutPut;};	//手动改变日志输出方式
	void CloseManualSet(){mLogOutPut = mOldLogOutPut;};							//关闭手动改变的输出方式，还原为默认

    void SetOpenKey(bool IsOpen);    
   // bool WriteToLog(eModuleType Module,eLogType LogType,char* EventLog); //按一定的格式写日志
    bool WriteToLog(eModuleType Module,eLogType LogType,string& EventLog); 
	bool WriteLog(eModuleType Module,eLogType LogType,std::string& EventLog,enumLogOutPut LogOutPut = LOG_OUTPUT_UNDEFINE);

	
    static CAppLog* GetInstance();
	static CAppLog* _Instance; 
    static SynClass* _Syn;
};



#endif