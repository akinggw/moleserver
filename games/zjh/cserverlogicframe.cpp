#include "cserverlogicframe.h"
#include <sstream>

#define IDI_GAMING_TIME			40				//服务器超时计时

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{
	memset(	m_PlayState,	0,	sizeof(m_PlayState));
	memset(	m_wCardData,	0,	sizeof(m_wCardData));
	memset(	m_nUserScore,	0,	sizeof(m_nUserScore));
	memset(	m_nAddScore,	0,	sizeof(m_nAddScore));
	memset(	m_bLook,	0,	sizeof(m_bLook));
	memset( m_nThanUser,	0,	sizeof(m_nThanUser));
	memset(	m_bOffLine,	0,	sizeof(m_bOffLine));

	m_nCanThanUser	=	INVALID_CHAIR;
	m_bCanThan	=	false;
	m_nAllUserNum	=	0;
	m_nBasicAddId	=	0;
	m_nBasicScore=0;
	m_nBankUser	=	INVALID_CHAIR;
	m_nCurrentUser	=	INVALID_CHAIR;
	m_bGameOver=false;
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

	ClearData();

	//计算下注

	assert(m_nBasicScore>0);
	Player *pPlayer;
	for (int i=0;i<IDD_MAX_ROOM_USERCOUNT;i++)			//遍历座位
	{
		pPlayer=m_g_GameRoom->GetPlayer(i);
		if (pPlayer!=NULL)
		{
			//总玩家数
			m_nAllUserNum++;
			//设置存在
			m_PlayState[i]=PLAYSTATE_PLAY;
			//获取玩家剩余金币
			m_nUserScore[i]=pPlayer->GetMoney();
			//下底注
			m_nAddScore[i]=m_nBasicScore;
			//保存姓名
			m_strUserName[i] = pPlayer->GetName().c_str();
		}
	}

	m_nBasicAddId = 0;

	//获取庄家
	if (m_nBankUser == INVALID_CHAIR)
	{
		m_nBankUser=rand()%IDD_MAX_ROOM_USERCOUNT;
	}
	else
		m_nBankUser= (m_nBankUser+1)%IDD_MAX_ROOM_USERCOUNT;

	while (m_PlayState[m_nBankUser] == PLAYSTATE_NULL)
	{
		m_nBankUser= (m_nBankUser+1)%IDD_MAX_ROOM_USERCOUNT;
	}

	m_nCanThanUser=m_nBankUser;

	//开始录像
	//m_g_GameRoom->StartVideoTape(m_g_GameRoom->GetPlayer(m_nBankUser), m_g_GameRoom);
	//获取当前玩家
	m_nCurrentUser=GetNextUser(m_nBankUser);
	//洗牌
	m_GameLogic.RandCardList(m_wCardData);

#ifdef ENABLE_DEBUG_TOOL
	ForTest();
#endif


	//机器人判断
	int64 pResult,pMax,pMin;
	pResult=pMax=pMin=0;

	int robotState = m_g_GameRoom->IsUserWin(&pResult,&pMax,&pMin);//1赢2输3不赢不输
	if (robotState == 2)
	{
		int nAndroidUser[IDD_MAX_ROOM_USERCOUNT];
		int nAndroidUserCount=0;

		//机器人个数
		for ( BYTE wChairID = 0; wChairID < IDD_MAX_ROOM_USERCOUNT; ++wChairID )
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer(wChairID);
			if (pPlayer==NULL)	continue;
			if (pPlayer->GetType()==1 )
			{
				nAndroidUser[ nAndroidUserCount++ ] = ( wChairID ) ;
			}
		}

		//本桌有机器人，交换扑克,让机器人获得最大牌
		if ( nAndroidUserCount > 0 )
		{
			WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
			wHaveGoodCardAndroidUser = nAndroidUser[ rand() % nAndroidUserCount ] ;

			bool hasBestCard=true;
			int bestCardUser=wHaveGoodCardAndroidUser;
			WORD tempCardData[IDD_MAX_HAND_COUNT];

			do
			{
				for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
				{
					if(i == bestCardUser)continue;
					hasBestCard=true;
					if (!m_GameLogic.CompareCard(m_wCardData[bestCardUser],m_wCardData[i]))
					{
						memcpy(tempCardData,&m_wCardData[i],sizeof(tempCardData));
						memcpy(&m_wCardData[i],&m_wCardData[bestCardUser],sizeof(tempCardData));
						memcpy(&m_wCardData[bestCardUser],tempCardData,sizeof(tempCardData));
					}
				}

				for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
				{
					if(i == bestCardUser)continue;
					if (!m_GameLogic.CompareCard(m_wCardData[bestCardUser],m_wCardData[i]))
					{
						hasBestCard=false;
						break;
					}
				}

				if (!hasBestCard)
				{
					m_GameLogic.RandCardList(m_wCardData);
				}
			}while(!hasBestCard);


			for ( int i=0 ; i< nAndroidUserCount ; ++i)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_BOOT_WIN;
                out["HaveGoodCardAndroidUser"] = wHaveGoodCardAndroidUser;
				m_g_GameRoom->SendTableMsg(nAndroidUser[i],out);
			}
		}
	}

	for (int i=0 ; i<IDD_MAX_ROOM_USERCOUNT; ++i)
	{
		m_GameLogic.SortCardByLogicValue(m_wCardData[i]);
	}

	//开始
    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_START;
    out["BankUser"] = m_nBankUser;
    out["CurrentUser"] = m_nCurrentUser;
    out["BasicScore"] = (Json::Int64)m_nBasicScore;
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
	{
		out["PlayState"][i] = (int)m_PlayState[i];
	}
	//发送开始消息
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	//启动定时器
	m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDI_GAMING_TIME);
}

