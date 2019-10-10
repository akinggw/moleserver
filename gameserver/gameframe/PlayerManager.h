#ifndef _PLAYER_MANAGER_H_INCLUDE_
#define _PLAYER_MANAGER_H_INCLUDE_

#include "../../include/molnet/Network.h"
#include "CPlayer.h"

#include <map>

class PlayerManager : public Singleton<PlayerManager>
{
public:
	PlayerManager();
	~PlayerManager();

	/// 清除所有的玩家
	void ClearAllPlayers(void);
	/// 添加一个玩家到系统中
	bool AddPlayer(CPlayer *pPlayer);
	/// 添加一个机器人到系统中
	bool AddRobot(CPlayer *pPlayer);
	/// 根据玩家的连接ID设置玩家
	void Reset(CPlayer *pPlayer);
	/// 得到空闲的机器人
	CPlayer* GetFreeRobot(void);
	/// 得到空闲机器人个数
	int GetFreeRobotCount(void);
	/// 从系统中删除指定的玩家
	void ClearPlayer(CPlayer *pPlayer);
	/// 从系统中摘除指定的玩家
	void DeletePlayer(CPlayer *pPlayer);
	///// 从系统中删除指定网络ID的玩家
	//void ClearPlayerByconnId(uint32 connId);
	/// 通过客户端连接ID得到客户端
	CPlayer* GetPlayer(uint32 connId);
	/// 通过客户端连接ID得到刚登陆的客户端
	CPlayer* GetNewPlayer(uint32 connId);
	/// 通过玩家ID得到客户端
	CPlayer* GetPlayerById(uint32 id);
	/// 得到玩家总的输赢
	int64 GetPlayerTotalResult(void);
	/// 得到机器人总的输赢
	int64 GetRobotTotalResult(void);
	/// 得到当前系统玩家个数
	inline int GetPlayerCount(void) { return (int)m_PlayerList.size(); }
	/// 得到当前系统机器人个数
	inline int GetRobotCount(void) { return (int)m_RobotList.size(); }
	/// 得到当前系统总的玩家人数
	inline int GetTotalCount(void) { return GetPlayerCount() + GetRobotCount() + GetLostPlayerCount(); }
	/// 锁定当前玩家
	inline void LockPlayerList(void) { m_PlayersLock.Acquire(); }
	/// 解锁当前玩家
	inline void UnlockPlayerList(void) { m_PlayersLock.Release(); }
	/// 得到在线玩家列表
	inline std::map<uint32,CPlayer*>& GetPlayerList(void) { return m_PlayerList; }
	/// 得到在线机器人列表
	inline std::map<uint32,CPlayer*>& GetRobotList(void) { return m_RobotList; }
	/// 得到掉线玩家列表
	inline std::map<uint32,CPlayer*>& GetLostPlayerList(void) { return m_LostPlayerList; }
	/// 给所有在线玩家提示座位信息
	void UpdatePlayersTableInfo(void);

	/// 发送消息给当前在线的所有玩家
	void SendMsgToEveryone(Json::Value &msg);
	/// 发送消息给除了指定玩家的所有在线玩家
	void SendMsgToOtherPlayer(uint32 connId,Json::Value &msg);
	/// 发送消息给指定ID的玩家
	void SendMsgToPlayer(uint32 UserID,Json::Value &msg);

	///时间事件
	bool OnEventTimer(uint32 dwTimerID, uint32 wBindParam);

	/// 添加一个玩家到掉线列表中
	void AddLostPlayer(CPlayer *pPlayer);
	/// 根据ID得到掉线玩家
	CPlayer* GetLostPlayer(uint32 userId);
	/// 根据ID删除掉线玩家
	void DeleteLostPlayer(uint32 userId);
	/// 得到掉线玩家人数
	inline int GetLostPlayerCount(void) { return (int)m_LostPlayerList.size(); }
	/// 得到检测到的已经掉线的玩家列表
	void GetRealLostPlayerList(void);

private:
	std::map<uint32,CPlayer*> m_PlayerList;                 /**< 玩家列表 */
	std::map<uint32,CPlayer*> m_RobotList;                  /**< 机器人列表 */
	std::map<uint32,CPlayer*> m_LostPlayerList;             /**< 掉线玩家列表 */

	Mutex m_PlayersLock;                                    /**< 保护当前在线玩家 */
};

#define ServerPlayerManager PlayerManager::getSingleton()

#endif
