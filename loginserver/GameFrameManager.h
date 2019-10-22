#ifndef _GAME_FRAME_MANAGER_H_INCLUDE_
#define _GAME_FRAME_MANAGER_H_INCLUDE_

#include "../include/molnet/Network.h"
#include "../include/Common/defines.h"
#include <string.h>

#include <jsoncpp/json/json.h>

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

private:
	/// 处理用户登录系统消息
	void OnProcessUserLoginMes(uint32 connId,Json::Value &mes);
	/// 处理用户信息更改消息
	void OnProcessUserInfoUpdateMes(uint32 connId,Json::Value &mes);
	/// 处理用户注册消息
	void OnProcessUserRegisterMes(uint32 connId,Json::Value &mes);
	/// 处理得到当前在线服务器列表
	void OnProcessUserGetOnlineRoomMes(uint32 connId);
	/// 处理钱包消息
	void OnProcessUserBankMes(uint32 connId,Json::Value &mes);
	/// 得到玩家的金币
	bool GetUserMoney(uint32 UserId,int64 *money,int64 *bankmoney);
	/// 得到所有的游戏信息
	void OnProcessGetGamesMes(uint32 connId,Json::Value &mes);
};

#define ServerGameFrameManager GameFrameManager::getSingleton()

#endif
