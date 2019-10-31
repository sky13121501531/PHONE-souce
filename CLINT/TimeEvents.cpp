
#include "Constants.h"
#include "TimeEvents.h"
#include <string>
#include "SetTime.h"
using namespace std;
TimeEvents::TimeEvents()
{
	tFlag = false;
}

int TimeEvents::svc(void)
{
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("[ %T | %t ]---- %s\n"), "定时器开始启动。。。"));
	while(!tFlag)
	{
		//定时器
		My_Timer_Handler myTimer(5,10);
		myTimer.start();
		Sleep(1000);
	}
	return 0;
}

void TimeEvents::open(void)
{
	this->activate();
	//ACE_Thread_Manager::instance()->wait();
}

TimeEvents::~TimeEvents()
{

}
