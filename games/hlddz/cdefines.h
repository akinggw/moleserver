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

#define GAME_PLAYER					3									//游戏人数

//服务端消息ID
#define SUB_S_SEND_CARD				IDD_MESSAGE_ROOM+10									//发牌命令
#define SUB_S_LAND_SCORE			IDD_MESSAGE_ROOM+11									//叫分命令
#define SUB_S_GAME_START			IDD_MESSAGE_ROOM+12									//游戏开始
#define SUB_S_BRIGHT_CARD			IDD_MESSAGE_ROOM+13									//玩家明牌
#define SUB_S_OUT_CARD				IDD_MESSAGE_ROOM+14									//用户出牌
#define SUB_S_PASS_CARD				IDD_MESSAGE_ROOM+15									//放弃出牌
#define SUB_S_TRUSTEE				IDD_MESSAGE_ROOM+16									//托管消息
#define SUB_S_GAME_END				IDD_MESSAGE_ROOM+17									//游戏结束
#define SUB_S_MATCH_END				IDD_MESSAGE_ROOM+18 								//比赛结束
//断线重连ID
#define SUB_S_LAND_SCORE_STATION	IDD_MESSAGE_ROOM+21									//叫分阶段
#define SUB_S_PLAYING_STATION		IDD_MESSAGE_ROOM+22									//玩牌阶段

#define SUB_S_SHOW_MSG				IDD_MESSAGE_ROOM+23									//聊天
#define SUB_S_SHOW_FACE				IDD_MESSAGE_ROOM+24									//表情

//客户端消息ID
#define SUB_C_LAND_SCORE			IDD_MESSAGE_ROOM+1									//叫分命令
#define SUB_C_OUT_CARD				IDD_MESSAGE_ROOM+2									//用户出牌
#define SUB_C_PASS_CARD				IDD_MESSAGE_ROOM+3									//放弃出牌
#define SUB_C_BRIGHT_CARD			IDD_MESSAGE_ROOM+4									//玩家明牌
#define SUB_C_TRUSTEE				IDD_MESSAGE_ROOM+5									//托管消息

#define SUB_C_SHOW_MSG				IDD_MESSAGE_ROOM+6									//聊天
#define SUB_C_SHOW_FACE				IDD_MESSAGE_ROOM+7									//表情

//游戏状态
#define GS_WK_FREE					11									//等待开始
#define GS_WK_SCORE					12									//叫分状态
#define GS_WK_PLAYING				13									//游戏进行

//叫分阶段变量
#define CSD_NORMAL		0
#define CSD_SNATCHLAND	1
#define CSD_GAMESTART	2

#define IDD_MAX_PLAYERS            3                                    // 最大游戏人数

#endif
