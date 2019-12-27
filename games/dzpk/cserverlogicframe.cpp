#include "cserverlogicframe.h"
#include <sstream>
#include<algorithm>

#define TIME_DEAL_CARD				1
#define TIME_GAME_OVER				2
#define IDD_TIME_GAMING				30

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{
	memset(m_nTurnAddGold,0,sizeof(m_nTurnAddGold));
	memset(m_bPlayStates,0,sizeof(m_bPlayStates));
	memset(m_nAddGold,0,sizeof(m_nAddGold));
	memset(m_nUserGold,0,sizeof(m_nUserGold));
	memset(m_nAddList,0,sizeof(m_nAddList));
	memset(m_nPot,0,sizeof(m_nPot));
	memset(m_wCardData,0,sizeof(m_wCardData));
	memset(m_wHandCard,0,sizeof(m_wHandCard));
	memset(m_wBoardData,0,sizeof(m_wBoardData));
	memset(m_bAddStates,0,sizeof(m_bAddStates));
	for(int i=0;i<IDD_MAX_ROOM_USERCOUNT;i++)
		m_sUserName[i].clear();

	m_nCurrentUser=INVALID_CHAIR;
	m_nBankUser=INVALID_CHAIR;
	m_nSmallBlindUser=INVALID_CHAIR;
	m_nBigBlindUser=INVALID_CHAIR;


	m_nAddUserCount=0;
	m_nAddCount=0;
	m_nBasicScore=0;
	m_nTurnBasicGold=0;
	m_nTurnBasicAdd=0;
	m_nAllPlayUserCount=0;
	m_nPotCount=0;
	m_nSendCardPos=0;
	m_nBoardCount=0;
	//m_nMaxTurnAdd=0;
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

#ifdef _DEBUG
	network::System_Log("游戏开始");
#endif

	ClearData();

	Player *pPlayer;


	for (int i=0;i<IDD_MAX_ROOM_USERCOUNT;i++)			//遍历座位
	{
		pPlayer = m_g_GameRoom->GetPlayer(i);
		if (pPlayer != NULL)
		{
			m_sUserName[i] = pPlayer->GetName().c_str();

			if (m_nUserGold[i]>=m_nBasicScore)
			{
				m_nTurnStartGold[i] = m_nUserGold[i];
				m_nAllPlayUserCount++;
				//设置存在
				m_bPlayStates[i]=true;
			}
		}


	}

	//确定庄家
	if (m_nBankUser==INVALID_CHAIR)
	{
		int tempBankNum=rand()%IDD_MAX_ROOM_USERCOUNT;
		m_nBankUser=GetNextPlayer(tempBankNum);
	}
	else
		m_nBankUser=GetNextPlayer(m_nBankUser);

	//录像
	//m_g_GameRoom->StartVideoTape(m_g_GameRoom->GetPlayer(m_nBankUser),m_g_GameRoom);

	//m_nMaxTurnAdd=GetMaxTurnAddd();

	//下盲注
	Blind();

	//洗牌
	m_GameLogic.RandCardList(m_wCardData);
#ifdef _DEBUG
	ForTest();
#endif

	//发牌
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		m_wHandCard[i][0]=m_wCardData[m_nSendCardPos++];
		m_wHandCard[i][1]=m_wCardData[m_nSendCardPos++];
	}

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


			//获取最大牌
			WORD wBestWeave[IDD_MAX_ROOM_USERCOUNT][IDD_WEAVE_COUNT];	//最佳组合数据
			WORD wWeaveType[IDD_MAX_ROOM_USERCOUNT];					//最佳组合牌型
			memset(wBestWeave,0,sizeof(wBestWeave));
			memset(wWeaveType,0,sizeof(wWeaveType));

			WORD wTempBoardData[IDD_WEAVE_COUNT];
			memcpy(wTempBoardData,&m_wCardData[m_nSendCardPos],sizeof(WORD)*IDD_WEAVE_COUNT);

			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				if (m_bPlayStates[i])
				{
					//获取摊牌玩家的最佳组合牌，以及类型
					wWeaveType[i]=m_GameLogic.GetCardType(m_wHandCard[i],wTempBoardData,IDD_WEAVE_COUNT,wBestWeave[i]);
				}
			}
			int nWinUser[IDD_MAX_ROOM_USERCOUNT];
			int nWinCount=0;
			for ( int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
			{
				if (m_bPlayStates[i])
				{
					if (nWinCount==0)
					{
						nWinUser[nWinCount++]=i;
						continue;
					}
					//赢家和后面牌型玩家比较，刷新赢家，同样大也保存起来
					int temp=m_GameLogic.CompareCard(wBestWeave[nWinUser[0]],wBestWeave[i],wWeaveType[nWinUser[0]],wWeaveType[i]);
					if (temp<0)
					{
						nWinUser[0]=i;
						nWinCount=1;
					}
					else if (temp==0)//保存同样大小牌型的玩家
					{
						nWinUser[nWinCount++]=i;
					}
				}
			}
			//交换扑克
			WORD tempHandCard[IDD_HAND_COUNT];
			memcpy(tempHandCard,&m_wHandCard[nWinUser[0]],sizeof(tempHandCard));
			memcpy(m_wHandCard[nWinUser[0]],&m_wHandCard[wHaveGoodCardAndroidUser],sizeof(tempHandCard));
			memcpy(&m_wHandCard[wHaveGoodCardAndroidUser],tempHandCard,sizeof(tempHandCard));

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

	memset(m_bAddStates,0,sizeof(m_bAddStates));
	//发送开始消息
	DealHandCard();
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
	case IDD_MESSAGE_ROOM_ADD_GOLD://加注
		{
			int64 nAddGold=mes["AddGold"].asInt64();

			assert(m_bPlayStates[playerId]);
			assert(playerId==m_nCurrentUser);

			if ( (playerId!=m_nCurrentUser) || (!m_bPlayStates[playerId]) )return;
			OnUserAddGold(playerId,nAddGold);
		}
		break;
	case IDD_MESSAGE_ROOM_GIVE_UP://放弃
		{
			assert(m_bPlayStates[playerId]);

			if (m_bPlayStates[playerId])
			{
				OnUserGiveUp(playerId);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_CASH_CHIPS://兑换筹码
		{
			if (m_bPlayStates[playerId])
			{
#ifdef _DEBUG
				network::System_Log("该玩家已经在游戏中，不能准备");
#endif

				return;//游戏中不可兑换筹码
			}

			int64 userGold=mes["cashChips"].asInt64();

			if (userGold<m_nBasicScore*10)
			{
#ifdef _DEBUG
				network::System_Log("兑换筹码低于最低兑换值");
#endif
				return;
			}
			if (userGold>m_g_GameRoom->GetGamePielement()*200)
			{
#ifdef _DEBUG
				std::stringstream stream;
				std::string result;

				stream <<userGold;
				result =stream.str();
				network::System_Log(result);
				network::System_Log("兑换筹码高于最高兑换值");

				stream.clear();
				result.clear();
#endif
				userGold=m_g_GameRoom->GetGamePielement()*200;
			}


			if (userGold>m_g_GameRoom->GetPlayer(playerId)->GetMoney())
			{
#ifdef _DEBUG
				network::System_Log("兑换筹码高于拥有筹码");
#endif
				userGold=m_g_GameRoom->GetPlayer(playerId)->GetMoney();
			}

			//计算金币
			m_nUserGold[playerId]=userGold;

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_CASH_CHIPS;
            out["playerId"] = playerId;
            out["cashChips"] = (Json::Int64)userGold;
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
		}
	}
}

/// 处理用户进入房间消息
void CServerLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if (m_nBasicScore==0)
	{
		m_nBasicScore=m_g_GameRoom->GetGamePielement();
	}

	m_nUserGold[playerId]=0;

#ifdef _DEBUG
	network::System_Log("玩家进入");
#endif

	if (m_g_GameRoom->GetRoomState() == ROOMSTATE_GAMING)
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_LOOK_ON;
        out["CurrentUser"] = m_nCurrentUser;
        out["BankUser"] = m_nBankUser;
        out["SmallBlindUser"] = m_nSmallBlindUser;
        out["BigBlindUser"] = m_nBigBlindUser;
        Json::Value out2;
		for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
		{
			out2[i]["UserOperate"] = (int)m_UserOperate[i];
			out2[i]["Pot"] = (Json::Int64)m_nPot[i];
			out2[i]["UserGold"] = (Json::Int64)m_nUserGold[i];
			out2[i]["TurnAddGold"] = (Json::Int64)m_nTurnAddGold[i];
			out2[i]["PlayStates"] = m_bPlayStates[i]?1:0;
		}
		out["Users"] = out2;
        out["PotCount"] = m_nPotCount;
        out["BasicScore"] = (Json::Int64)m_nBasicScore;
        out["BoardData0"] = m_wBoardData[0];
        out["BoardData1"] = m_wBoardData[1];
        out["BoardData2"] = m_wBoardData[2];
        out["BoardData3"] = m_wBoardData[3];
        out["BoardData4"] = m_wBoardData[4];
        out["BoardCount"] = m_nBoardCount;
		m_g_GameRoom->SendTableMsg(playerId,out);
#ifdef _DEBUG
		network::System_Log("给进入玩家发送准备消息 1010");
#endif
	}
	else
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_ENTER_ROOM;
        out["BasicScore"] = (Json::Int64)m_nBasicScore;
		for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
		{
			out["UserGold"] = (Json::Int64)m_nUserGold[i];
		}
		m_g_GameRoom->SendTableMsg(playerId,out);