/// 用于处理用户进入游戏房间后的消息
void CServerLogicFrame::OnProcessPlayerRoomMes(int playerId,Json::Value &mes)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	int subMsgID = mes["MsgSubId"].asInt();

	switch(subMsgID)
	{
	case IDD_MESSAGE_ROOM_SHOW_FACE:
		{
			int tempFaceId= mes["tempFaceId"].asInt();

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_SHOW_FACE;
            out["playerId"] = playerId;
            out["tempFaceId"] = tempFaceId;
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		}
		break;
	case IDD_MESSAGE_ROOM_SHOW_MSG:
		{
			int tempMsgId= mes["tempMsgId"].asInt();

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_SHOW_MSG;
            out["playerId"] = playerId;
            out["tempMsgId"] = tempMsgId;
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
		}
		break;
	case IDD_MESSAGE_ROOM_ADD_SCORE:
		{
			int addScore=mes["addScore"].asInt();

			//非当前玩家，不能加注
			assert(m_nCurrentUser == playerId);
			if (m_nCurrentUser!=playerId)break;

			//放弃玩家不能加注
			assert(m_PlayState[playerId] == PLAYSTATE_PLAY);
			if (m_PlayState[playerId] != PLAYSTATE_PLAY)break;

			OnUserAddScore(playerId,addScore);
		}
		break;
	case IDD_MESSAGE_ROOM_WATCH:
		{
			//非当前玩家，不能看牌
			assert(m_nCurrentUser == playerId);
			if (m_nCurrentUser!=playerId)break;

			//放弃玩家不能看牌
			assert(m_PlayState[playerId] == PLAYSTATE_PLAY);
			if (m_PlayState[playerId] != PLAYSTATE_PLAY)break;

			//已经看牌，不能再次看牌
			assert(!m_bLook[playerId]);
			if (m_bLook[playerId]) break;

			OnUserWatchCard(playerId);
		}
		break;
	case IDD_MESSAGE_ROOM_THAN:
		{
			int Player2=mes["Player2"].asInt();
			OnUserThanCard(playerId,Player2);
		}
		break;
	case IDD_MESSAGE_ROOM_GIVE_UP:
		{
			//放弃玩家应该在玩状态
			assert(m_PlayState[playerId] == PLAYSTATE_PLAY);
			if (m_PlayState[playerId] != PLAYSTATE_PLAY)break;

			//当前玩家才放弃
			assert(playerId == m_nCurrentUser);
			if (playerId != m_nCurrentUser)break;

			OnUserGiveUp(playerId);
		}
		break;
	}
}

