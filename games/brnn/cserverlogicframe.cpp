#include "cserverlogicframe.h"
#include <sstream>

static bool m_gamisrunning = false;
int64  m_minzhuangmoney = 0;

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{
	memset(&m_zhuangcards,0,sizeof(m_zhuangcards));
	memset(&m_othercards,0,sizeof(m_othercards));

	m_GameState=STATE_NULL;
	m_timegamestart=0;
	m_timexiazhu=0;
	m_timekaipai=0;
	m_timejiesuan=0;
	m_minzhuangmoney=0;
	m_timecurlost=0;
	m_issupercontrol=false;
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

	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_ROOM_CLEARJETTONS:
		{
			if(m_GameState == STATE_XIAZHU)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_CLEARJETTONS;
                out["playerId"]	= playerId;
                Json::Value arrayObj;
				std::map<YaZhuType,int64>::iterator iter = m_xiazhus[playerId].begin();
				for(int i=0;iter != m_xiazhus[playerId].end();++iter,i++)
				{
					arrayObj[i]["YaZhuType"] = (*iter).first;
					arrayObj[i]["Money"] = (Json::Int64)(*iter).second;
				}
				out["Jettons"] = arrayObj;
				m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_XIAZHU:
		{
			if(m_GameState == STATE_XIAZHU)
			{
				YaZhuType pYaZhuType = (YaZhuType)mes["YaZhuType"].asInt();
				int64 pTempMoney = mes["Money"].asInt();

				Player *pPlayer = m_g_GameRoom->GetPlayer(playerId);
				if(pPlayer && pPlayer->GetMoney() < (getuserjettontotal(playerId) + pTempMoney)*m_CGameLogic.GetMultiple(CT_SPECIAL_NIUNIUXW))
				{
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                    out["playerId"]	= playerId;
                    out["YaZhuType"] = (int)pYaZhuType;
                    out["Money"] = (Json::Int64)pTempMoney;
                    out["State"] = 0;
					m_g_GameRoom->SendTableMsg(playerId,out);

					return;
				}

				if(pPlayer/* && pPlayer->GetMoney() >= getuserjettontotal(playerId) + pTempMoney*/)
				{
					m_xiazhus[playerId][pYaZhuType]+=pTempMoney;

					if(m_ZhuangManager.getCurrentZhuang() != -1)
					{
						Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
						if(pPlayer)
						{
							int64 pTmpMoney = -getusertotalresult();

							if(pTmpMoney < 0 && -pTmpMoney > pPlayer->GetMoney())
							{
                                Json::Value out;
                                out["MsgId"] = IDD_MESSAGE_ROOM;
                                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                                out["playerId"]	= playerId;
                                out["YaZhuType"] = (int)pYaZhuType;
                                out["Money"] = (Json::Int64)pTempMoney;
                                out["State"] = 0;
								m_g_GameRoom->SendTableMsg(playerId,out);

								m_xiazhus[playerId][pYaZhuType]-=pTempMoney;

								return;
							}
						}
					}

                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                    out["playerId"]	= playerId;
                    out["YaZhuType"] = (int)pYaZhuType;
                    out["Money"] = (Json::Int64)pTempMoney;
                    out["State"] = 1;
					m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
				}
			}
		}
		break;
	case IDD_MESSAGE_ROOM_GETZHUANGLIST:
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GETZHUANGLIST;
            out["minzhuangmoney"] = (Json::Int64)m_minzhuangmoney;
            for(int i=0;i<(int)m_ZhuangManager.getShangZhuangList().size();i++)
                out[i] = m_ZhuangManager.getShangZhuangList()[i];
			m_g_GameRoom->SendTableMsg(playerId,out);
		}
		break;
	case IDD_MESSAGE_ROOM_SHANGZHUANG:
		{
			if(m_ZhuangManager.addZhuang(playerId))
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
                out["State"] = 1;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
			else
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
                out["State"] = 0;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_XIAZHUANG:
		{
			if(m_ZhuangManager.decZhuang(playerId,m_GameState == STATE_GAMING ? true : false))
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
                out["State"] = 1;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
			else
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
                out["State"] = 0;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_SUERCONTROL:
		{
			int pcontroltype = mes["controltype"].asInt();
			int pmasterstate = mes["masterstate"].asInt();

			memset(&m_zhuangcards,0,sizeof(m_zhuangcards));
			memset(&m_othercards,0,sizeof(m_othercards));
			memset(&m_cards,0,sizeof(m_cards));

			srand(time(0));
			m_CGameLogic.RandCardList(m_cards,25);

			if(pcontroltype == 0)
			{
				if(pmasterstate == 1)
					masterwinorlost(true);
				else
					masterwinorlost(false);
			}
			else if(pcontroltype == 1)
			{
				int pWinState = mes["WinState"].asInt();

				if(pWinState == 1)
					thetypewin((YaZhuType)pmasterstate,true);
				else
					thetypewin((YaZhuType)pmasterstate,false);
			}
			else if(pcontroltype == 2)
			{
				int pWinState = mes["WinState"].asInt();

				if(pWinState == 1)
					allareawinorlost(false);
				else
					allareawinorlost(true);
			}

			m_issupercontrol=true;
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

	if(m_gamisrunning == false)
	{
		LoadServerConfig();

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_STARTING,m_timegamestart);
		m_gamisrunning=true;
	}

    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_ENTERGAME;
    out["GameState"] = m_GameState;
    out["timegamestart"] = m_timegamestart;
    out["timexiazhu"] = m_timexiazhu;
    out["timekaipai"] = m_timekaipai;
    out["timejiesuan"] = m_timejiesuan;
    out["timecurlost"] = m_timecurlost;
    out["CurrentZhuang"] = m_ZhuangManager.getCurrentZhuang();
    out["TotalShangZhuangCount"] = m_ZhuangManager.getTotalShangZhuangCount();
    out["CurShangZhuangCount"] = m_ZhuangManager.GetCurShangZhuangCount();
    out["ZhuangTotalWinResult"] = (Json::Int64)m_ZhuangManager.GetZhuangTotalWinResult();
	m_g_GameRoom->SendTableMsg(playerId,out);
}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	m_ZhuangManager.OnProcessUserLeave(playerId);
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
    out["MsgSubId"] = IDD_MESSAGE_ROOM_REENTERGAME;
    out["GameState"] = m_GameState;
    out["timegamestart"] = m_timegamestart;
    out["timexiazhu"] = m_timexiazhu;
    out["timekaipai"] = m_timekaipai;
    out["timejiesuan"] = m_timejiesuan;
    out["timecurlost"] = m_timecurlost;
    out["CurrentZhuang"] = m_ZhuangManager.getCurrentZhuang();
    out["TotalShangZhuangCount"] = m_ZhuangManager.getTotalShangZhuangCount();
    out["CurShangZhuangCount"] = m_ZhuangManager.GetCurShangZhuangCount();
    out["ZhuangTotalWinResult"] = (Json::Int64)m_ZhuangManager.GetZhuangTotalWinResult();
	for(int i=0;i<4;i++) out["userareajetton"][i] = (Json::Int64)getuserareajetton((YaZhuType)i);
	for(int i=0;i<25;i++) out["cards"][i] = m_cards[i];
	out["zhuangcardtype"] = m_CGameLogic.GetCardType(m_cards,5);
	out["othercardtype1"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*5,5);
	out["othercardtype2"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*(5*2),5);
	out["othercardtype3"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*(5*3),5);
	out["othercardtype4"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*(5*4),5);
	m_g_GameRoom->SendTableMsg(playerId,out);
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	m_timecurlost = curTimer;

	if(timerId == IDD_TIMER_GAME_STARTING && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_STARTING);

		//startgame();
		readygame();
	}
	else if(timerId == IDD_TIMER_GAME_XIAZHU && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_XIAZHU);

		if(!m_issupercontrol)
			GetPlayerPanData();

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_FAPAI;
	out["GameState"] = STATE_GAMING;
        for(int i=0;i<25;i++) out["cards"][i] = m_cards[i];
        out["zhuangcardtype"] = m_CGameLogic.GetCardType(m_cards,5);
        out["othercardtype1"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*5,5);
        out["othercardtype2"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*(5*2),5);
        out["othercardtype3"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*(5*3),5);
        out["othercardtype4"] = m_CGameLogic.GetCardType(m_cards+sizeof(uint8)*(5*4),5);
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		m_GameState=STATE_GAMING;
		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_KAIPAI,m_timekaipai);
	}
	else if(timerId == IDD_TIMER_GAME_KAIPAI && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_KAIPAI);

		TongJiUserGamingResult();

		m_GameState=STATE_GAMEEND;
		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_JIESUAN,m_timejiesuan);
	}
	else if(timerId == IDD_TIMER_GAME_JIESUAN && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_JIESUAN);

		//startgame();
		readygame();
	}
}

