#include "crobotlogicframe.h"

//游戏时间
#ifdef _DEBUG
#define TIME_OUT_CARD				3									//出牌时间
#else
#define TIME_OUT_CARD				3									//出牌时间
#endif
#define TIME_START_GAME				5									//开始时间
#define TIME_CALL_SCORE				3									//叫分时间

//辅助时间
#define TIME_LESS					3									//最少时间
#define TIME_DISPATCH				5									//发牌时间

//游戏时间
#define IDI_OUT_CARD				(IDD_TIMER_GAME_START+30)			//出牌时间
#define IDI_START_GAME				(IDD_TIMER_GAME_START+31)			//开始时间
#define IDI_CALL_SCORE				(IDD_TIMER_GAME_START+32)			//叫分时间

/// 构造函数
CRobotLogicFrame::CRobotLogicFrame() :m_g_myself(NULL), m_g_myselfRoom(NULL)
{
	//扑克变量
	m_cbTurnCardCount=0;
	memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
	m_wOutCardUser = INVALID_CHAIR ;
	m_wBankerUser = INVALID_CHAIR ;

	//手上扑克
	m_cbHandCardCount=0;
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
	m_cbCurrentLandScore = 4 ;

	//叫分变量
	m_cbCallScorePhase=CSD_NORMAL;

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
	//扑克变量
	m_cbTurnCardCount=0;
	memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));

	//手上扑克
	m_cbHandCardCount=0;
	memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));
}

/// 用于处理用户结束游戏消息
void CRobotLogicFrame::OnProcessPlayerGameOverMes(void)
{
	//开始设置
	uint32 nElapse=rand()%TIME_START_GAME+TIME_LESS;
	m_g_myself->StartTimer(IDI_START_GAME,nElapse);
}

