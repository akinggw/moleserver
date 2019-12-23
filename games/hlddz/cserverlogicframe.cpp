#include "cserverlogicframe.h"
#include <sstream>

//宏定义
#define	GOOD_CARD_PERCENT				4									//好牌概率

//游戏定时器
#define IDI_LAND_SCORE_MISS					IDD_TIMER_GAME_START+5							//叫分断线定时器
#define IDI_OUT_CARD_MISS					IDD_TIMER_GAME_START+12							//出牌断线定时器
#define IDI_OUT_CARD						IDD_TIMER_GAME_START+18							//断线出牌定时器
#define IDI_FIRST_USER_OUT_CARD_MISS		IDD_TIMER_GAME_START+22							//首玩家断线出牌定时器
#define IDI_DELAY_READY						IDD_TIMER_GAME_START+23							//比赛玩家延时发准备消息
//#define IDI_DELAY_END						IDD_TIMER_GAME_START+25							//延时结束定时器
//#define IDI_GAME_OVER						IDD_TIMER_GAME_START+27
//#define IDI_START_USERMISS				IDD_TIMER_GAME_START+28

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{
	m_bCallScorePhase=CSD_NORMAL;
	//游戏变量
	m_BoomCount = 0;
	m_cbCount=0;
	m_lSnatchLand=1;
	m_lBrightCard=1;
	m_wBombTime=1;
	m_lThuntian=1;
	m_wShowTimes=1;
	m_bLandScore=0;
	m_breSendCardTimes=0;
	m_bEndFlag=false;
	m_MatchOver=false;
	//m_FirstUserLife=false;
	m_wFirstUser=rand()%GAME_PLAYER;
	m_bEndOrReadyFlag=true;
	m_wCloseWarmingFlash=false;
	m_GameOver=false;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wChairID=INVALID_CHAIR;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));
	m_UserLeaveCount=0;
	memset( m_bUserOffLine,0, sizeof( m_bUserOffLine ) );
	memset( m_PassTime, 0,sizeof( m_PassTime ) );

	//运行信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );
	memset(m_bUserPass,0,sizeof(m_bUserPass));
	memset(m_UserStandUp,0,sizeof(m_UserStandUp));
	memset( m_bGiveUpBanker, 0,sizeof( m_bGiveUpBanker ) );

	//扑克信息
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	memset(m_cbNowTurnOutCard,	0,	sizeof(m_cbNowTurnOutCard));
	memset(m_nNowTurnOutCount,	0,	sizeof(m_nNowTurnOutCount));
	memset(m_cbLastTurnOutCard,	0,	sizeof(m_cbLastTurnOutCard));
	memset(m_nLastTurnOutCount,	0,	sizeof(m_nLastTurnOutCount));
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

	Clear();

	m_bCallScorePhase=CSD_NORMAL;
	//设置状态
	m_curGameState=GS_WK_SCORE;
	//游戏变量
	memset( m_bGiveUpBanker, 0,sizeof( m_bGiveUpBanker ) );
	memset( m_bBackCard, 0,sizeof( m_bBackCard ) );
	memset( m_bHandCardData, 0,sizeof( m_bHandCardData ) );

	//开始录像
	Player *pPlayer = m_g_GameRoom->GetPlayer(m_wFirstUser);
	if(pPlayer == NULL) return;
	//m_g_GameRoom->StartVideoTape(pPlayer,m_g_GameRoom);

	//m_FirstUserLife=true;
	m_GameOver=false;
	m_UserLeaveCount=0;
	m_bLandScore=0;
	int64 pResult,pMax,pMin;
	pResult=pMax=pMin=0;
	int robotState = m_g_GameRoom->IsUserWin(&pResult,&pMax,&pMin);//1赢2输3不赢不输
	uint16 cbAndroidUserCount = 0 ;
	uint16 wAndroidUser[ GAME_PLAYER ] = {0};

	//机器人个数
	for ( uint8 wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer(wChairID);
		if (pPlayer==NULL)	continue;
		if (pPlayer->GetType()==1 )
		{
			wAndroidUser[ cbAndroidUserCount ] = ( wChairID ) ;
			++cbAndroidUserCount ;
		}
	}

	uint16 wHaveGoodCardAndroidUser = INVALID_CHAIR ;
	if ( 0 < cbAndroidUserCount )
	{
		wHaveGoodCardAndroidUser = wAndroidUser[ rand() % cbAndroidUserCount ] ;
	}

	//发送好牌
	if ( 2 == robotState && wHaveGoodCardAndroidUser != INVALID_CHAIR )
	{
		//混乱扑克
		uint8 bRandCard[54];
		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

		uint8 cbGoodCard[ NORMAL_COUNT ] ;
		m_GameLogic.GetGoodCardData( cbGoodCard ) ;

		//抽取好牌
		m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

		//分发扑克
		memcpy( m_bHandCardData[ wHaveGoodCardAndroidUser ], cbGoodCard, sizeof( uint8 ) *NORMAL_COUNT ) ;
		m_bCardCount[ wHaveGoodCardAndroidUser ] = NORMAL_COUNT;
		for ( uint8 i = 0, j = 0; i < GAME_PLAYER; i++ )
		{
			if ( i != wHaveGoodCardAndroidUser )
			{
				m_bCardCount[ i ] = 17;
				memcpy( &m_bHandCardData[ i ], &bRandCard[ j * 17 ], sizeof( uint8 ) * 17 );
				++j ;
			}
		}
		memcpy( m_bBackCard, &bRandCard[ 34 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
	}
	else
	{
		//混乱扑克
		uint8 bRandCard[54];
		//测试配牌
#ifdef _DEBUG
		if (!Test(bRandCard))
#endif
		{
			//if (rand()%100 < m_g_GameRoom->GetUserWinOrLostRate() && cbAndroidUserCount != GAME_PLAYER)
			//{
			//	m_GameLogic.GetGoodCard(bRandCard);
			//}
			//else
				m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );//混乱扑克
		}
		/*uint8 bRandCard[54];
		m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );*/

		//分发扑克
		for ( uint8 i = 0; i < GAME_PLAYER; i++ )
		{
			m_bCardCount[ i ] = 17;
			memcpy( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( uint8 ) * m_bCardCount[ i ] );
		}
		memcpy( m_bBackCard, &bRandCard[ 51 ], sizeof( m_bBackCard ) );
		m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
	}

	//设置首叫分用户
	if ( m_wCurrentUser == INVALID_CHAIR )
	{
		m_wCurrentUser = m_wFirstUser;
	}

    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = SUB_S_SEND_CARD;
    out["CurrentUser"] = m_wCurrentUser;
	for ( uint8 i = 0; i < 3; i++ )
	{
		out["bBackCard"][i] = m_GameLogic.ChangeCardDate(m_bBackCard[i]);
	}
	uint8 uindex = 0;
	for ( uint8 i = 0; i < GAME_PLAYER; i++ )
	{
		for ( uint8 j = 0; j < 17; j++ )
		{
			out["cbHandCardData"][uindex++] = m_GameLogic.ChangeCardDate(m_bHandCardData[i][j]);
		}
	}
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	//test
	/*CString str=TEXT("游戏开始！");
	OutputDebugString(str);*/

	//排列扑克
	for ( uint8 i = 0; i < GAME_PLAYER; i++ )
	{
		m_GameLogic.SortCardList( m_bHandCardData[ i ], NORMAL_COUNT, ST_ORDER );
	}
}