/// 导入服务器配置
void CServerLogicFrame::LoadServerConfig(void)
{
    std::string paramstr = m_g_GameRoom->GetRoomParameters();
    if(paramstr.empty()) return;

    Json::Reader reader;
    Json::Value json_object;

    if (!reader.parse(paramstr.c_str(), json_object))
    {
     std::stringstream ss;
        ss << "数据解析失败:" << paramstr;
        m_g_GameRoom->Room_Log(BASIC,ss.str());
        return;
    }

	m_ZhuangManager.setGameRoom(m_g_GameRoom);

	int m_countshangZhuang=json_object["shangzhuangcount"].asInt();
	m_ZhuangManager.setTotalShangZhuangCount(m_countshangZhuang);
	m_minzhuangmoney=json_object["minzhuangmoney"].asInt();
	m_ZhuangManager.setMinShangZhuangMoney(m_minzhuangmoney);

	m_timegamestart=json_object["gamestart"].asInt();
	m_timexiazhu=json_object["xiazhu"].asInt();
	m_timekaipai=json_object["kaipai"].asInt();
	m_timejiesuan=json_object["jiesuan"].asInt();
}

int64 CServerLogicFrame::getuserjettontotal(int pChairId)
{
	int64 pResult = 0;

	std::map<int32,std::map<YaZhuType,int64> >::iterator iter = m_xiazhus.find(pChairId);
	if(iter != m_xiazhus.end())
	{
		std::map<YaZhuType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
			pResult+=(*iterTwo).second;
	}

	return pResult;
}

