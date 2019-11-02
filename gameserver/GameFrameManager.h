#ifndef _GAME_FRAME_MANAGER_H_INCLUDE_
#define _GAME_FRAME_MANAGER_H_INCLUDE_

#include "../../include/Common/defines.h"
#include "../../include/molnet/Network.h"

#include "gameframe/DBOperator.h"
#include "gameframe/CPlayer.h"

#include <string.h>

using namespace network;

class GameFrameManager : public Singleton<GameFrameManager>
{
public:
	/// 构造函数
	GameFrameManager();
	/// 析构函数
	~GameFrameManager();

	/// 用于处理接收到的网络消息
	void OnProcessNetMes(uint32 connId,CMolMessageIn *mes);

	/// 用于处理接收网络连接消息
	void OnProcessConnectedNetMes(uint32 connId);

	/// 用于处理用于断开网络连接消息
	void OnProcessDisconnectNetMes(uint32 connId);

	/// 更新玩家信息
	void UpdatePlayerInfo(Player *pPlayer);
	/// 更新房间信息
	void UpdateGameRoomInfo(void);
    /// 更新玩家信息
    void OnProcessUserInfo(CPlayer *pPlayer);
    /// 玩家断线重回房间
    bool OnProcessReEnterRoomMes(uint32 connId);
    /// 新建一个新的玩家
    CPlayer* CreateNewPlayer(void);
    /// 发送指定玩家登陆成功的消息
    void SendPlayerLoginSuccess(CPlayer *pPlayer);
    /// 用于处理用户加入房间
    bool JoinPlayerToGameRoom(CPlayer *pPlayer,int pRoomIndex=-1,int pChairIndex=-1,bool isQueue=true);

private:
	/// 用于处理用户登录消息
	void OnProcessUserLoginMes(uint32 connId,Json::Value &mes);
    /// 先处理游戏框架消息
    void OnProcessFrameMes(uint32 connId,Json::Value &mes);
    /// 用于处理用户准备消息
    void OnProcessGameReadyMes(uint32 connId,Json::Value &mes);
    /// 用于处理用户离开消息
    void OnProcessGameLeaveRoomMes(uint32 connId,Json::Value &mes);

public:
	/// 加入一个玩家到排队列表中
	void AddQueueList(CPlayer *connId);
	/// 删除一个玩家从排队列表中
	void DelQueueList(uint32 connId);
	/// 更新排队玩家列表
	void UpdateQueueList(void);
	/// 更新机器人
	void UpdateRobot(void);
	/// 清空排队列表
	void ClearQueueList(void);

public:
	/// 发送当前排队人数
	void SendQueuingCount(void);
	/// 得到当前排队人数
    int GetCurrentQueuePlayerCount(void);
	/// 得到当前排队人数
	int GetQueueRealPlayerCount(void);
	/// 得到当前排队机器人数
	int GetQueueRobotPlayerCount(void);

private:
    /// 更新玩家身上的钱
    void UpdatePlayerMoney(Player *pPlayer);
    /// 用于处理用户进入房间
    bool AddPlayerInServer(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue,bool isGaming);

private:
	std::map<uint32,CPlayer*> m_PlayerQueueList;                 /**< 玩家排队列表 */
	Mutex m_PlayerQueueListLock;                                 /**< 玩家排队列表锁 */
};

#define ServerGameFrameManager GameFrameManager::getSingleton()

#endif
