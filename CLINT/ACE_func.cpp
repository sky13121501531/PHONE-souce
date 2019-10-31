#pragma warning(disable:4996)
#pragma warning(disable:4700)
#include "Constants.h"
#include "Clint.h"
#include "ACE_func.h"
#include <string>
#include <iostream>
#include "time.h"
#include "jsonParser.h"
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include "SetTime.h"
#include "./log/PAppLog.h"
#include <WinSock2.h>
#include <shlwapi.h>
#include "windows.h"
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib,"shlwapi.lib")
using namespace std;
using namespace ACE_Client;

string strCityName;
string strServerIP;
string strPort;
string strjsonvalue;
ACE_FUNC::ACE_FUNC(void)
{
	CmpFlag = false;
	ftpFirst = false;
}
ACE_FUNC::~ACE_FUNC()
{

}
int ACE_FUNC::svc(void)
{
	ACE_DEBUG((LM_DEBUG, ACE_TEXT("[ %T | %t ] ---- %s\n"),"发送线程开始启动。。。"));
	FileRW File;
	while(true)
	{
		//Client client((char*)strServerIP.c_str(),(char*)strPort.c_str()); //服务器的IP地址或者服务器名称	
		//加载磁盘文件
		//string str = File.CreatFile((char*)strCityName.c_str());
		//str += " = = = = = = = = = = = = = = = = = = = = = = ";
		//
		//size_t task1_len = str.size(); //第1个task的数据长度
		////char *task1_t = NULL; //无需修改
		////ACE_NEW_RETURN(task1_t, char[str.size()], -1); //无需修改
		//client.put_task((char *)str.c_str(), str.size()); //无需修改
		//client.open(); //将上面的task全部发到服务器
		//int cmp = strcmp(strOld.c_str(),str.c_str());
		Sleep(1000);
		if(!ftpFirst)/*程序启动，一直对比数据，直到有结果---有无更新数据*/
		{
			FtpSend(strCityName,GetFilename(strCityName));
		}

		if(!strcmp(strOld.c_str(),strjsonvalue.c_str()))
		{
			//printf("站点数据未更新！！！！\n");
			string strmsg = "站点数据未更新【0】！！！！" ;
			CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_SCREEN);
		}
		else
		{
			CmpFlag = true;
			strOld = strjsonvalue;
			string strmsg = "站点更新数据【0】："  + strjsonvalue;
			CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_SCREEN);
		}
		if(CmpFlag&&strjsonvalue!="")
		{
			CmpFlag = false;
			FtpSend(strCityName,GetFilename(strCityName));
		}
	}
	return 0;
}

void ACE_FUNC::open(void)
{
	this->activate();
}

bool ACE_FUNC::ReadXmlfromTable(const char* xml)
{
	//先清理容器
	if(R_New_v.size()>0)
	{
		R_New_v.erase(R_New_v.begin(),R_New_v.end());
	}
	//解析xml 对比ServiceID 获取节目name
	bool bLoadFileFlag_New = Parser_xml_New.LoadFromFile(xml);
	string strFreq,strServiceID,strServiceName,strdesc;
	//解析xml
	if (bLoadFileFlag_New)
	{
		pXMLNODE rootNode=Parser_xml_New.GetNodeFromPath("Msg");
		pXMLNODELIST pRealRecordList = Parser_xml_New.GetNodeList(rootNode);
		for (int i=0;i<pRealRecordList->Size();++i)
		{
			pXMLNODE pRecordNode = Parser_xml_New.GetNextNode(pRealRecordList);
			pXMLNODELIST pRecordList=Parser_xml_New.GetNodeList(pRecordNode);
			for(int i=0;i<pRecordList->Size();++i)
			{
				pXMLNODE pRealNode=Parser_xml_New.GetNextNode(pRecordList);
				pXMLNODELIST pRealList=Parser_xml_New.GetNodeList(pRealNode);
				for(int i=0;i<pRealList->Size();++i)
				{
					/*pXMLNODE pChildNode=Parser_xml_New.GetNextNode(pRealList);	

					pXMLNODELIST pChannelScanList=Parser_xml_New.GetNodeList(pChildNode);
					for(int i=0;i<pChannelScanList->Size();++i)*/
					{
						pXMLNODE ppChildNode=Parser_xml_New.GetNextNode(pRealList);
						Parser_xml_New.GetAttrNode(ppChildNode,"ProgramID",strServiceID);
						Parser_xml_New.GetAttrNode(ppChildNode,"Program",strServiceName);

						R_New_v.insert(make_pair(strServiceID,strServiceName));
					}	
				}
				if(pRealList != NULL)
				{	
					delete pRealList;
					pRealList = NULL;
				}
			}
			if(pRecordList != NULL)
			{	
				delete pRecordList;
				pRecordList = NULL;
			}
		}
		if(pRealRecordList != NULL)
		{	
			delete pRealRecordList;
			pRealRecordList = NULL;
		}
		return true;
	}
	return false;

}