/// 用于处理用户进入游戏房间后的消息
void CServerLogicFrame::OnProcessPlayerRoomMes(int playerId,Json::Value &mes)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	int subMsgID = mes["MsgSubId"].asInt();

	switch(subMsgID)
	{
	case SUB_C_SHOW_FACE:
		{
			int tempFaceId= mes["tempFaceId"].asInt();

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = SUB_S_SHOW_FACE;
            out["playerId"]	= playerId;
            out["tempFaceId"] = tempFaceId;
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		}
		break;
	case SUB_C_SHOW_MSG:
		{
			int tempMsgId= mes["tempMsgId"].asInt();

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = SUB_S_SHOW_MSG;
            out["playerId"]	= playerId;
            out["tempMsgId"] = tempMsgId;
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
		}
		break;
	case SUB_C_LAND_SCORE:
		{
			int bLandScore=mes["bLandScore"].asInt();
			OnUserLandScore(playerId,bLandScore);
		}
		break;
	case SUB_C_BRIGHT_CARD:
		{
			OnUserBrightCard(playerId);
		}
		break;
	case SUB_C_OUT_CARD:
		{
			int bCardCount=mes["bCardCount"].asInt();
			uint8 bTurnOutCard[20];
			for (int i=0;i<bCardCount;i++)
			{
				bTurnOutCard[i]=m_GameLogic.OnChangeCardDate(mes["bTurnOutCard"].asInt());
			}

			/*CString test;
			for (int i=0;i<bCardCount;i++)
			{
				test.Format(TEXT("test1:%d    "),bTurnOutCard[i]);
				OutputDebugString(test);
			}	*/

			OnUserOutCard(playerId,bTurnOutCard,bCardCount);
		}
		break;
	case SUB_C_PASS_CARD:
		{
			OnUserPassCard(playerId);
		}
		break;
	case SUB_C_TRUSTEE:
		{
			int isAutoPlay=mes["isAutoPlay"].asInt();
			OnUserTrustee(playerId,isAutoPlay);
		}
		break;
	default:
		break;
	}
}

/// 处理用户进入房间消息
void CServerLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if(m_g_GameRoom->GetRoomState() != ROOMSTATE_GAMING)
		return;

	OverGame(playerId,m_g_GameRoom,true);
}

/// 处理用户断线消息
void CServerLogicFrame::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	m_bUserOffLine[playerId] = true;

	//玩家断线托管状态取消
	//if (m_bUserTrustee[ playerId ]==true)
	//{
	//	//变量定义
	//	CMD_C_UserOffLineTrustee UserOffLineTrustee;
	//	ZeroMemory( &UserOffLineTrustee, sizeof( UserOffLineTrustee ) );
	//	UserOffLineTrustee.wUserChairID = playerId;
	//	UserOffLineTrustee.bUserOffLine=true;
	//	//发送消息
	//	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_OFFLINETRUSTEE, &UserOffLineTrustee,sizeof(UserOffLineTrustee));
	//}

	if (m_wCurrentUser==playerId )
	{
		if (m_curGameState==GS_WK_SCORE )
		{
			//当前玩家断线超时不叫
			if(m_bUserOffLine[m_wCurrentUser] && m_bCallScorePhase!=CSD_GAMESTART )
			{
				m_g_GameRoom->StartTimer(IDI_LAND_SCORE_MISS + playerId,30);
			}
		}
		//首玩家或当前玩家首出牌断线超时出最小牌
		if ( m_curGameState==GS_WK_PLAYING && m_bUserOffLine[m_wCurrentUser] && m_bTurnCardCount==0 )
		{
			m_g_GameRoom->StartTimer(IDI_FIRST_USER_OUT_CARD_MISS,30);
		}
		//当前玩家非首次出牌断线超时不出牌
		if ( m_curGameState==GS_WK_PLAYING && m_bUserOffLine[m_wCurrentUser] && m_bTurnCardCount!=0)
		{
			m_g_GameRoom->StartTimer(IDI_OUT_CARD_MISS + playerId,30);
		}
	}
}

