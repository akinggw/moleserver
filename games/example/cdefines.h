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

struct tagJettons
{
	tagJettons()
	{
		memset(jettons, 0, sizeof(jettons));
	}

	inline void clear(void)
	{
		memset(jettons,0,sizeof(jettons));
	}
	inline int64 getTotal(void)
	{
		int64 pTotal = 0;
		for(int i=0;i<5;i++)
			pTotal += jettons[i];

		return pTotal;
	}

	int64 jettons[5];
};

enum GameState
{
	GAMESTATE_KONGXIAN = 0,            // 空闲
	GAMESTATE_XIAZHU,                  // 下注
	GAMESTATE_KAIPAI,                  // 开牌
	GAMESTATE_NNULL
};

#define IDD_TIMER_GAME_STARTING   IDD_TIMER_GAME_START+1                   // 游戏开始定时器
#define IDD_TIMER_GAME_XIAZHU     IDD_TIMER_GAME_START+2                   // 下注
#define IDD_TIMER_GAME_KAIPAI     IDD_TIMER_GAME_START+3                   // 开牌

#define IDD_MESSAGE_ROOM_ENTERGAME       IDD_MESSAGE_ROOM+1         // 进入房间消息
#define IDD_MESSAGE_ROOM_STARTJETTON     IDD_MESSAGE_ROOM+2         // 开始下注消息
#define IDD_MESSAGE_ROOM_OPENCARD        IDD_MESSAGE_ROOM+3         // 开始开牌消息
#define IDD_MESSAGE_ROOM_GAMEOVER        IDD_MESSAGE_ROOM+4         // 游戏结束消息
#define IDD_MESSAGE_ROOM_JETTON          IDD_MESSAGE_ROOM+5         // 游戏下注消息
#define IDD_MESSAGE_ROOM_CLEARJETTON     IDD_MESSAGE_ROOM+6         // 清除下注消息
#define IDD_MESSAGE_ROOM_REENTERGAME     IDD_MESSAGE_ROOM+7         // 重回房间消息

#endif