bool ACE_FUNC::ReadXmlfromFile(const char* xml)
{
	if(pair.size()>0)
	{
		pair.erase(pair.begin(),pair.end());
	}
	if(vFreq.size()>0)
	{
		vFreq.erase(vFreq.begin(),vFreq.end());
	}
	bool bLoadFileFlag_new = Parser_xml_new.LoadFromFile(xml);
	string strFreq,strServiceID,strChassisID,strPosition,strModuleID,strTranscode;
	if (bLoadFileFlag_new)
	{
		pXMLNODE rootNode=Parser_xml_new.GetNodeFromPath("Msg");
		pXMLNODELIST pRealRecordList = Parser_xml_new.GetNodeList(rootNode);
		for (int i=0;i<pRealRecordList->Size();i++)
		{
			pXMLNODE pRecordNode = Parser_xml_new.GetNextNode(pRealRecordList);
			pXMLNODELIST pRecordList=Parser_xml_new.GetNodeList(pRecordNode);
			for (int i=0;i<pRecordList->Size();++i)
			{
				pXMLNODE pRealNode=Parser_xml_new.GetNextNode(pRecordList);
				pXMLNODELIST pRealList=Parser_xml_new.GetNodeList(pRealNode);
				for (int i=0;i<pRealList->Size();++i)
				{
					pXMLNODE pFreqInfoNode=Parser_xml_new.GetNextNode(pRealList);
					Parser_xml_new.GetAttrNode(pFreqInfoNode,"Freq",strFreq);//找到频点

					vFreq.insert(strFreq);
					//vFreq.sort(vFreq.begin(),vFreq.end(),mysort);
					pXMLNODELIST pFreqInfoList=Parser_xml_new.GetNodeList(pFreqInfoNode);
					for (int i=0;i<pFreqInfoList->Size();++i)
					{
						pXMLNODE pChildNode=Parser_xml_new.GetNextNode(pFreqInfoList);
						Parser_xml_new.GetAttrNode(pChildNode,"ServiceID",strServiceID);
						pair1.strFreq = strFreq;
						if(strServiceID=="")
						{
							strServiceID = strFreq;//如果节目号为空，频点号给节目号
						}
						//R_new_v.insert(make_pair(strServiceID,strFreq));

						pXMLNODELIST pChildInfoList=Parser_xml_new.GetNodeList(pChildNode);

						for (int i=0;i<pChildInfoList->Size();++i)
						{
							pXMLNODE ppChildNode=Parser_xml_new.GetNextNode(pChildInfoList);
							pXMLNODELIST ppChildInfoList=Parser_xml_new.GetNodeList(ppChildNode);

							for (int i=0;i<pChildInfoList->Size();++i)
							{
								pXMLNODE pppChildNode=Parser_xml_new.GetNextNode(ppChildInfoList);

								Parser_xml_new.GetAttrNode(pppChildNode,"ChassisID",strChassisID);
								Parser_xml_new.GetAttrNode(pppChildNode,"Position",strPosition);
								Parser_xml_new.GetAttrNode(pppChildNode,"ModuleID",strModuleID);
								Parser_xml_new.GetAttrNode(pppChildNode,"Transcode",strTranscode);
								string Transcodeid = strChassisID + "_" + strPosition + "_" + strModuleID + "_" + strTranscode;
								pair1.TranscodeID = Transcodeid;
								pair.insert(make_pair(strServiceID,pair1));
								//Tcoder_new.insert(make_pair(strServiceID,Transcodeid));//节目号 和 转码通道	
							}
							if(ppChildInfoList!=NULL)
							{
								delete ppChildInfoList;
								ppChildInfoList = NULL;
							}
						}
						if(pChildInfoList!=NULL)
						{
							delete pChildInfoList;
							pChildInfoList = NULL;
						}
					}
					if(pFreqInfoList!=NULL)
					{
						delete pFreqInfoList;
						pFreqInfoList = NULL;
					}
				}
				if(pRealList!=NULL)
				{
					delete pRealList;
					pRealList = NULL;
				}
			}
			if(pRecordList!=NULL)
			{
				delete pRecordList;
				pRecordList = NULL;
			}
		}
		if(pRealRecordList!=NULL)
		{
			delete pRealRecordList;
			pRealRecordList = NULL;
		}
		return true;//解读成功
	}
	return false;
}

