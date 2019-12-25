#ifndef _CMDHELPER_H_
#define _CMDHELPER_H_

#include "cdefines.h"
#include <string>
#include <sstream>
//////////////////////////////////////////////////////////////////////////
#define MAX_SELECT_GANG					4									/*!< 杠牌选择最大数 */
#define MAX_FAN_STYLE					20									/*!< 番数类型最大数 */
//////////////////////////////// 服务器信息 //////////////////////////////////////////

/*
//! 发送麻将
/*! 发牌,庄家可能发牌就有操作,如杠、胡 */
typedef struct tagCMD_S_DealMJ
{
	int32							iBankerUser;							/*!< 庄家 */
	int32							iIsRandomBanker;						/*!< 是否随机确定庄家 */
	int32 							iBaseScore;								/*!< 基础积分 */
	int32							iDiceNum01;								/*!< 骰子数01 确定庄家 */
	int32							iDiceNum02;								/*!< 骰子数02 确定庄家*/
	int32							iDiceNum11;								/*!< 骰子数11 确定拿牌位置*/
	int32							iDiceNum12;								/*!< 骰子数12 确定拿牌位置*/

	int32							iUserChairID;							/*!< 玩家座位号 */

	int32							iHandMJCount;							/*!< 麻将数目 */
	MJBYTE							bHandMJ[MAX_HAND_MJ];					/*!< 所发的麻将牌 */
	MJFLAG							bOperation;
	int32							iStatMJPos;								/*!< 开始摸牌位置 */

	int32							iGangCount;								/*!< 杠数目 */
	MJBYTE							bMJGang[MAX_SELECT_GANG];				/*!< 可杠的牌*/

	int size()
	{
		return sizeof(tagCMD_S_DealMJ);// - (MAX_HAND_MJ-iHandMJCount)*sizeof(MJBYTE) - (MAX_HAND_MJ-iMJGangCount)*sizeof(MJBYTE);
	}
}CMD_S_DealMJ, *PCMD_S_DealMJ;

/*! 出牌
/*! 出牌后,其他玩家可能有的操作 */
typedef struct tagCMD_S_OutMJ
{
	MJBYTE							bOutMJ;									/*!< 打出的牌 */
	int32							wOutMJUser;								/*!< 出牌玩家 */

	MJFLAG							bOperation;
	int32							isOtherPriority;

	int32							isTimeOut;								/*!< 是否超时由服务器出牌 */

	int size()
	{
		return sizeof(tagCMD_S_OutMJ);
	}
}CMD_S_OutMJ,*PCMD_S_OutMJ;

//! 摸牌
/*! 摸牌后,可能有的操作 */
typedef struct tagCMD_S_TouchMJ
{
	MJBYTE							bTouchMJ;								/*!< 摸牌 */
	int32							bTouchType;								/*!< 0：正常摸牌，1：从牌墙后面摸牌 */
	int32							wCurrentUser;							/*!< 当前玩家 */

	MJFLAG							bOperation;
	int32							isOtherPriority;
	MJFLAG							bGangType;								/*!< 杠的类型,明杠优先,bOperation中有GANG才有效 */

	int32							iGangCount;								/*!< 杠数目 */
	MJBYTE							bMJGang[MAX_SELECT_GANG];				/*!< 可杠的牌*/

	int size()
	{
		return sizeof(tagCMD_S_TouchMJ);
	}
}CMD_S_TouchMJ, *PCMD_S_TouchMJ;

//! 用户碰牌
/*! 用户碰牌后,其他玩家可能有的操作 */
typedef struct tagCMD_S_PengMJ
{
	int32							wLastOutUser;							/*!< 上次出牌玩家 */
	int32							wCurrentUser;							/*!< 碰牌玩家 */
	MJBYTE							bMJPeng;								/*!< 碰的牌 */

	MJFLAG							bOperation;
	int32							isOtherPriority;

	int size()
	{
		return sizeof(tagCMD_S_PengMJ);
	}
}CMD_S_PengMJ, *PCMD_S_PengMJ;

//! 杠牌
/*! 杠牌后,其他玩家可能有的操作 */
typedef struct tagCMD_S_GangMJ
{
	MJFLAG							bGangType;								/*!< 杠牌类型 */
	int32							wCurrentUser;							/*!< 操作玩家 */
	int32							wOutMJUser;								/*!< 出牌玩家 */
	MJBYTE							bMJGang;								/*!< 杠哪张牌 */

	MJFLAG							bOperation;
	int32							isOtherPriority;

	int32							iGangScore;									/*!< 刮风下雨扣除的分数 */

	int size()
	{
		return sizeof(tagCMD_S_GangMJ);
	}
}CMD_S_GangMJ, *PCMD_S_GangMJ;

