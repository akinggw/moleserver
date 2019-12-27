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

//消息定义
#define IDD_MESSAGE_ROOM_START_GAME     IDD_MESSAGE_ROOM+1			//开始游戏
#define IDD_MESSAGE_ROOM_DEAL_CARD		IDD_MESSAGE_ROOM+2			//发公共牌
#define IDD_MESSAGE_ROOM_ADD_GOLD		IDD_MESSAGE_ROOM+3			//玩家加注
#define IDD_MESSAGE_ROOM_GIVE_UP		IDD_MESSAGE_ROOM+4			//玩家放弃
#define IDD_MESSAGE_ROOM_GAME_OVER		IDD_MESSAGE_ROOM+5			//游戏结束
#define	IDD_MESSAGE_ROOM_CASH_CHIPS		IDD_MESSAGE_ROOM+6			//兑换筹码
#define IDD_MESSAGE_ROOM_ENTER_ROOM		IDD_MESSAGE_ROOM+7			//进入房间
#define IDD_MESSAGE_ROOM_RECONNECT		IDD_MESSAGE_ROOM+8			//断线重连
#define IDD_MESSAGE_ROOM_BOOT_WIN		IDD_MESSAGE_ROOM+9			//机器人输赢
#define IDD_MESSAGE_ROOM_LOOK_ON		IDD_MESSAGE_ROOM+10			//中途加入观看


#define IDD_MESSAGE_ROOM_SHOW_FACE		IDD_MESSAGE_ROOM+11			//表情
#define IDD_MESSAGE_ROOM_SHOW_MSG		IDD_MESSAGE_ROOM+12			//说话

////////////////////////////////////////////////////////////////////////////////////////
//时间定义
#define IDD_TIMER_CLIENT_GAMING         IDD_TIMER_GAME_START+1     // 客户端游戏中用到的定时器
#define IDD_TIMER_CLIENT_CASH_SHIP	    IDD_TIMER_GAME_START+2     // 客户端加入游戏兑换筹码计时
#define IDD_TIMER_CLIENT_OVER           IDD_TIMER_GAME_START+3     // 游戏结束
#define IDD_TIMER_CLIENT_OUT			IDD_TIMER_GAME_START+4     // 游戏结束

#define IDD_TIMER_SERVER_GAMING         IDD_TIMER_GAME_START+5     // 游戏服务器操作定时器
#define IDD_TIMER_SERVER_DEAL_CARD      IDD_TIMER_GAME_START+6     // 游戏服务器操作定时器


///////////////////////////////////////////////////////////////////////////////////////
//控件定义

#define IDD_GUI_BTN_CONTINUE            IDD_GUI_ID_START+1         // 继续游戏按钮
#define IDD_GUI_BTN_CHANGETABLE         IDD_GUI_ID_START+2         // 换桌按钮
#define IDD_GUI_BTN_EXIT                IDD_GUI_ID_START+3         // 退出按钮
#define IDD_GUI_BTN_ROBOT               IDD_GUI_ID_START+4         // 托管按钮

#define IDD_GUI_BTN_CASH_CHIPS           IDD_GUI_ID_START+5         //兑换筹码按钮
#define IDD_GUI_BTN_RECASH               IDD_GUI_ID_START+6         // 取消兑换按钮

#define IDD_GUI_BTN_FOLLOW              IDD_GUI_ID_START+7         // 跟注按钮
#define IDD_GUI_BTN_ADD_GOLD            IDD_GUI_ID_START+8         // 加注按钮
#define IDD_GUI_BTN_GIVE_UP				IDD_GUI_ID_START+9         // 放弃按钮
#define IDD_GUI_BTN_WATCH				IDD_GUI_ID_START+10        // 看牌按钮