string ACE_FUNC::WriteXml()
{

	//获取配置文件信息
	this->GetConfigInfo();

	char * source = "<RealRecord></RealRecord>";
	XmlParser retParser(source);
	pXMLNODE RootNode = retParser.GetNodeFromPath("RealRecord");
	pXMLNODE TypeNode;
	if(ReadXmlfromTable(this->strCTTB.c_str()) && ReadXmlfromFile(this->strcttb.c_str()))
	{
		TypeNode = retParser.CreateNodePtr(RootNode,"DVBType");
		retParser.SetAttrNode("DVBType",string("DTMB"),TypeNode);
		//遍历匹配ServiceID

		pXMLNODE ServiceInfoNode;
		pXMLNODE ChannelinfoNode;
		pXMLNODE InfoNode;

		set<string>::iterator vIter = vFreq.begin();
		for(;vIter!=vFreq.end();++vIter)//遍历频点
		{
			string fFreq = *vIter;
			ServiceInfoNode = retParser.CreateNodePtr(TypeNode,"ServiceInfo");
			ChannelinfoNode = retParser.CreateNodePtr(ServiceInfoNode,"ChannelInfo");
			pair_iter = pair.begin();
			while(pair_iter != pair.end())
			{
				string strFreqvalue = ((*pair_iter).second).strFreq;
				string strproid = (*pair_iter).first;
				string TranscodeID = ((*pair_iter).second).TranscodeID;	
				if(strFreqvalue == fFreq)
				{
					for(New_iter = R_New_v.begin();New_iter != R_New_v.end();++New_iter)
					{
						string strProID = (*New_iter).first;
						string strproName = (*New_iter).second;
						//节目号
						if(strProID == strproid)
						{		
							InfoNode = retParser.CreateNodePtr(ChannelinfoNode,"Info");								
							retParser.SetAttrNode("Freq",strFreqvalue,ServiceInfoNode); //频点	
							//retParser.SetAttrNode("ServiceID",strProID,InfoNode);//节目号
							retParser.SetAttrNode("ServiceName",strproName,InfoNode); //节目名字
							string strPlayaddr = strIP + TranscodeID;
							retParser.SetAttrNode("PlayerAddr",strPlayaddr,InfoNode); //通道号
							break;
						}	
					}
					//break;
				}
				pair_iter++;
			}

		}
	}
	if(ReadXmlfromTable(this->strDVBC.c_str()) && ReadXmlfromFile(this->strdvbc.c_str()))
	{
		TypeNode = retParser.CreateNodePtr(RootNode,"DVBType");
		retParser.SetAttrNode("DVBType",string("DVBC"),TypeNode);
		//遍历匹配ServiceID

		pXMLNODE ServiceInfoNode;
		pXMLNODE ChannelinfoNode;
		pXMLNODE InfoNode;
		
		set<string>::iterator vIter = vFreq.begin();
		for(;vIter!=vFreq.end();++vIter)//遍历频点
		{
			string fFreq = *vIter;
			ServiceInfoNode = retParser.CreateNodePtr(TypeNode,"ServiceInfo");
			ChannelinfoNode = retParser.CreateNodePtr(ServiceInfoNode,"ChannelInfo");
			pair_iter = pair.begin();
			while(pair_iter != pair.end())
			{
				string strFreqvalue = ((*pair_iter).second).strFreq;
				string strproid = (*pair_iter).first;
				string TranscodeID = ((*pair_iter).second).TranscodeID;	
				if(strFreqvalue == fFreq)
				{
					for(New_iter = R_New_v.begin();New_iter != R_New_v.end();++New_iter)
					{
						string strProID = (*New_iter).first;
						string strproName = (*New_iter).second;
						//节目号
						if(strProID == strproid)
						{		
							InfoNode = retParser.CreateNodePtr(ChannelinfoNode,"Info");								
							retParser.SetAttrNode("Freq",strFreqvalue,ServiceInfoNode); //频点	
							//retParser.SetAttrNode("ServiceID",strProID,InfoNode);//节目号
							//string ProgramName = strProID + strproName;
							retParser.SetAttrNode("ServiceName",strproName,InfoNode); //节目名字
							string strPlayaddr = strIP + TranscodeID;
							retParser.SetAttrNode("PlayerAddr",strPlayaddr,InfoNode); //通道号
							break;
						}	
					}
					//break;
				}
				pair_iter++;
			}
		}
	}
	if(ReadXmlfromFile(this->strRadio.c_str()))
	{
		TypeNode = retParser.CreateNodePtr(RootNode,"DVBType");
		retParser.SetAttrNode("DVBType",string("FM"),TypeNode);
		//遍历匹配ServiceID

		pXMLNODE ServiceInfoNode;
		pXMLNODE ChannelinfoNode;
		pXMLNODE InfoNode;

		set<string>::iterator vIter = vFreq.begin();
		for(;vIter!=vFreq.end();++vIter)//遍历频点
		{
			string fFreq = *vIter;
			ServiceInfoNode = retParser.CreateNodePtr(TypeNode,"ServiceInfo");
			ChannelinfoNode = retParser.CreateNodePtr(ServiceInfoNode,"ChannelInfo");
			pair_iter = pair.begin();
			while(pair_iter != pair.end())
			{
				string strFreqvalue = ((*pair_iter).second).strFreq;
				string strproid = (*pair_iter).first;
				string TranscodeID = ((*pair_iter).second).TranscodeID;	
				if(strFreqvalue == fFreq)
				{
					//for(New_iter = R_New_v.begin();New_iter != R_New_v.end();++New_iter)
					{
						string strProID = fFreq;
						string strproName = fFreq;
						//节目号
						if(strProID == strproid)
						{		
							InfoNode = retParser.CreateNodePtr(ChannelinfoNode,"Info");								
							retParser.SetAttrNode("Freq",strFreqvalue,ServiceInfoNode); //频点	
							//retParser.SetAttrNode("ServiceID",strProID,InfoNode);//节目号
							retParser.SetAttrNode("ServiceName",strproName,InfoNode); //节目名字
							string strPlayaddr = strIP + TranscodeID;
							retParser.SetAttrNode("PlayerAddr",strPlayaddr,InfoNode); //通道号
							break;
						}	
					}
					//break;
				}
				pair_iter++;
			}

		}
	}
	if(ReadXmlfromFile(this->stram.c_str()))
	{
		TypeNode = retParser.CreateNodePtr(RootNode,"DVBType");
		retParser.SetAttrNode("DVBType",string("AM"),TypeNode);
		//遍历匹配ServiceID

		pXMLNODE ServiceInfoNode;
		pXMLNODE ChannelinfoNode;
		pXMLNODE InfoNode;

		set<string>::iterator vIter = vFreq.begin();
		for(;vIter!=vFreq.end();++vIter)//遍历频点
		{
			string fFreq = *vIter;
			ServiceInfoNode = retParser.CreateNodePtr(TypeNode,"ServiceInfo");
			ChannelinfoNode = retParser.CreateNodePtr(ServiceInfoNode,"ChannelInfo");
			pair_iter = pair.begin();
			while(pair_iter != pair.end())
			{
				string strFreqvalue = ((*pair_iter).second).strFreq;
				string strproid = (*pair_iter).first;
				string TranscodeID = ((*pair_iter).second).TranscodeID;	
				if(strFreqvalue == fFreq)
				{
					//for(New_iter = R_New_v.begin();New_iter != R_New_v.end();++New_iter)
					{
						string strProID = fFreq;
						string strproName = fFreq;
						//节目号
						if(strProID == strproid)
						{		
							InfoNode = retParser.CreateNodePtr(ChannelinfoNode,"Info");								
							retParser.SetAttrNode("Freq",strFreqvalue,ServiceInfoNode); //频点	
							//retParser.SetAttrNode("ServiceID",strProID,InfoNode);//节目号
							retParser.SetAttrNode("ServiceName",strproName,InfoNode); //节目名字
							string strPlayaddr = strIP + TranscodeID;
							retParser.SetAttrNode("PlayerAddr",strPlayaddr,InfoNode); //通道号
							break;
						}	
					}
					//break;
				}
				pair_iter++;
			}

		}
	}
	if(ReadXmlfromFile(this->strdvbs.c_str()))
	{
		TypeNode = retParser.CreateNodePtr(RootNode,"DVBType");
		retParser.SetAttrNode("DVBType",string("DVBS"),TypeNode);
		//遍历匹配ServiceID

		pXMLNODE ServiceInfoNode;
		pXMLNODE ChannelinfoNode;
		pXMLNODE InfoNode;

		//set<string>::iterator vIter = vFreq.begin();
		//for(;vIter!=vFreq.end();++vIter)//遍历频点
		{
			//string fFreq = *vIter;
			ServiceInfoNode = retParser.CreateNodePtr(TypeNode,"ServiceInfo");
			ChannelinfoNode = retParser.CreateNodePtr(ServiceInfoNode,"ChannelInfo");
			pair_iter = pair.begin();
			for(;pair_iter != pair.end();++pair_iter)
			{
				string strFreqvalue = ((*pair_iter).second).strFreq;
				string strproid = (*pair_iter).first;
				string TranscodeID = ((*pair_iter).second).TranscodeID;	
//				if(strFreqvalue == fFreq)
				{
					//for(New_iter = R_New_v.begin();New_iter != R_New_v.end();++New_iter)
					{
						//string strProID = (*New_iter).first;
						string strproName = strproid;
						//节目号
						//if(strProID == strproid)
						{		
							InfoNode = retParser.CreateNodePtr(ChannelinfoNode,"Info");								
							retParser.SetAttrNode("Freq",strFreqvalue,ServiceInfoNode); //频点	
							//retParser.SetAttrNode("ServiceID",strProID,InfoNode);//节目号
							//string ProgramName = strProID + strproName;
							retParser.SetAttrNode("ServiceName",strproName,InfoNode); //节目名字
							string strPlayaddr = strIP + TranscodeID;
							retParser.SetAttrNode("PlayerAddr",strPlayaddr,InfoNode); //通道号
							continue;
						}	
					}
					//break;
				}
			}
		}
	}
	Downdisk = "c:\\ProgramList\\";
	if (!PathIsDirectory(Downdisk.c_str()))
	{
		CreateDirectory(Downdisk.c_str(), NULL);
	}

	string xml = ".xml";
	string station_xml = Downdisk+strStationName + xml;
	if(retParser.SaveAsFile(station_xml.c_str()))//保存到文件
	{
		if(ParserXml(station_xml.c_str()))
		{
			strjsonvalue = Xml2Json();//xml转成JSON
			Save2File(strjsonvalue,Downdisk);//保存到文件

			//CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strjsonvalue,LOG_OUTPUT_BOTH);
		}
	}	
	return strjsonvalue;
}