//! 放弃可以操作
/*! 放弃可以操作后,其他玩家可能有的操作 */
typedef struct tagCMD_S_GiveUp
{
	MJFLAG							bOperation;								/*!< 放弃的人的操作权限 */
	MJBYTE							bOutMJ;									/*!< 被吃的那张牌 */
	int32							isOtherPriority;						/*!< 别人是否由优先级高的操作（只针对当前玩家） */
	int32							isOtherHu;								/*!< 一炮多响放弃时，还有其他玩家可以胡*/
	int32							iOperUser;								/*!< 当前操作玩家 */
	int32							iLastOutUser;							/*!< 最后出牌玩家 */

	int size()
	{
		return sizeof(tagCMD_S_GiveUp);
	}
}CMD_S_GiveUp, *PCMD_S_GiveUp;

//! 玩家胡牌
/*! 胡牌后结算 */
typedef struct tagCMD_S_HU
{
	MJBYTE							bUserHuMJ;										/*!< 胡的那张牌 */
	int32							iHuUser;										/*!< 胡牌的人， 如果是有玩家强退，那么这里就是强退玩家 */
	int32							iDianPaoUser;									/*!< 点炮的人， 如果是自摸，那么点炮玩家为空 */

	enHuType						enhuType;										/*!< 游戏胡牌类型 */

	int64							iNomalFan;											/*!< 胡牌玩家的总番数*/
	int64							iExtraFan;										/*!< 胡牌玩家的总番数*/
	int64							iHuScore;										/*!< 胡的分数*/
	int64							iGangScore;										/*!< 刮风下雨的分数*/
	int64							iDianPaoScore;									/*!< 点别人炮的分数*/
	int64							iAllScore;										/*!< 玩家总分*/

	int32							iFanStyle[MAX_FAN_STYLE];						/*!< 番数类型*/

	int32							iAlreadyHu;										/*!< 已经胡过这张牌*/

	MJFLAG							bOperation;										/*!< 放弃的人的操作权限 */
	int32							isOtherPriority;								/*!< 别人是否由优先级高的操作（只针对当前玩家） */
	int32							isNowOper;										/*!< 强退玩家正在操作或者正在出牌 */
	int32							isOtherHu;										/*!< 一炮多响时，别人是否还能胡，有则为1 */

	MJFLAG							bGangType;
	int32							iGangCount;
	MJBYTE							bMJGang[MAX_SELECT_GANG];

	int size()
	{
		return sizeof(tagCMD_S_HU);
	}
}CMD_S_HU, *PCMD_S_HU;


//! 玩家胡牌
/*! 胡牌后结算 */
typedef struct tagCMD_S_GameOver
{
	MJBlock							bHUMj[GAME_PLAYER][MAX_BLOCK_COUNT];			/*!< 亮出的牌块(碰杠的牌) */
	MJBYTE							bHUMjCount[GAME_PLAYER];						/*!< 亮出牌块的数量 */
	MJBYTE							bHandMj[GAME_PLAYER][MAX_HAND_MJ];				/*!< 手中的牌 */
	MJBYTE							bHandMjCount[GAME_PLAYER];						/*!< 手中的牌的数量 */
	MJBYTE							bOutMj[GAME_PLAYER][MAX_OUT_MJ];				/*!< 已经出的牌 */
	MJBYTE							bOutMjCount[GAME_PLAYER];						/*!< 已经出的牌的数量 */

	int32							iDianPaoUser[GAME_PLAYER];						/*!< 点炮的人， 如果是自摸，那么点炮玩家为空 */

	enEndGameType					enEndType;										/*!< 游戏结算类型 */
	enHuType						enhuType[GAME_PLAYER];							/*!< 游戏胡牌类型 */

	int64							iNomalFan[GAME_PLAYER];							/*!< 每个玩家的总番数*/
	int64							iExtraFan[GAME_PLAYER];
	int64							iFanScore[GAME_PLAYER];							/*!< 每个玩家番数得分*/
	int64							iGangScore[GAME_PLAYER];						/*!< 每个玩家刮风下雨得分*/
	int64							iTotalScore[GAME_PLAYER];						/*!< 最终总分数*/

	enUserEndType					enUSEndType[GAME_PLAYER];						/*!< 游戏结束时玩家的状态，平胡、自摸、未胡*/

	int32							isChangeTable;									/*!< 是否换桌*/

	int32							iFanStyle[GAME_PLAYER];							/*!< 番数类型*/

	int size()
	{
		return sizeof(tagCMD_S_GameOver);
	}
}CMD_S_GameOver, *PCMD_S_GameOver;