#ifdef _DEBUG
		network::System_Log("给进入玩家发送进入房间消息 1007");
#endif

	}
}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

#ifdef _DEBUG
	network::System_Log("玩家离开房间");
#endif

	//计算金币
	enScoreKind ScoreKind=enScoreKind_Lost;
	if(m_g_GameRoom->GetRoomState() == ROOMSTATE_GAMING)
	{
		//强退放弃
		if (m_bPlayStates[playerId])
		{
			OnUserGiveUp(playerId);
		}
	}

	m_nUserGold[playerId]=0;
	m_UserOperate[playerId] = OPERATE_NULL;

	m_g_GameRoom->UpdateUserScore(m_g_GameRoom->GetPlayer(playerId));
}

/// 处理用户断线消息
void CServerLogicFrame::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

}

/// 处理用户断线重连消息
void CServerLogicFrame::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_RECONNECT;
    out["CurrentUser"] = m_nCurrentUser;
    out["BankUser"] = m_nBankUser;
    out["SmallBlindUser"] = m_nSmallBlindUser;
    out["BigBlindUser"] = m_nBigBlindUser;
    out["AddCount"] = m_nAddCount;
    out["BasicScore"] = (Json::Int64)m_nBasicScore;
    out["TurnBasicGold"] = (Json::Int64)m_nTurnBasicGold;
    out["TurnBasicAdd"] = (Json::Int64)m_nTurnBasicAdd;
    Json::Value out2;
    for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
    {
        out2[i]["UserOperate"] = (int)m_UserOperate[i];
        out2[i]["Pot"] = (Json::Int64)m_nPot[i];
        out2[i]["UserGold"] = (Json::Int64)m_nUserGold[i];
        out2[i]["AddGold"] = (Json::Int64)m_nAddGold[i];
        out2[i]["TurnAddGold"] = (Json::Int64)m_nTurnAddGold[i];
        out2[i]["PlayStates"] = m_bPlayStates[i]?1:0;
    }
    out["Users"] = out2;
    out["PotCount"] = m_nPotCount;
	out["HandCard0"] = m_wHandCard[playerId][0];
	out["HandCard1"] = m_wHandCard[playerId][1];
    out["BoardData0"] = m_wBoardData[0];
    out["BoardData1"] = m_wBoardData[1];
    out["BoardData2"] = m_wBoardData[2];
    out["BoardData3"] = m_wBoardData[3];
    out["BoardData4"] = m_wBoardData[4];
    out["BoardCount"] = m_nBoardCount;
	m_g_GameRoom->SendTableMsg(playerId,out);
