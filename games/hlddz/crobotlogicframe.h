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

	//组件变量
protected:
	CGameLogic						m_GameLogic;						//游戏逻辑

	//扑克变量
protected:
	uint16 							m_cbTurnCardCount;					//出牌数目
	uint8							m_cbTurnCardData[20];				//出牌列表
	uint16							m_wOutCardUser;						//出牌玩家
	uint16							m_wBankerUser;						//地主玩家

	//手上扑克
protected:
	uint8							m_cbBackCard[3];					//底牌数据
	uint16 							m_cbHandCardCount;					//扑克数目
	uint8							m_cbHandCardData[20];				//手上扑克
	uint16 							m_cbCurrentLandScore;				//已叫分数

	//叫分阶段变量
public:
	int							m_cbCallScorePhase;					//抢牌阶段
};

#endif