/// 用于处理用户进入游戏房间后的消息
void CRobotLogicFrame::OnProcessPlayerRoomMes(Json::Value &mes)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	int subMsgID = mes["MsgSubId"].asInt();

	switch(subMsgID)
	{
	case SUB_S_SEND_CARD://发牌
		{
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			//底牌数据
			uint16 bBackCard[3];
			for (int i=0;i<3;i++)
			{
				bBackCard[i]=mes["bBackCard"][i].asInt();
				m_cbBackCard[i]=m_GameLogic.OnChangeCardDate(bBackCard[i]);
			}
			//所有扑克
			uint16 cbHandCardData[3][20];
			uint8 ChangeHandCardData[3][20];
			uint8 uindex = 0;
			for ( uint16 i = 0; i < GAME_PLAYER; i++ )
			{
				for ( uint16 j = 0; j < 17; j++ )
				{
					cbHandCardData[i][j]=mes["cbHandCardData"][uindex++].asInt();
					ChangeHandCardData[i][j]=m_GameLogic.OnChangeCardDate(cbHandCardData[i][j]);
				}
				m_GameLogic.SetUserCard(i, ChangeHandCardData[i], 17) ;
			}

			m_cbCallScorePhase=CSD_NORMAL;

			//扑克变量
			m_cbTurnCardCount=0;
			memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			m_cbCurrentLandScore = 4 ;

			//叫牌扑克
			uint8 cbLandScoreCardData[20] ;
			memcpy(cbLandScoreCardData, ChangeHandCardData[m_g_myself->GetChairIndex()], sizeof( uint8 )*17) ;
			memcpy(cbLandScoreCardData+17, m_cbBackCard, sizeof( uint8 )*BACK_COUNT) ;
			m_GameLogic.SetLandScoreCardData(cbLandScoreCardData, 20) ;

			//手上扑克
			m_cbHandCardCount=17;
			memcpy(m_cbHandCardData, ChangeHandCardData[m_g_myself->GetChairIndex()], sizeof( uint8 )*17) ;

			//排列扑克
			m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount,ST_ORDER);

			//玩家处理
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				uint32 nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_g_myself->StartTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
			}
		}
		return;
	case SUB_S_LAND_SCORE://叫分
		{
			uint16 pChairId = mes["ChairId"].asInt();
			m_cbCurrentLandScore = mes["CurrentLandScore"].asInt();
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			int wShowTimes = mes["ShowTimes"].asInt();
			m_cbCallScorePhase = mes["cbCallScorePhase"].asInt();
			int cbPreCallScorePhase = mes["PreCallScorePhase"].asInt();

			//设置庄家
		/*	m_wBankerUser = pCallScore->bLandUser ;
			m_GameLogic.SetBanker(pCallScore->bLandUser) ;*/

			//用户处理
			if (m_g_myself->GetChairIndex()==wCurrentUser && m_cbCallScorePhase!=CSD_GAMESTART)
			{
				uint32 nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_g_myself->StartTimer(IDI_CALL_SCORE,nElapse);
			}
		}
		return;
	case SUB_S_GAME_START://开始
		{
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			m_wBankerUser =  mes["wBankerUser"].asInt();

			if (m_cbHandCardCount>17)
			{
				return ;
			}

			//设置庄家
			m_GameLogic.SetBanker(m_wBankerUser) ;

			////设置底牌
			m_GameLogic.SetBackCard(m_wBankerUser, m_cbBackCard, BACK_COUNT) ;

			//设置扑克
			if (m_wBankerUser==m_g_myself->GetChairIndex())
			{
				//设置扑克
				m_cbHandCardCount+=BACK_COUNT;
				memcpy(&m_cbHandCardData[NORMAL_COUNT],m_cbBackCard,sizeof( uint8 )*BACK_COUNT);

				//排列扑克
				m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount,ST_ORDER);

			}

			//玩家设置
			if (wCurrentUser==m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
			}
		}
		return;
	case SUB_S_BRIGHT_CARD://明牌
		{
			uint16 pChairId = mes["ChairId"].asInt();
			int wShowTimes = mes["ShowTimes"].asInt();
		}
		return;
	case SUB_S_OUT_CARD://出牌
		{
			uint16 pChairId = mes["ChairId"].asInt();
			int wShowTimes=mes["ShowTimes"].asInt();
			uint16 wCurrentUser=mes["CurrentUser"].asInt();
			bool bYaPaiFlag;
			bYaPaiFlag=((mes["YaPaiFlag"].asInt()==1)?true:false);
			uint16 bOutCardCount=mes["OutCardCount"].asInt();
			uint16 bOutCardData[20];
			uint8 ChangeHandCardData[20];
			for (int i=0;i<bOutCardCount;i++)
			{
				bOutCardData[i]=mes["OutCardData"][i].asInt();
				ChangeHandCardData[i]=m_GameLogic.OnChangeCardDate(bOutCardData[i]);
			}

			//出牌变量
			if (wCurrentUser==pChairId)
			{
				m_cbTurnCardCount=0;
				memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			}
			else
			{
				m_cbTurnCardCount=bOutCardCount;
				m_wOutCardUser = pChairId;
				memcpy(m_cbTurnCardData,ChangeHandCardData,bOutCardCount*sizeof(uint8));
			}

			//类型判断
			//int bCardType=m_GameLogic.GetCardType(m_cbTurnCardData,bOutCardCount);

			//玩家设置
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				/*if (bCardType==CT_MISSILE_CARD)
				{
					m_g_myself->StartTimer(IDI_OUT_CARD,3);
				}
				else
				{*/
					uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
					m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
				//}
			}

			//设置变量
			m_GameLogic.RemoveUserCardData(pChairId, m_cbTurnCardData, m_cbTurnCardCount) ;
		}
		return;
	case SUB_S_PASS_CARD://不出
		{
			uint16 pChairId = mes["ChairId"].asInt();
			uint16 wCurrentUser=mes["CurrentUser"].asInt();
			int bNewTurn=mes["bNewTurn"].asInt();

			//一轮判断
			if (bNewTurn==1)
			{
				m_cbTurnCardCount=0;
				memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			}
			//玩家设置
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
			}
		}
		return;
	case SUB_S_TRUSTEE://托管
		{
			uint16 pChairId = mes["ChairId"].asInt();
			int isAutoPlay=mes["isAutoPlay"].asInt();
		}
		return;
	case SUB_S_GAME_END://结束
		{
			//设置变量
			m_cbTurnCardCount=0;
			m_cbHandCardCount=0;
			memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));

			//删除时间
			m_g_myself->StopAllTimer();
		}
		return;
	case SUB_S_MATCH_END:
		{
			//设置变量
			m_cbTurnCardCount=0;
			m_cbHandCardCount=0;
			memset(m_cbTurnCardData,0,sizeof(m_cbTurnCardData));
			memset(m_cbHandCardData,0,sizeof(m_cbHandCardData));

			//删除时间
			m_g_myself->StopAllTimer();

/*			m_g_myself->SetMatchCount(m_g_myself->GetMatchCount()-1);

			if(m_g_myself->GetMatchCount() > 0)
			{
				m_g_myself->SendReadyMsg();
			}		*/
		}
		return;
	case SUB_S_LAND_SCORE_STATION://叫分阶段断线重连
		{
			uint16 pChairId = mes["ChairId"].asInt();
			uint16 wCurrentUser = mes["CurrentUser"].asInt();
			int wShowTimes = mes["ShowTimes"].asInt();
			int bCallScorePhase = mes["CallScorePhase"].asInt();

			//显示各玩家分字
			int bScore[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bScore[i]=mes["bScore"][i].asInt();
			}
			//玩家托管
			bool bUserTrustee[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bUserTrustee[i]=(mes["bUserTrustee"][i].asInt()==0)?false:true;
			}
			//底牌
			uint16 bBackCard[3];
			for (int i=0;i<3;i++)
			{
				bBackCard[i]=mes["bBackCard"][i].asInt();
			}

			//各玩家牌数据
			uint16 bHandCardData[3][20];
			uint8 ChangeHandCardData[3][20];
			uint8 uindex = 0;
			for ( uint16 i = 0; i < GAME_PLAYER; i++ )
			{
				for ( uint16 j = 0; j < 17; j++ )
				{
					bHandCardData[i][j]=mes["bHandCardData"][uindex++].asInt();
					ChangeHandCardData[i][j]=m_GameLogic.OnChangeCardDate(bHandCardData[i][j]);
				}
			}

			//扑克数据
			m_cbHandCardCount=NORMAL_COUNT;
			memcpy(m_cbHandCardData,ChangeHandCardData[m_g_myself->GetChairIndex()],sizeof(uint8)*m_cbHandCardCount);

			//叫分设置
			if (m_g_myself->GetChairIndex()==wCurrentUser)
			{
				uint32 nElapse=rand()%TIME_CALL_SCORE+TIME_LESS;
				m_g_myself->StartTimer(IDI_CALL_SCORE,nElapse+TIME_DISPATCH);
			}
		}
		return;
	case SUB_S_PLAYING_STATION://出牌阶段断线重连
		{
			uint16 pChairId = mes["ChairId"].asInt();
			uint16 wCurrentUser=mes["wCurrentUser"].asInt();
			int wShowTimes=mes["wShowTimes"].asInt();
			uint16 wLandUser=mes["wLandUser"].asInt();
			uint16 wTurnWiner=mes["wTurnWiner"].asInt();
			bool wMatchOver=(mes["wMatchOver"].asInt()==1)?true:false;
			//明牌
			int bBrightCard[GAME_PLAYER];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bBrightCard[i]=mes["bBrightCard"][i].asInt();
			}
			//托管
			bool bUserTrustee[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				bUserTrustee[i]=(mes["bUserTrustee"][i].asInt()==0)?false:true;
			}
			//不出
			bool IsPass[3];
			for (int i=0;i<GAME_PLAYER;i++)
			{
				IsPass[i]=(mes["IsPass"][i].asInt()==0)?false:true;
			}

			//打出的牌数据
			m_cbTurnCardCount=mes["cbTurnCardCount"].asInt();
			for (int i=0;i<m_cbTurnCardCount;i++)
			{
				m_cbTurnCardData[i]=mes["cbTurnCardData"][i].asInt();
			}

			//底牌
			uint16 bBackCard[3];
			for (int i=0;i<3;i++)
			{
				bBackCard[i]=mes["bBackCard"][i].asInt();
			}

			//各玩家牌数据
			int bHandCardCount[3];
			uint16 bHandCardData[3][20];
			uint8 ChangeHandCardData[3][20];

			for ( uint16 i = 0; i < GAME_PLAYER; i++ )
			{
				bHandCardCount[i]=mes["bHandCardCount"][i].asInt();
				for ( uint16 j = 0; j < bHandCardCount[i]; j++ )
				{
					bHandCardData[i][j]=mes["bHandCardData"][i][j].asInt();
					ChangeHandCardData[i][j]=m_GameLogic.OnChangeCardDate(bHandCardData[i][j]);
				}
			}

			//扑克数据
			m_cbHandCardCount=bHandCardCount[m_g_myself->GetChairIndex()];
			memcpy(m_cbHandCardData,ChangeHandCardData[m_g_myself->GetChairIndex()],sizeof(uint8)*m_cbHandCardCount);

			//玩家设置
			if (wCurrentUser==m_g_myself->GetChairIndex())
			{
				uint32 nElapse=rand()%TIME_OUT_CARD+TIME_LESS;
				m_g_myself->StartTimer(IDI_OUT_CARD,nElapse);
			}
		}
		return;
	}
}

