#ifndef _ROOM_MANAGER_H_INCLUDE_
#define _ROOM_MANAGER_H_INCLUDE_

#include "../../include/molnet/Network.h"

#include "CRoom.h"
#include "CPlayer.h"

#include <vector>

class RoomManager : public Singleton<RoomManager>
{
public:
	RoomManager();
	~RoomManager();

	/// 清除所有的房间
	void ClearAllRooms(void);

	/// 添加一个房间到房间管理器中
	void AddRoom(CRoom *pRoom);
	/// 添加一个玩家到房间中
	bool AddPlayer(CPlayer *pPlayer,int roomIndex=-1,int chairIndex=-1,bool isQueue=true,bool isGaming=true);
	/// 添加一个旁观玩家到房间中
	bool AddLookOnPlayer(CPlayer *pPlayer,int roomIndex,int chairIndex);
	/// 改变一个玩家的游戏房间
	bool ChangeGameRoom(CPlayer *pPlayer);
	/// 从房间中清除指定的玩家
	void ClearPlayer(CPlayer *pPlayer);
	/// 从当前房间中删除指定的玩家
	bool DeletePlayer(CPlayer *pPlayer);
	/// 重置所有的游戏房间
	void ResetAllGameRooms(void);
	/// 根据房间ID号得到房间
	CRoom* GetRoomById(int id);
	/// 得到一个空房间
	CRoom* GetEmptyRoom(void);
	/// 得到一个未满但不为空的房间
	CRoom* GetUnderFullRoom(void);
	/// 得到一个空闲的房间
	CRoom* GetFreeRoom(void);
	/// 得到当前系统中的房间列表
	inline std::vector<CRoom*>& GetRoomList(void) { return m_RoomList; }
	/// 得到当前系统中房间数量
	inline int GetRoomCount(void) { return (int)m_RoomList.size(); }
	/// 得到当前房间中还有多少人
	int GetRoomPlayerCount(void);
	/// 锁住当前房间
	inline void LockRoomList(void) { m_RoomLock.Acquire(); }
	/// 解锁当前房间
	inline void UnlockRoomList(void) { m_RoomLock.Release(); }

	/// 用于处理接收到的网络消息
	void OnProcessNetMes(uint32 connId,Json::Value &mes);
	/// 用于处理用于断开网络连接消息
	void OnProcessDisconnectNetMes(uint32 connId);
	/// 用于处理玩家改变游戏房间
	void OnProcessChangeGameRoomMes(uint32 connId);

	///时间事件
	bool OnEventTimer(uint32 dwTimerID, uint32 wBindParam);

private:
	std::vector<CRoom*> m_RoomList;           /**< 房间列表 */
	Mutex m_RoomLock;                      /**< 用于保护当前服务器上所有的房间 */
};

#define ServerRoomManager RoomManager::getSingleton()


#endif
