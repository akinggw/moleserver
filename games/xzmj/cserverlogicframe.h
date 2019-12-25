#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "CmdHelper.h"
#include "GameLogic.h"
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

class CServerLogicFrame : public ServerLogicFrame
{
public:
	/// 构造函数
	CServerLogicFrame();
	/// 析构函数
	~CServerLogicFrame();

	/// 设置当前应用房间
	virtual void SetGameRoom(Room* pRoom) { m_g_GameRoom = pRoom; }
	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes();
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
	/// 处理用户进入房间消息
	virtual void OnProcessEnterRoomMsg(int playerId);
	/// 处理用户离开房间消息
	virtual void OnProcessLeaveRoomMsg(int playerId);
	/// 处理用户断线重连消息
	virtual void OnProcessReEnterRoomMes(int playerId);
	/// 处理用户断线消息
	virtual void OnProcessOfflineRoomMes(int playerId);
	/// 处理用户定时器消息
	virtual void OnProcessTimerMsg(int timerId,int curTimer);

	//////////////////////////////////////////////////////////////////////////
	void Reset();
	//////////////////////////////////////////////////////////////////////////

	void OperOut(int iChairID, CMD_C_OutMJ &cmdOut, bool isTimeOut = false);

	void OperAUTO(int iChairID, bool isTimeOut);

	void OperTouch(int iChairID, int iTouchTpye = 0);

	void OperPeng(int playerId);

	void OperGang(int iChairID, CMD_C_GangMJ &cmdGang);

	void FanCount(int &iChairID, MJBYTE bMJ, int FanStyle[], enHuType enhuType, int64 &iHuPaiFan, int64 &iExtraFan);

	void LookHuaZhuDaJiao(CMD_S_GameOver &cmdsGameOver);

	void OperHu(int playerId, int iDianPaoPlayer, enHuType enhuType);

	void OperGameOver(enEndGameType endGameType);

	void OperGiveUp(int playId);

	void OperTrustee(int iChairID, bool isTimeOut = false);

	//iWalkType行牌类型 0表示摸牌 1表示出牌
	bool isHu(int &iChairID, MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfHandMJ[], int iSelfHandMJCount, MJBYTE bMJ, bool iWalkType = false);

	void UpdateOperation(int &iChairID, MJBYTE &iOutMJ, MJBlock bHuMJ[], MJBYTE iHuMJCount, MJBYTE bHandMJ[], MJBYTE iHandMJCount, MJBYTE bOutMJ[], MJBYTE iOutMJCount, int &OperationType, bool iWalkType = false);
	int  UpdatePriority(int &iChairID, int &iOutPlayer);

	//取得玩家结束时的状态
	void getUserEndType(int iHuUser, enHuType enhuType, enUserEndType enUSEendType[]);

	void GoodMJToRobot(MJBYTE bAllMJ[], int iAllMJCount, int iChairID);

#ifdef ENABLE_DEBUG_TOOL
	BOOL ForTest();
#endif

//枚举定义 用枚举替换所有的宏
private:

	enum Timer_Msg
	{
		//服务器定时器的使用
		TIMER_Nomal	= 1,				//自动出牌
		TIMER_BUFFER,				//玩家托管或者听牌后，延迟2秒出牌。等待客户端的动画完成
		TIMER_CHANGETABLE,			//换桌
		TIMER_USER_OFFLINE,			//玩家掉线

		TIMER_QUIT = 9,
		TIMER_STRATDROP,			//玩家开局掉线
		TIMER_START,
		TIMER_DELAY_READY,
		TIMER_DHU,
		TIMER_DHUNomal,
		TIMER_NewNomal,				//代替自动出牌正常
	};

	//其他的相关宏定义
	enum Other_HongDefine
	{
		GuaFengMaxCount = 16,
	};

