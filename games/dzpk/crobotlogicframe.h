#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"

class CRobotLogicFrame : public RobotLogicFrame
{
public:
	/// 构造函数
	CRobotLogicFrame();
	/// 析构函数
	~CRobotLogicFrame();

	/// 设置当前玩家
	virtual void SetGamePlayer(Player* pPlayer,Room* pRoom) { m_g_myself = pPlayer; m_g_myselfRoom = pRoom;}

	/// 用于处理用户准备后的消息
	virtual void OnProcessPlayerReadyMes(int playerId);
	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes();
	/// 用于处理用户结束游戏消息
	virtual void OnProcessPlayerGameOverMes(void);
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(Json::Value &mes);
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

private:
	Player *m_g_myself;
    Room *m_g_myselfRoom;

	CGameLogic m_GameLogic;


	int64				m_nUserGold[IDD_MAX_ROOM_USERCOUNT];		//拥有筹码
	int64				m_nBasicScore;								//基础下注

	int64				m_nTurnAddGold[IDD_MAX_ROOM_USERCOUNT];		//加注

	WORD				m_wHandCard[IDD_HAND_COUNT];				//手牌

	int64				m_nTurnBasicAdd;							//本轮最少加注
	int64				m_nTurnBasicGold;							//本轮最少跟注

	//int64				m_nMaxTurnAdd;								//本轮最大下注

	int					m_nBankUser;								//庄家
	int					m_nSmallBlindUser;							//小盲注
	int					m_nBigBlindUser;							//大盲注
	int					m_nCurrentUser;								//当前玩家


	WORD				m_wBoartCard[IDD_WEAVE_COUNT];				//公共牌数据
	int					m_nBoartCount;								//公共牌数目

	WORD				m_wBestType;								//最佳牌型
	WORD				m_wBestWeave[IDD_WEAVE_COUNT];				//最佳组合

	int					m_winType;									//控制输赢。
	int					m_nAddCount;								//加注次数

	bool				m_bIsGameOver;

	int64				m_nWinGold[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_ROOM_USERCOUNT];

public:
	//ai下注
	void OnAiAddGold();
	//
	void ClearData();
};

#endif