void ACE_FUNC::GetConfigInfo()
{
	//获取配置文件信息
	//PropConfig config;
	//config.LoadPropFile("ClientInfo.xml");

	//strIP = config.GetNodeAttribute("PlayAddr-Transcoder","info","PlayAddr");
	//strCTTB = config.GetNodeAttribute("ChannelScanQuery","info","cttb-path");
	//strDVBC = config.GetNodeAttribute("ChannelScanQuery1","info","dvbc-path");
	//strRADIO = config.GetNodeAttribute("ChannelScanQuery2","info","radio-path");
	//strDVBS = config.GetNodeAttribute("ChannelScanQuery4","info","dvbs-path");
	//strAM = config.GetNodeAttribute("ChannelScanQuery3","info","am-path");
	//strcttb = config.GetNodeAttribute("RealRecordSet","info","cttb-path");
	//strdvbc = config.GetNodeAttribute("RealRecordSet1","info","dvbc-path");
	//strRadio = config.GetNodeAttribute("RealRecordSet2","info","radio-path");
	//strdvbs = config.GetNodeAttribute("RealRecordSet4","info","dvbs-path");
	//stram = config.GetNodeAttribute("RealRecordSet3","info","am-path");
	//strStationName = config.GetNodeAttribute("Station-name","info","Station");
	//strCityName = config.GetNodeAttribute("LoadDiskJson","info","Station-path");

	//string strIP,strCTTB,strDVBC,strRADIO,strDVBS,strAM,strcttb,strdvbc,strRadio,strdvbs,stram,strStationName;
	XmlParser FXML;
	bool sLoadFileFlag = FXML.LoadFromFile("ClientInfo.xml");
	if(sLoadFileFlag)
	{
		pXMLNODE rootNode=FXML.GetNodeFromPath("clientinfo");
		pXMLNODELIST ptypeList = FXML.GetNodeList(rootNode);
		//信号类型
		for (int i=0;i<ptypeList->Size();++i)
		{
			pXMLNODE pStationnameNode = FXML.GetNextNode(ptypeList);
			FXML.GetAttrNode(pStationnameNode,"Station",strStationName);
			FXML.GetAttrNode(pStationnameNode,"PlayAddr",strIP);
			FXML.GetAttrNode(pStationnameNode,"Stationpath",strCityName);

			FXML.GetAttrNode(pStationnameNode,"CTTBpath",strCTTB);
			FXML.GetAttrNode(pStationnameNode,"DVBCpath",strDVBC);
			FXML.GetAttrNode(pStationnameNode,"RADIOpath",strRADIO);
			FXML.GetAttrNode(pStationnameNode,"DVBSpath",strDVBS);
			FXML.GetAttrNode(pStationnameNode,"AMpath",strAM);

			FXML.GetAttrNode(pStationnameNode,"cttbpath",strcttb);
			FXML.GetAttrNode(pStationnameNode,"dvbcpath",strdvbc);
			FXML.GetAttrNode(pStationnameNode,"radiopath",strRadio);
			FXML.GetAttrNode(pStationnameNode,"dvbspath",strdvbs);
			FXML.GetAttrNode(pStationnameNode,"ampath",stram);
			
		}
		if(ptypeList!=NULL)
		{
			delete ptypeList;
			ptypeList=NULL;
		}
	}
	
	//strServerIP = config.GetNodeAttribute("ServerAddr","info","ServerIP");
	//strPort = config.GetNodeAttribute("ServerAddr","info","Port");
	/*if(strcttb!="")
	{
		string strmsg = "------ 地面数字信号配置成功";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
	}
	if(strdvbc!="")
	{
		string strmsg = "------ 有线数字信号配置成功";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
	}
	if(strRadio!="")
	{
		string strmsg = "------ 调频广播信号配置成功";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
	}
	if(strdvbs!="")
	{
		string strmsg = "------ 卫星信号配置成功";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
	}
	if(stram!="")
	{
		string strmsg = "------ 中波信号配置成功";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
	}*/
}

