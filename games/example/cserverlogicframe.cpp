#include "cserverlogicframe.h"
#include <sstream>

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{

}

/// 析构函数
CServerLogicFrame::~CServerLogicFrame()
{

}

/// 用于处理用户开始游戏开始消息
void CServerLogicFrame::OnProcessPlayerGameStartMes()
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;
}

/// 用于处理用户进入游戏房间后的消息
void CServerLogicFrame::OnProcessPlayerRoomMes(int playerId,Json::Value &mes)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

}

/// 处理用户进入房间消息
void CServerLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    std::stringstream ss;
    ss << "玩家" << playerId << "进入房间了.";
	m_g_GameRoom->Room_Log(BASIC,ss.str());

	//m_g_GameRoom->GameStart();

	m_g_GameRoom->StartTimer(100,10);
}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    std::stringstream ss;
    ss << "玩家" << playerId << "离开房间了.";
	m_g_GameRoom->Room_Log(BASIC,ss.str());

	m_g_GameRoom->StopTimer(100);
}

/// 处理用户断线消息
void CServerLogicFrame::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    std::stringstream ss;
    ss << "玩家" << playerId << "从房间掉线了.";
	m_g_GameRoom->Room_Log(BASIC,ss.str());
}

/// 处理用户断线重连消息
void CServerLogicFrame::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    std::stringstream ss;
    ss << "玩家" << playerId << "重回房间了.";
	m_g_GameRoom->Room_Log(BASIC,ss.str());
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if(timerId == 100 && curTimer <= 0)
	{
	    m_g_GameRoom->StopTimer(100);

	    m_g_GameRoom->Room_Log(BASIC,"执行一个定时器消息.");

        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_ROOM;
        root["MsgSubId"] = IDD_MESSAGE_ROOM + 10;

	    m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

	    m_g_GameRoom->StartTimer(100,10);
	}

}
