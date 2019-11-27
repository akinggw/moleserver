#ifndef _C_DEFINES_H_INCLUDE_
#define _C_DEFINES_H_INCLUDE_

#include "../../include/molnet/Network.h"

#include "../../include/Common/ccommon.h"
#include "../../include/Common/defines.h"
#include "../../include/Common/Player.h"
#include "../../include/Common/Room.h"
#include "../../include/Common/ServerServiceManager.h"
#include "../../include/Common/Robotlogicframe.h"
#include "../../include/Common/serverlogicframe.h"

#include <jsoncpp/json/json.h>

using namespace network;

/**
 * 游戏阶段
 */
enum GameState
{
	STATE_XIAZHU = 0,          // 下注状态
	STATE_GAMING,              // 游戏状态
	STATE_GAMEEND,             // 游戏结束状态
	STATE_NULL
};

struct tagGamingRecord
{
	tagGamingRecord():decGamingResult(0) {}
	tagGamingRecord(int pdecGamingResult,std::string jhstr)
		: decGamingResult(pdecGamingResult)
	{
		strncpy(JvHaoStr,jhstr.c_str(),256);
	}

	int decGamingResult;
	char JvHaoStr[256];
};

struct tagSuperUserControl
{
	tagSuperUserControl():controlmoney(0),userid(-1)
	{
		memset(account,0,sizeof(account));
	}
	tagSuperUserControl(std::string ac,int64 cm)
		: controlmoney(cm),userid(-1)
	{
		strncpy(account,ac.c_str(),256);
	}

	int32 userid;
	char account[256];
	int64 controlmoney;
};

extern 	int64                          m_minzhuangmoney;

#define IDD_MESSAGE_ROOM_FAPAI      IDD_MESSAGE_ROOM+1         // 发牌消息
#define IDD_MESSAGE_ROOM_XIAZHU     IDD_MESSAGE_ROOM+2         // 下注消息
#define IDD_MESSAGE_ROOM_GAMEEND    IDD_MESSAGE_ROOM+3         // 结算消息
#define IDD_MESSAGE_ROOM_ENTERGAME  IDD_MESSAGE_ROOM+4         // 进入游戏消息
#define IDD_MESSAGE_ROOM_STARTXIAZHU  IDD_MESSAGE_ROOM+5         // 开始下注消息
#define IDD_MESSAGE_ROOM_REENTERGAME IDD_MESSAGE_ROOM+6        // 重回房间
#define IDD_MESSAGE_ROOM_SUPERCONTROL IDD_MESSAGE_ROOM+7       // 超级控制
#define IDD_MESSAGE_ROOM_CLEARJETTONS IDD_MESSAGE_ROOM+8       // 清除下注
#define IDD_MESSAGE_ROOM_SUPERUSERCONTROL IDD_MESSAGE_ROOM+9   // 控制个人
#define IDD_MESSAGE_ROOM_CHEXIAOJETTON IDD_MESSAGE_ROOM+10     // 撤销下注
#define IDD_MESSAGE_ROOM_SHANGZHUANG IDD_MESSAGE_ROOM+11         // 上庄
#define IDD_MESSAGE_ROOM_XIAZHUANG   IDD_MESSAGE_ROOM+12         // 下庄
#define IDD_MESSAGE_ROOM_GETZHUANGLIST IDD_MESSAGE_ROOM+13       // 得到装列表

#define IDD_TIMER_GAME_STARTING   IDD_TIMER_GAME_START+1                   // 游戏卡开始定时器
#define IDD_TIMER_GAME_XIAZHU     IDD_TIMER_GAME_START+2                   // 下注
#define IDD_TIMER_GAME_KAIPAI     IDD_TIMER_GAME_START+3                   // 开牌
#define IDD_TIMER_GAME_JIESUAN    IDD_TIMER_GAME_START+4                   // 结算

#endif