#ifdef _DEBUG
	network::System_Log("给断线重连玩家发送断线重连消息 1008");
#endif
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if (timerId==IDD_TIMER_SERVER_GAMING && curTimer<=0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);
		OnUserGiveUp(m_nCurrentUser);
	}
	if (timerId == IDD_TIMER_SERVER_DEAL_CARD && curTimer<=0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_DEAL_CARD);
		DealBoardCard(m_nBoardCount);
	}
}

//用户加注
void CServerLogicFrame::OnUserAddGold(int playerId, int64 nAddGold)
{
	if (m_g_GameRoom->GetRoomState() != ROOMSTATE_GAMING)
	{
#ifdef _DEBUG
		network::System_Log("不在游戏中时加注！");
#endif
		return;
	}

	//加注必须是allin，或者是前面下注的2倍。
	if (nAddGold!=m_nTurnBasicGold)
	{
		if (nAddGold-m_nTurnAddGold[playerId]<m_nTurnBasicAdd)
		{
			if(nAddGold!=m_nUserGold[playerId])return;
		}
		if (m_nAddCount>=3)
		{
			//加注超过3次。
			if(nAddGold>m_nTurnBasicGold)
			{
#ifdef _DEBUG
				network::System_Log("加注超过3次");
#endif
				return;
			}
			if(nAddGold!=m_nUserGold[playerId])
			{
#ifdef _DEBUG
				network::System_Log("加注超过3次，并且不是跟注");
#endif
				return;
			}
		}
	}
	//筹码不够跟注，错误下注
	if (m_nUserGold[playerId]<nAddGold)
	{
#ifdef _DEBUG
		network::System_Log("加注超过自己拥有筹码数");
#endif
		return;
	}

//	//当达到最高下注时，只能跟注
//	if (nAddGold>m_nMaxTurnAdd)
//	{
//#ifdef _DEBUG
//		network::System_Log("加注超过最大限注");
//#endif
//		ASSERT(false);
//		return;
//	}

	m_bAddStates[playerId]=true;

	m_UserOperate[playerId]=OPERATE_FOLLOW;

	if (nAddGold>m_nTurnBasicGold)
	{
		m_nTurnBasicGold=nAddGold;								//保存本轮最少下注

		if((nAddGold-m_nTurnAddGold[playerId])>=m_nTurnBasicAdd)
		{
			m_nTurnBasicAdd=(nAddGold-m_nTurnAddGold[playerId])*2;		//保存最小加注
			m_nAddCount++;
			m_UserOperate[playerId]=OPERATE_ADD;
		}
	}

	//all_in
	if (nAddGold==m_nUserGold[playerId])
	{
		m_UserOperate[playerId]=OPERATE_ALLIN;
		--m_nAllPlayUserCount;
	}

	//停止定时器
	m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);

	//保存本轮此玩家下注
	m_nTurnAddGold[playerId]=nAddGold;

	//获取下一个玩家
	m_nCurrentUser=GetNextPlayer(m_nCurrentUser);

	//判断是否下注完成
	if(isAddOver())
		m_nCurrentUser=INVALID_CHAIR;


	//广播加注信息
    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_ADD_GOLD;
    out["playerId"] = playerId;
    out["AddGold"] = (Json::Int64)nAddGold;
    out["CurrentUser"] = m_nCurrentUser;
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);


	if(m_nCurrentUser==INVALID_CHAIR)
	{
		if (m_nBoardCount==5)
		{
			m_endType=NORMAL;
			OnGameOver(m_endType);
			return;
		}
		//加注结束，发牌
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_DEAL_CARD,TIME_DEAL_CARD);
		return;
	}
	else//还需要继续加注，开启定时器
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDD_TIME_GAMING);


	return;
}
//用户放弃
void CServerLogicFrame::OnUserGiveUp(int playerId)
{
	if (m_g_GameRoom->GetRoomState() != ROOMSTATE_GAMING)
	{
		return;
	}

	m_bAddStates[playerId]=true;
	m_bPlayStates[playerId]=false;
	m_nAllPlayUserCount--;

	m_UserOperate[playerId]=OPERATE_GIVEUP;

	if (m_nCurrentUser==playerId)
	{
		//关闭定时器
		m_g_GameRoom->StopTimer(IDD_TIMER_SERVER_GAMING);

		m_nCurrentUser=GetNextPlayer(m_nCurrentUser);

		if (isAddOver())
		{//该玩家放弃后，正好本轮结束
			m_nCurrentUser=INVALID_CHAIR;
		}
		if (m_nAllPlayUserCount == 1)
		{
			m_nCurrentUser = INVALID_CHAIR;
		}

		if (m_nCurrentUser!=INVALID_CHAIR)
		{	//当前玩家放弃，对新的操作玩家开启定时器
			m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDD_TIME_GAMING);
		}
	}


	//m_nMaxTurnAdd=GetMaxTurnAddd();

    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_GIVE_UP;
    out["playerId"] = playerId;
    out["CurrentUser"] = m_nCurrentUser;
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);


	m_nUserGold[playerId] -= m_nTurnAddGold[playerId];
	assert(m_nUserGold[playerId]>=0);
	m_nAddGold[playerId] += m_nTurnAddGold[playerId];
	m_nTurnAddGold[playerId]=0;


	//写入数据库
	m_g_GameRoom->WriteUserScore(playerId,-(m_nAddGold[playerId]),0,enScoreKind_Lost);


	//玩家放弃至只剩下一个人，进行结算
	int tempUserCount=0;
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if (m_bPlayStates[i])
			tempUserCount++;
	}
	if (tempUserCount==1)
	{
		m_endType=GIVEUP;

		OnGameOver(m_endType);
		return;
	}

	//该玩家放弃后，正好本轮结束，进行统计
	if (m_nCurrentUser==INVALID_CHAIR)
	{
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_DEAL_CARD,TIME_DEAL_CARD);
		return;
	}

	return;
}