//! 托管
/*! 托管 */
typedef struct tagCMD_S_Trustee
{
	int32							bUserTrustee;				//是否托管	0：取消托管，1：开始托管
	int32							iUserChairID;				//操作的玩家

	int size()
	{
		return sizeof(tagCMD_S_Trustee);
	}
}CMD_S_Trustee, *PCMD_S_Trustee;


//! 玩家重回游戏
/*! 玩家重回游戏，发送重回数据 */
typedef struct tagCMD_S_ReEnterRoom
{
	int32							iBankerUser;							/*!< 庄家 */
	int32 							iBaseScore;								/*!< 基础积分 */
	int32							iIsRandomBanker;						/*!< 是否随机确定庄家 */
	int32							iDiceNum01;								/*!< 骰子数01 确定庄家 */
	int32							iDiceNum02;								/*!< 骰子数02 确定庄家*/
	int32							iDiceNum11;								/*!< 骰子数11 确定拿牌位置*/
	int32							iDiceNum12;								/*!< 骰子数12 确定拿牌位置*/

	int32							iUserChairID;							/*!< 玩家座位号 */

	int32							iHandMJCount[GAME_PLAYER];				/*!< 手牌麻将数目 */
	MJBYTE							bHandMJ[GAME_PLAYER][MAX_HAND_MJ];		/*!< 手牌麻将牌 */
	int32							iHuMJCount[GAME_PLAYER];				/*!< 胡牌麻将数目 */
	MJBlock							bHuMJ[GAME_PLAYER][MAX_BLOCK_COUNT];	/*!< 胡牌麻将 */
	int32							iOutMJCount[GAME_PLAYER];				/*!< 出牌麻将数目 */
	MJBYTE							bOutMJ[GAME_PLAYER][MAX_OUT_MJ];		/*!< 出牌麻将 */

	enPlayerStatu					enUserStatu[GAME_PLAYER];				/*!< 玩家状态 */

	int32							iOldOperUser;							/*!< 操作玩家 */
	int32							iCurrentUser;							/*!< 当前玩家 */

	int32							isOtherPriority;

	int64							iHuFan;										/*!< 胡牌的总番数*/
	int64							iGangFan;									/*!< 刮风下雨的总番数*/
	int64							iHuScore;									/*!< 胡的分数*/
	int64							iGangScore;									/*!< 刮风下雨扣除的分数 */
	int64							iFanStyle[MAX_FAN_STYLE];					/*!< 番数类型*/

	enHuType						enhuType;										/*!< 游戏胡牌类型 */
	int32							iDianPaoUser;								/*!< 点炮的人， 如果是自摸，那么点炮玩家为空 */
	MJBYTE							bDianPaoMJ[GAME_PLAYER];							/*!< 仅限于点炮使用，未胡为0*/

	int32							isForceQuit;								/*< 是否已经被踢出房间 */
	int32							isYiPaoDuoXiang;							/*< 如果有人胡牌，是不是一炮多响 */
	int32							iReadyPlayer;								/*< 如果掉线玩家在发完牌之前回来，检测有几个人已经准备好了 */

	int size()
	{
		return sizeof(tagCMD_S_ReEnterRoom);
	}
}CMD_S_ReEnterRoom, *PCMD_S_ReEnterRoom;

//麻将数据
typedef struct tagCMD_S_MJ
{
	MJBlock							bHUMj[GAME_PLAYER][MAX_BLOCK_COUNT];			/*!< 亮出的牌块(碰杠的牌) */
	MJBYTE							bHUMjCount[GAME_PLAYER];						/*!< 亮出牌块的数量 */
	MJBYTE							bHandMj[GAME_PLAYER][MAX_HAND_MJ];				/*!< 手中的牌 */
	MJBYTE							bHandMjCount[GAME_PLAYER];						/*!< 手中的牌的数量 */
	MJBYTE							bOutMj[GAME_PLAYER][MAX_OUT_MJ];				/*!< 已经出的牌 */
	MJBYTE							bOutMjCount[GAME_PLAYER];						/*!< 已经出的牌的数量 */

	int size()
	{
		return sizeof(tagCMD_S_MJ);
	}
}CMD_S_MJ, *PCMD_S_MJ;

