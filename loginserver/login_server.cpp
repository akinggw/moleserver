#include <stdio.h>
#include <string>

#include "../include/molnet/Network.h"

#include "DBOperator.h"
#include "GameFrameManager.h"

#define LOGINSERVER_CONFIG "/configs/server_config.ini"

using namespace network;

int main()
{
    char strTmp[1024];
	char m_curProgress_Path[256];
   	NetMessage myMes;
	
	getcwd(m_curProgress_Path,256);	

	new GameFrameManager();
	new DBOperator();

	InitMolNet("login_server");

	std::string loginserver_config_file = m_curProgress_Path;
	loginserver_config_file += LOGINSERVER_CONFIG;
	
	std::string serverip = GetIniSectionItem(loginserver_config_file.c_str(),"LoginServer","ipaddress");
	int serverport = atoi(GetIniSectionItem(loginserver_config_file.c_str(),"LoginServer","port").c_str());
	std::string dbip = GetIniSectionItem(loginserver_config_file.c_str(),"database","ipaddress");
	int dbport = atoi(GetIniSectionItem(loginserver_config_file.c_str(),"database","port").c_str());
	std::string username = GetIniSectionItem(loginserver_config_file.c_str(),"database","username");
	std::string userpwd = GetIniSectionItem(loginserver_config_file.c_str(),"database","userpwd");
	std::string dbname = GetIniSectionItem(loginserver_config_file.c_str(),"database","dbname");	

	serverip[serverip.length()-2] = '\0';

	if(!StartMolNet(serverip.c_str(),
                    serverport))
	{
	    sprintf(strTmp,"【系统】 服务器启动失败,IP地址:%s,端口:%d。",serverip.c_str(),serverport);
		LOG_BASIC(strTmp);

		CleanMolNet();
		return 1;
	}

	LOG_BASIC("登陆服务器:%s,端口:%d 启动成功。",serverip.c_str(),serverport);

    // start connect database
    if(!ServerDBOperator.Initilize(dbip,
                                   username,
                                   userpwd,
                                   dbname,
                                   dbport))
    {
	    sprintf(strTmp,"【系统】 数据库:%s %s %s %s 连接失败。",dbip.c_str(),username.c_str(),userpwd.c_str(),dbname.c_str());
		LOG_BASIC(strTmp);

		CleanMolNet();
		return 1;
    }

    LOG_BASIC("数据库:%s %s %s %s 连接成功。",dbip.c_str(),username.c_str(),userpwd.c_str(),dbname.c_str());

    LOG_BASIC("服务器启动成功，开始处理...");

	while(IsConnected())
	{
		GetNetMessage(myMes);

		for(int i=0;i<myMes.GetCount();i++)
		{
			MessageStru *mes = myMes.GetMesById(i);
			if(mes==NULL) continue;

			switch(mes->GetType())
			{
			case MES_TYPE_ON_CONNECTED:
				{
                    ServerGameFrameManager.OnProcessConnectedNetMes(mes->GetSocket());
				}
				break;
			case MES_TYPE_ON_DISCONNECTED:
				{
                    ServerGameFrameManager.OnProcessDisconnectNetMes(mes->GetSocket());
				}
				break;
			case MES_TYPE_ON_READ:
				{
                    ServerGameFrameManager.OnProcessNetMes(mes->GetSocket(),mes->GetMes());
				}
				break;
			default:
				break;
			}
		}

        ServerDBOperator.Update();

		usleep(1000);
	}

    ServerDBOperator.Shutdown();
    CleanMolNet();

	delete DBOperator::getSingletonPtr();
	delete GameFrameManager::getSingletonPtr();

    return 0;
}