string ACE_FUNC::Xml2Json()
{
	cJSON* array_DTMB_date = cJSON_CreateArray();
	cJSON* array_DVBC_date = cJSON_CreateArray();
	cJSON* array_RADIO_date = cJSON_CreateArray();
	cJSON* array_AM_date = cJSON_CreateArray();
	cJSON* array_DVBS_date = cJSON_CreateArray();
	
	cJSON *array_DTMB_name = NULL;
	cJSON *array_DVBC_name = NULL;
	cJSON *array_RADIO_name = NULL;
	cJSON *array_AM_name = NULL;
	cJSON *array_DVBS_name = NULL;

	if (array_DTMB_date == NULL || array_DVBC_date == NULL ||array_RADIO_date == NULL ||array_AM_date == NULL ||array_DVBS_date == NULL)
	{
		//创建失败
		return FALSE;
	}
	//****************************************** 节点
	for(pair2_iter = pair_addr.begin();pair2_iter!=pair_addr.end();++pair2_iter)
	{
		if(pair2_iter->first =="DTMB" || pair2_iter->first =="CTTB")
		{
			array_DTMB_name = cJSON_CreateObject();	
			if (array_DTMB_name == NULL)
			{
				//创建失败
				return FALSE;
			}
			cJSON_AddItemToArray(array_DTMB_date, array_DTMB_name);
			cJSON_AddStringToObject(array_DTMB_name, "name",pair2_iter->second.ProgramName.c_str());
			cJSON_AddStringToObject(array_DTMB_name, "url", pair2_iter->second.PlayAddr.c_str());
		}
		if(pair2_iter->first =="DVBC")
		{
			array_DVBC_name = cJSON_CreateObject();
			if (array_DVBC_name == NULL)
			{
				//创建失败
				return FALSE;
			}
			cJSON_AddItemToArray(array_DVBC_date, array_DVBC_name);
			cJSON_AddStringToObject(array_DVBC_name, "name",pair2_iter->second.ProgramName.c_str());
			cJSON_AddStringToObject(array_DVBC_name, "url", pair2_iter->second.PlayAddr.c_str());
		}
		if(pair2_iter->first =="FM" || pair2_iter->first =="RADIO")
		{
			array_RADIO_name = cJSON_CreateObject();
			if (array_RADIO_name == NULL)
			{
				//创建失败
				return FALSE;
			}
			cJSON_AddItemToArray(array_RADIO_date, array_RADIO_name);
			cJSON_AddStringToObject(array_RADIO_name, "name",pair2_iter->second.ProgramName.c_str());
			cJSON_AddStringToObject(array_RADIO_name, "url", pair2_iter->second.PlayAddr.c_str());
		}
		if(pair2_iter->first =="AM")
		{
			array_AM_name = cJSON_CreateObject();
			if (array_AM_name == NULL)
			{
				//创建失败
				return FALSE;
			}
			cJSON_AddItemToArray(array_AM_date, array_AM_name);
			cJSON_AddStringToObject(array_AM_name, "name",pair2_iter->second.ProgramName.c_str());
			cJSON_AddStringToObject(array_AM_name, "url", pair2_iter->second.PlayAddr.c_str());
		}
		if(pair2_iter->first =="DVBS")
		{
			array_DVBS_name = cJSON_CreateObject();
			if (array_DVBS_name == NULL)
			{
				//创建失败
				return FALSE;
			}
			cJSON_AddItemToArray(array_DVBS_date, array_DVBS_name);
			cJSON_AddStringToObject(array_DVBS_name, "name",pair2_iter->second.ProgramName.c_str());
			cJSON_AddStringToObject(array_DVBS_name, "url", pair2_iter->second.PlayAddr.c_str());
		}		
	}

	cJSON * array_DTMB_z = cJSON_CreateArray();
	cJSON * temp_dtmb = NULL;
	//****************************************** 信号--CTTB
	if(array_DTMB_name!=NULL)
	{
		temp_dtmb = cJSON_CreateObject();
		if (array_DTMB_z == NULL || temp_dtmb == NULL)
		{
			//创建失败
			return FALSE;
		}
		cJSON_AddItemToArray(array_DTMB_z, temp_dtmb);
		cJSON_AddStringToObject(temp_dtmb, "type", "DTMB");
		cJSON_AddItemToObject(temp_dtmb, "data", array_DTMB_date);
	}
	cJSON *temp_dvbc = NULL;
	if(array_DVBC_name!=NULL)
	{
		//****************************************** 信号--DVBC
		temp_dvbc = cJSON_CreateObject();
		if (temp_dvbc == NULL || array_DTMB_z == NULL)
		{
			//创建失败
			return FALSE;
		}
		cJSON_AddItemToArray(array_DTMB_z, temp_dvbc);
		cJSON_AddStringToObject(temp_dvbc, "type", "DVBC");
		cJSON_AddItemToObject(temp_dvbc, "data", array_DVBC_date);
	}
	cJSON *temp_Radio = NULL;
	if(array_RADIO_name!=NULL)
	{
		//****************************************** 信号--RADIO
		temp_Radio = cJSON_CreateObject();
		if (temp_Radio == NULL || array_DTMB_z == NULL)
		{
			//创建失败
			return FALSE;
		}
		cJSON_AddItemToArray(array_DTMB_z, temp_Radio);
		cJSON_AddStringToObject(temp_Radio, "type", "FM");
		cJSON_AddItemToObject(temp_Radio, "data", array_RADIO_date);
	}
	cJSON *temp_am = NULL;
	if(array_AM_name!=NULL)
	{
		//****************************************** 信号--AM
		temp_am = cJSON_CreateObject();
		if (temp_am == NULL || array_DTMB_z == NULL)
		{
			//创建失败
			return FALSE;
		}
		cJSON_AddItemToArray(array_DTMB_z,temp_am);
		cJSON_AddStringToObject(temp_am, "type", "AM");
		cJSON_AddItemToObject(temp_am, "data", array_AM_date);
	}
	cJSON *temp_dvbs = NULL;
	if(array_DVBS_name!=NULL)
	{
		//****************************************** 信号--dvbs
		temp_dvbs = cJSON_CreateObject();
		if (temp_dvbs == NULL || array_DTMB_z == NULL)
		{
			//创建失败
			return FALSE;
		}
		cJSON_AddItemToArray(array_DTMB_z, temp_dvbs);
		cJSON_AddStringToObject(temp_dvbs, "type", "DVBS");
		cJSON_AddItemToObject(temp_dvbs, "data", array_DVBS_date);
	}
	
	//****************************************** 创建根节点
	cJSON * station1 = cJSON_CreateObject();	
	cJSON * arraystation = cJSON_CreateArray();
	if (arraystation == NULL || station1 == NULL)
	{
		//创建失败
		return FALSE;
	}

	cJSON_AddItemToArray(arraystation, station1);
	cJSON_AddStringToObject(station1, "station", strStationName.c_str());
	cJSON_AddItemToObject(station1, "data", array_DTMB_z);

	string strson = cJSON_Print2String(station1);
	string strJSON = string("[") + strson + string("]");

	if(arraystation!=NULL)
	{
		delete arraystation;
		arraystation = NULL;
	}
	
	if(station1!=NULL)
	{
		cJSON_Delete(station1);
	}
	return strJSON;
}

bool ACE_FUNC::ParserXml(const string& strxml)
{
	if(pair_addr.size()>0)
	{
		pair_addr.erase(pair_addr.begin(),pair_addr.end());
	}
	bool bLoadFileFlag = Parser_xml.LoadFromFile(strxml.c_str());
	string strType,strServiceName,strPlayAddr;
	//解析xml
	if (bLoadFileFlag)
	{
		pXMLNODE rootNode=Parser_xml.GetNodeFromPath("RealRecord");
		pXMLNODELIST ptypeList = Parser_xml.GetNodeList(rootNode);
		for (int i=0;i<ptypeList->Size();++i)
		{
			pXMLNODE pServiceInfoNode = Parser_xml.GetNextNode(ptypeList);
			//信号类型
			Parser_xml.GetAttrNode(pServiceInfoNode,"DVBType",strType);

			pXMLNODELIST pChannelInfoList=Parser_xml.GetNodeList(pServiceInfoNode);
			for(int i=0;i<pChannelInfoList->Size();++i)
			{
				pXMLNODE pinfoNode=Parser_xml.GetNextNode(pChannelInfoList);
				pXMLNODELIST pinfoList=Parser_xml.GetNodeList(pinfoNode);
				for(int i=0;i<pinfoList->Size();++i)
				{
					pXMLNODE pChildNode=Parser_xml.GetNextNode(pinfoList);	
					pXMLNODELIST pChannelScanList=Parser_xml.GetNodeList(pChildNode);
					for(int i=0;i<pChannelScanList->Size();++i)
					{			
						pXMLNODE ppChildNode=Parser_xml.GetNextNode(pChannelScanList);
						Parser_xml.GetAttrNode(ppChildNode,"ServiceName",strServiceName);
						pair2.ProgramName = strServiceName;
						Parser_xml.GetAttrNode(ppChildNode,"PlayerAddr",strPlayAddr);
						pair2.PlayAddr = strPlayAddr;

						pair_addr.insert(make_pair(strType,pair2));
					}
					if(pChannelScanList!=NULL)
					{
						delete pChannelScanList;
						pChannelScanList=NULL;
					}
				}
				if(pinfoList!=NULL)
				{
					delete pinfoList;
					pinfoList=NULL;
				}
			}
			if(pChannelInfoList!=NULL)
			{
				delete pChannelInfoList;
				pChannelInfoList=NULL;
			}
		}
		if(ptypeList!=NULL)
		{
			delete ptypeList;
			ptypeList=NULL;
		}
		return true;
	}
	return false;
}

void ACE_FUNC::Save2File(const string& strjson,string strDownDisk)
{
	ofstream File; 
	string Fname = ".json";
	string Filename = strDownDisk + strStationName + Fname;
	//Filename = strStationName + Fname;
	File.open(Filename.c_str(),ios::out); 
	File<<strjson<<endl; 
	File.close();
}