/// 处理用户进入房间消息
void CServerLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if (m_nBasicScore == 0)
	{
		m_nBasicScore = m_g_GameRoom->GetGamePielement();
	}

	if (m_g_GameRoom->GetRoomState() == ROOMSTATE_GAMING)
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_LOOK_ON;
        out["BankUser"] = m_nBankUser;
        out["CurrentUser"] = m_nCurrentUser;
        out["BasicScore"] = (Json::Int64)m_nBasicScore;
        out["BasicAddId"] = m_nBasicAddId;
		for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
		{
			out["PlayState"][i] = (int)m_PlayState[i];
			out["AddScore"][i] = (Json::Int64)m_nAddScore[i];
			out["bLook"][i] = m_bLook[i]?1:0;
		}
		m_g_GameRoom->SendTableMsg(playerId,out);

	}
	else
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_ENTER_ROOM;
        out["BasicScore"] = (Json::Int64)m_nBasicScore;
		m_g_GameRoom->SendTableMsg(playerId,out);
	}
}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if(m_bGameOver)return;

	if(m_g_GameRoom->GetRoomState() == ROOMSTATE_GAMING)
	{
		if (m_PlayState[playerId] == PLAYSTATE_PLAY)
		{
			OnUserGiveUp(playerId);
		}
	}
	else
	{
		if (m_nBankUser == playerId)
		{
			m_nBankUser = INVALID_CHAIR;
		}
	}

	m_g_GameRoom->UpdateUserScore(m_g_GameRoom->GetPlayer(playerId));
}

/// 处理用户断线消息
void CServerLogicFrame::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if (playerId == m_nCurrentUser  && !m_bOffLine[playerId])
	{
		m_bOffLine[playerId] = true;
		m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,30);
	}
}

/// 处理用户断线重连消息
void CServerLogicFrame::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_RE_ENTER;
    out["BankUser"]	= m_nBankUser;
    out["CurrentUser"]	= m_nCurrentUser;
    out["BasicScore"]	= (Json::Int64)m_nBasicScore;
    out["BasicAddId"]	= m_nBasicAddId;
    out["bCanThan"]	= m_bCanThan?1:0;
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
	{
		out["PlayState"][i]	= (int)m_PlayState[i];
		out["AddScore"][i]	= (Json::Int64)m_nAddScore[i];
		out["bLook"][i]	= m_bLook[i]?1:0;
	}
	if (m_bLook[playerId] || m_PlayState[playerId] == PLAYSTATE_LOSE || m_PlayState[playerId] == PLAYSTATE_GIVEUP)
	{
		out["CardData0"]	= m_wCardData[playerId][0];
		out["CardData1"]	= m_wCardData[playerId][1];
		out["CardData2"]	= m_wCardData[playerId][2];
	}
	m_g_GameRoom->SendTableMsg(playerId,out);
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	switch(timerId)
	{
	case IDD_TIMER_SERVER_GAMING:
		{
			if (curTimer<=0)
			{
				m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);

				assert(m_nCurrentUser!=INVALID_CHAIR);
				OnUserGiveUp(m_nCurrentUser);
			}
		}
		break;

	//case IDD_TIMER_SERVER_OFFLINE:
	//	{
	//		if (curTimer<=0)
	//		{
	//			m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_OFFLINE);
	//			ASSERT(m_nCurrentUser!=INVALID_CHAIR);
	//			if (m_g_GameRoom->GetPlayer(m_nCurrentUser)->GetState() == PLAYERSTATE_LOSTLINE)
	//			{
	//				OnUserGiveUp(m_nCurrentUser);
	//			}
	//
	//		}
	//	}
	//	break;
	case IDD_TIMER_SERVER_GAMEOVER:
		{
			if (curTimer<=0)
			{
				m_g_GameRoom->StopAllTimer();

				for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
				{
					//构造结束消息
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEOVER;
					for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; ++j)
					{
						out["WinScore"][j] = (Json::Int64)m_nWinScore[j];					//写入结束积分
						out["strUserName"][j] = m_strUserName[j];	//写入玩家名字

						out["wCardData0"][j] = m_wCardData[j][0];//写入牌数据
						out["wCardData1"][j] = m_wCardData[j][1];
						out["wCardData2"][j] = m_wCardData[j][2];
					}
					out["nThanUser"] = m_nThanUser[i];//比牌序列

					m_g_GameRoom->SendTableMsg(i,out);
				}

				//游戏结束
				m_g_GameRoom->GameEnd();
				ClearData();
				//停止录像
				//m_g_GameRoom->EndVideoTape();
			}
		}
	}
}

