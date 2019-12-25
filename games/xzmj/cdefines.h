#ifndef _C_DEFINES_H_INCLUDE_
#define _C_DEFINES_H_INCLUDE_

#include "../../include/molnet/Network.h"
#include "../../include/MahJong/MahJongLogic.h"

#include "../../include/Common/ccommon.h"
#include "../../include/Common/defines.h"
#include "../../include/Common/Player.h"
#include "../../include/Common/Room.h"
#include "../../include/Common/ServerServiceManager.h"
#include "../../include/Common/Robotlogicframe.h"
#include "../../include/Common/serverlogicframe.h"

#include <jsoncpp/json/json.h>

using namespace network;

//////////////////////////////////////////////////////////////////////////
#define GAME_PLAYER						4									/*!< 游戏人数 */
#define MAX_ORG_MJ						108									/*!< 牌的总数,9*4*3 + 4*4 + 3*4 = 136*/
#define MAX_SINGLE_BLOCK_COUNT			4									/*!< 每个块里最大的麻将数目，吃和碰有3个，杠有4个 */
#define MAX_OUT_MJ						60									/*!< 出牌的最大数 */
#define MAX_SELECT_GANG					4									/*!< 最多能选择的杠牌数 */
//////////////////////////////////////////////////////////////////////////

///////////////////番数/////////////////////////////
#define DIANPAO							1
#define ZIMOHU							2

//服务端命令
#define ID_S_GAMEMSG_DEALMJ					IDD_MESSAGE_ROOM+1			//发牌
#define ID_S_GAMEMSG_START					IDD_MESSAGE_ROOM+2			//开始
#define ID_S_GAMEMSG_OUT					IDD_MESSAGE_ROOM+3			//出牌
#define ID_S_GAMEMSG_TOUCH					IDD_MESSAGE_ROOM+4			//摸牌
#define ID_S_GAMEMSG_PENG					IDD_MESSAGE_ROOM+5			//碰牌
#define ID_S_GAMEMSG_GANG					IDD_MESSAGE_ROOM+6			//杠牌
#define ID_S_GAMEMSG_HU						IDD_MESSAGE_ROOM+7			//胡牌
#define ID_S_GAMEMSG_GAMEOVER				IDD_MESSAGE_ROOM+8			//游戏结束
#define ID_S_GAMEMSG_GIVEUP					IDD_MESSAGE_ROOM+9			//弃牌
#define ID_S_GAMEMSG_TRUSTEE				IDD_MESSAGE_ROOM+10			//托管
#define ID_S_GAMEMSG_USERREBACK				IDD_MESSAGE_ROOM+11			//断线重连
#define ID_S_GAMEMSG_CHANGETABLE			IDD_MESSAGE_ROOM+12			//换桌
#define ID_S_GAMEMSG_MATCHEND				IDD_MESSAGE_ROOM+13			//比赛结束

#define ID_S_GAMEMSG_MJ						IDD_MESSAGE_ROOM+14			//麻将数据
#define ID_S_GAMEMSG_CHAT					IDD_MESSAGE_ROOM+15			//聊天

//客户端命令
#define ID_C_GAMEMSG_READY					IDD_MESSAGE_ROOM+21			//准备
#define ID_C_GAMEMSG_OUT					IDD_MESSAGE_ROOM+22			//出牌
#define ID_C_GAMEMSG_PENG					IDD_MESSAGE_ROOM+23			//碰牌
#define ID_C_GAMEMSG_GANG					IDD_MESSAGE_ROOM+24			//杠牌
#define ID_C_GAMEMSG_HU						IDD_MESSAGE_ROOM+25			//胡牌
#define ID_C_GAMEMSG_GIVEUP					IDD_MESSAGE_ROOM+26			//弃牌
#define ID_C_GAMEMSG_TRUSTEE				IDD_MESSAGE_ROOM+27			//托管
#define ID_C_GAMEMSG_MATCHEND				IDD_MESSAGE_ROOM+28			//比赛结束

#define ID_C_GAMEMSG_MJ						IDD_MESSAGE_ROOM+29			//麻将数据
#define ID_C_GAMEMSG_CHAT					IDD_MESSAGE_ROOM+30			//聊天

//////////////////////////////////////////////////////////////////////////
//时间(ms)
//#define TIME_Nomal							10000						//10秒的出牌等待时间
#define TIME_Dice							2000						//掷骰子
#define TIME_BuildWall						2000						//建牌墙
#define TIME_Deal							7000						//发牌
#define TIME_OperAni						1000						//操作动画（出吃碰杠）
#define TIME_MaxOut							15000						//出牌超时时间
#define TIME_MaxOper						15000						//操作超时时间
#define TIME_ServerDelay					2000						//客户端与服务器端延迟
#define TIME_Continue						20000						//计算界面继续时间
//////////////////////////////////////////////////////////////////////////