/// 处理用户进入房间消息
void CRobotLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_myself != NULL);
	if(m_g_myself == NULL) return;

	if(playerId == m_g_myself->GetChairIndex())
		m_g_myself->SendReadyMsg();
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

	switch (timerId)
	{
	case IDI_START_GAME:	//开始游戏
		{
			//开始判断
			if (curTimer==0)
			{
				m_g_myself->SendReadyMsg();
				m_g_myself->StopTimer(IDI_START_GAME);
			}
		}
		return ;
	case IDI_CALL_SCORE:	//用户叫分
		{
			if(curTimer==0)
			{
				m_g_myself->StopTimer(IDI_CALL_SCORE);
				//设置变量
				uint16 bLandScore;
				bLandScore=m_GameLogic.LandScore(m_g_myself->GetChairIndex(), m_cbCurrentLandScore, m_cbCallScorePhase);

				//发送数据
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = SUB_C_LAND_SCORE;
                out["LandScore"] = bLandScore;
				m_g_myself->SendGameMsg(out);
			}
		}
		return ;
	case IDI_OUT_CARD:		//用户出牌
		{
			if(curTimer==0)
			{
				m_g_myself->StopTimer(IDI_OUT_CARD);
				//扑克分析
				tagOutCardResult OutCardResult;
			/*try
				{
			#ifdef _DEBUG
					CString str0 = m_pIAndroidUserItem->GetMeUserItem()->GetUserData()->szAccounts;
					str0 += "\n\n" ;
					OutputDebugString(str0) ;

			#endif*/
					m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbTurnCardData,m_cbTurnCardCount, m_wOutCardUser, m_g_myself->GetChairIndex(), OutCardResult);

			//#ifdef _DEBUG
			//		CString str1 = "finish search";
			//		str1 += "\n\n" ;
			//		OutputDebugString(str1) ;

			//#endif
			//	}
			//	catch(...)
			//	{
			//		//这里的设置，使得进入下面的if处理
			//		ZeroMemory(OutCardResult.cbResultCard, sizeof(OutCardResult.cbResultCard)) ;
			//		OutCardResult.cbCardCount = 10 ;
			//	}
				//牌型合法判断
				if(OutCardResult.cbCardCount>0 && CT_ERROR==m_GameLogic.GetCardType(OutCardResult.cbResultCard, OutCardResult.cbCardCount))
				{
			/*#ifdef _DEBUG
					CString str ;
					for(WORD i=0; i<OutCardResult.cbCardCount; ++i)
					{
						CString strTmp ;
						strTmp.Format("%d,", m_GameLogic.GetCardValue(OutCardResult.cbResultCard[i])) ;
						str += strTmp ;
					}
					str += "\n\n" ;
					OutputDebugString(str) ;

			#endif*/
					memset(&OutCardResult,0, sizeof(OutCardResult)) ;
				}

				//先出牌不能为空
				if(m_cbTurnCardCount==0)
				{
					assert(OutCardResult.cbCardCount>0) ;
					if(OutCardResult.cbCardCount==0)
					{
						//最小一张
						OutCardResult.cbCardCount = 1 ;
						OutCardResult.cbResultCard[0]=m_cbHandCardData[m_cbHandCardCount-1] ;
					}
				}

				//结果判断
				if (OutCardResult.cbCardCount>0)
				{
					//合法判断
					if(m_cbTurnCardCount>0 && !m_GameLogic.CompareCard(m_cbTurnCardData, OutCardResult.cbResultCard, m_cbTurnCardCount, OutCardResult.cbCardCount))
					{

						//放弃出牌
                        Json::Value out;
                        out["MsgId"] = IDD_MESSAGE_ROOM;
                        out["MsgSubId"] = SUB_C_PASS_CARD;
						m_g_myself->SendGameMsg(out);
						return ;
					}

					//删除扑克
					m_cbHandCardCount-=OutCardResult.cbCardCount;
					m_GameLogic.RemoveCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount,m_cbHandCardData,m_cbHandCardCount+OutCardResult.cbCardCount);

                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = SUB_C_OUT_CARD;
                    Json::Value arrayvalue;
					for (int i=0;i<OutCardResult.cbCardCount;i++)
					{
						arrayvalue[i]=m_GameLogic.ChangeCardDate(OutCardResult.cbResultCard[i]);
					}
					out["cards"] = arrayvalue;
					m_g_myself->SendGameMsg(out);

			#ifdef _DEBUG
					CString str1 = TEXT("success search");
					str1 += "\n\n" ;
					OutputDebugString(str1) ;

			#endif
				}
				else
				{
					//放弃出牌
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = SUB_C_PASS_CARD;
					m_g_myself->SendGameMsg(out);
					return ;
				}

			#ifdef _DEBUG
				CString str = TEXT("finish all");
				str += "\n\n" ;
				OutputDebugString(str) ;

			#endif
				return ;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
