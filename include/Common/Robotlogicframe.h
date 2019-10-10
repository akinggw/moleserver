#ifndef _ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "ccommon.h"

class Player;
class Room;
class CMolMessageIn;

class RobotLogicFrame
{
public:
	/// 构造函数
	RobotLogicFrame() {}
	/// 析构函数
	virtual ~RobotLogicFrame() {}

	/// 设置当前玩家
	virtual void SetGamePlayer(Player* pPlayer,Room* pRoom) = 0;

	/// 用于处理用户准备后的消息
	virtual void OnProcessPlayerReadyMes(int playerId) = 0;
	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes() = 0;
	/// 用于处理用户结束游戏消息
	virtual void OnProcessPlayerGameOverMes(void) = 0;
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(Json::Value &mes) = 0;
	/// 处理用户进入房间消息
	virtual void OnProcessEnterRoomMsg(int playerId) = 0;
	/// 处理用户离开房间消息
	virtual void OnProcessLeaveRoomMsg(int playerId) = 0;
	/// 处理用户断线重连消息
	virtual void OnProcessReEnterRoomMes(int playerId) = 0;
	/// 处理用户断线消息
	virtual void OnProcessOfflineRoomMes(int playerId) = 0;
	/// 处理用户定时器消息
	virtual void OnProcessTimerMsg(int timerId,int curTimer) = 0;
};

#endif
