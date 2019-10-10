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

private:
	/// 用于处理用户登录消息
	void OnProcessUserLoginMes(uint32 connId,Json::Value &mes);
    /// 先处理游戏框架消息
    void OnProcessFrameMes(uint32 connId,Json::Value &mes);
    /// 用于处理用户准备消息
    void OnProcessGameReadyMes(uint32 connId,Json::Value &mes);
    /// 用于处理用户离开消息
    void OnProcessGameLeaveRoomMes(uint32 connId,Json::Value &mes);
    /// 用于处理用户加入房间
    bool JoinPlayerToGameRoom(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue);

private:
    /// 发送指定玩家登陆成功的消息
    void SendPlayerLoginSuccess(CPlayer *pPlayer);
    /// 更新玩家信息
    void OnProcessUserInfo(CPlayer *pPlayer);
    /// 更新玩家身上的钱
    void UpdatePlayerMoney(Player *pPlayer);
    /// 用于处理用户进入房间
    bool AddPlayerInServer(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue,bool isGaming);
};

#define ServerGameFrameManager GameFrameManager::getSingleton()

#endif
