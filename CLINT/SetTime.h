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
	int handle_timeout(const ACE_Time_Value& , const void *act /* = 0 */);//��ʱ�����ں�ִ�еĻص�����
	
	void start();
	bool stop_event_loop;//�Ƿ���Ҫ��ֹ��ʱ������
	//int n_;//ѭ����ʱ�Ĵ���
	long time_handle_;//�ڼ�ʱ�������е�ID
	time_t timeBegin;//ʱ����
	int num;
	int count_num;//���ݿ����Ӽ��ʱ��
	ACE_FUNC Frw;
	
};