//发手牌
void CServerLogicFrame::DealHandCard()
{
	m_nCurrentUser=GetNextPlayer(m_nBigBlindUser);

	m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDD_TIME_GAMING);


	WORD wWeaveCard[IDD_GONG_COUNT];
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if (m_bPlayStates[i])
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_START_GAME;
			for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; j++)
			{
				out["UserGold"][j] = (Json::Int64)m_nUserGold[j];
			}
			out["BankUser"] = m_nBankUser;
			out["SmallBlindUser"] = m_nSmallBlindUser;
			out["BigBlindUser"] = m_nBigBlindUser;
			out["CurrentUser"] = m_nCurrentUser;
			Json::Value out2;
			for(int k=0;k<IDD_MAX_ROOM_USERCOUNT;k++)
			{
				out[k]["HandCard0"] = m_wHandCard[k][0];
				out[k]["HandCard1"] = m_wHandCard[k][1];
			}
            out["BasicScore"] = (Json::Int64)m_nBasicScore;
            out["CardType"] = m_GameLogic.GetCardType(m_wHandCard[i],m_wBoardData,0,wWeaveCard);
			m_g_GameRoom->SendTableMsg(i,out);
		}
	}
#ifdef _DEBUG
	network::System_Log("给所有玩家发送游戏开始消息 1001");
