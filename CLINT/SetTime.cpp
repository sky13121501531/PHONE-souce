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
	this->time_handle_ = this->reactor()->schedule_timer(this,//������ע�ᶨʱ��
		0,
		ACE_Time_Value(delay),//����һ��ʼ�ӳ�delay�뿪ʼ�״�ִ�е��ں���
		ACE_Time_Value(interval));//ѭ����ʱ��ÿ��interval���ظ�ִ��
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
		string strmsg = "---   ׼��ȡ����ʱ��";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
		num = 0;
		timeBegin = time(0);
		ACE_Reactor::instance()->cancel_timer(this->time_handle_);
		stop_event_loop = false;
	}
	if(num == 0 && count_num >= 3)//ÿ��20s����һ�����ݿ⣬�������ݿ����ݱ����ڴ���
	{
		num++;
		count_num = 0;
		string strmsg = "---   �������ݿ� -- ���������������";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
		if(Frw.Conn_Readsql())
		{
			//�ļ����浽����
			Frw.ParserDiscXML();
		}
	}
	time_t m_time = time(0);
	if(m_time - timeBegin >= 5)
	{
		count_num++;//���ݿ���������
		ACE_FUNC Fec;
		Sleep(50);
		string strstr ="����Json�ַ��� : \n";
		
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
			string strmsg = "---   ��ʱ���˳�";
			CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
			stop_event_loop = true;
			break;
		}
		ACE_Reactor::instance()->handle_events();
	}
}