//加注
void CServerLogicFrame::OnUserAddScore(int playerID, int addId)
{
	assert(addId >= m_nBasicAddId);
	if (addId<m_nBasicAddId)return;
	assert(addId>=0 && addId<4);
	if(addId <0 || addId >3)return;

	int64 addScore=(addId+1)*m_nBasicScore;
	if (addId == 3) addScore = m_nBasicScore*5;
	if (m_bLook[playerID])
		addScore*=2;


	assert(addScore<=m_nUserScore[playerID] - m_nAddScore[playerID] );
	if (addScore > m_nUserScore[playerID] - m_nAddScore[playerID])return;

	m_nBasicAddId=addId;

	//关闭定时器
	m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);
	m_nAddScore[playerID]+=addScore;
	m_nCurrentUser=GetNextUser(m_nCurrentUser);

	//发送可以比牌消息
	if(m_nCurrentUser == m_nCanThanUser)
	{
		m_bCanThan=true;
		SendCanThan();
	}


	m_bOffLine[playerID]=false;

	//加注消息
    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_SCORE;
    out["playerID"] = playerID;
    out["addId"] = addId;
    out["CurrentUser"] = m_nCurrentUser;
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	//启动定时器
	m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDI_GAMING_TIME);

	return;
}

//看牌
void CServerLogicFrame::OnUserWatchCard(int playerID)
{
	m_bLook[playerID]= true;
	//关闭定时器
	m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);

	//看牌消息
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_WATCH;
        out["playerID"]	= playerID;
		if (i==playerID)
		{
			out["wCardData0"]	= m_wCardData[playerID][0];
			out["wCardData1"]	= m_wCardData[playerID][1];
			out["wCardData2"]	= m_wCardData[playerID][2];
		}
		m_g_GameRoom->SendTableMsg(i,out);
	}

	//开启新的定时器
	m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDI_GAMING_TIME);


	return;
}

//比牌
void CServerLogicFrame::OnUserThanCard(int player1,int player2)
{
	//玩家校验
	assert(player1 == m_nCurrentUser);
	if (m_nCurrentUser!=player1)return;
	assert(m_PlayState[player1] == PLAYSTATE_PLAY);
	if (m_PlayState[player1] != PLAYSTATE_PLAY)return;
	assert(m_PlayState[player2] == PLAYSTATE_PLAY);
	if (m_PlayState[player2] != PLAYSTATE_PLAY)return;
	assert(m_bCanThan);
	if (!m_bCanThan)return;


	int64 thanGold= (m_nBasicAddId+1)*m_nBasicScore*2;
	if (m_nBasicAddId== 3)
		thanGold = 5*m_nBasicScore*2;
	if (m_bLook[player1])
		thanGold *= 2;

	//下注不够比牌
	if (thanGold > m_nUserScore[player1] - m_nAddScore[player1])return;


	m_nAddScore[player1]+=thanGold;


	//保存比牌序列
	m_nThanUser[player1] |= (1 << player2);
	m_nThanUser[player2] |= (1 << player1);


	//停止定时器
	m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);

	//比牌
	bool win = m_GameLogic.CompareCard(m_wCardData[player1],m_wCardData[player2]);
	//获取输家
	int nLostUser = win ? player2 : player1;
	//设置输家状态
	m_PlayState[nLostUser] = PLAYSTATE_LOSE;
	//设置还可加注玩家数
	m_nAllUserNum--;
	//获取新的当前玩家
	m_nCurrentUser=GetNextUser(m_nCurrentUser);
	//如果只剩下1个玩家，则该玩家胜利。
	if (m_nAllUserNum==1)
	{
		m_nCurrentUser=INVALID_CHAIR;


		////给所有玩家显示最后比牌玩家的牌
		//for (int i=0; i< IDD_MAX_ROOM_USERCOUNT; ++i)
		//{
		//	m_nThanUser[i] |= (1 << player1);
		//	m_nThanUser[i] |= (1 << player2);
		//}
	}

	//比牌消息
	for (int i=0 ;i<IDD_MAX_ROOM_USERCOUNT ;++i)
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_THAN;
        out["player1"] = player1;
        out["player2"] = player2;
        out["LostUser"] = nLostUser;
        out["CurrentUser"] = m_nCurrentUser;
		if (i==nLostUser && !m_bLook[nLostUser])
		{
			out["wCardData0"] = m_wCardData[nLostUser][0];
			out["wCardData1"] = m_wCardData[nLostUser][1];
			out["wCardData2"] = m_wCardData[nLostUser][2];
		}
		m_g_GameRoom->SendTableMsg(i,out);
	}


	//比牌后剩余一个玩家
	if (m_nAllUserNum==1)
	{
		OnUserGameEnd(false);
		return;
	}
	m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDI_GAMING_TIME);
	return;
}