void CServerLogicFrame::copycardtocards(uint8 *pcard,YaZhuType ptype)
{
	int index = 0;

	if(ptype < YAZHUTYPE_NULL)
		index=5+ptype*5;

	for(int k=0;k<5;k++)
	{
		m_cards[index++] = pcard[k];
	}
}

void CServerLogicFrame::thetypewin(YaZhuType ptype,bool isWin)
{
	uint8 pcards[5][5];
	int index = 0;

	for(int i=0;i<5;i++)
	{
		for(int k=0;k<5;k++)
		{
			pcards[i][k] = m_cards[index++];
		}
	}

	int pMax = 0;

	for(int k=0;k<5;k++)
	{
		uint8 pMultiple = 1;

		int pstate = m_CGameLogic.CompareCard(pcards[pMax],5,pcards[k],5,pMultiple);

		if(isWin)
		{
			if(pstate == 1)
				pMax = k;
		}
		else
		{
			if(pstate == -1)
				pMax = k;
		}
	}

	memset(&m_cards,0,sizeof(m_cards));
	index = 5;

	copycardtocards(pcards[pMax],ptype);

	std::vector<YaZhuType> pYaZhuTypes;
	std::vector<int> pCardTypes;
	for(int i=0;i<=YAZHUTYPE_NULL;i++)
		if(i != ptype) pYaZhuTypes.push_back((YaZhuType)i);
	for(int i=0;i<5;i++)
		if(i != pMax) pCardTypes.push_back(i);

	for(int i=0;i<(int)pYaZhuTypes.size();i++)
		copycardtocards(pcards[pCardTypes[i]],pYaZhuTypes[i]);

	memcpy(&m_zhuangcards,m_cards,sizeof(uint8)*5);
	memcpy(&m_othercards,m_cards+sizeof(uint8)*5,sizeof(uint8)*20);
}