#endif

	return;
}
//发公共牌
void CServerLogicFrame::DealBoardCard(int Postion)
{
	//统计彩池
	StatisticsPot();
	//m_nMaxTurnAdd=GetMaxTurnAddd();

	m_nAddCount=0;
	m_nTurnBasicAdd=m_nBasicScore;
	m_nTurnBasicGold=0;
	memset(m_bAddStates,0,sizeof(m_bAddStates));


	m_nCurrentUser=GetNextPlayer(m_nBankUser);
	if (m_nAllPlayUserCount == 1)
	{
		m_nCurrentUser=INVALID_CHAIR;
	}

	//if (isAddOver())
	//{
	//	m_nCurrentUser=INVALID_CHAIR;
	//}

	for (int i=0 ; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if (m_UserOperate[i]!= OPERATE_OFFLINE)
		{
			m_UserOperate[i]=OPERATE_NULL;
		}


	}



	WORD wBestWeave[IDD_MAX_ROOM_USERCOUNT][IDD_WEAVE_COUNT];	//最佳组合数据
	WORD wWeaveType[IDD_MAX_ROOM_USERCOUNT];					//最佳组合牌型
	memset(wBestWeave,0,sizeof(wBestWeave));
	memset(wWeaveType,0,sizeof(wWeaveType));




	switch(Postion)
	{
	case 0:
		{
			m_wBoardData[m_nBoardCount++]=m_wCardData[m_nSendCardPos++];
			m_wBoardData[m_nBoardCount++]=m_wCardData[m_nSendCardPos++];
			m_wBoardData[m_nBoardCount++]=m_wCardData[m_nSendCardPos++];
		}
		break;
	case 3:
		{
			m_wBoardData[m_nBoardCount++]=m_wCardData[m_nSendCardPos++];
		}
		break;
	case 4:
		{
			m_wBoardData[m_nBoardCount++]=m_wCardData[m_nSendCardPos++];
		}
		break;
	}



	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		//获取摊牌玩家的最佳组合牌，以及类型
		wWeaveType[i]=m_GameLogic.GetCardType(m_wHandCard[i],m_wBoardData,m_nBoardCount,wBestWeave[i]);
	}

	for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; ++j)
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_DEAL_CARD;
        out["CurrentUser"] = m_nCurrentUser;
		switch(Postion)
		{
		case 0:
			{
				out["BoardData0"] = m_wBoardData[0];
				out["BoardData1"] = m_wBoardData[1];
				out["BoardData2"] = m_wBoardData[2];
			}
			break;
		case 3:
			{
				out["BoardData3"] = m_wBoardData[3];
			}
			break;
		case 4:
			{
				out["BoardData4"] = m_wBoardData[4];
			}
			break;
		}
		for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; ++i)
		{
			out["Pot"][i] = (Json::Int64)m_nPot[i];
		}
		out["PotCount"] = m_nPotCount;
		out["WeaveType"] = wWeaveType[j];
		m_g_GameRoom->SendTableMsg(j,out);
	}








	//all-in 无人再次加注
	if (m_nCurrentUser==INVALID_CHAIR)
	{
		if (m_nBoardCount==5)
		{
			m_endType=NORMAL;
			OnGameOver(m_endType);
			return;
		}
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_DEAL_CARD,TIME_DEAL_CARD);
	}
	else
	{
		//对新的操作玩家开始定时器
		m_g_GameRoom->StartTimer(IDD_TIMER_SERVER_GAMING,IDD_TIME_GAMING);
	}



	return;
}

bool CServerLogicFrame::isAddOver()
{
	//判断是否下注完成一轮
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if ( !m_bPlayStates[i])
		{
			continue;
		}




		if ( m_nTurnAddGold[i] != m_nUserGold[i] )
		{
			if ( !m_bAddStates[i] && m_nAllPlayUserCount>1)
			{
				return false;
			}

			if (m_nTurnAddGold[i]<m_nTurnBasicGold )
			{
				return false;
			}


		}
	}

	return true;
}


int CServerLogicFrame::GetNextPlayer(int playerId)
{
	int tempId=playerId;
	do
	{
		tempId=(tempId+1)%IDD_MAX_ROOM_USERCOUNT;
		if (tempId==playerId)break;

	} while ((!m_bPlayStates[tempId]) || m_nUserGold[tempId]==m_nTurnAddGold[tempId]);//玩家盖牌，玩家allin 都不用再加注

	if (tempId==playerId)
	{
		return INVALID_CHAIR;
	}
	return tempId;
}

