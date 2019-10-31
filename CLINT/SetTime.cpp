#include "SetTime.h"
#include "ACE_func.h"
#include "Constants.h"
#include "TimeEvents.h"
#include "./log/PAppLog.h"
My_Timer_Handler::My_Timer_Handler(const int delay,const int interval)
{
	num = 0;
	count_num = 0;
	this->reactor(ACE_Reactor::instance());
	this->time_handle_ = this->reactor()->schedule_timer(this,//在这里注册定时器
		0,
		ACE_Time_Value(delay),//程序一开始延迟delay秒开始首次执行到期函数
		ACE_Time_Value(interval));//循环计时，每隔interval秒重复执行
}

My_Timer_Handler::My_Timer_Handler()
{
	
}

My_Timer_Handler::~My_Timer_Handler()
{
}
int My_Timer_Handler::handle_timeout(const ACE_Time_Value& , const void *act /* = 0 */)
{
	if(num > 0)
	{
		string strmsg = "---   准备取消定时器";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
		num = 0;
		timeBegin = time(0);
		ACE_Reactor::instance()->cancel_timer(this->time_handle_);
		stop_event_loop = false;
	}
	if(num == 0 && count_num >= 3)//每隔20s访问一次数据库，并把数据库数据保存在磁盘
	{
		num++;
		count_num = 0;
		string strmsg = "---   连接数据库 -- 相关数保存至磁盘";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
		if(Frw.Conn_Readsql())
		{
			//文件保存到磁盘
			Frw.ParserDiscXML();
		}
	}
	time_t m_time = time(0);
	if(m_time - timeBegin >= 5)
	{
		count_num++;//数据库连接条件
		ACE_FUNC Fec;
		Sleep(50);
		string strstr ="生成Json字符串 : \n";
		
		string str = Fec.WriteXml();
		strstr += str;
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strstr,LOG_OUTPUT_BOTH);
	}
	return 0;
}
void My_Timer_Handler::start()
{
	while(true)
	{
		if(!stop_event_loop)
		{
			string strmsg = "---   定时器退出";
			CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
			stop_event_loop = true;
			break;
		}
		ACE_Reactor::instance()->handle_events();
	}
}