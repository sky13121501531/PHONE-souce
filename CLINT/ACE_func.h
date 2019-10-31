#ifndef __ACE_FUNC_H_
#define __ACE_FUNC_H_
#include "./sql/Ado.h"
#include "./sql/SqlServerDBConnection.h"
#include <iostream>
#include "XmlParser.h"
#include "./File/PropConfig.h"
#include <vector>
#include <string>
#include <map>
#include <set>
#include "ace/Task.h"
#include "ace/INET_Addr.h"
#include "ace/Synch.h"
#include "FileCreat.h"
using namespace std;


class ACE_FUNC : public ACE_Task<ACE_MT_SYNCH>
{
public:
	ACE_FUNC();
    ~ACE_FUNC();
	virtual int svc();
	void open();
	bool ReadXmlfromTable(const char* xml);
	bool ReadXmlfromFile(const char* xml);
	string WriteXml(void);
	void GetConfigInfo();

	string Xml2Json(void);

	bool ParserXml(const string& strxml);
	void Save2File(const string& strjson,string strDownDisk);

	//���ݿ����
	bool Conn_Readsql();
	//�������ݿ�xml
	bool ParserSqlXML(vector<string>& strxml);

	void ParserDiscXML();

	bool FtpSend(string &strjson,string &Filename);
    string GetFilename(string &strname);
public:
	map<string,string> R_New_v;//��Ŀ��  ��  ��Ŀ����
	map<string,string> R_new_v;//��Ŀ��  ��  Ƶ��
	map<string,string> Tcoder_new;//��Ŀ�� �� ͨ����

	XmlParser Parser_xml,Parser_xml_new,Parser_xml_New;

	map<string,string>::iterator new_iter;
	map<string,string>::iterator New_iter;

	map<string,string> RealRecordList;

	struct _pair1
	{
		string TranscodeID;
		string strFreq;
	}pair1;

	map<string,_pair1> pair;
	map<string,_pair1>::iterator pair_iter;
	set<string> vFreq;

	struct _pair2
	{
		string PlayAddr;
		string ProgramName;
	}pair2;
	multimap<string,_pair2> pair_addr;
	multimap<string,_pair2>::iterator pair2_iter;
	string strStationName;//վ������

	string SendAddr;
	
	string Downdisk;//�ĵ�����

	//���ݿ����
	MySql mysql;
	vector<string> vecXML;
	
	struct _pair3
	{
		string strServiceID;
		string strFreq;
		string strChassisID;
		string strPosition;
		string strModuleID;
		string strTranscode;
	}Typeinfo;

	string strDVBType;
	multimap<string,_pair3> pair3_Typeinfo;
	multimap<string,_pair3>::iterator pair3_iter;

	//�����ļ���Ϣ
	string strCTTB,strcttb,strDVBC,strdvbc,strRadio,strRADIO,
		strDVBS,strdvbs,strAM,stram,strIP,strStation,strDstURL;
	
	//������Ϣ
	//int SendInfo();
	string strOld;
	bool CmpFlag;
	bool ftpFirst;
	string Filename;
	FileRW fFile;
};

#endif