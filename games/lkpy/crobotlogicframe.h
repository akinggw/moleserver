#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"

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

	// 消息处理
protected:
  bool OnSubGameStatus(void* data, WORD data_size);
  bool OnSubGameConfig(void* data, WORD data_size);
  bool OnSubExchangeFishScore(void* data, WORD data_size);
  bool OnSubUserFire(void* data, WORD data_size);
  bool OnSubCatchFish(void* data, WORD data_size);
  bool OnSubBulletIonTimeout(void* data, WORD data_size);
  bool OnSubCatSweepFishResult(void* data, WORD data_size);
  bool OnSubSwitchScene(void* data, WORD data_size);

private:
  void ExchangeFishScore();
  void Fire();
  float GetAngle(WORD chair_id);

private:
	Player *m_g_myself;
    Room *m_g_myselfRoom;

private:
  int exchange_ratio_userscore_;
  int exchange_ratio_fishscore_;
  int exchange_count_;

  int min_bullet_multiple_;
  int max_bullet_multiple_;

  BulletKind current_bullet_kind_;
  int current_bullet_mulriple_;
  bool allow_fire_;

  SCORE exchange_fish_score_;
  SCORE fish_score_;

  float last_fire_angle_;
  int exchange_times_;
};

#endif
