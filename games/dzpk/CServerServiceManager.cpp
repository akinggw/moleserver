#include "CServerServiceManager.h"

#include "cserverlogicframe.h"
#include "crobotlogicframe.h"

/// 构造函数
CServerServiceManager::CServerServiceManager():m_serverport(0)
{

}

/// 析构函数
CServerServiceManager::~CServerServiceManager()
{

}

/// 设置玩家人数
void CServerServiceManager::SetGamePlayerCount(int count)
{
	m_maxPlayerCount = count;
}

/// 得到玩家人数
int CServerServiceManager::GetGamePlayerCount(void)
{
	return m_maxPlayerCount;
}

/// 设置房间标识
void CServerServiceManager::SetRoomMarking(int marking)
{
	m_GameMarking = marking;
}

/// 得到房间标识
int CServerServiceManager::GetRoomMarking(void)
{
	return m_GameMarking;
}

/// 设置游戏名称
void CServerServiceManager::SetGameName(const char* name)
{
	m_GameName = name;
}

/// 得到游戏名称
const char* CServerServiceManager::GetGameName(void)
{
	return m_GameName.c_str();
}

/// 设置游戏客户端组件名称
void CServerServiceManager::SetGameModuleName(const char* name)
{
	m_GameModuleName = name;
}

/// 得到游戏客户端组件名称
const char* CServerServiceManager::GetGameModuleName(void)
{
	return m_GameModuleName.c_str();
}

/// 设置房间最小进入倍数
void CServerServiceManager::SetRoomLastDouble(int pdouble)
{
	m_RoomLastDouble = pdouble;
}

/// 得到房间最小进入倍数
int CServerServiceManager::GetRoomLastDouble(void)
{
	return m_RoomLastDouble;
}

//创建桌子
void * CServerServiceManager::CreateTableFrameSink(void)
{
	//建立对象
	CServerLogicFrame * pTableFrameSink=NULL;

	try
	{
		pTableFrameSink=new CServerLogicFrame();
		if (pTableFrameSink==NULL) throw "桌子创建失败";

		return pTableFrameSink;
	}
	catch (...)
	{
		printf("桌子创建失败！\n");
	}

	//清理对象
	SafeDelete(pTableFrameSink);

	return NULL;
}

//创建机器
void * CServerServiceManager::CreateAndroidUserItemSink(void)
{
	//建立对象
	CRobotLogicFrame * pRobotFrameSink=NULL;

	try
	{
		pRobotFrameSink=new CRobotLogicFrame();
		if (pRobotFrameSink==NULL) throw "机器人创建失败";

		return pRobotFrameSink;
	}
	catch (...)
	{
		printf("机器人创建失败！\n");
	}

	//清理对象
	SafeDelete(pRobotFrameSink);

	return NULL;
}