	//刮风下雨相关宏定义
	enum GuaFengRatio
	{
		GuaFeng_Null = 0,
		GuaFeng_Ming,
		GuaFeng_An_ZiMoMing,
	};
//结构体的定义
private:
	/*!< 玩家刮风下雨操作信息 */
	typedef struct tagGuaFengInfo
	{
		enOperationGangType			eGangType;										/*!< 杠的类型 */
		//MJBYTE						eMJ;
		int							eGangPlayer;									/*!< 杠的玩家 */
		int							eYinGanger[GAME_PLAYER];						/*!< 引杠的玩家,-1为没有人引杠，0123表示引杠玩家ID */
		int64							eGangSorce[GAME_PLAYER];										/*!< 杠的分数，为operhu提供方便 */
	}GuaFengInfo;

	/*!< 玩家可用操作信息 */
	typedef struct tagOperationInfo
	{
		MJFLAG						bOperation;										/*!< 玩家可用操作 */
		enOperationGangType			eGangType;										/*!< 如果有杠，杠的类型。只能一种 */
		int32						iGangCount;										/*!< 杠数目 */
		MJBYTE						bMJGang[MAX_SELECT_GANG];						/*!< 可杠的牌 */
	}OperationInfo;

	/*!< 玩家过水不胡信息 */
	typedef struct tagGuoShuiInfo
	{
		MJBYTE						bGuoShuiMJ;
		int							bGSStartUser;
		int							bWhoHuUser;
		int							bGangPao;
	}GuoShuiInfo;

	/*!< 玩家胡牌信息 */
	typedef struct tagEnHuInfo
	{
		enHuType					iHuInfoType;
		int							iHuUser;
		int							iDianPaoUser;
		MJBYTE						bHuMJ;
		int64							iHuFan;
		int64							iExtraFan;
		int							iFanStyle[MAX_FAN_STYLE];
	}EnHuInfo;

private:
	Room *m_g_GameRoom;                                  /**< 游戏房间 */

	CMahJongLogic				*m_pGameLogic;

	GuaFengInfo					m_GuaFengInfo[GuaFengMaxCount];						/*!< 存储刮风下雨相关信息，用于返回玩家刮风下雨得分 */
	OperationInfo				m_OperationInfo[GAME_PLAYER];						/*!< 玩家的相关信息 */
	GuoShuiInfo					m_GuoShuiInfo[GAME_PLAYER];							/*!< 过水不胡标志 */
	EnHuInfo					m_HuInfo[GAME_PLAYER];								/*!< 玩家胡的信息 */
	int64							m_AllGangSorce[GAME_PLAYER];						/*!< 玩家杠的总分，便于总结算和断线时使用 */
	int64							m_AllHuSorce[GAME_PLAYER];							/*!< 玩家胡的总分，便于总结算使用 */
	int64							m_AllTotal[GAME_PLAYER];							/*!< 玩家最后的总分，便于总结算使用 */

	static const MJBYTE			bOriginalMJ[MAX_ORG_MJ];							/*!< 原始的麻将数据 */
	int							m_iBisicCore;
	MJBYTE						m_bAllMJ[MAX_ORG_MJ];								/*!< 麻将牌数据 */
	int							m_iTouchPos;										/*!< 摸牌位置 */
	int							m_iDiceNum01;										/*!< 骰子数1 */
	int							m_iDiceNum02;										/*!< 骰子数2 */
	int							m_iDiceNum11;										/*!< 骰子数2 */
	int							m_iDiceNum12;										/*!< 骰子数2 */
	int							m_iIsReRandomBanker;								/*!< 4个玩家一起玩牌的局数 */

	int							m_OperPriority[GAME_PLAYER];						/*!< 各玩家可用操作优先级 */
	int							m_TruBuffer[GAME_PLAYER];
	int							m_iBankerUser;										/*!< 庄 */

	//////////////////////////////////////////////////////////////////////////
	//出牌玩家 m_iOldOperateUer和 m_iOldCurrentUer可能不相等
	int							m_iOldOperateUer;									/*!<  上次操作玩家*/
	int							m_iOperateUer;										/*!<  当前操作玩家*/
	int							m_iOldCurrentUer;									/*!<  上一次出牌的操作玩家*/
	bool						m_StartDrop;

