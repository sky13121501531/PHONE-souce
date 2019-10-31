#ifndef __TIMEEVENTS_H_
#define __TIMEEVENTS_H_

#include "ace/Task.h"
#include "ace/INET_Addr.h"
#include "ace/Synch.h"

class TimeEvents: public ACE_Task<ACE_MT_SYNCH>
{
public:
	TimeEvents();
    ~TimeEvents();
	virtual int svc(void);
	void open(void);
private:
	ACE_INET_Addr addr;
	bool tFlag;
};
#endif