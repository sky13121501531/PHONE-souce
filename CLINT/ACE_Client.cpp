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

	//������־�ļ��м��ļ�
	CFileLogs filelog;
	filelog.OpenFile();
	filelog.CreatFilebox();

	//�������� ����һ�����ݿ�  ֮��ÿ20s����һ��
	if(Frw->Conn_Readsql())
	{
		//�ļ����浽����
		Frw->ParserDiscXML();
	}
	
	//������ʱ��  ���ݴ���
	time->open();
	
	//��������
	Frw->open();

	//�����߳���Դ
	ACE_Thread_Manager::instance()->wait();

	delete time;
	delete Frw;

	return 0;
}