//彩池统计
void CServerLogicFrame::StatisticsPot()
{
	//下注统计
	memset(m_nAddList,0,sizeof(m_nAddList));
	m_nAddUserCount=0;

	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if (m_bPlayStates[i])
		{
			m_nUserGold[i] -= m_nTurnAddGold[i];
			m_nAddGold[i] += m_nTurnAddGold[i];
			m_nTurnAddGold[i]=0;

			m_nAddList[m_nAddUserCount++]=m_nAddGold[i];
		}
	}

	if (m_nAddUserCount==1)
	{
		memset(m_nPot,0,sizeof(m_nPot));
		m_nPotCount=1;
		for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
		{
			m_nPot[0]+=m_nAddGold[i];
		}
	}
	else
	{
		//加注排序
		bool bSorted=true;
		int nLast=m_nAddUserCount-1;
		int64 nTemp;
		do
		{
			bSorted=true;
			for (int i=0;i<nLast;i++)
			{
				if (m_nAddList[i]>m_nAddList[i+1])
				{
					//交换位置
					nTemp=m_nAddList[i];
					m_nAddList[i]=m_nAddList[i+1];
					m_nAddList[i+1]=nTemp;
					bSorted=false;
				}
			}
			nLast--;
		} while(bSorted==false);


		nLast=m_nAddUserCount;
		int64 TempAllIn[IDD_MAX_ROOM_USERCOUNT];
		memset(TempAllIn,0,sizeof(TempAllIn));
		int AddUserCountTemp=0;
		for (int i=0; i<nLast; i++)
		{
			if (m_nAddList[i]>0)
			{
				TempAllIn[AddUserCountTemp++]=m_nAddList[i];
				while(i<nLast-1 && (m_nAddList[i]==m_nAddList[i+1]))
					i++;
			}
		}
		memcpy(m_nAddList,TempAllIn,sizeof(TempAllIn));
		m_nPotCount=AddUserCountTemp;
		m_nAddUserCount=AddUserCountTemp;


		//统计彩池
		memset(m_nPot,0,sizeof(m_nPot));
		for (int i=0; i<m_nPotCount; i++)
		{
			for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; j++)
			{
				m_nPot[i]+=std::min(m_nAddGold[j],m_nAddList[i]);
			}
		}
		for (int i=m_nAddUserCount-1; i>0; i--)
		{
			m_nPot[i]-=m_nPot[i-1];
		}
	}
	return;
}


//下盲注
void CServerLogicFrame::Blind()
{
	if (m_nAllPlayUserCount>2)
	{
		m_nSmallBlindUser=GetNextPlayer(m_nBankUser);
	}
	else
	{
		m_nSmallBlindUser=m_nBankUser;
	}

	m_nBigBlindUser=GetNextPlayer(m_nSmallBlindUser);

	m_nTurnAddGold[m_nSmallBlindUser]=m_nBasicScore/2;
	m_nTurnAddGold[m_nBigBlindUser]=m_nBasicScore;

	m_nTurnBasicAdd=m_nBasicScore*2;
	m_nTurnBasicGold=m_nBasicScore;

	m_UserOperate[m_nSmallBlindUser]=OPERATE_SML_BLD;
	m_UserOperate[m_nBigBlindUser]=OPERATE_BIG_BLD;

}