/// 处理用户断线重连消息
void CServerLogicFrame::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	m_bUserOffLine[playerId] = false;
	m_PassTime[playerId]=false;
	if (playerId==m_wCurrentUser)
	{
		if (m_curGameState==GS_WK_SCORE)
		{
			m_g_GameRoom->StopTimer(IDI_LAND_SCORE_MISS+playerId);
		}
		if (m_curGameState==GS_WK_PLAYING && m_bTurnCardCount==0 )
		{
			m_g_GameRoom->StopTimer(IDI_FIRST_USER_OUT_CARD_MISS);
		}
		if (m_curGameState==GS_WK_PLAYING && m_bTurnCardCount!=0 )
		{
			m_g_GameRoom->StopTimer(IDI_OUT_CARD_MISS+playerId);
		}
	}
	switch(m_curGameState)
	{
		case GS_WK_SCORE:
			{
				//发送消息
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = SUB_S_LAND_SCORE_STATION;
                out["ChairId"]=playerId;
                out["CurrentUser"]=m_wCurrentUser;
                out["ShowTimes"]=m_wShowTimes;
                out["CallScorePhase"]=m_bCallScorePhase;

				for (int i=0;i<GAME_PLAYER;i++)
				{
					out["bScore"][i] = m_bScoreInfo[i];
				}
				for (int i=0;i<GAME_PLAYER;i++)
				{
					out["bUserTrustee"][i] = ((m_bUserTrustee[i])?1:0);
				}
				for ( uint16 i = 0; i < 3; i++ )
				{
					mes["bBackCard"][i] = m_GameLogic.ChangeCardDate(m_bBackCard[i]);
				}

				uint8 uindex = 0;
				for ( uint16 i = 0; i < GAME_PLAYER; i++ )
				{
					for ( uint16 j = 0; j < 17; j++ )
					{
						out["bHandCardData"][uindex++] = m_GameLogic.ChangeCardDate(m_bHandCardData[i][j]);

					}
				}

				m_g_GameRoom->SendTableMsg(playerId,out);
			}
			break;
		case GS_WK_PLAYING:
			{
                /*CString test;
				test.Format(TEXT("OnProcessReEnterRoomMes:%d,%d   "),playerId,m_wCurrentUser);
				OutputDebugString(test);*/

                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = SUB_S_PLAYING_STATION;
                out["ChairId"] = playerId;
                out["wCurrentUser"] = m_wCurrentUser;
                out["wShowTimes"] = m_wShowTimes;
                out["wLandUser"] = m_wBankerUser;
                out["wTurnWiner"] = m_wTurnWiner;
                out["wMatchOver"] = (m_MatchOver) ? 1 : 0;
				int bBrightCard=0;
				for (int i=0;i<GAME_PLAYER;i++)
				{
					bBrightCard=(m_bBrightCard[i]) ? 1 : 0;
					out["bBrightCard"][i] = bBrightCard;
				}
				for (int i=0;i<GAME_PLAYER;i++)
				{
					out["bUserTrustee"][i] = ((m_bUserTrustee[i]) ? 1 : 0);
				}
				for (int i=0;i<GAME_PLAYER;i++)
				{
					out["IsPass"][i] = ((m_bUserPass[i]) ? 1 : 0);
				}
				out["cbTurnCardCount"] = m_bTurnCardCount;
				for (int i=0;i<m_bTurnCardCount;i++)
				{
					out["cbTurnCardData"][i]=m_GameLogic.ChangeCardDate(m_bTurnCardData[i]);
				}
				for ( uint16 i = 0; i < 3; i++ )
				{
					out["bBackCard"][i]=m_GameLogic.ChangeCardDate(m_bBackCard[i]);
				}

				for ( uint16 i = 0; i < GAME_PLAYER; i++ )
				{
					out["bHandCardCount"][i] = m_bCardCount[i];
					for ( uint16 j = 0; j < m_bCardCount[i]; j++ )
					{
						out["bHandCardData"][i][j]=m_GameLogic.ChangeCardDate(m_bHandCardData[i][j]);
					}
				}
				//上轮出牌记录
				for (int i=0; i < GAME_PLAYER; i++)
				{
					out["LastTurnOutCount"][i] = m_nLastTurnOutCount[i];
					for (int j=0; j<m_nLastTurnOutCount[i]; j++)
					{
						out["LastTurnOutCard"][i][j]=m_GameLogic.ChangeCardDate(m_cbLastTurnOutCard[i][j]);
					}
				}
				//本轮出牌记录
				for (int i=0; i < GAME_PLAYER; i++)
				{
					out["NowTurnOutCount"][i]=m_nNowTurnOutCount[i];
					for (int j=0; j<m_nNowTurnOutCount[i]; j++)
					{
						out["NowTurnOutCard"][i][j]=m_GameLogic.ChangeCardDate(m_cbNowTurnOutCard[i][j]);
					}
				}
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
			break;
		default:
			break;
	}
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	switch(timerId)
	{
		//断线超时不叫
	case IDI_LAND_SCORE_MISS:
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_LAND_SCORE_MISS);
				if(m_bUserOffLine[0] == true)
				{
					OnUserLandScore( 0, 4);
					m_PassTime[0]=true;
				}
			}
			return ;
		}

	case IDI_LAND_SCORE_MISS+1:
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_LAND_SCORE_MISS+1);

				if(m_bUserOffLine[1] == true)
				{
					OnUserLandScore( 1, 4);
					m_PassTime[1]=true;
				}
			}
			return ;

		}
	case IDI_LAND_SCORE_MISS+2:
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_LAND_SCORE_MISS+2);
				if(m_bUserOffLine[2] == true)
				{
					OnUserLandScore( 2, 4);
					m_PassTime[2]=true;
				}
			}
			return ;

		}
		//断线超时托管不出
	case IDI_OUT_CARD_MISS:
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_OUT_CARD_MISS);

				if(m_bUserOffLine[0] == true)
				{
					//发送数据
					OnUserPassCard(0);
					m_PassTime[0]=true;
				}
			}
			return ;
		}
	case IDI_OUT_CARD_MISS+1:
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_OUT_CARD_MISS+1);

				if(m_bUserOffLine[1] ==true)
				{
					//发送数据
					OnUserPassCard(1);
					m_PassTime[1]=true;
				}
			}
			return ;
		}
	case IDI_OUT_CARD_MISS+2:
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_OUT_CARD_MISS+2);

				if(m_bUserOffLine[2] == true)
				{
					//发送数据
					OnUserPassCard(2);
					m_PassTime[2]=true;
				}
			}
			return ;
		}

	case IDI_FIRST_USER_OUT_CARD_MISS:
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_FIRST_USER_OUT_CARD_MISS);

				m_PassTime[m_wCurrentUser]=true;

				uint8 pCardData[1];
				uint16 pCardCount;
				pCardCount=m_bCardCount[m_wCurrentUser];
				pCardData[0] = 	m_bHandCardData[m_wCurrentUser][pCardCount-1];

				OnUserOutCard(m_wCurrentUser, pCardData,1);
			}
			return ;
		}
	//case IDI_DELAY_END:
	//	{
	//		if (curTimer==0)
	//		{
	//			m_g_GameRoom->StopTimer(IDI_DELAY_END);
	//
	//			//OnEventGameEnd(m_wChairID,NULL,GER_NORMAL);
	//		}
	//		return ;
	//	}
	//case IDI_GAME_OVER:
	//	{
	//		if (curTimer==0)
	//		{
	//			m_g_GameRoom->StopTimer(IDI_GAME_OVER);
	//			//m_pITableFrame->SendTableData( INVALID_CHAIR, SUB_S_USER_FREE );
	//		}
	//		return ;
	//	}
	//case IDI_START_USERMISS:
	//	{
	//		if (curTimer==0)
	//		{
	//			m_g_GameRoom->StopTimer(IDI_START_USERMISS);
	//
	//			////用户开始游戏后离开
	//			//for (WORD i=0;i<GAME_PLAYER;i++)
	//			//{
	//			//	IServerUserItem *pIServerUserItem = m_pITableFrame->GetServerUserItem(i);
	//			//	tagServerUserData * pUserData= pIServerUserItem->GetUserData();
	//			//	if (m_UserStandUp[i]==true && pUserData->cbUserStatus==US_PLAY)
	//			//	{
	//			//		// 发送用户强退消息
	//			//		if(pIServerUserItem)
	//			//			OnEventGameEnd(i,pIServerUserItem,GER_USER_LEFT);
	//			//		else
	//			//			OnEventGameEnd(i,NULL,GER_FREE);

	//			//		break;
	//			//	}
	//			//}
	//		}
	//		return ;
	//	}


	case IDI_DELAY_READY:	//延时发准备消息
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(IDI_DELAY_READY);
				for(int i=0;i<GAME_PLAYER;i++)
				{
					Player *pPlayer = m_g_GameRoom->GetPlayer(i);
					if(pPlayer == NULL) continue;

					//if(pPlayer->GetType() == PLAYERTYPE_ROBOT) continue;

					//if(pPlayer->GetMatchCount() > 0)
					//{
					//	pPlayer->SendReadyMsg();
					//}
				}
			}
			return ;
		}
	}
}