bool ACE_FUNC::Conn_Readsql()
{
	if(mysql.QueryTask(vecXML))//数据库查询成功   取出对应表中数据
	{
		//cout<<"------ 数据库操作取值成功"<<endl;
		string strmsg = "------ 数据库操作取值成功";
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
		string strSqlXml;
		//vector<string>::iterator pIter = vecXML.begin();
		int num = 0;
		if(ParserSqlXML(vecXML))
		{
			string strmsg ="------ 数据库文件解析成功";
			CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);

			return true;
		}
		/*for(;pIter!=vecXML.end();++pIter)
		{
			strSqlXml = *pIter;
			if(ParserSqlXML(strSqlXml))
			{
				num++;
				cout<<"第 [ "<<num<<" ]数据库xml解析成功"<<endl;
			}
		}*/
		return false;
	}
	string strmsg = "------ 数据库操作失败";
	CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
	return false;
}

bool ACE_FUNC::ParserSqlXML(vector<string>& strxml)
{
	for(vector<string>::iterator pitr = strxml.begin();pitr!=strxml.end();++pitr)
	{
		XmlParser vXML;
		string Vxml = *pitr;
		bool sLoadFileFlag = vXML.Set_xml(Vxml);
		if(sLoadFileFlag)
		{
			string strType;
			pXMLNODE rootNode=vXML.GetNodeFromPath("Msg");
			pXMLNODELIST ptypeList = vXML.GetNodeList(rootNode);
			//信号类型
			vXML.GetAttrNode(rootNode,"DVBType",strType);
			for (int i=0;i<ptypeList->Size();++i)
			{
				pXMLNODE pServiceInfoNode = vXML.GetNextNode(ptypeList);

				pXMLNODELIST pChannelInfoList=vXML.GetNodeList(pServiceInfoNode);
				for(int i=0;i<pChannelInfoList->Size();++i)
				{
					string vFreq,vServiceID,vCChassisID,vCPosition,vCModuleID,vCTranscode;
					pXMLNODE pinfoNode=vXML.GetNextNode(pChannelInfoList);
					vXML.GetAttrNode(pinfoNode,"Freq",vFreq);
					Typeinfo.strFreq = vFreq;
					vXML.GetAttrNode(pinfoNode,"ServiceID",vServiceID);
					Typeinfo.strServiceID = vServiceID;
					vXML.GetAttrNode(pinfoNode,"CChassisID",vCChassisID);
					Typeinfo.strChassisID = vCChassisID;
					vXML.GetAttrNode(pinfoNode,"CPosition",vCPosition);
					Typeinfo.strPosition = vCPosition;
					vXML.GetAttrNode(pinfoNode,"CModuleID",vCModuleID);
					Typeinfo.strModuleID = vCModuleID;
					vXML.GetAttrNode(pinfoNode,"CTranscode",vCTranscode);
					Typeinfo.strTranscode = vCTranscode;
					pair3_Typeinfo.insert(make_pair(strType,Typeinfo));
				}
				if(pChannelInfoList!=NULL)
				{
					delete pChannelInfoList;
					pChannelInfoList=NULL;
				}
			}
			if(ptypeList!=NULL)
			{
				delete ptypeList;
				ptypeList=NULL;
			}
		}
	}
	return true;
}

