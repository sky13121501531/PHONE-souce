#ifndef __NETUSE_H_
#define __NETUSE_H_

#include "ace/Task.h"
#include "ace/INET_Addr.h"
#include "ace/Synch.h"

class UpLoad: public ACE_Task<ACE_MT_SYNCH>
{
public:
    UpLoad();
    ~UpLoad();
    virtual int svc(void);
    void start(void);
    void open(void);
private:
    ACE_INET_Addr addr;
};
#endif