//! 聊天
/*! 聊天消息 */
typedef struct tagCMD_S_Chat
{
	int32							iChatUser;										/*!< 发送聊天消息玩家 */
	int32							iFaceID;										/*!< 表情ID */
	int32							iTalkID;											/*!< 文字ID */
	enChatType						bType;											/*!< 消息类型（表情、文字） */

	int size()
	{
		return sizeof(tagCMD_S_Chat);
	}
}CMD_S_Chat,*PCMD_S_Chat;

/////////////////////////////客户端信息/////////////////////////////////////////////
//! 出牌
/*! 能出牌操作的玩家出牌 */
typedef struct tagCMD_C_OutMJ
{
	MJBYTE							bOutMJ;									/*!< 打出的牌 */
	int size()
	{
		return sizeof(tagCMD_C_OutMJ);
	}
}CMD_C_OutMJ,*PCMD_C_OutMJ;

//! 杠牌
/*! 杠的牌及类型 */
typedef struct tagCMD_C_GangMJ
{
	MJBYTE							bGangMJ;									/*!< 打出的牌 */
	enOperationGangType				bGangType;
	int size()
	{
		return sizeof(tagCMD_C_GangMJ);
	}
}CMD_C_GangMJ,*PCMD_C_GangMJ;

//! 聊天
/*! 聊天消息 */
typedef struct tagCMD_C_Chat
{
	int32							iFaceID;										/*!< 表情ID */
	int32							iTalkID;											/*!< 文字ID */
	enChatType						bType;											/*!< 消息类型（表情、文字） */

	int size()
	{
		return sizeof(tagCMD_C_Chat);
	}
}CMD_C_Chat,*PCMD_C_Chat;

//////////////////////////////////////////////////////////////////////////
class CmdHelper
{
public:
	//////服务端数据
	///////////////////////////// CMD_S_DealMJ /////////////////////////////////////////////
	static bool CodeMessage(tagCMD_S_DealMJ &info, Json::Value &msg);

	static bool DeCodeMessage(tagCMD_S_DealMJ &info, Json::Value &msg);

	////////////////////////////// CMD_S_OutMJ ////////////////////////////////////////////
	static bool CodeMessage(CMD_S_OutMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_OutMJ &info, Json::Value &msg);

	/////////////////////////////// CMD_S_TouchMJ ///////////////////////////////////////////
	static bool CodeMessage(CMD_S_TouchMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_TouchMJ &info, Json::Value &msg);

	///////////////////////////////// CMD_S_PengMJ /////////////////////////////////////////
	static bool CodeMessage(CMD_S_PengMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_PengMJ &info, Json::Value &msg);

	///////////////////////////////// CMD_S_GangMJ /////////////////////////////////////////
	static bool CodeMessage(CMD_S_GangMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_GangMJ &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_GiveUp ///////////////////////////////////////
	static bool CodeMessage(CMD_S_GiveUp &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_GiveUp &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_TRUSTEE ///////////////////////////////////////
	static bool CodeMessage(CMD_S_Trustee &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_Trustee &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_HU ///////////////////////////////////////
	static bool CodeMessage(CMD_S_HU &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_HU &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_GameOver ///////////////////////////////////////
	static bool CodeMessage(CMD_S_GameOver &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_GameOver &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_ReEnterRoom ///////////////////////////////////////
	static bool CodeMessage(CMD_S_ReEnterRoom &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_ReEnterRoom &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_MJ ///////////////////////////////////////
	static bool CodeMessage(CMD_S_MJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_MJ &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_Chat ///////////////////////////////////////
	static bool CodeMessage(CMD_S_Chat &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_Chat &info, Json::Value &msg);

	//////客户端数据
	//////////////////////////////// CMD_C_OutMJ //////////////////////////////////////////
	static bool CodeMessage(CMD_C_OutMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_C_OutMJ &info, Json::Value &msg);

	//////////////////////////////// CMD_C_GangMJ //////////////////////////////////////////
	static bool CodeMessage(CMD_C_GangMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_C_GangMJ &info, Json::Value &msg);

	//////////////////////////////// CMD_C_Chat //////////////////////////////////////////
	static bool CodeMessage(CMD_C_Chat &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_C_Chat &info, Json::Value &msg);

};


#endif