void CServerLogicFrame::OnGameOver(GameEndType endType)
{
	m_g_GameRoom->StopAllTimer();

	StatisticsPot();
	switch(endType)
	{
	case NORMAL:
		{
			WORD wBestWeave[IDD_MAX_ROOM_USERCOUNT][IDD_WEAVE_COUNT];	//最佳组合数据
			WORD wWeaveType[IDD_MAX_ROOM_USERCOUNT];					//最佳组合牌型
			memset(wBestWeave,0,sizeof(wBestWeave));
			memset(wWeaveType,0,sizeof(wWeaveType));

			int64 winGold[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_ROOM_USERCOUNT];
			memset(winGold,0,sizeof(winGold));


			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				if (m_bPlayStates[i])
				{
					//获取摊牌玩家的最佳组合牌，以及类型
					wWeaveType[i]=m_GameLogic.GetCardType(m_wHandCard[i],m_wBoardData,m_nBoardCount,wBestWeave[i]);
				}
			}

			//计算彩池 pot
			int WinUser[IDD_MAX_ROOM_USERCOUNT];		//最大牌型玩家列表
			int winCount=0;								//同牌型玩家数

			for (int i=m_nPotCount; i>0; i--)
			{
				//赢家获取
				for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; j++)
				{
					if (m_nAddGold[j]==m_nAddList[i-1])
					{
						//跳过盖牌放弃玩家
						if (!m_bPlayStates[j])continue;

						//保存第一个玩家，不比较
						if (winCount==0)
						{
							WinUser[winCount++]=j;
							continue;
						}

						//赢家和后面牌型玩家比较，刷新赢家，同样大也保存起来
						int temp=m_GameLogic.CompareCard(wBestWeave[WinUser[0]],wBestWeave[j],wWeaveType[WinUser[0]],wWeaveType[j]);
						if (temp<0)
						{
							WinUser[0]=j;
							winCount=1;
						}
						else if (temp==0)//保存同样大小牌型的玩家
							WinUser[winCount++]=j;

					}
				}

				//对赢家进行金币收入统计
				int64 everyWinGold = m_nPot[i-1]/winCount;
				for (int j=0; j<winCount; j++)
				{
					m_nUserGold[WinUser[j]]+=everyWinGold;
				}

				//不能平分金币处理

				int64 lastGold= m_nPot[i-1]-everyWinGold*winCount;
				if ( lastGold!=0)
				{
					int lastUser=m_nBankUser;;
					bool islast=true;
					while(islast)
					{
						for (int j=0; j<winCount; j++)
						{
							if (lastUser==WinUser[j])
							{
								m_nUserGold[lastUser]+=lastGold;
								islast=false;
								break;
							}
						}
						if (islast)
							lastUser=(lastUser+1)%IDD_MAX_ROOM_USERCOUNT;
					}
				}

				//保存每轮结算信息
				memcpy(winGold[m_nPotCount-i], m_nUserGold, sizeof(m_nUserGold));
			}



			//扣税
			enScoreKind ScoreKind;
			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				if (m_bPlayStates[i])
				{
					Player *pPlayer = m_g_GameRoom->GetPlayer(i);
					if(pPlayer == NULL) continue;

					int64 revenue=0;
					int64 lAgentRevenue=0;
					int64 winScore=m_nUserGold[i]-m_nTurnStartGold[i];

					if (winScore>0)
					{
						ScoreKind=enScoreKind_Win;
						revenue=(int64)(winScore*m_g_GameRoom->GetChouShui());
						m_nUserGold[i] -= revenue;
						winScore -= revenue;
						winGold[m_nPotCount][i] -= revenue;
					}
					else if (winScore==0)
					{
						ScoreKind=enScoreKind_Draw;
					}
					else
						ScoreKind=enScoreKind_Lost;

					/*if(winScore > 0)
					{
						float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
						lAgentRevenue = (int64)((float)winScore * pAgentRate);
						winScore -= lAgentRevenue;
					}*/

					m_g_GameRoom->WriteUserScore(i,winScore,revenue,ScoreKind,lAgentRevenue);
				}
			}

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GAME_OVER;
            out["NORMAL"] = NORMAL;
            out["PotCount"] = m_nPotCount;
            for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; ++j)
				{
					out["winGold"][i][j] = (Json::Int64)winGold[i][j];
				}
			}

			Json::Value out2;
			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				out2[i]["UserName"] = m_sUserName[i];		//昵称
				out2[i]["PlayStates"] = m_bPlayStates[i]?1:0 ;	//在玩状态
				out2[i]["Pot"] = (Json::Int64)m_nPot[i];					//彩池
				out2[i]["HandCard0"] = m_wHandCard[i][0];			//手牌1
				out2[i]["HandCard1"] = m_wHandCard[i][1];			//手牌2
				out2[i]["WeaveType"] = wWeaveType[i];				//最佳组合类型
				out2[i]["BestWeave0"] = wBestWeave[i][0];			//最佳组合牌1
				out2[i]["BestWeave1"] = wBestWeave[i][1];			//最佳组合牌2
				out2[i]["BestWeave2"] = wBestWeave[i][2];			//最佳组合牌3
				out2[i]["BestWeave3"] = wBestWeave[i][3];			//最佳组合牌4
				out2[i]["BestWeave4"] = wBestWeave[i][4];			//最佳组合牌5
			}
			out["Users"] = out2;
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

			memset(m_bPlayStates,0,sizeof(m_bPlayStates));
			m_g_GameRoom->GameEnd();
		}
		break;
	case GIVEUP:
		{
			int winUser=INVALID_CHAIR;
			int64 winGold[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_ROOM_USERCOUNT];
			memset(winGold,0,sizeof(winGold));

			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				if (m_bPlayStates[i])
					winUser=i;
			}
			m_nUserGold[winUser]+=m_nPot[0];


			//扣税
			enScoreKind ScoreKind;
			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				if (m_bPlayStates[i])
				{
					Player *pPlayer = m_g_GameRoom->GetPlayer(i);
					if(pPlayer == NULL) continue;

					int64 revenue=0;
					int64 lAgentRevenue=0;
					int64 winScore=m_nUserGold[i]-m_nTurnStartGold[i];

					if (winScore>0)
					{
						ScoreKind=enScoreKind_Win;
						revenue=(int64)(winScore*m_g_GameRoom->GetChouShui());
						m_nUserGold[i] -= revenue;
						winScore -= revenue;
						winGold[0][i] = m_nUserGold[i];
					}
					else if (winScore==0)
					{
						ScoreKind=enScoreKind_Draw;
					}
					else
						ScoreKind=enScoreKind_Lost;

					/*if(winScore > 0)
					{
						float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
						lAgentRevenue = (int64)((float)winScore * pAgentRate);
						winScore -= lAgentRevenue;
					}*/

					m_g_GameRoom->WriteUserScore(i,winScore,revenue,ScoreKind,lAgentRevenue);
				}
			}

			memcpy(&winGold[0],m_nUserGold,sizeof(m_nUserGold));

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GAME_OVER;
            out["GIVEUP"] = GIVEUP;
            out["m_nPotCount"] = m_nPotCount;
			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				for (int j=0; j<IDD_MAX_ROOM_USERCOUNT; ++j)
				{
					out["winGold"][i][j] = (Json::Int64)winGold[i][j];
				}
			}
			Json::Value out2;
			for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
			{
				out[i]["UserName"] = m_sUserName[i];
				out[i]["PlayStates"] = m_bPlayStates[i]?1:0 ;
				out[i]["Pot"] = (Json::Int64)m_nPot[i];
				out[i]["HandCard0"] = m_wHandCard[i][0];
				out[i]["HandCard1"] = m_wHandCard[i][1];
			}
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

			memset(m_bPlayStates,0,sizeof(m_bPlayStates));
			m_g_GameRoom->GameEnd();

		}
		break;
	}

	//结束录像
	m_g_GameRoom->EndVideoTape();

	return;
}