//! 操作定义
//! 行牌过程中的可用操作
enum enOperation
{
	Operation_Null			= 0x00,										//无
	Operation_Peng			= 0x01, 									//碰
	Operation_Gang			= 0x02, 									//杠
	Operation_Hu			= 0x04, 									//胡
	Operation_GiveUp		= 0x08, 									//放弃
};

//! 座位的方向
/*! 座位的方向,以逆时针方向定义 */
enum enChairLocation
{
	ChairLoc_Invalid = -1,											/*!< 非法值 */
	ChairLoc_Up = 0,												/*!< 上 */
	ChairLoc_Left,													/*!< 左 */
	ChairLoc_Down,													/*!< 下 */
	ChairLoc_Right													/*!< 右 */
};

//! 玩家状态
/*! 玩家状态 */
enum enPlayerStatu
{
	PlayerStatu_Normal = 0,											/*!< 普通 */
	PlayerStatu_Dropline,											/*!< 掉线 */
	PlayerStatu_Trustee,											/*!< 托管 */
	PlayerStatu_TingPai,											/*!< 听牌 */
	PlayerStatu_WeiTing,											/*!< 未听 */
	PlayerStatu_HuaZhu,												/*!< 花猪 */
	PlayerStatu_ZiMo,												/*!< 自摸胡 */
	PlayerStatu_PingHu,												/*!< 放炮胡 */
	PlayerStatu_Escape,												/*!< 逃跑 */
	PlayerStatu_GameOver,											/*!< 安全退出 */
	PlayerStatu_ForceQuit,											/*!< 强制退出 */
};

//! 胡牌的类型
/*! 胡牌的类型 */
enum enHuType
{
	HuType_Null = 0,												/*!< 空 */
	HuType_ZiMo,													/*!< 自摸 */
	HuType_PingHu,													/*!< 平胡，即别人放炮后的胡牌 */
	HuType_QiangGang,
	HuType_QiangTui,												/*!< 强退 */
	HuType_GameOver,												/*!< 安全退出 */
	HuType_ForceQuit,												/*!< 强制退出 */
};

//! 游戏结束的类型
/*! 游戏结束的类型,根据游戏不同自定 */
enum enEndGameType
{
	EndGameType_NotJudge = 0,										/*!< 无效值 */
	EndGameType_ZiMo,												/*!< 自摸胡牌 */
	EndGameType_Hu,													/*!< 放炮胡牌 */
	EndGameType_LiuJu,												/*!< 流局 */
	EndGameType_QiangTui											/*!< 强退 */
};

//! 摸牌的类型
/*! 一般摸牌、杠后摸牌 */
enum enTouchType
{
	TouchType_Normal = 0,											/*!< 一般摸牌 */
	TouchType_Gang													/*!< 杠后摸牌(从牌墙末尾处拿牌) */
};

//! 游戏结束时各玩家的状态
/*! 用来显示结束时玩家标签 */
enum enUserEndType
{
	UserEndType_Normal = 0,
	UserEndType_PingHu,												/*!<平胡 */
	UserEndType_ZiMo,												/*!<自摸 */
	UserEndType_Ting,												/*!<已听 */
	UserEndType_NotTing,											/*!<未听 */
	UserEndType_HuaZhu,												/*!<花猪 */
	UserEndType_Escape,												/*!<逃跑 */
	UserEndType_ForceQuit,											/*!<金币不足，强制退出 */
};

//! 玩家番数
/*! 玩家番薯类型 */
enum enFanStyle
{
	FanStyle_Null = -1,
	FanStyle_PingHu,
	FanStyle_DuiDuiHu,
	FanStyle_QingYiSe,
	FanStyle_DaiYaoJiu,
	FanStyle_QiDui,
	FanStyle_QingDui,
	FanStyle_JiangDui,
	FanStyle_LongQiDui,
	FanStyle_QingQiDui,
	FanStyle_QingJiuYao,
	FanStyle_TianHu,
	FanStyle_DiHu,
	FanStyle_QingLongQiDui,
	FanStyle_Gang,
	FanStyle_Gen,
	FanStyle_GangShangHua,
	FanStyle_GangShangPao,
	FanStyle_QiangGang,
};

//! 聊天消息类型
/*! 区分聊天和表情 */
enum enChatType
{
	ChatType_Null = 0,											/*!< 空 */
	ChatType_Face,												/*!< 表情 */
	ChatType_Talk,												/*!< 文字 */
};

#endif
