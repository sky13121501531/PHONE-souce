#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

#include "ace/Time_Value.h"
#include <string>
namespace ACE_Client
{
	static const size_t BUFFER_SIZE = 4096; //���ݻ�������С
	static const size_t SERVER_PORT_NUM = 11111; //��������ͨ�Ŷ˿ں�
	static const ACE_Time_Value TIME_INTERVAL(0, 1000000); //�������ݷ���֮���ʱ����(0 s + 1000000 us = 1 s)
}

#endif