//获取本轮最大下注
int64 CServerLogicFrame::GetMaxTurnAddd()
{
	int64 TempUserGOld[IDD_MAX_ROOM_USERCOUNT];
	memset(TempUserGOld,0,sizeof(TempUserGOld));
	for (int i=0; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		if (m_bPlayStates[i])
		{
			TempUserGOld[i]=m_nUserGold[i];
		}
	}

	//排序操作
	bool bSorted=true;
	int64 nTempData,wLast=IDD_MAX_ROOM_USERCOUNT-1;
	do
	{
		bSorted=true;
		for (int i=0;i<wLast;i++)
		{
			if (TempUserGOld[i]<TempUserGOld[i+1])
			{
				//交换位置
				nTempData=TempUserGOld[i];
				TempUserGOld[i]=TempUserGOld[i+1];
				TempUserGOld[i+1]=nTempData;
				bSorted=false;
			}
		}
		wLast--;
	} while(bSorted==false);

	return TempUserGOld[1];
}



void CServerLogicFrame::ClearData()
{
	memset(m_nTurnAddGold,0,sizeof(m_nTurnAddGold));
	memset(m_bPlayStates,0,sizeof(m_bPlayStates));
	memset(m_nAddGold,0,sizeof(m_nAddGold));
	memset(m_nAddList,0,sizeof(m_nAddList));
	memset(m_nPot,0,sizeof(m_nPot));
	memset(m_wCardData,0,sizeof(m_wCardData));
	memset(m_wHandCard,0,sizeof(m_wHandCard));
	memset(m_wBoardData,0,sizeof(m_wBoardData));
	memset(m_bAddStates,0,sizeof(m_bAddStates));
	memset(m_nTurnStartGold,0,sizeof(m_nTurnStartGold));

	for(int i=0;i<IDD_MAX_ROOM_USERCOUNT;i++)
		m_sUserName[i].clear();

	m_nCurrentUser=INVALID_CHAIR;
	m_nSmallBlindUser=INVALID_CHAIR;
	m_nBigBlindUser=INVALID_CHAIR;

	m_nAddUserCount=0;
	m_nAddCount=0;
	m_nTurnBasicGold=0;
	m_nTurnBasicAdd=0;
	m_nAllPlayUserCount=0;
	m_nPotCount=0;
	m_nSendCardPos=0;
	m_nBoardCount=0;
	//m_nMaxTurnAdd=0;


	for (int i=0 ; i<IDD_MAX_ROOM_USERCOUNT; i++)
	{
		m_UserOperate[i]=OPERATE_NULL;
	}
}




#ifdef _DEBUG
bool CServerLogicFrame::ForTest()
{
	/*TCHAR cIsDebug[5]=TEXT("");
	GetPrivateProfileString(TEXT("Test"), TEXT("IsDebug"),TEXT(""), cIsDebug, 5*sizeof(TCHAR), TEXT("./ForTest.ini"));
	int iIsDebug = _ttoi(cIsDebug);
	if (iIsDebug)
	{
		TCHAR cData[7*IDD_MAX_CARD_COUNT+1] = TEXT("");
		GetPrivateProfileString(TEXT("Test"), TEXT("dzpk"), TEXT(""), cData, 7*IDD_MAX_CARD_COUNT*sizeof(TCHAR), TEXT("./ForTest.ini"));
		TCHAR cTmp[7] = TEXT("");
		TCHAR ccTmp[1] = TEXT("");
		for (int i=0;i<IDD_MAX_CARD_COUNT;i++)
		{
			memcpy(cTmp,&cData[i*7], 7 * sizeof(TCHAR));
			ccTmp[0] = cTmp[3];
			int aaa = _ttoi(ccTmp)<<8;
			ccTmp[0] = cTmp[5];
			aaa += _tcstol(ccTmp,0,16);
			m_wCardData[i] = aaa;
		}
	}
	return iIsDebug > 0 ? true : false;*/
	return true;
}
#endif
