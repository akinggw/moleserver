#include "crobotlogicframe.h"
#include "GameLogic.h"

// 定时器标识
#define IDI_SWITCH_SCENE				  1									// 切换场景不能打炮
#define IDI_EXCHANGE_FISHSCORE    2                 // 交换子弹
#define IDI_FIRE                  3
#define IDI_STAND_UP              4

// 时间标识
#define TIME_SWITCH_SCENE				  7									// 切换场景不能打炮 大概7秒钟 客户端有修改 这个应该也要修改
#define TIME_EXCHANGE_FISHSCORE   2
#define TIME_FIRE                 2											// 这里不能改成1 会造成死循环, 如果想要加快子弹发射速度只能改平台.

/// 构造函数
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{
  exchange_ratio_userscore_ = 1;
  exchange_ratio_fishscore_ = 1;
  exchange_count_ = 1;
  exchange_times_ = 1;
  allow_fire_ = false;
  current_bullet_kind_ = BULLET_KIND_2_NORMAL;
  current_bullet_mulriple_ = 10;
  exchange_fish_score_ = 0;
  fish_score_ = 0;
  last_fire_angle_ = 0.f;
  min_bullet_multiple_ = 1;
  max_bullet_multiple_ = 9900;
}

/// 析构函数
CRobotLogicFrame::~CRobotLogicFrame()
{

}

/// 用于处理用户准备后的消息
void CRobotLogicFrame::OnProcessPlayerReadyMes(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;
}

/// 用于处理用户开始游戏开始消息
void CRobotLogicFrame::OnProcessPlayerGameStartMes()
{

}

/// 用于处理用户结束游戏消息
void CRobotLogicFrame::OnProcessPlayerGameOverMes(void)
{

}

/// 用于处理用户进入游戏房间后的消息
void CRobotLogicFrame::OnProcessPlayerRoomMes(Json::Value &mes)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

}

/// 处理用户进入房间消息
void CRobotLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

}

/// 处理用户离开房间消息
void CRobotLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{

}

/// 处理用户断线消息
void CRobotLogicFrame::OnProcessOfflineRoomMes(int playerId)
{

}

/// 处理用户断线重连消息
void CRobotLogicFrame::OnProcessReEnterRoomMes(int playerId)
{

}

/// 处理用户定时器消息
void CRobotLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