void ACE_FUNC::ParserDiscXML()
{
	/*int nCTTB = pair3_Typeinfo.count("CTTB");
	int nDVBC = pair3_Typeinfo.count("DVBC");
	int nRADIO = pair3_Typeinfo.count("RADIO");
	int nAM = pair3_Typeinfo.count("AM");
	int nDVBS = pair3_Typeinfo.count("DVBS");*/
	
	this->GetConfigInfo();
	string strDTMB,strDVBC,strRADIO,strAM,strDVBS;
	XmlParser xmlDTMB,xmlDVBC,xmlRADIO,xmlAM,xmlDVBS;
	pXMLNODE TypeNodeDTMB;
	pXMLNODE SetNodeDTMB;

	pXMLNODE TypeNodeDVBC;
	pXMLNODE SetNodeDVBC;

	pXMLNODE TypeNodeRADIO;
	pXMLNODE SetNodeRADIO;

	pXMLNODE TypeNodeAM;
	pXMLNODE SetNodeAM;

	pXMLNODE TypeNodeDVBS;
	pXMLNODE SetNodeDVBS;

	char * source = "<?xml version=\"1.0\" encoding=\"GB2312\" standalone=\"yes\"?><Msg><Return></Return></Msg>";
	if(pair3_Typeinfo.count("CTTB")>0)
	{
		xmlDTMB.Set_xml(source);
		TypeNodeDTMB = xmlDTMB.GetNodeFromPath("Msg");
		pXMLNODE ReturnNode = xmlDTMB.GetNodeFromPath("Msg/Return");
		pXMLNODE ReNode = xmlDTMB.CreateNodePtr(TypeNodeDTMB,"ReturnInfo");
		SetNodeDTMB = xmlDTMB.CreateNodePtr(ReNode,"SetAutoRecordChannel");	
	}
	if(pair3_Typeinfo.count("DVBC")>0)
	{
		xmlDVBC.Set_xml(source);
		TypeNodeDVBC = xmlDVBC.GetNodeFromPath("Msg");
		pXMLNODE ReturnNode = xmlDVBC.GetNodeFromPath("Msg/Return");
		pXMLNODE ReNode = xmlDVBC.CreateNodePtr(TypeNodeDVBC,"ReturnInfo");
		SetNodeDVBC = xmlDVBC.CreateNodePtr(ReNode,"SetAutoRecordChannel");	
	}
	if(pair3_Typeinfo.count("RADIO")>0)
	{
		xmlRADIO.Set_xml(source);
		TypeNodeRADIO = xmlRADIO.GetNodeFromPath("Msg");
		pXMLNODE ReturnNode = xmlRADIO.GetNodeFromPath("Msg/Return");
		pXMLNODE ReNode = xmlRADIO.CreateNodePtr(TypeNodeRADIO,"ReturnInfo");
		SetNodeRADIO = xmlRADIO.CreateNodePtr(ReNode,"SetAutoRecordChannel");	
	}
	if(pair3_Typeinfo.count("AM")>0)
	{
		xmlAM.Set_xml(source);
		TypeNodeAM = xmlAM.GetNodeFromPath("Msg");
		pXMLNODE ReturnNode = xmlAM.GetNodeFromPath("Msg/Return");
		pXMLNODE ReNode = xmlAM.CreateNodePtr(TypeNodeAM,"ReturnInfo");
		SetNodeAM = xmlAM.CreateNodePtr(ReNode,"SetAutoRecordChannel");	
	}
	if(pair3_Typeinfo.count("DVBS")>0)
	{
		xmlDVBS.Set_xml(source);
		TypeNodeDVBS = xmlDVBS.GetNodeFromPath("Msg");
		pXMLNODE ReturnNode = xmlDVBS.GetNodeFromPath("Msg/Return");
		pXMLNODE ReNode = xmlDVBS.CreateNodePtr(TypeNodeDVBS,"ReturnInfo");
		SetNodeDVBS = xmlDVBS.CreateNodePtr(ReNode,"SetAutoRecordChannel");	
	}

	for(pair3_iter = pair3_Typeinfo.begin();pair3_iter != pair3_Typeinfo.end();++pair3_iter)
	{	
		if(pair3_iter->first == "CTTB")
		{
			strDTMB = "DTMB";
			xmlDTMB.SetAttrNode("Type",strDTMB,TypeNodeDTMB);
			pXMLNODE FreqInfoNode = xmlDTMB.CreateNodePtr(SetNodeDTMB,"FreqInfo");
			xmlDTMB.SetAttrNode("Freq",pair3_iter->second.strFreq,FreqInfoNode);
			pXMLNODE ChannelInfoNode = xmlDTMB.CreateNodePtr(FreqInfoNode,"ChannelInfo");
			xmlDTMB.SetAttrNode("ServiceID",pair3_iter->second.strServiceID,ChannelInfoNode);	
			pXMLNODE TranscodeNode = xmlDTMB.CreateNodePtr(ChannelInfoNode,"TranscodeInfo");	
			pXMLNODE InfoNode = xmlDTMB.CreateNodePtr(TranscodeNode,"Info");
			xmlDTMB.SetAttrNode("ChassisID",pair3_iter->second.strChassisID,InfoNode);
			xmlDTMB.SetAttrNode("Position",pair3_iter->second.strPosition,InfoNode);
			xmlDTMB.SetAttrNode("ModuleID",pair3_iter->second.strModuleID,InfoNode);
			xmlDTMB.SetAttrNode("Transcode",pair3_iter->second.strTranscode,InfoNode);	

			continue;
		}
		if(pair3_iter->first == "DVBC")
		{
			strDVBC = "DVBC";
			xmlDVBC.SetAttrNode("Type",strDVBC,TypeNodeDVBC);
			
			pXMLNODE FreqInfoNode = xmlDVBC.CreateNodePtr(SetNodeDVBC,"FreqInfo");
			xmlDVBC.SetAttrNode("Freq",pair3_iter->second.strFreq,FreqInfoNode);
			pXMLNODE ChannelInfoNode = xmlDVBC.CreateNodePtr(FreqInfoNode,"ChannelInfo");
			xmlDVBC.SetAttrNode("ServiceID",pair3_iter->second.strServiceID,ChannelInfoNode);	
			pXMLNODE TranscodeNode = xmlDVBC.CreateNodePtr(ChannelInfoNode,"TranscodeInfo");	
			pXMLNODE InfoNode = xmlDVBC.CreateNodePtr(TranscodeNode,"Info");
			xmlDVBC.SetAttrNode("ChassisID",pair3_iter->second.strChassisID,InfoNode);
			xmlDVBC.SetAttrNode("Position",pair3_iter->second.strPosition,InfoNode);
			xmlDVBC.SetAttrNode("ModuleID",pair3_iter->second.strModuleID,InfoNode);
			xmlDVBC.SetAttrNode("Transcode",pair3_iter->second.strTranscode,InfoNode);
			
			continue;
		}
		if(pair3_iter->first == "RADIO")
		{
			strRADIO = "RADIO";
			xmlRADIO.SetAttrNode("Type",strRADIO,TypeNodeRADIO);

			pXMLNODE FreqInfoNode = xmlRADIO.CreateNodePtr(SetNodeRADIO,"FreqInfo");
			xmlRADIO.SetAttrNode("Freq",pair3_iter->second.strFreq,FreqInfoNode);
			pXMLNODE ChannelInfoNode = xmlRADIO.CreateNodePtr(FreqInfoNode,"ChannelInfo");
			xmlRADIO.SetAttrNode("ServiceID",pair3_iter->second.strServiceID,ChannelInfoNode);	
			pXMLNODE TranscodeNode = xmlRADIO.CreateNodePtr(ChannelInfoNode,"TranscodeInfo");	
			pXMLNODE InfoNode = xmlRADIO.CreateNodePtr(TranscodeNode,"Info");
			xmlRADIO.SetAttrNode("ChassisID",pair3_iter->second.strChassisID,InfoNode);
			xmlRADIO.SetAttrNode("Position",pair3_iter->second.strPosition,InfoNode);
			xmlRADIO.SetAttrNode("ModuleID",pair3_iter->second.strModuleID,InfoNode);
			xmlRADIO.SetAttrNode("Transcode",pair3_iter->second.strTranscode,InfoNode);
			continue;
		}
		if(pair3_iter->first == "AM")
		{
		
			strAM = "AM";
			xmlAM.SetAttrNode("Type",strAM,TypeNodeAM);
			
			pXMLNODE FreqInfoNode = xmlAM.CreateNodePtr(SetNodeAM,"FreqInfo");
			xmlAM.SetAttrNode("Freq",pair3_iter->second.strFreq,FreqInfoNode);
			pXMLNODE ChannelInfoNode = xmlAM.CreateNodePtr(FreqInfoNode,"ChannelInfo");
			xmlAM.SetAttrNode("ServiceID",pair3_iter->second.strServiceID,ChannelInfoNode);	
			pXMLNODE TranscodeNode = xmlAM.CreateNodePtr(ChannelInfoNode,"TranscodeInfo");	
			pXMLNODE InfoNode = xmlAM.CreateNodePtr(TranscodeNode,"Info");
			xmlAM.SetAttrNode("ChassisID",pair3_iter->second.strChassisID,InfoNode);
			xmlAM.SetAttrNode("Position",pair3_iter->second.strPosition,InfoNode);
			xmlAM.SetAttrNode("ModuleID",pair3_iter->second.strModuleID,InfoNode);
			xmlAM.SetAttrNode("Transcode",pair3_iter->second.strTranscode,InfoNode);
			
			continue;
		}
		if(pair3_iter->first == "DVBS")
		{
			strDVBS = "DVBS";
			xmlDVBS.SetAttrNode("Type",strDVBS,TypeNodeDVBS);
			
			pXMLNODE FreqInfoNode = xmlDVBS.CreateNodePtr(SetNodeDVBS,"FreqInfo");
			xmlDVBS.SetAttrNode("Freq",pair3_iter->second.strFreq,FreqInfoNode);
			pXMLNODE ChannelInfoNode = xmlDVBS.CreateNodePtr(FreqInfoNode,"ChannelInfo");
			xmlDVBS.SetAttrNode("ServiceID",pair3_iter->second.strServiceID,ChannelInfoNode);	
			pXMLNODE TranscodeNode = xmlDVBS.CreateNodePtr(ChannelInfoNode,"TranscodeInfo");	
			pXMLNODE InfoNode = xmlDVBS.CreateNodePtr(TranscodeNode,"Info");
			xmlDVBS.SetAttrNode("ChassisID",pair3_iter->second.strChassisID,InfoNode);
			xmlDVBS.SetAttrNode("Position",pair3_iter->second.strPosition,InfoNode);
			xmlDVBS.SetAttrNode("ModuleID",pair3_iter->second.strModuleID,InfoNode);
			xmlDVBS.SetAttrNode("Transcode",pair3_iter->second.strTranscode,InfoNode);
		
			continue;	
		}
	}
	if(strDTMB =="DTMB")
	{
		//保存到文件
		xmlDTMB.SaveAsFile(strcttb.c_str());
	}
	if(strDVBC == "DVBC")
	{
		//保存到文件
		xmlDVBC.SaveAsFile(strdvbc.c_str());
	}
	if(strRADIO == "RADIO")
	{
		//保存到文件
		xmlRADIO.SaveAsFile(strRadio.c_str());
	}
	if(strAM == "AM")
	{
		//保存到文件
		xmlAM.SaveAsFile(stram.c_str());
	}
	if(strDVBS == "DVBS")
	{
		//保存到文件
		xmlDVBS.SaveAsFile(strdvbs.c_str());
	}
}

