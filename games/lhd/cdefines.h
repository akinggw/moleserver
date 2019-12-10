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

#define IDD_MESSAGE_ROOM_FAPAI      IDD_MESSAGE_ROOM+1         // 发牌消息
#define IDD_MESSAGE_ROOM_XIAZHU     IDD_MESSAGE_ROOM+2         // 下注消息
#define IDD_MESSAGE_ROOM_GAMEEND    IDD_MESSAGE_ROOM+3         // 结算消息
#define IDD_MESSAGE_ROOM_ENTERGAME  IDD_MESSAGE_ROOM+4         // 进入游戏消息
#define IDD_MESSAGE_ROOM_STARTXIAZHU  IDD_MESSAGE_ROOM+5         // 开始下注消息
#define IDD_MESSAGE_ROOM_REENTERGAME IDD_MESSAGE_ROOM+6        // 重回房间
#define IDD_MESSAGE_ROOM_SUPERCONTROL    IDD_MESSAGE_ROOM+7         // 游戏控制消息
#define IDD_MESSAGE_ROOM_XIAZHUCLEAR IDD_MESSAGE_ROOM+8         //清除下注

enum tagCardType
{
	CARTYPE_LONG = 0,                  // 龙
	CARTYPE_HU,                        // 虎
	CARTYPE_HONG,                      // 和
	CARTYPE_NULL
};

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

#endif
