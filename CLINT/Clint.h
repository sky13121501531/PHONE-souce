#ifndef __CLIENT_H_
#define __CLIENT_H_

#include "ace/Task.h"
#include "ace/INET_Addr.h"
#include "ace/Synch.h"

namespace ACE_Client
{
	class Client: public ACE_Task<ACE_MT_SYNCH>
	{
	public:
		Client(char *server,char *Port);
		virtual int svc(void);
		char *put_task(char *msg_s, size_t msg_len);
		void open(void);

	private:
		ACE_INET_Addr addr;
		bool FFlag;
		ACE_Thread_Mutex Mutex;
	};
}

#endif
