#include "crobotlogicframe.h"

#include<algorithm>

#define TIME_LESS						3
#define TIME_CASH_CHIP					7
#define TIME_GAMEING					10


#define	IDD_TIMER_CHSH_CHIP				IDD_TIMER_GAME_START+10
#define	IDD_TIMER_GAMING				IDD_TIMER_GAME_START+11
#define IDD_TIMER_READY					IDD_TIMER_GAME_START+12

/// 构造函数
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{
	memset( m_nUserGold, 0, sizeof(m_nUserGold) );
	memset( m_nTurnAddGold, 0, sizeof(m_nTurnAddGold) );
	memset( m_wHandCard, 0, sizeof(m_wHandCard) );
	memset( m_wBoartCard, 0, sizeof(m_wBoartCard) );
	memset( m_wBestWeave, 0, sizeof(m_wBestWeave) );

	m_nBasicScore = 0;
	m_nTurnBasicAdd = 0;
	m_nTurnBasicGold = 0;
	//m_nMaxTurnAdd = 0;
	m_nBankUser = INVALID_CHAIR;
	m_nSmallBlindUser = INVALID_CHAIR;
	m_nBigBlindUser = INVALID_CHAIR;
	m_nCurrentUser = INVALID_CHAIR;
	m_nBoartCount = 0;
	m_wBestType = 0;
	m_winType = 0;
	m_nAddCount = 0;
	m_bIsGameOver=false;
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
	if (m_nUserGold[m_g_myself->GetChairIndex()]<m_nBasicScore)
	{
		uint32 nElapse=rand()%3+TIME_LESS;
		m_g_myself->StartTimer(IDD_TIMER_CHSH_CHIP,nElapse);
		return;
	}

	if (m_nUserGold[m_g_myself->GetChairIndex()] >= m_nBasicScore)
	{
		m_g_myself->StartTimer(IDD_TIMER_READY,3);
	}
}

