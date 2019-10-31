//#include "stdafx.h"
#include "jsonParser.h"
#include <stdlib.h>
#define new new(_NORMAL_BLOCK,__FILE__,__LINE__)

JsonParser::JsonParser(const string& jsonStr)
	:mRoot(NULL), mJsonStr("")
{
	mJsonStr = jsonStr;
}

//JsonParser::JsonParser(const string& jsonStr, const StrVec& keys)
//{
//
//}

JsonParser::~JsonParser()
{
	ClearResult();
	if(mRoot != NULL)
	{
		cJSON_Delete(mRoot);
		mRoot = NULL;
	}
}

cJSON* JsonParser::GetVal(const string& key)
{
	cJSON* retVal = NULL;

	JsonParserResult::iterator findItr = mResult.find(key);
	if(findItr == mResult.end())	//缓存中没有找到结果
	{
		if(mRoot == NULL)
		{
			mRoot = cJSON_Parse(mJsonStr.c_str());
		}

		retVal = GetVal(mRoot, key);
		mResult.insert(std::pair<string,cJSON*>(key, retVal));
	}
	else
	{
		retVal = findItr->second;
	}

	return retVal;
}

cJSON* JsonParser::GetVal(cJSON* root, const string& key) const
{
	if(root == NULL)
	{
		return NULL;
	}

	string restKey;
	string tmpKey = GetFirstKey(key, restKey);

	cJSON* item = cJSON_GetObjectItem(root, tmpKey.c_str());
	if(item == NULL)
	{
		return NULL;
	}

	cJSON* retItem = NULL;

	switch(item->type)
	{
	case cJSON_String:
		{
			if(restKey.empty())
			{
				retItem = cJSON_DetachItemFromObject(root, tmpKey.c_str());
			}
			else
			{
				cJSON* subRoot = cJSON_Parse(item->valuestring);
				retItem = GetVal(subRoot, restKey);

				if(subRoot != NULL)
				{
					cJSON_Delete(subRoot);;
					subRoot = NULL;
				}
			}
			break;
		}
	case cJSON_Object:
		{
			if(restKey.empty())
			{
				retItem = cJSON_DetachItemFromObject(root, tmpKey.c_str());
			}
			else
			{
				retItem = GetVal(item, restKey);
			}
			break;
		}
	default:
		{
			if(restKey.empty())
			{
				retItem = cJSON_DetachItemFromObject(root, tmpKey.c_str());
			}
			break;
		}
	}
	
	return retItem;
}

string JsonParser::GetFirstKey(const string& key, string& restKey) const
{
	size_t pos = key.find_first_of(':');

	if(pos == string::npos)	//只有一个key
	{
		return key;
	}

	string firstKey = key.substr(0, pos);
	restKey = key.substr(pos+1);

	return firstKey;
}

void JsonParser::ClearResult()
{
	JsonParserResult::iterator itr = mResult.begin();
	for(; itr != mResult.end(); ++itr)
	{
		cJSON* item = itr->second;
		if(item != NULL)
		{
			cJSON_Delete(item);
		}
	}
}


string cJSON_Print2String(cJSON *root)
{
	char *pStr = NULL;
	string outString;
	pStr = cJSON_PrintUnformatted(root);
	if (pStr != NULL)
	{
		outString = pStr;
		free(pStr);
	}
	return outString;
}