#include "ace/OS.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Connector.h"
#include "ace/Message_Block.h"
#include "ace/Thread_Manager.h"
#include "ace/INET_Addr.h"

#include "Constants.h"
#include "Clint.h"
#include <string>
#include "./log/PAppLog.h"
using namespace std;
namespace ACE_Client
{
	Client::Client(char *server,char *Port)
	{
		addr = ACE_INET_Addr(Port,server);
		FFlag = false;
	}
	int Client::svc(void)
	{
		ACE_SOCK_Stream stream;
		ACE_SOCK_Connector connector;
		if(connector.connect(stream,addr) == -1)
		{
			//string strmsg = "---- 连接服务器失败";
			//CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
			FFlag = true;
			return -1;
		}
		
		if(!FFlag)
		{
			ACE_Guard<ACE_Thread_Mutex> guard(Mutex);
			ACE_Message_Block *mb =0;
			ACE_Time_Value timev(5);
			if(getq(mb,&timev) == -1)
			{
				//string strmsg = "---- 无数据，发送失败";
				//CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				FFlag  = true;
				//break;
			}
			char pbuf[1024*3] ={0};
			memcpy(pbuf,mb->rd_ptr(),mb->length());
			ACE_DEBUG((LM_DEBUG, ACE_TEXT("[ %T | %t ] Sending \n%s\n"), pbuf));
			if(stream.send_n(mb->rd_ptr(), mb->length())>0 && !FFlag)
			{
				//string strmsg = "---- 发送成功";
				//CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
			}
			mb->release();
			ACE_OS::sleep(TIME_INTERVAL);
		}	
		
		stream.close();

		return 0;
	}

	char *Client::put_task(char *msg_s, size_t msg_len)
	{
		//memset(msg_t,0,msg_len);
		/*for(int i = 0; i < 4; i++)
		{
			msg_t[i] = (char)((msg_len >> (8 * i)) & 0xff);
		}*/
		char ibuf[1024*3]={0};
		ACE_OS::memcpy(ibuf, msg_s, msg_len);

		ACE_Message_Block *mb = NULL;
		ACE_NEW_RETURN(mb, ACE_Message_Block(msg_len,ACE_Message_Block::MB_DATA,0,ibuf),0);
		
		mb->wr_ptr(msg_len);
		this->putq(mb);

		ACE_OS::sleep(TIME_INTERVAL);
		return ibuf;
	}

	void Client::open(void)
	{
		this->activate();
	}
}
