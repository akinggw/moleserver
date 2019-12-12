#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"

#define IDD_TIMER_GAME_XIAZHU1     IDD_TIMER_GAME_START+1                   // 下注

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

	int m_curZhuang;
	bool m_isAddZhuang;
};

#endif
