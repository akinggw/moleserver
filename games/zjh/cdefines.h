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

#define IDD_MESSAGE_ROOM_START			IDD_MESSAGE_ROOM+1			//开始（发牌）
#define IDD_MESSAGE_ROOM_ADD_SCORE		IDD_MESSAGE_ROOM+2			//加注
#define IDD_MESSAGE_ROOM_GIVE_UP		IDD_MESSAGE_ROOM+3			//放弃
#define IDD_MESSAGE_ROOM_WATCH			IDD_MESSAGE_ROOM+4			//看牌
#define IDD_MESSAGE_ROOM_THAN			IDD_MESSAGE_ROOM+5			//比牌
#define IDD_MESSAGE_ROOM_GAMEOVER       IDD_MESSAGE_ROOM+6			// 游戏结束
#define IDD_MESSAGE_ROOM_BOOT_WIN		IDD_MESSAGE_ROOM+7

#define IDD_MESSAGE_ROOM_ENTER_ROOM     IDD_MESSAGE_ROOM+8			// 玩家进入
#define IDD_MESSAGE_ROOM_CAN_THAN		IDD_MESSAGE_ROOM+9			// 可以比牌
#define IDD_MESSAGE_ROOM_LOOK_ON		IDD_MESSAGE_ROOM+10			// 旁观
#define IDD_MESSAGE_ROOM_RE_ENTER		IDD_MESSAGE_ROOM+11			// 断线重连

#define IDD_MESSAGE_ROOM_SHOW_FACE		IDD_MESSAGE_ROOM+12			//表情
#define IDD_MESSAGE_ROOM_SHOW_MSG		IDD_MESSAGE_ROOM+13			//说话



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IDD_TIMER_CLIENT_GAMING         IDD_TIMER_GAME_START+1		// 客户端游戏中用到的定时器
#define IDD_TIMER_CLIENT_START			IDD_TIMER_GAME_START+2		// 客户端游戏中用到的开始结束定时器
#define IDD_TIMER_CLIENT_OUT_ROOM		IDD_TIMER_GAME_START+3


#define IDD_TIMER_SERVER_GAMING			IDD_TIMER_GAME_START+4		// 服务器游戏中操作计时
#define IDD_TIMER_SERVER_OFFLINE		IDD_TIMER_GAME_START+5		// 服务器游戏中断线等待
#define IDD_TIMER_SERVER_GAMEOVER		IDD_TIMER_GAME_START+6		// 服务器游戏中结束等待


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IDD_GUI_BTN_CONTINUE            IDD_GUI_ID_START+1			// 继续按钮
#define IDD_GUI_BTN_CHANGETABLE         IDD_GUI_ID_START+2			// 换桌按钮
#define IDD_GUI_BTN_EXIT                IDD_GUI_ID_START+3			// 退出按钮
#define IDD_GUI_BTN_START               IDD_GUI_ID_START+4			// 开始按钮
#define IDD_GUI_BTN_GIVE_UP				IDD_GUI_ID_START+5			// 放弃按钮
#define IDD_GUI_BTN_THAN_CARD			IDD_GUI_ID_START+6			// 比牌按钮
#define IDD_GUI_BTN_WATCH_CARD			IDD_GUI_ID_START+7			// 看牌按钮

#define IDD_GUI_BTN_SHOW_FACE			IDD_GUI_ID_START+8			//表情
#define IDD_GUI_BTN_SHOW_MSG			IDD_GUI_ID_START+9			//说话

#define IDD_GUI_BTN_SET					IDD_GUI_ID_START+10			// 设置
#define IDD_GUI_BTN_SOUND				IDD_GUI_ID_START+11			// 音效
#define IDD_GUI_BTN_MUSIC				IDD_GUI_ID_START+12			// 音乐
#define IDD_GUI_BTN_MSG					IDD_GUI_ID_START+13			// 消息
#define IDD_GUI_BTN_VIDEO				IDD_GUI_ID_START+14			// 消息

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define IDD_MAX_ROOM_USERCOUNT          6							// 房间人数
#define IDD_MAX_CARD_COUNT				52							// 牌个数
#define IDD_MAX_HAND_COUNT				3							//手牌数目

enum PlayStates
{
	PLAYSTATE_NULL = 0,		//没有玩
	PLAYSTATE_PLAY,			//在玩
	PLAYSTATE_GIVEUP,		//放弃
	PLAYSTATE_LOSE,			//比牌输掉
};

#define				ENABLE_DEBUG_TOOL

#endif