//放弃
void CServerLogicFrame::OnUserGiveUp(int playerID)
{
	m_PlayState[playerID] = PLAYSTATE_GIVEUP;
	m_nAllUserNum--;

	if (m_nAddScore[playerID]>0)
	{
		m_g_GameRoom->WriteUserScore(playerID,-m_nAddScore[playerID],0,enScoreKind_Lost);
	}

	if (playerID == m_nCanThanUser)
	{
		m_nCanThanUser = GetNextUser(m_nCanThanUser);
	}

	//当前玩家放弃，获取新的操作玩家
	if (m_nCurrentUser==playerID)
	{
		m_nCurrentUser=GetNextUser(playerID);
	}
	if(m_nAllUserNum ==1)
		m_nCurrentUser=INVALID_CHAIR;


	//发送可以比牌消息
	if(m_nCurrentUser == m_nCanThanUser)
	{
		m_bCanThan=true;
		SendCanThan();
	}

	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
        out["playerID"]	= playerID;
        out["CurrentUser"] = m_nCurrentUser;
		if (i==playerID)
		{
			out["CardData0"] = m_wCardData[playerID][0];
			out["CardData1"] = m_wCardData[playerID][1];
			out["CardData2"] = m_wCardData[playerID][2];
		}
		m_g_GameRoom->SendTableMsg(i,out);
	}


	if (m_nAllUserNum==1)
	{
		OnUserGameEnd(true);
		return;
	}
	m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDI_GAMING_TIME);
}
//游戏结束
void CServerLogicFrame::OnUserGameEnd(bool isGiveUp)
{

	m_g_GameRoom->StopAllTimer();


	int WinUser=INVALID_CHAIR;
	int64 allScore=0;
	memset( m_nWinScore, 0, sizeof(m_nWinScore));
	//计算输赢积分
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if (m_PlayState[i]== PLAYSTATE_PLAY)WinUser=i;
		allScore+=m_nAddScore[i];
		m_nWinScore[i]=-m_nAddScore[i];
	}
	//校验
	assert(WinUser != INVALID_CHAIR);
	//统计赢家
	m_nWinScore[WinUser]+=allScore;


	//写入积分
	int64 nRevenue=0;
	enScoreKind ScoreKind;
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if(m_PlayState[i] == PLAYSTATE_GIVEUP)continue;

		ScoreKind=(m_nWinScore[i]>0)?enScoreKind_Win:enScoreKind_Lost;
		nRevenue= 0;
		int64 lAgentRevenue = 0;

		if (m_nWinScore[i]>0)
		{
			float choushui=m_g_GameRoom->GetChouShui();
			nRevenue=(int64)(m_nWinScore[i]*choushui);
			m_nWinScore[i] -= nRevenue;
		}

		/*Player *pPlayer = m_g_GameRoom->GetPlayer(i);
		if(m_nWinScore[i] > 0 && pPlayer)
		{
			float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
			lAgentRevenue = (int64)((float)m_nWinScore[i] * pAgentRate);
			m_nWinScore[i] -= lAgentRevenue;
		}*/

		m_g_GameRoom->WriteUserScore(i,m_nWinScore[i],nRevenue,ScoreKind,lAgentRevenue);
	}

	m_bGameOver=true;
	memset(m_nAddScore,0,sizeof(m_nAddScore));

	if (isGiveUp)
	{
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMEOVER,1);
	}
	else
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMEOVER,3);
}

