#include "../../include/molnet/Network.h"

ListenSocket<NetClient> *m_ServerSocket = NULL;

namespace network
{

void InitMolNet(const char *logfile)
{
    new SocketMgr;
    new SocketGarbageCollector;

    ThreadPool.Startup();
    sLog.Init(-1,LOGON_LOG,logfile);

    m_ServerSocket = new ListenSocket<NetClient>();
	m_NetworkUpdate = new MolNetworkUpdate();
}

void CleanMolNet(void)
{
    if(m_ServerSocket == NULL) return;

    if(m_ServerSocket)
        m_ServerSocket->Close();

    ThreadPool.Shutdown();
    sLog.Close();

    sSocketMgr.CloseAll();
    sSocketMgr.ClearMesList();
	sSocketGarbageCollector.DeleteSocket();

    delete SocketMgr::getSingletonPtr();
    delete SocketGarbageCollector::getSingletonPtr();

    SafeDelete(m_ServerSocket);
    SafeDelete(m_NetworkUpdate);
}

bool StartMolNet(const char * ListenAddress, uint32 Port)
{
    if(!m_ServerSocket || !m_ServerSocket->Start(ListenAddress,Port))
        return false;

    sSocketMgr.SpawnWorkerThreads();

    if(m_ServerSocket->IsOpen())
    {
		ThreadPool.ExecuteTask(m_NetworkUpdate);
		return true;
    }

    return false;
}

bool IsConnected(void)
{
	if(m_ServerSocket == NULL) return false;

	return m_ServerSocket->IsOpen();
}

bool Disconnect(uint32 index)
{
	bool isOk = false;

	Socket *pSocket = NULL;
	sSocketMgr.LockSocketList();
	pSocket = sSocketMgr.GetSocket(index);
	if(pSocket &&
		pSocket->IsConnected())
	{
		pSocket->Disconnect();
		isOk = true;
	}
	sSocketMgr.UnlockSocketList();

	return isOk;
}

bool IsConnected(uint32 index)
{
	bool isOk = false;

	Socket *pSocket = NULL;
	sSocketMgr.LockSocketList();
	pSocket = sSocketMgr.GetSocket(index);
	if(pSocket)
		isOk = pSocket->IsConnected();
	sSocketMgr.UnlockSocketList();

	return isOk;
}

bool Send(uint32 index,CMolMessageOut &out)
{
	if(index <= 0 || out.getLength() <= 0) return false;

	bool isOk = false;

	Socket *pSocket = NULL;
	sSocketMgr.LockSocketList();
	pSocket = sSocketMgr.GetSocket(index);
	if(pSocket && pSocket->IsConnected())
		isOk = pSocket->Send(out);
	sSocketMgr.UnlockSocketList();

	return isOk;
}
	
bool Sendhtml5(uint32 index,const char* data,uint32 size)
{
	if(index <= 0 || data == NULL) return false;

	bool isOk = false;

	Socket *pSocket = NULL;
	sSocketMgr.LockSocketList();
	pSocket = sSocketMgr.GetSocket(index);
	if(pSocket && pSocket->IsConnected()) 
		isOk = pSocket->SendHtml5((const uint8*)data,size);
	sSocketMgr.UnlockSocketList();

	return isOk;
}

std::string GetIpAddress(uint32 index)
{
	std::string remoteIP;

	Socket *pSocket = NULL;
	sSocketMgr.LockSocketList();
	pSocket = sSocketMgr.GetSocket(index);
	if(pSocket && pSocket->IsConnected())
		remoteIP = pSocket->GetRemoteIP();
	sSocketMgr.UnlockSocketList();

	return remoteIP;
}

int GetNetMessage(NetMessage & mes)
{
	mes.Clear();

	if(sSocketMgr.GetMesCount() <= 0 ||
		mes.GetMaxCount() <= 0)
		return 0;

	int count = 0;

	// 如果当前系统中的消息个数小于我们要读取的个数时，读取全部的消息；
	// 否则读取我们设置的消息个数的消息
	if(sSocketMgr.GetMesCount() < mes.GetMaxCount())
	{
		if(sSocketMgr.LockMesList())
		{
			std::list<MessageStru> *meslist = sSocketMgr.GetMesList();
			if(meslist == NULL || meslist->empty())
			{
				sSocketMgr.UnlockMesList();
				return 0;
			}

			std::list<MessageStru>::iterator iter = meslist->begin();
			for(;iter != meslist->end();)
			{
				mes.AddMessage((*iter));
				iter = meslist->erase(iter);
				count+=1;
			}
			sSocketMgr.UnlockMesList();
		}
	}
	else
	{
		if(sSocketMgr.LockMesList())
		{
			std::list<MessageStru> *meslist = sSocketMgr.GetMesList();
			if(meslist == NULL || meslist->empty())
			{
				sSocketMgr.UnlockMesList();
				return 0;
			}

			std::list<MessageStru>::iterator iter = meslist->begin();
			for(int i=0;iter != meslist->end(),i<mes.GetMaxCount();i++)
			{
				if(iter == meslist->end()) break;

				mes.AddMessage((*iter));
				iter = meslist->erase(iter);
				count+=1;
			}

			sSocketMgr.UnlockMesList();
		}
	}

	return count;
}

void ExecuteTask(ThreadBase * ExecutionTarget)
{
	sSocketMgr.AddTask(ExecutionTarget);

	ThreadPool.ExecuteTask(ExecutionTarget);
}

void System_Log(std::string const &logstr)
{
    LOG_BASIC(logstr.c_str());
}

void System_Update(void)
{
	if(m_ServerSocket == NULL || !m_ServerSocket->IsOpen()) return;

   // m_ServerSocket->OnAccept();

    sSocketMgr.Update();

    sSocketGarbageCollector.Update();

    ThreadPool.IntegrityCheck();
}

std::string GetIniSectionItem(const char* FileName,const char* Section,const char* Item)
{
	std::string myvalue;
	char *value,Sect[30],c;
	char linebuf[1024],oldline[1024];
	FILE *inifp;
	int i;
	strcpy(Sect,"[");
	strcat(Sect,Section);
	strcat(Sect,"]");
	/*strcpy(Ite,"[");strcat(Ite,Item);strcat(Ite,"]");*/
	if((inifp=fopen(FileName,"rb"))==NULL)
	{
		return "nofile" ;
	}
	/*printf("Sect = %s,Item = %s /n",Sect,Item);*/
	while((c=fgetc(inifp))!=EOF)
	{
		if(c=='[')
		{
		ungetc(c,inifp);
		fgets(linebuf,1024,inifp);
		if(strstr(linebuf,Sect))
		{
			while((c=fgetc(inifp))!='[' && c!=EOF)
			{
				ungetc(c,inifp);
				fgets(linebuf,1024,inifp);
				if(strstr(linebuf,Item))
				{
					if(value=strchr(linebuf,'='))
					{
					value++;
					/*printf("value = %s /n",value);*/
					fclose(inifp);
					if(*value=='/n')
						return "";

					myvalue = value;
					myvalue[myvalue.length()-2]='\0';
					return myvalue;
					}
				}
			}
			if(c==EOF)
			{
			 break;
			}
			ungetc(c,inifp);
		}
		}
		else
		{
		ungetc(c,inifp);
		fgets(linebuf,1024,inifp);
		}
	}
	fclose(inifp);
	return "";
}

}