void CServerLogicFrame::allareawinorlost(bool iswin)
{
	uint8 pcards[5][5];
	int index = 0;

	for(int i=0;i<5;i++)
	{
		for(int k=0;k<5;k++)
		{
			pcards[i][k] = m_cards[index++];
		}
	}

	for(int index=0;index<5;index++)
	{
		for(int k=0;k<5;k++)
		{
			uint8 pMultiple = 1;

			if(iswin)
			{
				int pstate = m_CGameLogic.CompareCard(pcards[index],5,pcards[k],5,pMultiple);

				if(pstate == 1)
				{
					uint8 ptmpCard[5];
					memset(ptmpCard,0,sizeof(ptmpCard));

					memcpy(ptmpCard,pcards[index],sizeof(ptmpCard));
					memcpy(pcards[index],pcards[k],sizeof(ptmpCard));
					memcpy(pcards[k],ptmpCard,sizeof(ptmpCard));
				}
			}
			else
			{
				int pstate = m_CGameLogic.CompareCard(pcards[index],5,pcards[k],5,pMultiple);

				if(pstate == -1)
				{
					uint8 ptmpCard[5];
					memset(ptmpCard,0,sizeof(ptmpCard));

					memcpy(ptmpCard,pcards[index],sizeof(ptmpCard));
					memcpy(pcards[index],pcards[k],sizeof(ptmpCard));
					memcpy(pcards[k],ptmpCard,sizeof(ptmpCard));
				}
			}
		}
	}

	memset(&m_cards,0,sizeof(m_cards));
	memcpy(m_cards,pcards,sizeof(m_cards));

	memcpy(&m_zhuangcards,m_cards,sizeof(uint8)*5);
	memcpy(&m_othercards,m_cards+sizeof(uint8)*5,sizeof(uint8)*20);
}

void CServerLogicFrame::masterwinorlost(bool iswin)
{
	uint8 pcards[5][5];
	int index = 0;

	for(int i=0;i<5;i++)
	{
		for(int k=0;k<5;k++)
		{
			pcards[i][k] = m_cards[index++];
		}
	}

	int pMax = 0;

	for(int k=0;k<5;k++)
	{
		uint8 pMultiple = 1;

		if(iswin)
		{
			int pstate = m_CGameLogic.CompareCard(pcards[pMax],5,pcards[k],5,pMultiple);

			if(pstate == 1)
				pMax = k;
		}
		else
		{
			int pstate = m_CGameLogic.CompareCard(pcards[pMax],5,pcards[k],5,pMultiple);

			if(pstate == -1)
				pMax = k;
		}
	}

	memset(&m_cards,0,sizeof(m_cards));
	index = 5;

	for(int i=0;i<5;i++)
	{
		if(i == pMax)
		{
			for(int k=0;k<5;k++)
				m_cards[k] = pcards[pMax][k];

			continue;
		}

		for(int k=0;k<5;k++)
		{
			m_cards[index++] = pcards[i][k];
		}
	}

	memcpy(&m_zhuangcards,m_cards,sizeof(uint8)*5);
	memcpy(&m_othercards,m_cards+sizeof(uint8)*5,sizeof(uint8)*20);
}

int64 CServerLogicFrame::getrealuserjettontotal(void)
{
	if(m_xiazhus.empty()) return 0;

	int64 ptotal = 0;

	std::map<int32,std::map<YaZhuType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		//Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		//if(pPlayer == NULL || pPlayer->GetType() != PLAYERTYPE_NORMAL)
		//	continue;

		std::map<YaZhuType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
			ptotal += (*iterTwo).second;
	}

	return ptotal;
}

