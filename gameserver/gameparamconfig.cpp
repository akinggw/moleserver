#include "gameparamconfig.h"
#include "gameserver_common.h"
#include "gameframe/CRoom.h"
#include "gameframe/RoomManager.h"
#include "gameframe/CRobotManager.h"
#include "gameframe/DBOperator.h"

#include <dlfcn.h>
#include <string>

typedef void * (ModuleCreateProc)(void);

bool LoadServerConfig(std::string curPath)
{
    std::string pCurConfigfile = curPath + LOGINSERVER_CONFIG;

    strcpy(m_ServerSet.m_sDBIpAddr,GetIniSectionItem(pCurConfigfile.c_str(),"database","ipaddress").c_str());
    strcpy(m_ServerSet.m_sDBUser,GetIniSectionItem(pCurConfigfile.c_str(),"database","username").c_str());
    strcpy(m_ServerSet.m_sDBPswd,GetIniSectionItem(pCurConfigfile.c_str(),"database","userpwd").c_str());
    strcpy(m_ServerSet.m_sDBName,GetIniSectionItem(pCurConfigfile.c_str(),"database","dbname").c_str());
    m_ServerSet.m_iDBPort = atoi(GetIniSectionItem(pCurConfigfile.c_str(),"database","port").c_str());

    return true;
}

bool LoadGameParamConfig(std::string curPath)
{
   	void *handle=NULL;
    std::string pCurConfigfile = curPath;
	char *error;

	tagGameRoom ptagGameRoom;
    if(!ServerDBOperator.GetGameRoomInfo(m_ServerSet.RoomId,&ptagGameRoom))
        return false;

    strcpy(m_ServerSet.m_sServerIPAddr,ptagGameRoom.serverip);
    m_ServerSet.m_iServerPort = ptagGameRoom.serverport;
    strcpy(m_ServerSet.GameName,ptagGameRoom.servername);
    strcpy(m_ServerSet.ClientMudleName,ptagGameRoom.processname);
    m_ServerSet.GameType = ptagGameRoom.gamingtype;
    m_ServerSet.TableCount = ptagGameRoom.tablecount;
    m_ServerSet.PlayerCount = ptagGameRoom.tableplayercount;
    m_ServerSet.lastMoney = ptagGameRoom.lastmoney;
    m_ServerSet.m_RoomRevenue = ptagGameRoom.roomrevenue;
    m_ServerSet.m_Pielement = ptagGameRoom.pielement;
    m_ServerSet.m_GameId = ptagGameRoom.gameid;
    m_ServerSet.m_QueueGaming = ptagGameRoom.QueueGaming > 0 ? true : false;

    pCurConfigfile += "/games/";
    pCurConfigfile += m_ServerSet.ClientMudleName;

    handle = dlopen (pCurConfigfile.c_str(), RTLD_LAZY);
    if (!handle) {
        printf("模块 %s 导入失败:%s.\n",m_ServerSet.ClientMudleName,dlerror());
        return 0;
    }

    ModuleCreateProc * CreateFunc=(ModuleCreateProc *)dlsym(handle,"CreateServerServiceManager");
    if ((error = dlerror()) != NULL)
    {
        dlclose(handle);
        printf("CreateServerServiceManager失败:%s.\n",dlerror());
        return 0;
    }

	m_g_ServerServiceManager=(ServerServiceManager *)CreateFunc();
	if (m_g_ServerServiceManager==NULL)
	{
        dlclose(handle);
        printf("CreateFunc()失败:%s.\n",dlerror());
		return 0;
	}

	m_g_ServerServiceManager->SetServerPort(m_ServerSet.m_iServerPort);
	m_g_ServerServiceManager->SetGameModuleName(m_ServerSet.ClientMudleName);
	m_g_ServerServiceManager->SetGamePlayerCount(m_ServerSet.PlayerCount);
	m_g_ServerServiceManager->SetRoomGameType((enStartMode)m_ServerSet.GameType);
	m_g_ServerServiceManager->SetRoomLastDouble(m_ServerSet.lastMoney);

	for(int i=0;i<m_ServerSet.TableCount;i++)
	{
		CRoom *pRoom = new CRoom((RoomType)m_ServerSet.GameType);
		pRoom->SetName(m_ServerSet.GameName);
		pRoom->SetMaxPlayer(m_ServerSet.PlayerCount);
		pRoom->SetLastMoney(m_ServerSet.lastMoney);
		pRoom->SetGamePielement(m_ServerSet.m_Pielement);
		pRoom->SetChouShui(m_ServerSet.m_RoomRevenue / 100.0f);
		pRoom->SetGameType(m_ServerSet.m_GameId);
		pRoom->SetGameId(ptagGameRoom.gameid);
		pRoom->SetRoomMarking(m_ServerSet.RoomId);
		pRoom->SetRoomGameType(m_ServerSet.m_GameStartMode);

		pRoom->SetServerLogicFrame((ServerLogicFrame*)m_g_ServerServiceManager->CreateTableFrameSink());

		ServerRoomManager.AddRoom(pRoom);
	}

	// 导入机器人
	RobotManager.LoadGameRobot();

	return true;
}
