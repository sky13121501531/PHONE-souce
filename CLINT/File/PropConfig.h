#ifndef _PROPCONFIG_H_
#define _PROPCONFIG_H_


#include <map>
#include <string>
#include <list>
#include "../XmlParser.h"


//�����ļ�������
class PropConfig
{
public:
	PropConfig(void);
	virtual~PropConfig(void);
	PropConfig(const char * PropFileName);
	bool LoadPropFile(const char* PropFileName );//���������ļ�
	string GetNodeText(const char * parNode,const char * chilNode);//��������ļ���ĳ�ڵ���ı�����ֵ
	string GetNodeAttribute(const char * parNode,const char * chilNode,const char * attribute);//��������ļ���ĳ�ڵ������ֵ
	bool SetNodeText(const char * parNode,const char * chilNode,const char * valu);//���������ļ���ĳ�ڵ���ı�����ֵ
	
	bool isInitiated(){return mInitiated;};//���������ļ��Ƿ�ɹ�

	pXMLNODE CreateNode(char* parNode,char* childNode);
	pXMLNODE CreateNode( char* parNode,char* childNode,string dvbtype,string freq,string alarmthreshold );
	
	bool DelNode(char* parNode,char* childNode);
	bool SetNodeAttribute(const char * parNode,const char * chilNode,const char * attribute,const char * valu);//���������ļ���ĳ�ڵ������ֵ
	bool SetNodeAttribute(pXMLNODE node,string attribute,string valu);
	bool SetNodeAttribute(string dvbtype,const char * parNode,const char * attribute,const char * valu);
	bool SetNodeAttribute(string dvbtype,string freq,const char * parNode,const char * attribute,const char * valu);

private:
	XmlParser mXmlProp;
	bool mInitiated;
};
#endif