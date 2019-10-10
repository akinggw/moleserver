#ifndef _SERVER_LOGIC_FRAME_H_INCLUDE_
#define _SERVER_LOGIC_FRAME_H_INCLUDE_

#include "ccommon.h"

class Room;
class CMolMessageIn;

class ServerLogicFrame
{
public:
	/// 构造函数
	ServerLogicFrame() {}
	/// 析构函数
	virtual ~ServerLogicFrame() {}

	/// 设置当前应用房间
	virtual void SetGameRoom(Room* pRoom) = 0;
	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes() = 0;
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes) = 0;
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