////////////////////////////////////////////////////////////////////////////
//叫分事件
bool CServerLogicFrame::OnUserLandScore(WORD wChairID, int bLandScore)
{
	//效验状态
	if (m_curGameState!=GS_WK_SCORE) return true;

	//test
	//CString str;
	//str.Format(TEXT("%d/n"),bLandScore);
	//OutputDebugString(str);

	//变量定义
	bool bContinue=false;
	bool m_delayshowtimer=false;
	int cbPreCallScorePhase = m_bCallScorePhase;

	//叫地主阶段
	if (m_bCallScorePhase==CSD_NORMAL)
	{
		//效验参数
		ASSERT(((bLandScore==1)||(bLandScore==4)));
		if ((bLandScore!=1)&&(bLandScore!=4)) return false;

		//设置变量
		if (bLandScore!=4)
		{
			m_bLandScore=bLandScore;
			m_wBankerUser=wChairID;
		/*	CString test;
			test.Format(TEXT("m_wBankerUser==%d  "),m_wBankerUser);
			OutputDebugString(test);*/
		}
		else
		{
			bContinue = true;
			m_bGiveUpBanker[wChairID] = true;

			//切换玩家
			m_wCurrentUser=(wChairID+1)%GAME_PLAYER;

			//重新发牌
			if ( m_bGiveUpBanker[m_wCurrentUser] )
			{
				//重新发牌
				if ( m_wBankerUser == INVALID_CHAIR )
				{
					m_breSendCardTimes++;

					if(m_breSendCardTimes==3)
					{
						m_breSendCardTimes=0;
						OverGame(INVALID_CHAIR,m_g_GameRoom,false);
						return true;
					}
					else
					{
						//发送消息
						CMolMessageOut out(IDD_MESSAGE_ROOM);
						out.write16(SUB_S_LAND_SCORE);
						out.write16(wChairID);
						out.write16(bLandScore);
						out.write16(m_wCurrentUser);
						out.write16(m_wShowTimes);
						out.write16(m_bCallScorePhase);
						out.write16(cbPreCallScorePhase);
						m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

						m_bCallScorePhase=CSD_NORMAL;
						//设置状态
						m_curGameState=GS_WK_SCORE;


						//游戏变量
						ZeroMemory( m_bGiveUpBanker, sizeof( m_bGiveUpBanker ) );
						ZeroMemory( m_bBackCard, sizeof( m_bBackCard ) );
						ZeroMemory( m_bHandCardData, sizeof( m_bHandCardData ) );
						ZeroMemory( m_bBrightCard, sizeof( m_bBrightCard ) );

						//开始录像
						Player *pPlayer = m_g_GameRoom->GetPlayer(m_wFirstUser);
						if(pPlayer == NULL) return false;
						m_g_GameRoom->StartVideoTape(pPlayer,m_g_GameRoom);

						m_bLandScore=0;
						int64 pResult,pMax,pMin;
						pResult=pMax=pMin=0;
						int robotState = m_g_GameRoom->IsUserWin(&pResult,&pMax,&pMin);//1赢2输3不赢不输
						WORD cbAndroidUserCount = 0 ;
						WORD wAndroidUser[ GAME_PLAYER ] = {0};

						//机器人个数
						for ( BYTE wChairID = 0; wChairID < GAME_PLAYER; ++wChairID )
						{
							Player *pPlayer = m_g_GameRoom->GetPlayer(wChairID);
							if (pPlayer==NULL)	continue;
							if (pPlayer->GetType()==1 )
							{
								wAndroidUser[ cbAndroidUserCount ] = ( wChairID ) ;
								++cbAndroidUserCount ;
							}
						}

						WORD wHaveGoodCardAndroidUser = INVALID_CHAIR ;
						if ( 0 < cbAndroidUserCount )
						{
							wHaveGoodCardAndroidUser = wAndroidUser[ rand() % cbAndroidUserCount ] ;
						}

						//发送好牌
						if ( 2 == robotState && wHaveGoodCardAndroidUser != INVALID_CHAIR )
						{
							//混乱扑克
							BYTE bRandCard[54];
							m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );

							BYTE cbGoodCard[ NORMAL_COUNT ] ;
							m_GameLogic.GetGoodCardData( cbGoodCard ) ;

							//抽取好牌
							m_GameLogic.RemoveGoodCardData( cbGoodCard, NORMAL_COUNT, bRandCard, FULL_COUNT ) ;

							//分发扑克
							CopyMemory( m_bHandCardData[ wHaveGoodCardAndroidUser ], cbGoodCard, sizeof( BYTE ) *NORMAL_COUNT ) ;
							m_bCardCount[ wHaveGoodCardAndroidUser ] = NORMAL_COUNT;
							for ( WORD i = 0, j = 0; i < GAME_PLAYER; i++ )
							{
								if ( i != wHaveGoodCardAndroidUser )
								{
									m_bCardCount[ i ] = 17;
									CopyMemory( &m_bHandCardData[ i ], &bRandCard[ j * 17 ], sizeof( BYTE ) * 17 );
									++j ;
								}
							}
							CopyMemory( m_bBackCard, &bRandCard[ 34 ], sizeof( m_bBackCard ) );
							m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
						}
						else
						{
							//混乱扑克
							BYTE  bRandCard[ 54 ];
							if (rand()%100 < m_g_GameRoom->GetUserWinOrLostRate())
							{
								m_GameLogic.GetGoodCard(bRandCard);
							}
							else
								m_GameLogic.RandCardList( bRandCard, sizeof( bRandCard ) / sizeof( bRandCard[ 0 ] ) );//混乱扑克

							//分发扑克
							for ( WORD i = 0; i < GAME_PLAYER; i++ )
							{
								m_bCardCount[ i ] = m_bCardCount[ i ];
								CopyMemory( &m_bHandCardData[ i ], &bRandCard[ i * m_bCardCount[ i ] ], sizeof( BYTE ) * m_bCardCount[ i ] );
							}
							CopyMemory( m_bBackCard, &bRandCard[ 51 ], sizeof( m_bBackCard ) );
							m_GameLogic.SortCardList( m_bBackCard, 3, ST_ORDER );
						}

						//设置用户
						if ( m_wCurrentUser == INVALID_CHAIR )
						{
							m_wCurrentUser = m_wFirstUser;
						}

						CMolMessageOut out2(IDD_MESSAGE_ROOM);
						out2.write16(SUB_S_SEND_CARD);
						out2.write16(m_wCurrentUser);
						for ( WORD i = 0; i < 3; i++ )
						{
							out2.write16(m_GameLogic.ChangeCardDate(m_bBackCard[i]));
						}
						for ( WORD i = 0; i < GAME_PLAYER; i++ )
						{
							for ( WORD j = 0; j < 17; j++ )
							{
								out2.write16(m_GameLogic.ChangeCardDate(m_bHandCardData[i][j]));
							}
						}
						for ( WORD i = 0; i < GAME_PLAYER; i++ )
						{
							m_g_GameRoom->SendTableMsg(i,out2);
						}

						//排列扑克
						for ( WORD i = 0; i < GAME_PLAYER; i++ )
						{
							m_GameLogic.SortCardList( m_bHandCardData[ i ], m_bCardCount[ i ], ST_ORDER );
						}

						//首叫玩家断线超时不叫
						if(m_bUserOffLine[m_wFirstUser])
						{
							if( !m_PassTime[m_wFirstUser] )
							{
								m_g_GameRoom->StartTimer(IDI_LAND_SCORE_MISS+m_wFirstUser,30);
							}
							else
							{
								m_g_GameRoom->StartTimer(IDI_LAND_SCORE_MISS+m_wFirstUser,4);
							}
						}

						return true;
					}
				}
				else
				{
					//设置玩家
					m_wCurrentUser = m_wBankerUser;

					//切换状态
					m_bCallScorePhase=CSD_GAMESTART;

					//设置变量
					bContinue = false;
				}
			}
		}

		//m_bScoreInfo[wChairID]=bLandScore;

		//状态切换
		if (m_bLandScore==1)
		{
			bContinue=true;
			WORD wNextUser=(wChairID+1)%GAME_PLAYER;
			if (wNextUser==m_wFirstUser)
			{
				m_bCallScorePhase=CSD_GAMESTART;
			}
			else
			{
				m_bCallScorePhase=CSD_SNATCHLAND;
			}

			m_wFirstUser=wChairID;
			//切换玩家
			m_wCurrentUser = INVALID_CHAIR;
			WORD wUser = (wChairID+1)%GAME_PLAYER;
			while (  wUser != wChairID )
			{
				if ( m_bGiveUpBanker[wUser] == false )
				{
					m_wCurrentUser = wUser;
					break;
				}
				wUser = (wUser+1)%GAME_PLAYER ;
			}

			if ( m_wCurrentUser == INVALID_CHAIR )
			{
				m_wCurrentUser = wChairID;
				bContinue = false;
			}
		}
	}
	//抢地主阶段
	else if (m_bCallScorePhase==CSD_SNATCHLAND)
	{
		//效验参数
		ASSERT((bLandScore==2)||(bLandScore==4));
		if ((bLandScore!=2)&&(bLandScore!=4)) return false;

		//设置变量
		if (bLandScore==2)
		{
			m_lSnatchLand*=2;
			m_wShowTimes*=2;
			m_wBankerUser=wChairID;

			/*CString test;
			test.Format(TEXT("m_wBankerUser==%d  "),m_wBankerUser);
			OutputDebugString(test);*/
		}

		//状态切换
		if (m_wFirstUser==wChairID)
		{
			bContinue=true;
			m_wFirstUser=m_wBankerUser;
			m_wCurrentUser = m_wBankerUser;
			m_bCallScorePhase=CSD_GAMESTART;
		}
		else
		{
			bContinue=true;
		}

		//切换玩家
		if ( m_bCallScorePhase == CSD_SNATCHLAND )
		{
			m_wCurrentUser = INVALID_CHAIR;
			WORD wUser = (wChairID+1)%GAME_PLAYER;
			while (  wUser != wChairID && wUser != (m_wFirstUser+1)%GAME_PLAYER )
			{
				if ( m_bGiveUpBanker[wUser] == false )
				{
					m_wCurrentUser = wUser;
					break;
				}
				wUser = (wUser+1)%GAME_PLAYER ;
			}

			if ( m_wCurrentUser == INVALID_CHAIR || ( m_wFirstUser == m_wBankerUser && m_wFirstUser == m_wCurrentUser ))
			{
				m_wCurrentUser = m_wBankerUser;
				bContinue=true;
				m_wFirstUser=m_wBankerUser;
				m_bCallScorePhase=CSD_GAMESTART;
			}
		}
	}


	if (m_bCallScorePhase==CSD_GAMESTART)
	{
		m_delayshowtimer=true;
	}
	m_bScoreInfo[wChairID]=bLandScore;

	//发送消息
	CMolMessageOut out(IDD_MESSAGE_ROOM);
	out.write16(SUB_S_LAND_SCORE);
	out.write16(wChairID);
	out.write16(bLandScore);
	out.write16(m_wCurrentUser);
	out.write16(m_wShowTimes);
	out.write16(m_bCallScorePhase);
	out.write16(cbPreCallScorePhase);
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	//叫分结束游戏延时开始
	if ( m_bCallScorePhase==CSD_GAMESTART )
	{
		m_cbCount++;
		if (m_cbCount>=2)
		{
			::OutputDebugString(TEXT("测试，多次发牌！！！"));
			return false;
		}
		//设置状态
		m_curGameState=GS_WK_PLAYING;
		m_bEndFlag=false;
		//发送底牌
		m_bCardCount[m_wBankerUser]=20;
		CopyMemory(&m_bHandCardData[m_wBankerUser][17],m_bBackCard,sizeof(m_bBackCard));
		m_GameLogic.SortCardList(m_bHandCardData[m_wBankerUser],m_bCardCount[m_wBankerUser],ST_ORDER);

		//发送消息
		CMolMessageOut out(IDD_MESSAGE_ROOM);
		out.write16(SUB_S_GAME_START);
		out.write16(m_wCurrentUser);
		out.write16(m_wBankerUser);
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		//出牌信息
		m_bTurnCardCount=0;
		m_wTurnWiner=m_wBankerUser;
		m_wCurrentUser=m_wBankerUser;

		//玩家叫分后断线且为地主首出牌
		if(m_bUserOffLine[m_wCurrentUser] && m_bTurnCardCount==0)
		{
			m_g_GameRoom->StartTimer(IDI_FIRST_USER_OUT_CARD_MISS,30);
		}
	}

	//玩家断线超时不叫或不抢
	if(m_bUserOffLine[m_wCurrentUser] && m_bCallScorePhase!=CSD_GAMESTART && m_wCurrentUser!=INVALID_CHAIR)
	{
		WORD wNextChairID=(wChairID+1)%GAME_PLAYER;
		if( !m_PassTime[wNextChairID] )
		{
			m_g_GameRoom->StartTimer(IDI_LAND_SCORE_MISS+wNextChairID,30);
		}
		else
		{
			m_g_GameRoom->StartTimer(IDI_LAND_SCORE_MISS+wNextChairID,1);
		}
	}
	return true;
}
//玩家明牌
bool CServerLogicFrame::OnUserBrightCard(WORD wChairID)
{
	m_bBrightCard[wChairID]= true;
	m_lBrightCard*=2;
	m_wShowTimes*=2;

	////变量定义
	//CMD_S_Bright BrightCard;
	//ZeroMemory( &BrightCard, sizeof( BrightCard ) );
	//BrightCard.wShowTimes = m_wShowTimes;
	//BrightCard.wBrightUser = wChairID;
	//BrightCard.bBrightCard = bBright;

	//发送消息
	CMolMessageOut out(IDD_MESSAGE_ROOM);
	out.write16(SUB_S_BRIGHT_CARD);
	out.write16(wChairID);
	out.write16(m_wShowTimes);
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	return true;
}