#define IDD_GUI_BTN_FOLLOW_EVERY        IDD_GUI_ID_START+11        // 跟任何按钮
#define IDD_GUI_BTN_FOLLOW_NOW          IDD_GUI_ID_START+12         // 跟目前按钮
#define IDD_GUI_BTN_NEXT_GIVE_UP        IDD_GUI_ID_START+13        // 看牌或放弃按钮
#define IDD_GUI_BTN_NEXT_WATCH          IDD_GUI_ID_START+14        // 下次看牌按钮

#define IDD_GUI_BTN_ADD_1				IDD_GUI_ID_START+15        // 1按钮
#define IDD_GUI_BTN_ADD_5				IDD_GUI_ID_START+16        // 5按钮
#define IDD_GUI_BTN_ADD_10				IDD_GUI_ID_START+17        // 10按钮
#define IDD_GUI_BTN_ADD_50				IDD_GUI_ID_START+18        // 50按钮
#define IDD_GUI_BTN_ADD_100				IDD_GUI_ID_START+19        // 100按钮
#define IDD_GUI_BTN_ADD_500				IDD_GUI_ID_START+20        // 500按钮
#define IDD_GUI_BTN_ADD_1000			IDD_GUI_ID_START+21        // 1000按钮
#define IDD_GUI_BTN_ADD_5000			IDD_GUI_ID_START+22        // 5000按钮
#define IDD_GUI_BTN_ADD_10000			IDD_GUI_ID_START+23        // 10000按钮

#define IDD_GUI_BTN_CHANGE_CHIPS		IDD_GUI_ID_START+24        // 修改带入额按钮

#define IDD_GUI_BTN_SET					IDD_GUI_ID_START+25			//设置
#define IDD_GUI_BTN_START_VIDEO			IDD_GUI_ID_START+26			//开始录像
#define IDD_GUI_BTN_STOP_VIDEO			IDD_GUI_ID_START+27			//停止录像
#define IDD_GUI_BTN_START_MUSIC			IDD_GUI_ID_START+28			//开启音乐
#define IDD_GUI_BTN_STOP_MUSIC			IDD_GUI_ID_START+29			//关闭音乐
#define IDD_GUI_BTN_START_SOUND			IDD_GUI_ID_START+30			//开启音效
#define IDD_GUI_BTN_STOP_SOUND			IDD_GUI_ID_START+31			//关闭音效

#define IDD_GUI_SCR_ADD_GOLD			IDD_GUI_ID_START+32			//加注滑块

#define IDD_GUI_EDT_CASH_CHIPS			IDD_GUI_ID_START+40			//兑换筹码输入框
#define IDD_GUI_EDT_ADD_GOLD			IDD_GUI_ID_START+41			//加注输入框


#define IDD_GUI_BTN_SHOW_FACE			IDD_GUI_ID_START+42			//表情
#define IDD_GUI_BTN_SHOW_MSG			IDD_GUI_ID_START+43			//说话

///////////////////////////////////////////////////////////////////////////////////////
//变量定义
#define IDD_MAX_ROOM_USERCOUNT          9							// 房间人数
#define IDD_MAX_CARD_COUNT				52							// 牌个数

#define IDD_HAND_COUNT					2								//最大手牌数
#define IDD_GONG_COUNT					5								//最大公共牌数
#define IDD_MAX_WEAVE_CARD				7								//最大组合牌数
#define IDD_WEAVE_COUNT					5								//最佳组合牌数



enum GameEndType
{
	//正常结束
	NORMAL=0,
	//放弃结束
	GIVEUP,
};


enum OperateType
{
	OPERATE_BIG_BLD=0,		//大盲注
	OPERATE_SML_BLD,		//小盲注
	OPERATE_PASS,			//过牌
	OPERATE_ADD,			//加注
	OPERATE_FOLLOW,			//跟注
	OPERATE_ALLIN,			//全下
	OPERATE_OFFLINE,		//断线
	OPERATE_GIVEUP,			//放弃
	OPERATE_NULL,			//空
};

#endif
