#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <string>
//#include <vector>
#include <map>
#include "./json/cJSON.h"


using std::string;
//using std::vector;
using std::map;

//typedef vector<string>		StrVec;

class JsonParser
{

public:
	JsonParser(const string& jsonStr);
	//JsonParser(const string& jsonStr, const StrVec& keys);

	~JsonParser();
	/*
	**	key  获取值的key，例如key=a:b:c
	*/
	cJSON* GetVal(const string& key);

private:
	cJSON* GetVal(cJSON* root, const string& key) const;
	string GetFirstKey(const string& key, string& restKey) const;
	void ClearResult();
private:
	typedef map<string, cJSON*>	JsonParserResult;

	cJSON* mRoot;
	string mJsonStr;
	JsonParserResult	mResult;
	
};

//json转换为string,并释放内存
string cJSON_Print2String(cJSON *root);
#endif