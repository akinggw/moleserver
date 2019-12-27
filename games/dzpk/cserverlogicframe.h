#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include <map>
#include <vector>

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

private:
	Room *m_g_GameRoom;                                  /**< 游戏房间 */
	CGameLogic					m_GameLogic;										//游戏逻辑


	std::map<uint32,std::string>	m_sUserName;


	int							m_nCurrentUser;				 						//当前玩家
	int							m_nBankUser;										//庄家
	int							m_nSmallBlindUser;									//小盲注玩家
	int							m_nBigBlindUser;									//大盲注玩家

	int							m_nAddCount;										//加注次数

	int64						m_nBasicScore;										//基础下注
	int64						m_nTurnBasicGold;									//本轮下注最少下注
	int64						m_nTurnBasicAdd;									//本轮下注最少加注
	int64						m_nTurnAddGold[IDD_MAX_ROOM_USERCOUNT];				//本轮玩家下注
	int64						m_nAddGold[IDD_MAX_ROOM_USERCOUNT];					//玩家加注
	int64						m_nUserGold[IDD_MAX_ROOM_USERCOUNT];				//玩家筹码
	int64						m_nTurnStartGold[IDD_MAX_ROOM_USERCOUNT];


	bool						m_bPlayStates[IDD_MAX_ROOM_USERCOUNT];				//玩家状态
	bool						m_bAddStates[IDD_MAX_ROOM_USERCOUNT];				//加注状态
	int							m_nAllPlayUserCount;								//可以下注玩家数

	OperateType					m_UserOperate[IDD_MAX_ROOM_USERCOUNT];


	int64						m_nAddList[IDD_MAX_ROOM_USERCOUNT];					//下注数目列表
	int							m_nAddUserCount;									//下注玩家数目
	int64						m_nPot[IDD_MAX_ROOM_USERCOUNT];						//彩池筹码情况
	int							m_nPotCount;										//彩池数目


	//牌数据信息
	WORD						m_wCardData[IDD_MAX_CARD_COUNT];					//牌堆
	int							m_nSendCardPos;										//发牌下标

	WORD						m_wHandCard[IDD_MAX_ROOM_USERCOUNT][IDD_HAND_COUNT];		//手牌数据
	WORD						m_wBoardData[IDD_WEAVE_COUNT];						//公共牌数据
	int							m_nBoardCount;										//公共牌数目

	//int64						m_nMaxTurnAdd;

	GameEndType					m_endType;



	//用户加注
	void OnUserAddGold(int playerId, int64 nAddGold);
	//用户放弃
	void OnUserGiveUp(int playerId);
	//游戏结束
	void OnGameOver(GameEndType endType);


	//加注结束
	bool isAddOver();
	//彩池统计
	void StatisticsPot();
	//下盲注
	void Blind();
	//发手牌
	void DealHandCard();
	//发公共牌
	void DealBoardCard(int dealCardCount);
	//下一个玩家
	int GetNextPlayer(int playerId);
	//清空数据
	void ClearData();
	//获取本轮最大下注
	int64 GetMaxTurnAddd();

#ifdef _DEBUG
	bool ForTest();
#endif
};

#endif