//玩家出牌
bool CServerLogicFrame::OnUserOutCard(WORD wChairID, BYTE bCardData[], int bCardCount)
{
	//效验状态

	if (m_curGameState!=GS_WK_PLAYING) return true;

	/*CString test;
	test.Format(TEXT("OnUserOutCard:%d,%d    "),wChairID,m_wCurrentUser);
	OutputDebugString(test);*/

	if (wChairID!=m_wCurrentUser)
	{
		return false;
	}

	m_bUserPass[wChairID]=false;

	//排序扑克
	m_GameLogic.SortCardList(bCardData,bCardCount,ST_ORDER);

	//类型判断
	int bCardType=m_GameLogic.GetCardType(bCardData,bCardCount);
	if (bCardType==CT_ERROR)
		return false;

	////出炸弹统计
	if (bCardType == CT_BOMB_CARD || bCardType == CT_MISSILE_CARD)
	{
		m_BoomCount++;
	}

	//更随出牌
	m_bYaPaiFlag=false;
	if (m_bTurnCardCount==0)
	{
		m_bTurnCardCount=bCardCount;
		m_bYaPaiFlag=true;
	}
	else if (m_GameLogic.CompareCard(m_bTurnCardData,bCardData,m_bTurnCardCount,bCardCount)==false)
		return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(bCardData,bCardCount,m_bHandCardData[wChairID],m_bCardCount[wChairID])==false)
		return false;
	m_bCardCount[wChairID]-=bCardCount;

	//最后出牌记录
	m_bTurnCardCount=bCardCount;
	m_bOutCardCount[wChairID]++;
	CopyMemory(m_bTurnCardData,bCardData,sizeof(BYTE)*bCardCount);


	//本轮出牌记录
	CopyMemory(m_cbNowTurnOutCard[wChairID],bCardData,sizeof(BYTE)*bCardCount);
	m_nNowTurnOutCount[wChairID] = bCardCount;


	//炸弹判断
	if ((bCardType==CT_BOMB_CARD)||(bCardType==CT_MISSILE_CARD))
	{
		m_wBombTime*=2;
		m_wShowTimes*=2;
	}
	//切换用户
	m_wTurnWiner=wChairID;
	if (m_bCardCount[wChairID]!=0)
	{
		//if (bCardType!=CT_MISSILE_CARD)
			m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	}
	else
	{
		m_wCurrentUser=INVALID_CHAIR;
		m_wCloseWarmingFlash=true;	// 出最后一首牌同时关闭警灯FLASH
	}

	if (m_wCurrentUser != INVALID_CHAIR)
	{
		CopyMemory(m_cbLastTurnOutCard[m_wCurrentUser],m_cbNowTurnOutCard[m_wCurrentUser],sizeof(BYTE)*m_nNowTurnOutCount[m_wCurrentUser]);
		m_nLastTurnOutCount[m_wCurrentUser] = m_nNowTurnOutCount[m_wCurrentUser];
		m_nNowTurnOutCount[m_wCurrentUser] = 0;
	}

	//构造数据
	int bYaPaiFlag=((m_bYaPaiFlag==true)?1:0);
	CMolMessageOut out(IDD_MESSAGE_ROOM);
	out.write16(SUB_S_OUT_CARD);
	out.write16(wChairID);
	out.write16(m_wShowTimes);
	out.write16(m_wCurrentUser);
	out.write16(bYaPaiFlag);
	out.write16(m_bTurnCardCount);
	for (int i=0;i<m_bTurnCardCount;i++)
	{
		out.write16(m_GameLogic.ChangeCardDate(m_bTurnCardData[i]));
	}

	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);




	//玩家断线超时不出
	if( m_wCurrentUser!=INVALID_CHAIR)
	{
		//当前玩家非首出牌断线超时不出
		if(m_bUserOffLine[m_wCurrentUser] && m_bTurnCardCount!=0)
		{
			WORD wNextChairID=(wChairID+1)%GAME_PLAYER;
			if( !m_PassTime[wNextChairID] )
			{
				m_g_GameRoom->StartTimer(IDI_OUT_CARD_MISS+wNextChairID,30);
			}
			else
			{
				m_g_GameRoom->StartTimer(IDI_OUT_CARD_MISS+wNextChairID,1);
			}

		}
		//当前玩家首出牌断线超时出最小牌
		if (m_bUserOffLine[m_wCurrentUser] && m_bTurnCardCount==0 )
		{
			if( !m_PassTime[m_wCurrentUser] )
			{
				m_g_GameRoom->StartTimer(IDI_FIRST_USER_OUT_CARD_MISS,30);
			}
			else
			{
				m_g_GameRoom->StartTimer(IDI_FIRST_USER_OUT_CARD_MISS,1);
			}
		}
	}

	//出牌最大
	//if (bCardType==CT_MISSILE_CARD) m_bTurnCardCount=0;

	//结束判断
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		m_wChairID=wChairID;
	/*	m_g_GameRoom->StartTimer(IDI_DELAY_END,1);*/
		OverGame(wChairID,m_g_GameRoom,false);
	}

	return true;

}

