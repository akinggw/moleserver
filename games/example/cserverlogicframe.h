#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"

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
};

#endif