void CServerLogicFrame::GetPlayerPanData(void)
{
	memset(&m_zhuangcards,0,sizeof(m_zhuangcards));
	memset(&m_othercards,0,sizeof(m_othercards));
	memset(&m_cards,0,sizeof(m_cards));

	m_CGameLogic.RandCardList(m_cards,25);

	memcpy(&m_zhuangcards,m_cards,sizeof(uint8)*5);
	memcpy(&m_othercards,m_cards+sizeof(uint8)*5,sizeof(uint8)*20);

	int64 pResult,pMax,pMin;
	pResult=pMax=pMin=0;

	int pstate = m_g_GameRoom->IsUserWin(&pResult,&pMax,&pMin);

	if(pstate == 2)
	{
		if(m_ZhuangManager.getCurrentZhuang() == -1)
		{
			masterwinorlost(true);
		}
		else
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
			if(pPlayer)
			{
				if(pPlayer->GetType() == PLAYERTYPE_ROBOT)
					masterwinorlost(true);
				else
					masterwinorlost(false);
			}
		}
	}
	else
	{
		int64 pMasterResult = getuserswinandlostresult();

		if(m_ZhuangManager.getCurrentZhuang() == -1)
		{
			if(pMasterResult > pMax-pResult)
				masterwinorlost(true);
		}
		else
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
			if(pPlayer)
			{
				if(pMasterResult > pMax-pResult)
				{
					if(pPlayer->GetType() == PLAYERTYPE_NORMAL)
					{
						masterwinorlost(false);
					}
					else
					{
						masterwinorlost(true);
					}
				}
			}
		}
	}
}

int64 CServerLogicFrame::getusertotalresult(void)
{
	int64 pResult = 0;

	std::map<int32,std::map<YaZhuType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		std::map<YaZhuType,int64>::iterator iter2 = (*iter).second.begin();
		for(;iter2 != (*iter).second.end();++iter2)
		{
			int64 pTmpResult = 0;

			uint8 ptianMultiple=m_CGameLogic.GetMultiple(CT_SPECIAL_NIUNIUXW);
			//int ptianstate = m_CGameLogic.CompareCard(m_zhuangcards,5,m_othercards[(int)(*iter2).first],5,ptianMultiple);

			//if(ptianstate > 0)
				pTmpResult = (*iter2).second * ptianMultiple;
	/*		else if(ptianstate < 0)
				pTmpResult = -((*iter2).second * ptianMultiple);*/

			pResult += pTmpResult;
		}
	}

	return pResult;
}

int64 CServerLogicFrame::getuserswinandlostresult(void)
{
	int64 pTotalResult = 0;

	std::map<int32,std::map<YaZhuType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL || pPlayer->GetType() != PLAYERTYPE_NORMAL)
			continue;

		int64 pResult = 0;

		std::map<YaZhuType,int64>::iterator iter2 = (*iter).second.begin();
		for(;iter2 != (*iter).second.end();++iter2)
		{
			int64 pTmpResult = 0;

			uint8 ptianMultiple=1;
			int ptianstate = m_CGameLogic.CompareCard(m_zhuangcards,5,m_othercards[(int)(*iter2).first],5,ptianMultiple);

			if(ptianstate > 0)
			{
				pTmpResult = (*iter2).second * ptianMultiple;
				//pTmpResult -= (*iter2).second;
			}
			else if(ptianstate < 0)
				pTmpResult = -((*iter2).second * ptianMultiple);

			pResult += pTmpResult;
		}

		pTotalResult += pResult;
	}

	//pTotalResult = -pTotalResult;

	return pTotalResult;
}

