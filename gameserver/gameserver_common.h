#ifndef GAMESERVER_COMMON_H_INCLUDED
#define GAMESERVER_COMMON_H_INCLUDED

#include <time.h>

#include "../include/molnet/Network.h"

using namespace network;

#include "../include/Common/Player.h"
#include "../include/Common/Room.h"
#include "../include/Common/ServerServiceManager.h"

#define IDD_ROBOT_MAX_COUNT              1000                   // 游戏最多支持机器人数量
#define IDD_ROBOT_SOCKET_START           700000                 // 机器人socket的开始位置
#define IDD_LOSTONLINE_SOCKET_START      800000                 // 掉线玩家socket的开始位置

#define LOGINSERVER_CONFIG "/configs/server_config.ini"

/**
 * 服务器配置结构
 */
struct ServerSet
{
	ServerSet()
		: GameType(0),TableCount(0),PlayerCount(0),lastMoney(0),m_RoomRevenue(0),m_Pielement(0),
		  m_GameType(0),m_iDBPort(0),m_iServerPort(0),m_GameStartMode(enStartMode_FullReady),
		  m_QueueGaming(false),RoomId(0)
	{
	}

	int RoomId;                         // 房间ID
	char GameName[128];                 // 游戏名称
	char ClientMudleName[128];          // 游戏客户端组件名称
	uint8 GameType;                      // 游戏类型
	int TableCount;                     // 桌子数量
	int PlayerCount;                    // 玩家数量
	uint32 lastMoney;                     // 最小金币值
	char m_sServerPWD[128];             //房间密码

	int m_RoomRevenue;					// 房间税收 *
	int m_Pielement;					// 房间单元积分 *

	char MainGameName[128];             // 游戏真正的名称
	int m_GameType;                     // 游戏类型

	enStartMode                     m_GameStartMode;            //游戏开始模式
	bool                            m_QueueGaming;              //是否排队进入游戏

	char							m_sDBIpAddr[128];				//数据库IP地址
	int								m_iDBPort;					//数据库端口
	char							m_sDBUser[128];					//数据库用户名
	char							m_sDBPswd[128];					//数据库密码
	char							m_sDBName[128];					//数据库服务名

	char							m_sServerIPAddr[128];			//服务器IP地址
	int								m_iServerPort;				//服务器端口
};

//时间子项
struct tagSubTimerItem
{
	tagSubTimerItem()
		: nTimerID(0),nTimeLeave(0),nIsEnable(false)
	{}

	uint32							nTimerID;							//时间标识
	uint32							nTimeLeave;							//剩余时间
	bool                            nIsEnable;                          //是否可用
};

//类说明
typedef std::map<uint32,tagSubTimerItem> CTimerItemArray;					//时间数组

extern ServerSet m_ServerSet;
extern ServerServiceManager         *m_g_ServerServiceManager;        //游戏逻辑接口

#endif // GAMESERVER_COMMON_H_INCLUDED
