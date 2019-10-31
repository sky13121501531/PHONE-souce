#include <iostream>
#include "ace/Log_Msg.h"
#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "time.h"
#include "ACE_func.h"
#include <list>
using namespace std;

class My_Timer_Handler:public ACE_Event_Handler
{
public:
	My_Timer_Handler();
	My_Timer_Handler(const int delay,const int interval);
	~My_Timer_Handler();
	int handle_timeout(const ACE_Time_Value& , const void *act /* = 0 */);//计时器到期后执行的回调函数
	
	void start();
	bool stop_event_loop;//是否需要终止计时器服务
	//int n_;//循环计时的次数
	long time_handle_;//在计时器队列中的ID
	time_t timeBegin;//时间间隔
	int num;
	int count_num;//数据库连接间隔时间
	ACE_FUNC Frw;
	
};