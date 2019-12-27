#include "crobotlogicframe.h"
#include "GameLogic.h"

#include<algorithm>

#define		IDD_TIMER_BOT_GAMING		IDD_TIMER_GAME_START+10
#define		IDD_TIMER_BOT_OVER			IDD_TIMER_GAME_START+11

#define		IDI_GAMING_TIME			15
#define		IDI_TIME_LESS			3
#define		IDI_READY_TIME			6

/// 构造函数
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{
	m_nMinAddId=0;
	m_bCanThan=false;
	m_nBasicScore = 0;
	m_nWinUser= INVALID_CHAIR;

	memset(m_States,0,sizeof(m_States));
	memset(m_bHasLook, 0, sizeof(m_bHasLook));
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

	int nElapse;

	int subMsgID = mes["MsgSubId"].asInt();

	switch(subMsgID)
	{
	case IDD_MESSAGE_ROOM_START:
		{
			int banker= mes["banker"].asInt();
			m_nCurrentUser = mes["CurrentUser"].asInt();
			m_nBasicScore = mes["BasicScore"].asInt64();

			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
			{
				m_States[i] = (PlayStates)mes["PlayStates"].asInt();
				m_nUserAdd[i]+=m_nBasicScore;
			}

 			if ( m_nCurrentUser == m_g_myself->GetChairIndex() )
 			{
 				nElapse = rand()%IDI_GAMING_TIME + IDI_TIME_LESS;
 				m_g_myself->StartTimer( IDD_TIMER_BOT_GAMING , nElapse );
 			}
		}
		break;
	case IDD_MESSAGE_ROOM_ADD_SCORE:
		{
			int addUser = mes["addUser"].asInt();
			m_nMinAddId = mes["MinAddId"].asInt();
			m_nCurrentUser = mes["CurrentUser"].asInt();



			int64 addScore=(m_nMinAddId+1)*m_nBasicScore;
			if (m_nMinAddId == 3) addScore = m_nBasicScore*5;
			if (m_bHasLook[addUser])
				addScore*=2;

			m_nUserAdd[addUser]+=addScore;

			if ( m_nCurrentUser == m_g_myself->GetChairIndex() )
			{
				nElapse = rand()%IDI_GAMING_TIME + IDI_TIME_LESS;
				m_g_myself->StartTimer( IDD_TIMER_BOT_GAMING , nElapse );
			}
		}
		break;
	case IDD_MESSAGE_ROOM_GIVE_UP:
		{
			int giveUpUser = mes["giveUpUser"].asInt();
			m_nCurrentUser = mes["CurrentUser"].asInt();

			//设置状态，方便比牌选择人员
			m_States[giveUpUser] = PLAYSTATE_GIVEUP;

			if ( m_nCurrentUser == m_g_myself->GetChairIndex() )
			{
				nElapse = rand()%IDI_GAMING_TIME + IDI_TIME_LESS;
				m_g_myself->StartTimer( IDD_TIMER_BOT_GAMING , nElapse );
			}
		}
		break;
	case IDD_MESSAGE_ROOM_WATCH:
		{
			int watchUser = mes["watchUser"].asInt();
			m_nCurrentUser = watchUser;

			m_bHasLook[watchUser] = true;

			if ( m_nCurrentUser == m_g_myself->GetChairIndex() )
			{
				nElapse = rand()%IDI_GAMING_TIME + IDI_TIME_LESS;
				m_g_myself->StartTimer( IDD_TIMER_BOT_GAMING , nElapse );
			}
		}
		break;
	case IDD_MESSAGE_ROOM_THAN:
		{
			int thanUser1 = mes["thanUser1"].asInt();
			int thanUser2 = mes["thanUser2"].asInt();
			int lostUser = mes["lostUser"].asInt();
			m_nCurrentUser = mes["CurrentUser"].asInt();

			m_States[lostUser] = PLAYSTATE_LOSE;
			//比牌加注
			int64 thanAdd=(m_nMinAddId+1)*m_nBasicScore*2;
			if(m_nMinAddId==3)thanAdd = 10*m_nBasicScore;
			if (m_bHasLook[thanUser1])thanAdd *= 2;

			m_nUserAdd[thanUser1]+=thanAdd;

			if ( m_nCurrentUser == m_g_myself->GetChairIndex() )
			{
				nElapse = rand()%IDI_GAMING_TIME + IDI_TIME_LESS;
				m_g_myself->StartTimer( IDD_TIMER_BOT_GAMING , nElapse );
			}
		}
		break;
	case IDD_MESSAGE_ROOM_GAMEOVER:
		{
			ClearData();
			m_g_myself->StopAllTimer();
 			int nElapse=rand()%IDI_READY_TIME+IDI_TIME_LESS;
 			m_g_myself->StartTimer(IDD_TIMER_BOT_OVER,nElapse);
		}
		break;
	case IDD_MESSAGE_ROOM_CAN_THAN:
		{
			m_bCanThan=true;
		}
		break;
	case IDD_MESSAGE_ROOM_ENTER_ROOM:
		{
 			int nElapse=rand()%IDI_READY_TIME+IDI_TIME_LESS;
 			m_g_myself->StartTimer(IDD_TIMER_BOT_OVER,nElapse);
		}
		break;
	case IDD_MESSAGE_ROOM_BOOT_WIN:
		{
			m_nWinUser = mes["WinUser"].asInt();
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

	switch(timerId)
	{
	case IDD_TIMER_BOT_GAMING:
		{
			if (curTimer<=0)
			{
				m_g_myself->StopTimer(IDD_TIMER_BOT_GAMING);
				OnAI();
			}
		}
		break;
	case IDD_TIMER_BOT_OVER:
		{
			if (curTimer<=0)
			{
				m_g_myself->StopTimer(IDD_TIMER_BOT_OVER);
				m_g_myself->SendReadyMsg();
			}
		}
		break;
	}
}

void CRobotLogicFrame::OnAI()
{
	int tempSelect = rand()%100;
	if (m_nWinUser == m_g_myself->GetChairIndex())
	{
		if (m_bCanThan )
		{
			int allPlayer=0;
			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				if (m_States[i])
				{
					allPlayer++;
				}
			}
			if (tempSelect>=80 ||(m_g_myself->GetMoney()-m_nUserAdd[m_g_myself->GetChairIndex()])<=(allPlayer-1)*m_nBasicScore*20)
			{
				int thanUser=rand()%IDD_MAX_ROOM_USERCOUNT;
				while(m_States[thanUser] != PLAYSTATE_PLAY || thanUser == m_g_myself->GetChairIndex())
				{
					thanUser=(thanUser+1)%IDD_MAX_ROOM_USERCOUNT;
				}

                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_THAN;
                out["thanUser"] = thanUser;
				m_g_myself->SendGameMsg(out);
			}
			else
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_SCORE;
                out["MinAddId"] = 3;
				m_g_myself->SendGameMsg(out);
			}

		}
		else
		{
			int64 lastGold = m_g_myself->GetMoney()-m_nUserAdd[m_g_myself->GetChairIndex()];
			int64 canAddId = lastGold/m_nBasicScore;
			if (m_bHasLook[m_g_myself->GetChairIndex()])
			{
				canAddId = canAddId/2;
			}
			canAddId--;

			if (canAddId>=4)
			{
				canAddId =3;
			}
			//if (canAddId>=2)
			//{
			//	canAddId =2;
			//}

			if (canAddId>=m_nMinAddId)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_SCORE;
                out["MinAddId"] = m_nMinAddId;
				m_g_myself->SendGameMsg(out);
			}
			else
			{
				if (!m_bHasLook[m_g_myself->GetChairIndex()])
				{
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_WATCH;
					m_g_myself->SendGameMsg(out);
					return;
				}
				//放弃
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
				m_g_myself->SendGameMsg(out);
				return;
			}
		}

	}
	else
	{
		if (!m_bHasLook[m_g_myself->GetChairIndex()])
		{//50%概率看牌
			if (tempSelect <= 50)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_WATCH;
				m_g_myself->SendGameMsg(out);
				return;
			}
		}

		//10%概率放弃
		if (tempSelect <= 10)
		{
			if (!m_bHasLook[m_g_myself->GetChairIndex()])
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_WATCH;
				m_g_myself->SendGameMsg(out);
				return;
			}
			//放弃
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
			m_g_myself->SendGameMsg(out);
			return;
		}
		//50%跟注
		if (tempSelect>10 && tempSelect <= 60)
		{

			int64 lastGold = m_g_myself->GetMoney()-m_nUserAdd[m_g_myself->GetChairIndex()];
			int64 canAddId = lastGold/m_nBasicScore;
			if (m_bHasLook[m_g_myself->GetChairIndex()])
			{
				canAddId = canAddId/2;
			}
			canAddId--;

			if (canAddId>=4)
			{
				canAddId =3;
			}
			if (canAddId>=2)
			{
				canAddId =2;
			}

			if (canAddId>=m_nMinAddId)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_SCORE;
                out["MinAddId"] = m_nMinAddId;
				m_g_myself->SendGameMsg(out);
				return;
			}
			else
			{
				if (!m_bHasLook[m_g_myself->GetChairIndex()])
				{
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_WATCH;
					m_g_myself->SendGameMsg(out);
					return;
				}
				//放弃
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
				m_g_myself->SendGameMsg(out);
				return;
			}

		}
		//20%概率加注 或者不能比牌时加注
		if (tempSelect>60 && tempSelect <= 80  || !m_bCanThan)
		{

			int64 lastGold = m_g_myself->GetMoney()-m_nUserAdd[m_g_myself->GetChairIndex()];
			int64 canAddId = lastGold/m_nBasicScore;
			if (m_bHasLook[m_g_myself->GetChairIndex()])
			{
				canAddId = canAddId/2;
			}
			canAddId--;

			if (canAddId>=4)
			{
				canAddId =3;
			}
			if (canAddId>=2)
			{
				canAddId =2;
			}

			int tempAddId = std::min((m_nMinAddId+rand()%3),3);
			if (tempAddId >3)
				tempAddId=3;

			if (canAddId>=m_nMinAddId && tempAddId<=canAddId)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_SCORE;
                out["MinAddId"] = tempAddId;
				m_g_myself->SendGameMsg(out);
				return;
			}
			else
			{
				if (!m_bHasLook[m_g_myself->GetChairIndex()])
				{
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_WATCH;
					m_g_myself->SendGameMsg(out);
					return;
				}
				//放弃
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
				m_g_myself->SendGameMsg(out);
				return;
			}



		}
		//20%概率比牌
		if (tempSelect>80)
		{
			if (!m_bCanThan)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_SCORE;
                out["MinAddId"]	= m_nMinAddId;
				m_g_myself->SendGameMsg(out);
				return;
			}
			int thanUser=rand()%IDD_MAX_ROOM_USERCOUNT;
			while(m_States[thanUser] != PLAYSTATE_PLAY || thanUser == m_g_myself->GetChairIndex())
			{
				thanUser=(thanUser+1)%IDD_MAX_ROOM_USERCOUNT;
			}

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_THAN;
            out["thanUser"]	= thanUser;
			m_g_myself->SendGameMsg(out);
			return;
		}

	}
}

void CRobotLogicFrame::ClearData()
{
	m_nMinAddId=0;
	m_bCanThan=false;
	m_nBasicScore = 0;
	m_nWinUser= INVALID_CHAIR;

	memset(m_States,0,sizeof(m_States));
	memset(m_nUserAdd,0,sizeof(m_nUserAdd));
	memset(m_bHasLook,0,sizeof(m_bHasLook));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
