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

protected:
	int								 m_bCallScorePhase;			//叫牌阶段
	int								 m_curGameState;			//当前游戏状态

	//组件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑

	//扑克信息
protected:
	uint8							m_bBackCard[3];						//游戏底牌
	int								m_bCardCount[GAME_PLAYER];			//扑克数目
	int								m_bOutCardCount[GAME_PLAYER];		//出牌次数
	uint8							m_bHandCardData[GAME_PLAYER][20];	//手上扑克

	uint8							m_cbLastTurnOutCard[GAME_PLAYER][MAX_COUNT];	//上轮出牌数据
	uint8							m_cbNowTurnOutCard[GAME_PLAYER][MAX_COUNT];		//本轮出牌数据
	int								m_nLastTurnOutCount[GAME_PLAYER];				//上轮出牌数目
	int								m_nNowTurnOutCount[GAME_PLAYER];				//本轮出牌数目

	//游戏变量
protected:
	int							m_cbCount;							//防止重发底牌
	int							m_lSnatchLand;						//抢地主倍数
	int							m_lBrightCard;						//明牌倍数
	int							m_wBombTime;						//炸弹倍数
	int							m_lThuntian;						//春天倍数
	int							m_bLandScore;						//地主分数
	int							m_wShowTimes;						//显示倍数
	uint16						m_wFirstUser;						//首叫用户
	uint16						m_wBankerUser;						//庄家用户
	uint16						m_wCurrentUser;						//当前玩家
	int							m_bScoreInfo[3];					//叫分信息
	uint16						m_wChairID;							//延时结束椅子号
	int							m_breSendCardTimes;					//重新发牌次数
	bool						m_GameOver;							//游戏结束
	uint16						m_GameOverChair;					//游戏结束断线回来玩家的椅子号
	bool						m_UserStandUp[GAME_PLAYER];			//用户起来标识

	//运行信息
protected:
	uint16							m_wTurnWiner;						//胜利玩家
	int								m_bTurnCardCount;					//出牌数目
	uint8							m_bTurnCardData[20];				//出牌列表
	bool							m_bUserTrustee[GAME_PLAYER];		//玩家托管
	bool                            m_bUserOffLine[GAME_PLAYER];        //玩家掉线状态
	bool							m_PassTime[GAME_PLAYER];			//断线“不出”时间
	bool							m_bUserPass[GAME_PLAYER];			//玩家“不出”
	bool							m_bEndFlag;
	bool							m_bYaPaiFlag;
	bool							m_bEndOrReadyFlag;					//排队或结束阶段
	//bool							m_FirstUserLife;					//结算阶段是不是第一个玩家离开
	bool							m_wCloseWarmingFlash;
	int                             m_UserLeaveCount;					//玩家离开个数
	bool							m_MatchOver;						//比赛场结束一局标识

	//叫分阶段变量
protected:
	bool							m_bBrightCard[GAME_PLAYER];			//明牌模式
	bool							m_bGiveUpBanker[GAME_PLAYER];		//放弃标识
	int							    m_cbLandScoreCount;					//叫分次数

protected:
	int								m_BoomCount;					//已出炸弹数(用于jackPot)
};

#endif