//int ACE_FUNC::SendInfo()
//{
//	Client client(ServerIP); //服务器的IP地址或者服务器名称
//	ACE_Message_Block *mb = NULL;
//	string str;
//	while(true)
//	{
//		if(this->getq(mb)!=NULL &&mb!=NULL)
//		{
//			char buf[1024*100] = {0};
//			memcpy(buf,mb->rd_ptr(),mb->length());//xml拷贝至内存
//			str = buf;
//		}
//
//		size_t task1_len = str.size(); //第1个task的数据长度
//		char *task1_t = NULL; //无需修改
//		ACE_NEW_RETURN(task1_t, char[str.size()], -1); //无需修改
//		client.put_task(task1_t, (char *)str.c_str(), str.size()+4); //无需修改
//
//		string strBB = "= = = = = = = = = = = = = = = = = = = = = = "; //第2个task的数据
//		size_t task2_len = strBB.size(); //第2个task的数据长度
//		char *task2_t = NULL; //无需修改
//		ACE_NEW_RETURN(task2_t, char[strBB.size()], -1); //无需修改
//		client.put_task(task2_t, (char *)strBB.c_str(), strBB.size()+4); //无需修改
//
//		client.send_tasks(); //将上面的task全部发到服务器
//
//		/*delete task1_t;
//		delete task2_t;*/
//
//		Sleep(1000);
//	}
//	return 0;
//}

bool ACE_FUNC::FtpSend(string &strjson,string &Filename)
{
	BOOL dRes,pRes;
	HINTERNET hInternet;
	HINTERNET hConnect;
	HINTERNET hFind;
	WIN32_FIND_DATA fd;
	string strFname = "\\shanxijiance\\" + Filename;
	string strFile = "./" + Filename;
	hInternet = InternetOpen("A3GS Sample", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_NO_CACHE_WRITE);
	if ( NULL == hInternet )
	{ 
		printf("InternetOpen Error===[0]:%d\n", GetLastError() );
	} 
	hConnect  = InternetConnect(hInternet, "121.198.17.71"/*FTP服务器地址*/,
		INTERNET_DEFAULT_FTP_PORT/*FTP端口号，此为默认值---21*/, 
		"hmw149071"/*用户名*/, 
		"jinshi123"/*密码*/,
		INTERNET_SERVICE_FTP, INTERNET_FLAG_EXISTING_CONNECT || INTERNET_FLAG_PASSIVE,0 );

	/*连接成功-----做逻辑*/
	if(hConnect!=NULL)
	{
		string strmsg = "连接成功【1】！！！！" ;
		CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_SCREEN);
		//查找目标文件
		hFind = FtpFindFirstFile(hConnect, strFname.c_str(), &fd, INTERNET_FLAG_RELOAD, 0);
		if(hFind!=NULL)
		{
			//找到目标，下载
			string strmsg = "找到目标文件【1】！！！！" ;
			CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_SCREEN);
			dRes = FtpGetFile(hConnect, strFname.c_str(), strFile.c_str(), false,FILE_ATTRIBUTE_ARCHIVE, FTP_TRANSFER_TYPE_UNKNOWN, 0);
			if (!dRes)
			{
				char buf[16];
				string strRrror = itoa(GetLastError(),buf,10);
				string strmsg = "下载目标文件失败,FtpGetFile Error:"  + strRrror;
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_SCREEN);
			}
			else
			{
				string strmsg = "下载目标文件成功【1】！！！！" ;
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_SCREEN);

				//读取文件到String串
				string strSerbuf = fFile.ReadFile(strFile);
				string strStabuf = fFile.ReadFile(strCityName);

				//做对比
				if(!strcmp(strStabuf.c_str(),strSerbuf.c_str()))
				{
					//无更新，不传
					ftpFirst = true;
					string strmsg = "------ 无更新，站点数据【1】：\n" + strSerbuf;
					CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				}
				else
				{
					//有更新，上传
					pRes = FtpPutFile(hConnect,strjson.c_str(),strFname.c_str(),FTP_TRANSFER_TYPE_ASCII,0);
					if(!pRes)
					{
						string strmsg = "------ 上传文件失败【1】";
						CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
						ftpFirst = false;
					}
					else
					{
						string strmsg = "------ 上传文件成功 【1】：\n" + strStabuf;
						CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
						ftpFirst = true;
					}
				}
				bool FDEL = DeleteFile(strFile.c_str());/*文件内容读取到内存后，删除，保证本地不存在该文件*/
				if(FDEL)
				{
					printf("删除文件--%s成功\n",Filename.c_str());
				}
				else
				{
					printf("删除文件--DeleteFile fail(%d)\n", GetLastError());
				}
			}
		}
		else
		{
			string strFoundmsg = "目标文件不存在【1】！！！！" ;
			CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strFoundmsg,LOG_OUTPUT_SCREEN);

			pRes = FtpPutFile(hConnect,strjson.c_str(),strFname.c_str(),FTP_TRANSFER_TYPE_ASCII,0);

			if(pRes==0)
			{
				string strmsg = "------ 上传文件失败【2】";
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				ftpFirst = false;
			}
			else
			{
				string strStabuf = fFile.ReadFile(strCityName);
				string strmsg = "------ 上传文件成功 【2】：\n" + strStabuf;
				CAppLog().WriteLog(TASK,LOG_EVENT_DEBUG,strmsg,LOG_OUTPUT_BOTH);
				ftpFirst = true;
			}
		}
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		if(pRes)//上传
			return true;
		else
			return false;
	}
	else
	{
		printf( "连接失败，InternetConnect Error===【1】:%d\n", GetLastError() );
		InternetCloseHandle(hInternet);
		InternetCloseHandle(hConnect);
		return false;
	}
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	InternetCloseHandle(hFind);
	if(dRes&&pRes)//下载 上传
	{
		return true;
	}
	else
		return false;

}
string ACE_FUNC::GetFilename(string &strname)
{
		int fpos = strname.rfind("\\");
		string str = strname.substr(++fpos);
		return str;
}