void CServerLogicFrame::TongJiUserGamingResult()
{
	int64 pTotalResult = 0;
	int64 pOldTotalResult = 0;

	std::map<int32,std::map<YaZhuType,int64> > m_userresultlist;
	std::map<YaZhuType,int64> m_usertotalresultlist,m_usertotaljettonlist;

	std::map<int32,std::map<YaZhuType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		int64 pResult = 0;
		std::string gametip;

		std::map<YaZhuType,int64>::iterator iter2 = (*iter).second.begin();
		for(;iter2 != (*iter).second.end();++iter2)
		{
			int64 pTmpResult = 0;

			uint8 ptianMultiple=1;
			int ptianstate = m_CGameLogic.CompareCard(m_zhuangcards,5,m_othercards[(int)(*iter2).first],5,ptianMultiple);

			if(ptianstate > 0)
			{
				pTmpResult = (*iter2).second * ptianMultiple;
				//pTmpResult -= (*iter2).second;
			}
			else if(ptianstate < 0)
				pTmpResult = -((*iter2).second * ptianMultiple);

			m_userresultlist[(*iter).first][(*iter2).first] = pTmpResult;
			m_usertotalresultlist[(*iter2).first] += pTmpResult;
			m_usertotaljettonlist[(*iter2).first] += (*iter2).second;
			pResult += pTmpResult;

			char str[256];
			sprintf(str,"%s:%ld",getCardTypeName((*iter2).first).c_str(),(*iter2).second);
			gametip += str;
		}

		pTotalResult += pResult;

		int64 lRevenue = 0;
		int64 lAgentRevenue = 0;

		if(pResult > 0)
		{
			lRevenue = (int64)((float)pResult * m_g_GameRoom->GetChouShui());
			pResult -= lRevenue;
		}

		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer && pResult < 0 && -pResult > pPlayer->GetMoney())
			pResult = -pPlayer->GetMoney();

		enScoreKind ScoreKind = pResult > 0 ? enScoreKind_Win : enScoreKind_Lost;

		//if(pResult > 0)
		//{
		//	float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
		//	lAgentRevenue = (int64)((float)pResult * pAgentRate);
		//	pResult -= lAgentRevenue;
		//}

		m_g_GameRoom->WriteUserScore((*iter).first, pResult, lRevenue, ScoreKind,lAgentRevenue,true,0,gametip.c_str());

		if(pPlayer) m_g_GameRoom->UpdateUserScore(pPlayer);
	}

	pTotalResult = -pTotalResult;
	pOldTotalResult = pTotalResult;

	if(m_ZhuangManager.getCurrentZhuang() != -1)
	{
		int64 lRevenue = 0;
		int64 lAgentRevenue = 0;

		if(pTotalResult > 0)
		{
			lRevenue = (int64)((float)pTotalResult * m_g_GameRoom->GetChouShui());
			pTotalResult -= lRevenue;
		}

		Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
		if(pPlayer && pTotalResult < 0 && -pTotalResult > pPlayer->GetMoney())
			pTotalResult = -pPlayer->GetMoney();

		enScoreKind ScoreKind = pTotalResult > 0 ? enScoreKind_Win : enScoreKind_Lost;

		//if(pTotalResult > 0)
		//{
		//	float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
		//	lAgentRevenue = (int64)((float)pTotalResult * pAgentRate);
		//	pTotalResult -= lAgentRevenue;
		//}

		std::string jettonstr2="闲家下注:";
		std::map<YaZhuType,int64>::iterator iter = m_usertotaljettonlist.begin();
		for(;iter != m_usertotaljettonlist.end();++iter)
		{
			char str[256];
			sprintf(str,"%s:%ld",getCardTypeName((*iter).first).c_str(),(*iter).second);
			jettonstr2+=str;
		}

		m_g_GameRoom->WriteUserScore(m_ZhuangManager.getCurrentZhuang(), pTotalResult, lRevenue, ScoreKind,lAgentRevenue,true,0,jettonstr2.c_str());

		if(pPlayer)
		{
			m_ZhuangManager.SetZhuangTotalWinResult(m_ZhuangManager.getCurrentZhuang(),pOldTotalResult);
			m_g_GameRoom->UpdateUserScore(pPlayer);
		}

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
        out["OldTotalResult"] = (Json::Int64)pOldTotalResult;
	out["State"] = 1;
        Json::Value arrayValue;
		std::map<YaZhuType,int64>::iterator iter4 = m_usertotalresultlist.begin();
		for(int i=0;iter4 != m_usertotalresultlist.end();++iter4,i++)
		{
			arrayValue[i]["YaZhuType"] = (int)(*iter4).first;
			arrayValue[i]["Money"] = (Json::Int64)(-(*iter4).second);
		}
		out["Jettons"] = arrayValue;
		m_g_GameRoom->SendTableMsg(m_ZhuangManager.getCurrentZhuang(),out);
	}

	int count = m_g_GameRoom->GetPlayerCount();

	for(int i=0;i<m_g_GameRoom->GetMaxPlayer();i++)
	{
		if(i == m_ZhuangManager.getCurrentZhuang()) continue;

		Player *pPlayer = m_g_GameRoom->GetPlayer(i);
		if(pPlayer == NULL) continue;

		std::map<int32,std::map<YaZhuType,int64> >::iterator iter3 = m_userresultlist.find(pPlayer->GetChairIndex());
		if(iter3 != m_userresultlist.end())
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
            out["OldTotalResult"] = (Json::Int64)pOldTotalResult;
            out["State"] = 1;
            Json::Value arrayValue;
			std::map<YaZhuType,int64>::iterator iter4 = (*iter3).second.begin();
			for(int i=0;iter4 != (*iter3).second.end();++iter4,i++)
			{
				arrayValue[i]["YaZhuType"] = (int)(*iter4).first;
				arrayValue[i]["Money"] = (Json::Int64)(*iter4).second;
			}
			out["Jettons"] = arrayValue;
			m_g_GameRoom->SendTableMsg((*iter3).first,out);
		}
		else
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
            out["OldTotalResult"] = (Json::Int64)pOldTotalResult;
            out["State"] = 0;
			m_g_GameRoom->SendTableMsg(pPlayer->GetChairIndex(),out);
		}
	}

	//if(m_ZhuangManager.getCurrentZhuang() != -1)
	//{
	//	CMolMessageOut out(IDD_MESSAGE_ROOM);
	//	out.write16(IDD_MESSAGE_ROOM_GAMEEND);
	//	out.write64(pOldTotalResult);
	//	out.write64(0);
	//	m_g_GameRoom->SendTableMsg(m_ZhuangManager.getCurrentZhuang(),out);
	//}

	m_ZhuangManager.OnProcessGameEnd();
	m_g_GameRoom->GameEnd();
}

