
#include "FileCreat.h"


FileRW::FileRW()
{

}

string FileRW::CreatFile(char* PathFileName)
{
	ifstream pfile;
	pfile.open(PathFileName,ios::in);
	string str = "";
	
	getline(pfile, str);
	return str;
}

string FileRW::ReadFile(string PathFileName)
{
	const char *filepath = PathFileName.c_str();
	char stringbuf[3*1024];
	string str;
	FILE *file = fopen(filepath,"r");
	if(!file)
		return NULL;
	else
	{
		while(!feof(file))
		{
			if(fgets(stringbuf,sizeof(stringbuf),file)==NULL)
			{
				printf("��ȡ�ַ���ʧ�ܣ�����\n");
			}
			else
			{
				printf("��ȡ�ַ����ɹ�������\n");
				str = stringbuf;
				break;
			}
		}
	}
	fclose(file);
	return str;
}
