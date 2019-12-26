#include <stdio.h>
#include <string>

#include "../include/molnet/Network.h"

#include "DBOperator.h"
#include "GameFrameManager.h"

#define LOGINSERVER_CONFIG "/configs/database.ini"

int m_accountserverid = 0;

using namespace network;

int main(int argc,char *argv[])
{
    char strTmp[1024];
	char m_curProgress_Path[256];
   	NetMessage myMes;

	if(argc < 2)
    {
        printf("使用方法:account_server 账号服务器配置ID\n");
        return 0;
    }

    m_accountserverid = atoi(argv[1]);
	if(getcwd(m_curProgress_Path,256) == NULL)
	{
        LOG_ERROR("得到当前目录失败.");
        return 0;
	}

	new GameFrameManager();
	new DBOperator();

    sprintf(strTmp,"account%d_server",m_accountserverid);
	InitMolNet(strTmp);

	std::string loginserver_config_file = m_curProgress_Path;
	loginserver_config_file += LOGINSERVER_CONFIG;

	std::string dbip = GetIniSectionItem(loginserver_config_file.c_str(),"database","ipaddress");
	int dbport = atoi(GetIniSectionItem(loginserver_config_file.c_str(),"database","port").c_str());
	std::string username = GetIniSectionItem(loginserver_config_file.c_str(),"database","username");
	std::string userpwd = GetIniSectionItem(loginserver_config_file.c_str(),"database","userpwd");
	std::string dbname = GetIniSectionItem(loginserver_config_file.c_str(),"database","dbname");

    // start connect database
    if(!ServerDBOperator.Initilize(dbip,
                                   username,
                                   userpwd,
                                   dbname,
                                   dbport))
    {
	    sprintf(strTmp,"【系统】 数据库:%s %s %s %s 连接失败。",dbip.c_str(),username.c_str(),userpwd.c_str(),dbname.c_str());
		LOG_ERROR(strTmp);

		CleanMolNet();
		return 1;
    }

    LOG_BASIC("数据库:%s %s %s %s 连接成功。",dbip.c_str(),username.c_str(),userpwd.c_str(),dbname.c_str());

    //得到登录服务器配置
    tagServerSet ptagServerSet;
    if(!ServerDBOperator.GetAccountServerConfig(m_accountserverid,ptagServerSet))
    {
        LOG_ERROR("数据库配置导入失败，请重新配置。");
        return 1;
    }

	if(!StartMolNet("0.0.0.0"/*serverip.c_str()*/,
                    ptagServerSet.serverport))
	{
	    sprintf(strTmp,"【系统】 服务器启动失败,IP地址:%s,端口:%d。",ptagServerSet.serverip,ptagServerSet.serverport);
		LOG_ERROR(strTmp);

		CleanMolNet();
		return 1;
	}

	LOG_BASIC("登陆服务器:%s,端口:%d 启动成功。",ptagServerSet.serverip,ptagServerSet.serverport);

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
