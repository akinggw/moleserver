#include "crobotlogicframe.h"
#include "GameLogic.h"

/// 构造函数
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL), m_isAddZhuang(false)
{
	m_curZhuang = -1;
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

	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_ROOM_STARTXIAZHU:
		{
			int pGameState = mes["GameState"].asInt();
			std::string pcurStrJvHao = mes["curStrJvHao"].asString();
			m_curZhuang = mes["curZhuang"].asInt();

			if (m_curZhuang != m_g_myself->GetChairIndex())
				m_g_myself->StartTimer(IDD_TIMER_GAME_XIAZHU1, rand() % 6 + 2);
		}
		break;
	case IDD_MESSAGE_ROOM_XIAZHU:
	{
		if (m_curZhuang != m_g_myself->GetChairIndex() && rand() % 100 < 50)
			m_g_myself->StartTimer(IDD_TIMER_GAME_XIAZHU1, rand() % 6 + 2);
	}
		break;
	case IDD_MESSAGE_ROOM_GAMEEND:
	{
		if (rand() % 100 < 10)
		{
			if (!m_isAddZhuang && m_curZhuang != m_g_myself->GetChairIndex())
			{
				if (m_g_myself->GetMoney() > m_minzhuangmoney)
				{
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_ROOM;
                    root["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
                    m_g_myself->SendGameMsg(root);

					m_isAddZhuang = true;
				}
			}
			else
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_ROOM;
                root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
                m_g_myself->SendGameMsg(root);

				m_isAddZhuang = false;
			}
		}
	}
		break;
	default:
		break;
	}
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

	if(timerId == IDD_TIMER_GAME_XIAZHU1 && curTimer <= 0)
	{
		m_g_myself->StopTimer(IDD_TIMER_GAME_XIAZHU1);

		int pTepJettonMoney[] = {100,500,1000,5000,10000,50000,100000};

		int64 pTempMoney = pTepJettonMoney[rand()%7];

		if(pTempMoney <= m_g_myself->GetMoney())
		{
			YanZhuType pjettontype = (YanZhuType)(rand() % TYPE_NULL);

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_ROOM;
            root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
            root["jettontype"] = pjettontype;
            root["XiaZhuAreaCount"] = rand()%GetXiaZhuAreaCount(pjettontype);
            root["TempMoney"] = (int)pTempMoney;
            m_g_myself->SendGameMsg(root);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
