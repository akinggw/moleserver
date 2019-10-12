#include <stdio.h>
#include <dlfcn.h>
#include <string>
#include <time.h>

#include "gameserver_common.h"
#include "gameframe/DBOperator.h"
#include "gameparamconfig.h"
#include "GameFrameManager.h"
#include "gameframe/PlayerManager.h"
#include "gameframe/RoomManager.h"

ServerSet m_ServerSet;
ServerServiceManager         *m_g_ServerServiceManager = NULL;        //游戏逻辑接口

int main(int argc,char *argv[])
{
    char strTmp[1024];
   	NetMessage myMes;
   	void *handle = NULL;
	char m_curProgress_Path[256];
	uint32 m_curTime = time(NULL);
	int m_updateonlineplayercount = 0;

	if(argc < 2)
    {
        printf("使用方法:game_server 1\n");
        return 0;
    }

    m_ServerSet.RoomId = atoi(argv[1]);
	getcwd(m_curProgress_Path,256);
	srand((uint32)time(0));

	new GameFrameManager();
	new DBOperator();
	new PlayerManager();
	new RoomManager();

    if(!LoadServerConfig(m_curProgress_Path))
    {
        return 0;
    }

    sprintf(strTmp,"game%d_server",m_ServerSet.RoomId);
	InitMolNet(strTmp);

    if(!ServerDBOperator.Initilize(m_ServerSet.m_sDBIpAddr,
                                   m_ServerSet.m_sDBUser,
                                   m_ServerSet.m_sDBPswd,
                                   m_ServerSet.m_sDBName,
                                   m_ServerSet.m_iDBPort))
    {
	    sprintf(strTmp,"【系统】 数据库:%s %s %s %s 连接失败。",m_ServerSet.m_sDBIpAddr,m_ServerSet.m_sDBUser,m_ServerSet.m_sDBPswd,m_ServerSet.m_sDBName);
		LOG_BASIC(strTmp);

		CleanMolNet();
		return 1;
    }

    LOG_BASIC("数据库:%s %s %s %s 连接成功。",m_ServerSet.m_sDBIpAddr,m_ServerSet.m_sDBUser,m_ServerSet.m_sDBPswd,m_ServerSet.m_sDBName);

    if(!LoadGameParamConfig(m_curProgress_Path))
    {
        return 0;
    }

	if(!StartMolNet(m_ServerSet.m_sServerIPAddr,
                    m_ServerSet.m_iServerPort))
	{
	    sprintf(strTmp,"【系统】 服务器启动失败,IP地址:%s,端口:%d。",m_ServerSet.m_sServerIPAddr,m_ServerSet.m_iServerPort);
		LOG_BASIC(strTmp);

		CleanMolNet();
		return 1;
	}

	LOG_BASIC("游戏服务器%s:ip:%s,端口:%d 启动成功。",m_ServerSet.GameName,m_ServerSet.m_sServerIPAddr,m_ServerSet.m_iServerPort);

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

        if(time(NULL) - m_curTime >= 1)
        {
            m_curTime = time(NULL);
            m_updateonlineplayercount += 1;

            ServerPlayerManager.OnEventTimer(0,0);
            ServerRoomManager.OnEventTimer(0,0);
            ServerGameFrameManager.UpdateQueueList();

            if(m_updateonlineplayercount >= 30)
            {
                m_updateonlineplayercount = 0;

                ServerGameFrameManager.UpdateGameRoomInfo();
            }
        }

		usleep(1000);
	}

    ServerDBOperator.Shutdown();
	CleanMolNet();

	dlclose(handle);
	delete DBOperator::getSingletonPtr();
	delete GameFrameManager::getSingletonPtr();
	delete PlayerManager::getSingletonPtr();
	delete RoomManager::getSingletonPtr();

    return 0;
}