//用户不出
bool CServerLogicFrame::OnUserPassCard(WORD wChairID)
{
	//效验状态
	if (m_curGameState!=GS_WK_PLAYING) return true;
	if ((wChairID!=m_wCurrentUser)||(m_bTurnCardCount==0))
	{
		return false;
	}
	m_bUserPass[wChairID]=true;


	//设置变量
	m_wCurrentUser=(m_wCurrentUser+1)%GAME_PLAYER;
	if (m_wCurrentUser==m_wTurnWiner)
	{
		m_bTurnCardCount=0;

		CopyMemory(m_cbLastTurnOutCard,m_cbNowTurnOutCard,sizeof(m_cbLastTurnOutCard));
		CopyMemory(m_nLastTurnOutCount,m_nNowTurnOutCount,sizeof(m_nLastTurnOutCount));
	}

	//出牌记录
	m_nNowTurnOutCount[wChairID] = 0;
	CopyMemory(m_cbLastTurnOutCard[m_wCurrentUser],m_cbNowTurnOutCard[m_wCurrentUser],sizeof(BYTE)*m_nNowTurnOutCount[m_wCurrentUser]);
	m_nLastTurnOutCount[m_wCurrentUser] = m_nNowTurnOutCount[m_wCurrentUser];
	m_nNowTurnOutCount[m_wCurrentUser] = 0;


	//发送数据
	int bNewTurn=(m_bTurnCardCount==0)?1:0;		//1为true,0为false
	CMolMessageOut out(IDD_MESSAGE_ROOM);
	out.write16(SUB_S_PASS_CARD);
	out.write16(wChairID);
	out.write16(m_wCurrentUser);
	out.write16(bNewTurn);
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	//玩家断线超时不出
	if(m_wCurrentUser!=INVALID_CHAIR)
	{
		//当前玩家非首出牌断线超时不出牌
		if(m_bUserOffLine[m_wCurrentUser]&& m_bTurnCardCount!=0 )
		{
			WORD wNextChairID=(wChairID+1)%GAME_PLAYER;
			if( !m_PassTime[wNextChairID] )
			{
				m_g_GameRoom->StartTimer(IDI_OUT_CARD_MISS+wNextChairID,30);
			}
			else
			{
				m_g_GameRoom->StartTimer(IDI_OUT_CARD_MISS+wNextChairID,1);
			}

		}
		//当前玩家首出牌断线超时出最小牌
		if (m_bUserOffLine[m_wCurrentUser] && m_bTurnCardCount==0 )
		{
			if( !m_PassTime[m_wCurrentUser] )
			{
				m_g_GameRoom->StartTimer(IDI_FIRST_USER_OUT_CARD_MISS,30);
			}
			else
			{
				m_g_GameRoom->StartTimer(IDI_FIRST_USER_OUT_CARD_MISS,1);
			}
		}
	}

	return true;
}
//玩家托管
bool CServerLogicFrame::OnUserTrustee(WORD wChairID,int isAutoPlay)
{
	bool bTrustee=(isAutoPlay==1)?true:false;

	m_bUserTrustee[ wChairID ] = bTrustee;

	//变量定义
	CMolMessageOut out(IDD_MESSAGE_ROOM);
	out.write16(SUB_S_TRUSTEE);
	out.write16(wChairID);
	out.write16(isAutoPlay);
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	return true;
}

