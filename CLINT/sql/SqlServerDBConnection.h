#pragma once
#include "Ado.h"

#include <vector>
using namespace std;
class MySql
{
public:
	MySql(void);
	~MySql(void);
public:
	bool QueryTask(vector<string>& vecXml);//≤È—Ø»ŒŒÒ
};
