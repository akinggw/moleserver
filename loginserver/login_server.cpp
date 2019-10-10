#include <stdio.h>
#include <string>

#include "../include/molnet/Network.h"

#include "DBOperator.h"
#include "GameFrameManager.h"

#define LOGINSERVER_CONFIG "./configs/server_config.ini"

using namespace network;

int main()
{
    char strTmp[1024];
   	NetMessage myMes;

	new GameFrameManager();
	new DBOperator();

	InitMolNet("login_server");

	std::string serverip = GetIniSectionItem(LOGINSERVER_CONFIG,"LoginServer","ipaddress");
	int serverport = atoi(GetIniSectionItem(LOGINSERVER_CONFIG,"LoginServer","port").c_str());

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

	std::string dbip = GetIniSectionItem(LOGINSERVER_CONFIG,"database","ipaddress");
	int dbport = atoi(GetIniSectionItem(LOGINSERVER_CONFIG,"database","port").c_str());
	std::string username = GetIniSectionItem(LOGINSERVER_CONFIG,"database","username");
	std::string userpwd = GetIniSectionItem(LOGINSERVER_CONFIG,"database","userpwd");
	std::string dbname = GetIniSectionItem(LOGINSERVER_CONFIG,"database","dbname");

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