/// 用于处理用户进入游戏房间后的消息
void CRobotLogicFrame::OnProcessPlayerRoomMes(Json::Value &mes)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	int subMsgID = mes["MsgSubId"].asInt();

	switch(subMsgID)
	{
	case IDD_MESSAGE_ROOM_ENTER_ROOM:
		{
			m_nBasicScore=mes["BasicScore"].asInt64();
			for (int i=0; i< IDD_MAX_ROOM_USERCOUNT; i++)
			{
				m_nUserGold[i]=mes["UserGold"][i].asInt64();
			}

			uint32 nElapse=rand()%TIME_CASH_CHIP+TIME_LESS+TIME_LESS;
			m_g_myself->StartTimer(IDD_TIMER_CHSH_CHIP,nElapse);
		}
		break;
	case IDD_MESSAGE_ROOM_CASH_CHIPS:
		{
			int pChairId = mes["ChairId"].asInt();
			m_nUserGold[pChairId] = mes["UserGold"].asInt64();

			if ((pChairId == m_g_myself->GetChairIndex()) && (m_nUserGold[pChairId]>=m_nBasicScore) && (m_g_myselfRoom->GetRoomState() != ROOMSTATE_GAMING))
			{
				m_g_myself->SendReadyMsg();
			}
		}
		break;
	case IDD_MESSAGE_ROOM_START_GAME:
		{
			ClearData();

			for (int i=0; i< IDD_MAX_ROOM_USERCOUNT; i++)
			{
				m_nUserGold[i]=mes["UserGold"][i].asInt64();
			}

			m_nBankUser = mes["BankUser"].asInt();
			m_nSmallBlindUser = mes["SmallBlindUser"].asInt();
			m_nBigBlindUser = mes["BigBlindUser"].asInt();
			m_nCurrentUser = mes["CurrentUser"].asInt();

			for (int i=0; i< IDD_MAX_ROOM_USERCOUNT; i++)
			{
				int first = mes[i]["HandCard0"].asInt();
				int second = mes[i]["HandCard1"].asInt();

				if(m_g_myself->GetChairIndex() == i)
				{
					m_wHandCard[0] = first;
					m_wHandCard[1] = second;
				}
			}

			//m_nMaxTurnAdd = mes->read64();
			m_nBasicScore = mes["BasicScore"].asInt64();

			m_nTurnBasicGold = m_nBasicScore;
			m_nTurnBasicAdd = m_nBasicScore*2;

			m_nTurnAddGold[m_nSmallBlindUser]=m_nBasicScore/2;
			m_nTurnAddGold[m_nBigBlindUser]=m_nBasicScore;

			m_wBestType= m_GameLogic.GetCardType(m_wHandCard,m_wBoartCard,m_nBoartCount,m_wBestWeave);

			if (m_nCurrentUser == m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_GAMEING+TIME_LESS;
				m_g_myself->StartTimer(IDD_TIMER_GAMING,nElapse);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_ADD_GOLD:
		{
			int pChairId = mes["ChairId"].asInt();
			int64 nAddGold = mes["AddGold"].asInt64();
			m_nCurrentUser=mes["CurrentUser"].asInt();


			if (nAddGold>m_nTurnBasicGold)
			{
				m_nTurnBasicGold=nAddGold;
				if ((nAddGold-m_nTurnAddGold[pChairId])>=m_nTurnBasicAdd)
				{
					m_nTurnBasicAdd=(nAddGold-m_nTurnAddGold[pChairId])*2;
					m_nAddCount++;
				}
			}

			m_nTurnAddGold[pChairId]=nAddGold;


			if (m_nCurrentUser == m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_GAMEING+TIME_LESS;
				m_g_myself->StartTimer(IDD_TIMER_GAMING,nElapse);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_GIVE_UP:
		{
			int pChairId = mes["ChairId"].asInt();
			m_nCurrentUser = mes["CurrentUser"].asInt();
			//m_nMaxTurnAdd = mes->read64();

			if (m_nCurrentUser == m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_GAMEING+TIME_LESS;
				m_g_myself->StartTimer(IDD_TIMER_GAMING,nElapse);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_DEAL_CARD:
		{
			m_nTurnBasicAdd=m_nBasicScore;
			m_nTurnBasicGold=0;
			m_nAddCount=0;

			m_nCurrentUser = mes["CurrentUser"].asInt();
			//m_nMaxTurnAdd = mes->read64();

			switch(m_nBoartCount)
			{
			case 0:
				{
					m_wBoartCard[m_nBoartCount++]=mes["BoartCard0"].asInt();
					m_wBoartCard[m_nBoartCount++]=mes["BoartCard1"].asInt();
					m_wBoartCard[m_nBoartCount++]=mes["BoartCard2"].asInt();
				}
				break;
			case 3:
			case 4:
				{
					m_wBoartCard[m_nBoartCount++]=mes["BoartCard0"].asInt();
				}
				break;
			}

			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				m_nUserGold[i] -= m_nTurnAddGold[i];
				m_nTurnAddGold[i]=0;
			}

			m_wBestType=m_GameLogic.GetCardType(m_wHandCard,m_wBoartCard,m_nBoartCount,m_wBestWeave);

			if (m_nCurrentUser == m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_GAMEING+TIME_LESS;
				m_g_myself->StartTimer(IDD_TIMER_GAMING,nElapse);
			}


		}
		break;
	case IDD_MESSAGE_ROOM_GAME_OVER:
		{

			GameEndType endType=(GameEndType)mes["GameEndType"].asInt();
			int nPotCount = mes["PotCount"].asInt();

			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{

				for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; ++j)
				{
					m_nWinGold[i][j] = mes["WinGold"][i][j].asInt64();
				}

			}


			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				m_nUserGold[i] = m_nWinGold[nPotCount][i];
			}


			m_g_myself->StopAllTimer();
		}
		break;
	case IDD_MESSAGE_ROOM_BOOT_WIN:
		{
			WORD winUser = mes["winUser"].asInt();
			if ( winUser == m_g_myself->GetChairIndex() )
			{
				m_winType=1;
			}
			else
				m_winType=-1;
		}
		break;
	case IDD_MESSAGE_ROOM_LOOK_ON:
		{
			m_nCurrentUser = mes["CurrentUser"].asInt();
			m_nBankUser = mes["BankUser"].asInt();
			m_nSmallBlindUser = mes["SmallBlindUser"].asInt();
			m_nBigBlindUser = mes["BigBlindUser"].asInt();

			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				OperateType  a = (OperateType)mes["OperateType"][i].asInt();

				int64 nPot = mes["nPot"][i].asInt64();
				m_nUserGold[i] = mes["UserGold"][i].asInt64();
				m_nTurnAddGold[i] = mes["TurnAddGold"][i].asInt64();
				WORD PlayStates  = mes["PlayStates"][i].asInt64();
			}

			int32 nPotCount = mes["PotCount"].asInt();
			m_nBasicScore = mes["BasicScore"].asInt64();

			m_wBoartCard[0] = mes["BoartCard0"].asInt();
			m_wBoartCard[1] = mes["BoartCard1"].asInt();
			m_wBoartCard[2] = mes["BoartCard2"].asInt();
			m_wBoartCard[3] = mes["BoartCard3"].asInt();
			m_wBoartCard[4] = mes["BoartCard4"].asInt();
			m_nBoartCount = mes["BoartCount"].asInt();


			uint32 nElapse=rand()%TIME_CASH_CHIP+TIME_LESS+TIME_LESS;
			m_g_myself->StartTimer(IDD_TIMER_CHSH_CHIP,nElapse);
		}
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

	switch( timerId )
	{
	case IDD_TIMER_CHSH_CHIP:
		{
			if (curTimer <= 0)
			{
				int64 cashChips= std::min(m_nBasicScore*100,m_g_myself->GetMoney());

				assert(cashChips >= m_nBasicScore);

                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_CASH_CHIPS;
                out["cashChips"] = (Json::Int64)cashChips;
				m_g_myself->SendGameMsg( out );

				m_g_myself->StopTimer( IDD_TIMER_CHSH_CHIP );
			}
		}
		break;
	case IDD_TIMER_READY:
		{
			m_g_myself->SendReadyMsg();
			m_g_myself->StopTimer(IDD_TIMER_READY);
		}
		break;
	case IDD_TIMER_GAMING:
		{
			if (curTimer <= 0)
			{
				m_g_myself->StopTimer(IDD_TIMER_GAMING);
				if ( m_winType == 1 )
				{
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
                    out["UserGold"] = (Json::Int64)m_nUserGold[m_g_myself->GetChairIndex()];
					m_g_myself->SendGameMsg( out );
				}
				else if ( m_winType == -1 )
				{
					if ( m_nTurnBasicGold == m_nTurnAddGold[m_g_myself->GetChairIndex()])
					{
                        Json::Value out;
                        out["MsgId"] = IDD_MESSAGE_ROOM;
                        out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
                        out["UserGold"] = (Json::Int64)m_nTurnBasicGold;
						m_g_myself->SendGameMsg( out );
					}
					else
					{
                        Json::Value out;
                        out["MsgId"] = IDD_MESSAGE_ROOM;
                        out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
						m_g_myself->SendGameMsg( out );
					}
				}
				else
				{
					OnAiAddGold();
				}
			}

		}
		break;
	}
}

//ai下注
void CRobotLogicFrame::OnAiAddGold()
{
	assert(m_nCurrentUser == m_g_myself->GetChairIndex());
	if ( m_nAddCount >=3 )
	{
		if (m_wBestType*20 > rand()%100)
		{
			int64 addGold;
			addGold = std::min(m_nTurnBasicGold,m_nUserGold[m_g_myself->GetChairIndex()]);

			assert(addGold<=m_nUserGold[m_g_myself->GetChairIndex()]);

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
            out["UserGold"] = (Json::Int64)addGold;
			m_g_myself->SendGameMsg( out );
		}
		else
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
			m_g_myself->SendGameMsg(out);
		}

		return;
	}


	switch(m_nBoartCount)
	{
	case 0:
		{
			//手牌一对，
			if ( m_GameLogic.GetCardLogicValue( m_wHandCard[0] ) == m_GameLogic.GetCardLogicValue( m_wHandCard[1] ))
			{
				int64 addGold;
				//addGold = std::min(m_nMaxTurnAdd,m_nUserGold[m_g_myself->GetChairIndex()]) * ( 25+(rand()%40) )/100;
				addGold = m_nUserGold[m_g_myself->GetChairIndex()] * ( 25+(rand()%40) )/100;

				if ( addGold < m_nTurnBasicAdd + m_nTurnAddGold[ m_g_myself->GetChairIndex() ] )
				{
					addGold = m_nTurnBasicAdd + m_nTurnAddGold[ m_g_myself->GetChairIndex() ] ;
					addGold = std::min( addGold, m_nUserGold[m_g_myself->GetChairIndex()] );
					//addGold = std::min( addGold, m_nMaxTurnAdd );
				}

				assert(addGold<=m_nUserGold[m_g_myself->GetChairIndex()]);

                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
                out["UserGold"] = (Json::Int64)addGold;
				m_g_myself->SendGameMsg( out );



				break;
			}
			//手牌同花
			if ( m_GameLogic.GetCardColor( m_wHandCard[0] ) == m_GameLogic.GetCardColor( m_wHandCard[1] ))
			{
				int64 addGold;
				//addGold = std::min(m_nMaxTurnAdd,m_nUserGold[m_g_myself->GetChairIndex()]) * ( 10+(rand()%20) )/100;
				addGold = m_nUserGold[m_g_myself->GetChairIndex()] * ( 30+(rand()%20) )/100;

				if ( addGold < m_nTurnBasicAdd + m_nTurnAddGold[ m_g_myself->GetChairIndex() ] )
				{
					addGold = m_nTurnBasicAdd + m_nTurnAddGold[ m_g_myself->GetChairIndex() ] ;
					addGold = std::min( addGold, m_nUserGold[m_g_myself->GetChairIndex()] );
					//addGold = std::min( addGold, m_nMaxTurnAdd );
				}

				assert(addGold<=m_nUserGold[m_g_myself->GetChairIndex()]);

                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
                out["UserGold"] = (Json::Int64)addGold;
				m_g_myself->SendGameMsg( out );



				break;
			}
			if (m_nTurnBasicGold <= m_nBasicScore)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
                out["UserGold"] = (Json::Int64)std::min( m_nTurnBasicGold, m_nUserGold[m_g_myself->GetChairIndex()]);
				m_g_myself->SendGameMsg( out );

				break;
			}

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
			m_g_myself->SendGameMsg(out);

		}
		break;
	case 3:
	case 4:
	case 5:
		{
			int selects = rand()%100;
			if (m_wBestType*10 > selects )
			{
				int64 addGold;
				//addGold = std::min(m_nMaxTurnAdd,m_nUserGold[m_g_myself->GetChairIndex()]) * (  10+(rand()%20) )/100;
				addGold = m_nUserGold[m_g_myself->GetChairIndex()] * (  30+(rand()%20) )/100;

				if ( addGold < m_nTurnBasicAdd + m_nTurnAddGold[ m_g_myself->GetChairIndex() ] )
				{
					addGold = m_nTurnBasicAdd + m_nTurnAddGold[ m_g_myself->GetChairIndex() ] ;
					addGold = std::min( addGold, m_nUserGold[m_g_myself->GetChairIndex()] );
					//addGold = std::min( addGold, m_nMaxTurnAdd );
				}

				assert(addGold<=m_nUserGold[m_g_myself->GetChairIndex()]);

                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
                out["UserGold"] = (Json::Int64)addGold;
				m_g_myself->SendGameMsg( out );
			}
			else if(m_wBestType*10 > selects/2  ||  m_nTurnBasicGold <= m_nBasicScore   ||  (m_nTurnAddGold[m_g_myself->GetChairIndex()] >0 && selects>70) )
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
                out["UserGold"] = (Json::Int64)std::min( m_nTurnBasicGold, m_nUserGold[m_g_myself->GetChairIndex()]);
				m_g_myself->SendGameMsg( out );
			}
			else
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
				m_g_myself->SendGameMsg(out);
			}
		}
		break;
	}
}
void CRobotLogicFrame::ClearData()
{
	memset( m_nUserGold, 0, sizeof(m_nUserGold) );
	memset( m_nTurnAddGold, 0, sizeof(m_nTurnAddGold) );
	memset( m_wHandCard, 0, sizeof(m_wHandCard) );
	memset( m_wBoartCard, 0, sizeof(m_wBoartCard) );
	memset( m_wBestWeave, 0, sizeof(m_wBestWeave) );

	m_nTurnBasicAdd = 0;
	m_nTurnBasicGold = 0;
	//m_nMaxTurnAdd = 0;
	m_nBankUser = INVALID_CHAIR;
	m_nSmallBlindUser = INVALID_CHAIR;
	m_nBigBlindUser = INVALID_CHAIR;
	m_nCurrentUser = INVALID_CHAIR;
	m_nBoartCount = 0;
	m_wBestType = 0;
	m_winType = 0;
	m_nAddCount = 0;
	m_bIsGameOver=false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