int64 CServerLogicFrame::getuserareajetton(YaZhuType ptype)
{
	int64 pTotal = 0;

	std::map<int32,std::map<YaZhuType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		std::map<YaZhuType,int64>::iterator iter2 = (*iter).second.find(ptype);
		if(iter2 != (*iter).second.end())
			pTotal += (*iter2).second;
	}

	return pTotal;
}

void CServerLogicFrame::startgame(void)
{
	m_xiazhus.clear();

	m_issupercontrol=false;
	m_ZhuangManager.OnProcessGameStart();

    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_ROOM;
    out["MsgSubId"] = IDD_MESSAGE_ROOM_STARTXIAZHU;
    out["GameState"] = STATE_XIAZHU;
    out["CurrentZhuang"] = m_ZhuangManager.getCurrentZhuang();
    out["TotalShangZhuangCount"] = m_ZhuangManager.getTotalShangZhuangCount();
    out["CurShangZhuangCount"] = m_ZhuangManager.GetCurShangZhuangCount();
    out["ZhuangTotalWinResult"] = (Json::Int64)m_ZhuangManager.GetZhuangTotalWinResult();
	m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

	m_GameState = STATE_XIAZHU;
	m_g_GameRoom->StartTimer(IDD_TIMER_GAME_XIAZHU,m_timexiazhu);
}

void CServerLogicFrame::readygame(void)
{
	for(int i=0;i<m_g_GameRoom->GetMaxPlayer();i++)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer(i);
		if(pPlayer == NULL) continue;

		pPlayer->SendReadyMsg();
	}

	m_g_GameRoom->GameStart();
	startgame();
}