/// 游戏结束
void CServerLogicFrame::OverGame(WORD wChairID,Room *pRoom,bool isrunarray)
{
	//删除所有定时器
	pRoom->StopAllTimer();

	if(pRoom->GetRoomType() == ROOMTYPE_BISAI)
	{
		m_MatchOver=true;
		bool isok=false;
		LONG MatchScore[GAME_PLAYER];//比赛玩家积分
		memset(MatchScore,0,sizeof(MatchScore));
		for(int i=0;i<GAME_PLAYER;i++)
		{
			Player *pPlayer = pRoom->GetPlayer(i);
			if(pPlayer == NULL) continue;

			//每玩一局，局数减一
			//if(pPlayer->GetType() != PLAYERTYPE_ROBOT)
			//{
				pPlayer->SetMatchCount(pPlayer->GetMatchCount()-1);

				if(pPlayer->GetMatchCount() > 0)
				{
					isok=true;
				}
			//}

			//每局结束计算玩家积分
			LONG lCellScore=1/*m_g_GameRoom->GetGamePielement()*/;	//单元积分
			if (m_wBankerUser!=INVALID_CHAIR)//排除超三次重发牌结束的情况
			{
				bool bLandWin=(m_bCardCount[m_wBankerUser]==0)?true:false;

				//计算积分
				if (i==m_wBankerUser)
				{
					MatchScore[i] =lCellScore* m_lSnatchLand*m_lBrightCard*m_wBombTime*m_lThuntian*((bLandWin==true)?2:-2);
					pPlayer->SetMatchResult(pPlayer->GetMatchResult()+MatchScore[i]);
				}
				else
				{
					MatchScore[i] = lCellScore* m_lSnatchLand*m_lBrightCard*m_wBombTime*m_lThuntian*((bLandWin==true)?-1:1);
					pPlayer->SetMatchResult(pPlayer->GetMatchResult()+MatchScore[i]);
				}
			}
		}

		for(int i=0;i<GAME_PLAYER;i++)
		{
			Player *pPlayer = pRoom->GetPlayer(i);
			if(pPlayer == NULL) continue;

			// 得到比赛排名
			pPlayer->GetPlayerRanking();
		}

		//发送信息
		CMolMessageOut out(IDD_MESSAGE_ROOM);
		out.write16(SUB_S_MATCH_END);
		for (int i=0;i<GAME_PLAYER;i++)
		{
			out.write32(MatchScore[i]);
		}
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);


		//随机选择下一局首叫牌玩家
		//m_wFirstUser=rand()%GAME_PLAYER;
		////替断线玩家发送准备消息
		//if(m_bUserOffLine[i]==true && pPlayer->GetMatchCount() > 0)
		//{
		//	pPlayer->SendReadyMsg();
		//}

		if(isok)
		{
			m_g_GameRoom->StartTimer(IDI_DELAY_READY,3);	//延时发准备消息
		}
	}
	else
	{
		if (!isrunarray)	//常规结束
			{
				if (wChairID!=INVALID_CHAIR)
				{
					//设置状态
					m_curGameState=GS_WK_FREE;

					//春天判断
					if ( wChairID == m_wBankerUser )
					{
						WORD wUser1 = ( m_wBankerUser + 1 ) % GAME_PLAYER;
						WORD wUser2 = ( m_wBankerUser + 2 ) % GAME_PLAYER;
						if ( ( m_bOutCardCount[ wUser1 ] == 0 ) && ( m_bOutCardCount[ wUser2 ] == 0 ) ) m_lThuntian *= 2;
					}
					else
					{
						if ( m_bOutCardCount[ m_wBankerUser ] == 1 ) m_lThuntian *= 2;
					}

					LONG lCellScore=m_g_GameRoom->GetGamePielement();	//单元积分
					int64 lGameScore[GAME_PLAYER];						//玩家金币

					bool bLandWin=(m_bCardCount[m_wBankerUser]==0)?true:false;

					//计算积分
					for (WORD i=0;i<GAME_PLAYER;i++)
					{
						if (i==m_wBankerUser)
						{
							lGameScore[i] =lCellScore* m_lSnatchLand*m_lBrightCard*m_wBombTime*m_lThuntian*((bLandWin==true)?2:-2);
						}
						else
						{
							lGameScore[i] = lCellScore* m_lSnatchLand*m_lBrightCard*m_wBombTime*m_lThuntian*((bLandWin==true)?-1:1);
						}
					}

					//int wShowTimes=m_lSnatchLand*m_lBrightCard*m_wBombTime * m_lThuntian;//倍数显示

					//积分平衡
					for (WORD i=0;i<GAME_PLAYER;i++)
					{
						if (lGameScore[i]<0)
						{
							Player *pPlayer = pRoom->GetPlayer(i);
							int64 pUserMoney = pPlayer->GetMoney();
							if (pRoom->GetRoomType()==ROOMTYPE_JINBIN && (pUserMoney+lGameScore[i])<0)
							{
								//金币调整
								if (i==m_wBankerUser)
								{
									for (WORD j=0;j<GAME_PLAYER;j++)
									{
										if (j!=m_wBankerUser)
										{
											lGameScore[j]=pUserMoney/2;
										}
									}
								}
								else
								{
									lGameScore[m_wBankerUser]-=(-lGameScore[i]-pUserMoney);
								}

								//结果调整
								lGameScore[i]=-pUserMoney;
							}
						}
					/*	else
						{
							CString tmpStr;
							tmpStr.Format(TEXT("%s刚赢走了%ld金币，朋友您得加油了！"),pRoom->Utf8ConverToWideChar( pRoom->GetPlayer(i)->GetName()).GetBuffer(),lGameScore[i]);
							pRoom->SendTrumpetMes(IDD_MESSAGE_TYPE_SUPER_SMAILL_MSG,tmpStr.GetBuffer());
						}*/
					}

					//统计积分
					int64 lScore=0;
					int64 lRevenue=0;
					enScoreKind ScoreKind;
					for (WORD i=0;i<GAME_PLAYER;i++)
					{
						//计算税收
						lRevenue=0;
						if (pRoom->GetRoomType() == ROOMTYPE_JINBIN && lGameScore[i]>0)
						{
							lRevenue= (int64)(lGameScore[i]*pRoom->GetChouShui());
							/*char str[128];
							sprintf(str,"%ld %ld %f %f",lRevenue,lGameScore[i],pRoom->GetChouShui(),change);
							mole2d::network::System_Log(str);				*/
							lGameScore[i]=lGameScore[i]-lRevenue;

							CString tmpStr;
							tmpStr.Format(TEXT("%s刚赢走了%ld金币，朋友您得加油了！"),pRoom->Utf8ConverToWideChar( pRoom->GetPlayer(i)->GetName()).GetBuffer(),lGameScore[i]);
							pRoom->SendTrumpetMes(IDD_MESSAGE_TYPE_SUPER_SMAILL_MSG,tmpStr.GetBuffer());
						}
						lScore = lGameScore[i];
						ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Lost;
						//pRoom->WriteUserScore(i, lScore, lRevenue, ScoreKind );
					}

					//发送信息
					CMolMessageOut out(IDD_MESSAGE_ROOM);
					out.write16(SUB_S_GAME_END);
					out.write16(m_lSnatchLand);
					out.write16(m_lBrightCard);
					out.write16(m_wBombTime);
					out.write16(m_lThuntian);
					//out.write16(wShowTimes);
					for (int i=0;i<GAME_PLAYER;i++)
					{
						out.write64(lGameScore[i]);
					}
					for (int i=0;i<GAME_PLAYER;i++)
					{
						out.write16(m_bCardCount[i]);
						for (int j=0;j<m_bCardCount[i];j++)
						{
							out.write16(m_GameLogic.ChangeCardDate(m_bHandCardData[i][j]));
						}
					}
					m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

					//随机选择下一局首叫牌玩家
					m_wFirstUser=rand()%GAME_PLAYER;

					m_GameOver=true;

					//结束游戏
					pRoom->GameEnd();
				}
				else	//超三次重发牌解散游戏
				{
					//发送信息
					CMolMessageOut out(IDD_MESSAGE_ROOM);
					out.write16(SUB_S_GAME_END);
					out.write16(1);
					out.write16(1);
					out.write16(1);
					out.write16(1);
					for (int i=0;i<GAME_PLAYER;i++)
					{
						out.write64(0);
					}
					for (int i=0;i<GAME_PLAYER;i++)
					{
						out.write16(m_bCardCount[i]);
						for (int j=0;j<m_bCardCount[i];j++)
						{
							out.write16(m_GameLogic.ChangeCardDate(m_bHandCardData[i][j]));
						}
					}
					m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

					m_GameOver=true;

					//结束游戏
					pRoom->GameEnd();
				}
			}
		else	//玩家逃跑
		{
			if (m_curGameState==GS_WK_SCORE)
			{
				m_lSnatchLand=8;
				m_lBrightCard=1;
				m_wBombTime=1;
				m_lThuntian=1;
			}
			else
			{
				//手上未出炸弹计算
				for ( WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID )
				{
					//分析扑克
					tagAnalyseResult AnalyseResult ;
					ZeroMemory(&AnalyseResult, sizeof( AnalyseResult ) );
					m_GameLogic.AnalysebCardData(m_bHandCardData[wUserID], m_bCardCount[wUserID], AnalyseResult) ;

					//炸弹
					for (WORD i=1;i <= AnalyseResult.cbFourCount;i++)
					{
						m_wBombTime *= 2;
					}
					//王炸
					if ( m_bHandCardData[wUserID][0] == 0x4F && m_bHandCardData[wUserID][1] == 0x4E )
					{
						m_wBombTime *= 2;
					}
				}
			}

			m_bEndFlag=true;

				//设置状态
			m_curGameState=GS_WK_FREE;

			LONG lCellScore=m_g_GameRoom->GetGamePielement();	//单元积分
			int64 lGameScore[GAME_PLAYER];
			lGameScore[wChairID] = -(lCellScore*m_lSnatchLand*m_lBrightCard*m_wBombTime*m_lThuntian*2);

			int wShowTimes=m_lSnatchLand*m_lBrightCard*m_wBombTime*m_lThuntian;//显示倍数

			//金币房间
			if (pRoom->GetRoomType()==ROOMTYPE_JINBIN )
			{
				//金币平衡
				Player *pPlayer = pRoom->GetPlayer(wChairID);
				int64 pUserMoney = pPlayer->GetMoney();
				if ( pUserMoney + lGameScore[wChairID] < 0 )
				{
					lGameScore[wChairID] = - pUserMoney;
				}
				//计算积分
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					if ( i != wChairID ) lGameScore[i] = (-lGameScore[wChairID])/2;
				}

				//统计积分
				int64 lScore=0;
				int64 lRevenue=0;
				enScoreKind ScoreKind;
				for (WORD i=0;i<GAME_PLAYER;i++)
				{
					//计算税收
					lRevenue=0;
					if ( lGameScore[i]>0 )
					{
						lRevenue = (int64)(lGameScore[i]*pRoom->GetChouShui());
						lGameScore[i]=lGameScore[i]-lRevenue;
					}
					lScore = lGameScore[i];
					ScoreKind = lScore > 0 ? enScoreKind_Win : enScoreKind_Lost;
					//pRoom->WriteUserScore(i, lScore, lRevenue, ScoreKind );
				}
			}

			//发送信息
			CMolMessageOut out(IDD_MESSAGE_ROOM);
			out.write16(SUB_S_GAME_END);
			out.write16(m_lSnatchLand);
			out.write16(m_lBrightCard);
			out.write16(m_wBombTime);
			out.write16(m_lThuntian);
			//out.write16(wShowTimes);
			for (int i=0;i<GAME_PLAYER;i++)
			{
				out.write64(lGameScore[i]);
			}
			for (int i=0;i<GAME_PLAYER;i++)
			{
				out.write16(m_bCardCount[i]);
				for (int j=0;j<m_bCardCount[i];j++)
				{
					out.write16(m_GameLogic.ChangeCardDate(m_bHandCardData[i][j]));
				}
			}
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

			//随机选择下一局首叫牌玩家
			//m_wFirstUser=rand()%GAME_PLAYER;

			//结束游戏
			pRoom->GameEnd();
		}
	}

	//结束录像
	//pRoom->EndVideoTape();
}
/// 清空游戏资源
void CServerLogicFrame::Clear(void)
{
	//游戏变量
	m_BoomCount=0;
	m_cbCount=0;
	m_lSnatchLand=1;
	m_lBrightCard=1;
	m_wBombTime=1;
	m_lThuntian=1;
	m_wShowTimes=1;
	m_bLandScore=0;
	m_breSendCardTimes=0;
	m_bEndFlag=false;
	m_MatchOver=false;
	//m_FirstUserLife=false;
	m_bEndOrReadyFlag=false;
	m_wCloseWarmingFlash=false;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wChairID=INVALID_CHAIR;
	m_UserLeaveCount=0;
	memset(m_bScoreInfo,0,sizeof(m_bScoreInfo));

	//变量
	m_bCallScorePhase=CSD_NORMAL;
	memset( m_bGiveUpBanker, 0,sizeof( m_bGiveUpBanker ) );
	m_cbLandScoreCount=0;
	memset( m_bUserOffLine,0, sizeof( m_bUserOffLine ) );
	memset( m_PassTime, 0,sizeof( m_PassTime ) );

	//运行信息
	m_bTurnCardCount=0;
	m_wTurnWiner=INVALID_CHAIR;
	memset(m_bTurnCardData,0,sizeof(m_bTurnCardData));
	memset(m_bUserTrustee, 0, sizeof( m_bUserTrustee ) );
	memset(m_bBrightCard,0,sizeof(m_bBrightCard));
	memset(m_bUserPass,0,sizeof(m_bUserPass));
	memset(m_UserStandUp,0,sizeof(m_UserStandUp));

	//扑克信息
	memset(m_bBackCard,0,sizeof(m_bBackCard));
	memset(m_bCardCount,0,sizeof(m_bCardCount));
	memset(m_bOutCardCount,0,sizeof(m_bOutCardCount));
	memset(m_bHandCardData,0,sizeof(m_bHandCardData));

	memset(m_cbNowTurnOutCard,	0,	sizeof(m_cbNowTurnOutCard));
	memset(m_nNowTurnOutCount,	0,	sizeof(m_nNowTurnOutCount));
	memset(m_cbLastTurnOutCard,	0,	sizeof(m_cbLastTurnOutCard));
	memset(m_nLastTurnOutCount,	0,	sizeof(m_nLastTurnOutCount));

}
//测试用
bool CServerLogicFrame::Test( BYTE bAllCard[])
{
	/*//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	TCHAR szProgPath[MAX_PATH * 2];
	::GetModuleFileName(NULL, szProgPath, sizeof(szProgPath)/sizeof(TCHAR));
	std::wstring m_curWorkingDir = szProgPath;
	m_curWorkingDir = m_curWorkingDir.substr(0,m_curWorkingDir.find_last_of(TEXT("\\")));
	_tcscpy(szPath,m_curWorkingDir.c_str());

	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");

	//读取配置
	_sntprintf(szFileName,sizeof(szFileName),TEXT("%s\\HLDDZTest.ini"),szPath);

	TCHAR cIsDebug[5]=TEXT("");
	GetPrivateProfileString(TEXT("DebugTest"), TEXT("IsDebug"), TEXT(""), cIsDebug, 5*sizeof(TCHAR), szFileName);
	int iIsDebug = _ttoi(cIsDebug);
	if (iIsDebug)
	{
		TCHAR cData[1024] =TEXT("");
		GetPrivateProfileString(TEXT("DebugTest"), TEXT("CARD"), TEXT(""), cData, 1024,szFileName);

		std::wstring tempStr = cData;
		std::vector<std::wstring> tempStr2;

		int pos = tempStr.find_first_of(TEXT(","));
		while(pos > 0)
		{
			std::wstring tempStr3 = tempStr.substr(0,1);
			tempStr2.push_back(tempStr3);
			std::wstring tempStr4 = tempStr.substr(1,pos-1);
			tempStr2.push_back(tempStr4);

			tempStr = tempStr.substr(pos+1,tempStr.length());
			pos = tempStr.find_first_of(TEXT(","));
		}

		if(!tempStr2.empty())
		{
			for(int i=0,n=0;i<(int)tempStr2.size()-2,n<54;i+=2,n++)
			{
				uint16 Dex;
				int temNum = _ttoi(tempStr2[i].c_str());
				Dex = temNum<<4;
				int temNum2 = _ttoi(tempStr2[i+1].c_str());
				Dex += temNum2;
				bAllCard[n] = Dex;
			}
		}
	}
	bool flag=((iIsDebug==1)?true:false);*/
	return true;
}

//jackPot
bool CServerLogicFrame::OnJackPot(int nBoomCount)
{


	return true;
}