	MJBYTE						m_bTouchMJ;											/*!< 摸起来的牌 */
	MJBYTE						m_bGangKaiMJ[GAME_PLAYER];							/*!< 杠开的牌 */
	MJBYTE						m_bGangTouchMJ;										/*!< 杠摸起的牌 */
	bool						m_isLiuLie;											/*!< 是否流泪 */
	int							m_isLiuLieUser[GAME_PLAYER];
	//bool						m_LastOutMJLose;									/*!< 最近一次出牌麻将是否消失 */
	MJBlock						m_bHUMJ[GAME_PLAYER][MAX_BLOCK_COUNT];				/*!< 亮出的牌块(吃碰杠的牌) */
	int							m_iHUMJCount[GAME_PLAYER];							/*!< 亮出牌块的数量 */
	MJBYTE						m_bHandMJ[GAME_PLAYER][MAX_HAND_MJ];				/*!< 手中的牌 */
	int							m_iHandMJCount[GAME_PLAYER];						/*!< 手中的牌的数量 */
	MJBYTE						m_bOutMJ[GAME_PLAYER][MAX_OUT_MJ];					/*!< 已经出的牌 */
	int							m_iOutMJCount[GAME_PLAYER];							/*!< 已经出的牌的数量 */
	int							m_iAUTO_OutCount[GAME_PLAYER];						/*!< 服务端代打次数，连续代打超过两次就自动托管 */

	int							m_TouchPlayer;										//当前摸牌玩家

	enPlayerStatu				enUserStatu[GAME_PLAYER];							/*!< 玩家状态 */
	enPlayerStatu				enUserStatuOld[GAME_PLAYER];						/*!< 玩家掉线前的状态 */

	int							m_operationGaveUp[GAME_PLAYER];						/*!< 放弃胡的人,初始化为operation_hu都能胡，放弃后置为0，作用给放弃操作更新操作时，不在检测胡操作 */
	int							m_bQiangGangMJ[GAME_PLAYER];						/*!< 抢杠胡的那张麻将，只要能胡的玩家，每个人都存一份 */
	int							m_isAlreadyHu;										/*!< 是否已经胡过那张牌，下次出牌时情况 */

	//int							m_isTianDiHu;										/*!< 0什么都没有，1天胡 2地胡 */
	int							m_UserTouchFirst[GAME_PLAYER];						/*!< 玩家第一次摸牌，没有摸为0，摸了牌为1，为地胡准备 */
	int							m_isForceQuit[GAME_PLAYER];							/*!< 是否有强制退出玩家*/

	bool						m_StartQuit;										/*!< 开局的时候是否有玩家强退*/
	int64							m_QuitSorce[GAME_PLAYER];							/*!< 玩家强退扣的分*/
	MJBYTE						m_huMJ;												/*!< 胡的那张麻将*/

	int							m_zuozhuang;										/*!< 坐庄问题*/
	int							m_twice;
	int							m_DoubleHuPlayer;									/*!< 一炮多响时，要胡的人托管*/
	int							m_iOldHandCount[GAME_PLAYER];
	int							m_HasHuTouch;
	int							m_QiangHuCount;										/*!<抢杠胡的人数，-1没有赋值*/
	int							m_GiveQiangHuCount;									/*!<抢杠胡的人数，-1没有赋值*/
	int							m_BeiQiangUser;
	int							m_ReadyCount;
	int64						m_UserMoney[GAME_PLAYER];
	CMD_S_GameOver				m_CmdsGameOver;

	int							m_StartUserID[GAME_PLAYER];
	int							m_ForceUserID[GAME_PLAYER];
	int							m_huRanderUser[GAME_PLAYER];
	int							m_HuPos;
#ifdef ENABLE_DEBUG_TOOL
	BOOL						m_IsDebug;											//测试用
#endif
};

#endif