//发送可以比牌消息
void CServerLogicFrame::SendCanThan()
{
    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_CAN_THAN;
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
}

//是否必须比牌
bool CServerLogicFrame::isMustThan(int playerID)
{
	int64 listScore= m_nUserScore[playerID]-m_nAddScore[playerID];

	if (m_bLook[playerID])
	{
		if (listScore<(m_nAllUserNum-1)*20*m_nBasicScore)
		{
			return true;
		}
	}
	else
	{
		if (listScore<(m_nAllUserNum-1)*10*m_nBasicScore)
		{
			return true;
		}
	}

	return false;
}

//获取下一个操作玩家
int CServerLogicFrame::GetNextUser(int PlayerID)
{
	for (int i=1; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		int tempID=(PlayerID-i+IDD_MAX_ROOM_USERCOUNT)%IDD_MAX_ROOM_USERCOUNT;

		if (m_PlayState[tempID] == PLAYSTATE_PLAY)
		{
			return tempID;
		}
	}
	return INVALID_CHAIR;
}

void CServerLogicFrame::ClearData()
{
	memset(	m_wCardData,	0,	sizeof(m_wCardData));
	memset(	m_nUserScore,	0,	sizeof(m_nUserScore));
	memset(	m_nAddScore,	0,	sizeof(m_nAddScore));
	memset(	m_bLook,	0,	sizeof(m_bLook));
	memset( m_nThanUser,	0,	sizeof(m_nThanUser));
	memset(	m_bOffLine,	0,	sizeof(m_bOffLine));


	m_bGameOver= false;
	m_nCanThanUser	=	INVALID_CHAIR;
	m_bCanThan	=	false;
	m_nAllUserNum	=	0;
	m_nBasicAddId	=	0;
	m_nCurrentUser	=	INVALID_CHAIR;
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
	{
		m_strUserName[i].clear();
		m_PlayState[i]=PLAYSTATE_NULL;
	}
}





#ifdef ENABLE_DEBUG_TOOL
bool CServerLogicFrame::ForTest()
{
	/*char cIsDebug[5]="";
	GetPrivateProfileString(TEXT("Test"), TEXT("IsDebug"),TEXT(""), cIsDebug, 5*sizeof(TCHAR), TEXT("./ForTest.ini"));
	int iIsDebug = _ttoi(cIsDebug);
	if (iIsDebug)
	{
		TCHAR cData[7*IDD_MAX_HAND_COUNT*IDD_MAX_ROOM_USERCOUNT] = TEXT("");
		GetPrivateProfileString(TEXT("Test"), TEXT("JH"), TEXT(""), cData, 7*IDD_MAX_HAND_COUNT*IDD_MAX_ROOM_USERCOUNT*sizeof(TCHAR), TEXT("./ForTest.ini"));
		TCHAR cTmp[7] = TEXT("");
		TCHAR ccTmp[1] = TEXT("");
		for (int i=0;i<IDD_MAX_HAND_COUNT*IDD_MAX_ROOM_USERCOUNT;i++)
		{
			memcpy(cTmp,cData+i*7, 7 * sizeof(TCHAR));
			ccTmp[0] = cTmp[3];
			int aaa = _ttoi(ccTmp)<<8;
			ccTmp[0] = cTmp[5];
			aaa += _tcstol(ccTmp,0,16);
			m_wCardData[i/IDD_MAX_HAND_COUNT][i%IDD_MAX_HAND_COUNT] = aaa;
		}
	}
	return iIsDebug;*/
	return true;
}
#endif
