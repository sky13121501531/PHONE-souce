#include "Clint.h"
#include <fstream>
#include <string>
#include "FileCreat.h"
#include "ACE_func.h"
#include "./sql/Ado.h"
#include "./sql/SqlServerDBConnection.h"
#include<vector>
#include "time.h"
#include "./log/PAppLog.h"
#include "SetTime.h"
#include "TimeEvents.h"

int main(int argc, char *argv[])
{
	TimeEvents* time = new TimeEvents();
	ACE_FUNC* Frw = new ACE_FUNC();

	//创建日志文件夹及文件
	CFileLogs filelog;
	filelog.OpenFile();
	filelog.CreatFilebox();

	//程序启动 操作一次数据库  之后每20s操作一次
	if(Frw->Conn_Readsql())
	{
		//文件保存到磁盘
		Frw->ParserDiscXML();
	}
	
	//启动定时器  数据处理
	time->open();
	
	//发送数据
	Frw->open();

	//回收线程资源
	ACE_Thread_Manager::instance()->wait();

	delete time;
	delete Frw;

	return 0;
}
