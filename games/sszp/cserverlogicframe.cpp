#include "cserverlogicframe.h"
#include <sstream>

int64                          m_minzhuangmoney = 0;
static bool m_gamisrunning = false;

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL),m_GameState(STATE_NULL)
{
	m_timegamestart = m_timexiazhu = m_timekaipai = m_timejiesuan = 0;
	m_curGamingResult = -1;
	m_curDecCardType = CARDTYPE_NULL;
	m_issupercontrol = false;
	m_timecurlost = 0;
	m_isZhuangYing = 0;
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
	case IDD_MESSAGE_ROOM_CLEARJETTON:
	{
		if (m_GameState == STATE_XIAZHU)
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_CLEARJETTON;
            out["playerId"] = playerId;

            Json::Value arrayObj;
			std::map<tagCardType, int64>::iterator iter = m_xiazhus[playerId].begin();
			for (int i=0; iter != m_xiazhus[playerId].end(); ++iter,i++)
			{
				out[i]["CardType"] = (*iter).first;
				out[i]["Money"] = (int)(*iter).second;
			}
			out["jettons"] = arrayObj;

			m_g_GameRoom->SendTableMsg(INVALID_CHAIR, out);

			m_xiazhus[playerId].clear();
		}
	}
		break;
	case IDD_MESSAGE_ROOM_XIAZHU:
		{
			tagCardType pYaZhuType = (tagCardType)mes["CardType"].asInt();
			int64 pTempMoney = mes["Money"].asInt();

			if (m_GameState != STATE_XIAZHU)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                out["playerId"] = playerId;
				out["CardType"] = (int)pYaZhuType;
				out["Money"] = (int)pTempMoney;
				out["state"] = 0;
				m_g_GameRoom->SendTableMsg(playerId, out);

				return;
			}

			if (m_GameState == STATE_XIAZHU)
			{
				Player *pPlayer = m_g_GameRoom->GetPlayer(playerId);
				if (pPlayer && pPlayer->GetMoney() < (GetUserJettonTotal(playerId) + pTempMoney))
				{
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                    out["playerId"] = playerId;
                    out["CardType"] = (int)pYaZhuType;
                    out["Money"] = (int)pTempMoney;
                    out["state"] = 0;
					m_g_GameRoom->SendTableMsg(playerId, out);

					return;
				}

				if (pPlayer/* && pPlayer->GetMoney() >= getuserjettontotal(playerId) + pTempMoney*/)
				{
					m_xiazhus[playerId][pYaZhuType] += pTempMoney;

					if (m_ZhuangManager.getCurrentZhuang() != -1)
					{
						Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
						if (pPlayer)
						{
							int64 pTmpMoney = -getusertotalresult();

							if (pTmpMoney < 0 && -pTmpMoney > pPlayer->GetMoney())
							{
                                Json::Value out;
                                out["MsgId"] = IDD_MESSAGE_ROOM;
                                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                                out["playerId"] = playerId;
                                out["CardType"] = (int)pYaZhuType;
                                out["Money"] = (int)pTempMoney;
                                out["state"] = 0;
								m_g_GameRoom->SendTableMsg(playerId, out);

								m_xiazhus[playerId][pYaZhuType] -= pTempMoney;

								return;
							}
						}
					}

					Player *pPlayer = m_g_GameRoom->GetPlayer(playerId);

                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                    out["playerId"] = (pPlayer == NULL ? playerId : pPlayer->GetID());
                    out["CardType"] = (int)pYaZhuType;
                    out["Money"] = (int)pTempMoney;
                    out["state"] = 1;
					m_g_GameRoom->SendTableMsg(INVALID_CHAIR, out);
				}
			}
		}
		break;
	case IDD_MESSAGE_ROOM_GETZHUANGLIST:
	{
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_GETZHUANGLIST;
        out["minzhuangmoney"] = (int)m_minzhuangmoney;
        Json::Value arrayObj;
        for (int i = 0; i<(int)m_ZhuangManager.getShangZhuangList().size(); i++)
        {
            arrayObj[i] = m_ZhuangManager.getShangZhuangList()[i];
        }
        out["ShangZhuangList"] = arrayObj;

		m_g_GameRoom->SendTableMsg(playerId, out);
	}
		break;
	case IDD_MESSAGE_ROOM_SHANGZHUANG:
	{
		if (m_ZhuangManager.addZhuang(playerId))
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
            out["type"] = 1;
			m_g_GameRoom->SendTableMsg(playerId, out);
		}
		else
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
            out["type"] = 0;
			m_g_GameRoom->SendTableMsg(playerId, out);
		}
	}
		break;
	case IDD_MESSAGE_ROOM_XIAZHUANG:
	{
		if (m_ZhuangManager.decZhuang(playerId, m_GameState == STATE_GAMING ? true : false))
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
            out["type"] = 1;
			m_g_GameRoom->SendTableMsg(playerId, out);
		}
		else
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
            out["type"] = 0;
			m_g_GameRoom->SendTableMsg(playerId, out);
		}
	}
		break;
	case IDD_MESSAGE_ROOM_SUERCONTROL:
	{
		m_supercontroltype = mes["supercontroltype"].asInt();

		if (m_supercontroltype == 0)
			m_curDecCardType = (tagCardType)mes["CardType"].asInt();
		else
			m_isZhuangYing = mes["isZhuangYing"].asInt() > 0 ? true : false;

		m_issupercontrol = true;
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

	m_xiazhus[playerId].clear();

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
    out["ZhuangTotalWinResult"] = (int)m_ZhuangManager.GetZhuangTotalWinResult();

    Json::Value arrayObj;
    for(int i=0;i<(int)m_GameRecords.size();i++)
    {
        arrayObj[i] = m_GameRecords[i];
    }
    out["GameRecords"] = arrayObj;

    Json::Value arrayObj2;
	std::map<tagCardType, int>::iterator iter = m_carBeiLv.begin();
	for (int i=0; iter != m_carBeiLv.end(); ++iter,i++)
	{
        arrayObj2[i]["CardType"] = (*iter).first;
        arrayObj2[i]["BeiLv"] = (*iter).second;
	}
	out["BeiLvs"] = arrayObj2;

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
    out["ZhuangTotalWinResult"] = (int)m_ZhuangManager.GetZhuangTotalWinResult();

    Json::Value arrayObj;
    for(int i=0;i<(int)m_GameRecords.size();i++)
    {
        arrayObj[i] = m_GameRecords[i];
    }
    out["GameRecords"] = arrayObj;

 	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.find(playerId);
	if(iter != m_xiazhus.end())
	{
		out["xiazhucount"] = (int)(*iter).second.size();

		Json::Value arrayObj2;
		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(int i=0;iterTwo != (*iter).second.end();++iterTwo,i++)
		{
			arrayObj2[i]["CardType"] = (int)(*iterTwo).first;
			arrayObj2[i]["Money"] = (int)(*iterTwo).second;
		}
		out["xiazhus"] = arrayObj2;
	}
	else
	{
		out["xiazhucount"] = 0;
	}

	Json::Value arrayObj3;
	std::map<tagCardType, int>::iterator iter5 = m_carBeiLv.begin();
	for (int i=0; iter5 != m_carBeiLv.end(); ++iter5,i++)
	{
		arrayObj3[i]["CardType"] = (*iter5).first;
		arrayObj3[i]["Money"] = (*iter5).second;
	}
	out["BeiLvs"] = arrayObj3;

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

		std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.begin();
		for(;iter != m_xiazhus.end();++iter) (*iter).second.clear();
		m_curGamingResult = -1;

		m_g_GameRoom->GameStart();
		m_xiazhus.clear();
		m_ZhuangManager.OnProcessGameStart();
		m_GameState = STATE_XIAZHU;
		m_curDecCardType = CARDTYPE_NULL;
		m_issupercontrol = false;
		m_isZhuangYing = false;

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_STARTXIAZHU;
        out["GameState"] = m_GameState;
        out["CurrentZhuang"] = m_ZhuangManager.getCurrentZhuang();
        out["TotalShangZhuangCount"] = m_ZhuangManager.getTotalShangZhuangCount();
        out["CurShangZhuangCount"] = m_ZhuangManager.GetCurShangZhuangCount();
        out["ZhuangTotalWinResult"] = (int)m_ZhuangManager.GetZhuangTotalWinResult();
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_XIAZHU,m_timexiazhu);
	}
	else if(timerId == IDD_TIMER_GAME_XIAZHU && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_XIAZHU);

		srand(time(NULL));

		if (m_issupercontrol)
		{
			if (m_supercontroltype == 0)
				m_curGamingResult = m_GameLogic.GetPostionByCarType(m_curDecCardType);
			else
				m_curGamingResult = masterwinorlost(m_isZhuangYing,true);
		}
		else
		{
			m_curGamingResult = GetGamingResult();
		}

//#ifdef _DEBUG
//		m_curGamingResult = 3;
//#endif

		m_GameState=STATE_GAMING;

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_FAPAI;
        out["GameState"] = m_GameState;
        out["curGamingResult"] = m_curGamingResult;
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_KAIPAI,m_timekaipai);
	}
	else if(timerId == IDD_TIMER_GAME_KAIPAI && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_KAIPAI);

		if((int)m_GameRecords.size() >= 9)
			m_GameRecords.erase(m_GameRecords.begin());

		m_GameRecords.push_back(m_curGamingResult);

		m_GameState = STATE_NULL;
		GameEnd();

		m_g_GameRoom->GameEnd();

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_STARTING,m_timejiesuan);
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

	int m_countshangZhuang = json_object["shangzhuangcount"].asInt();
	m_ZhuangManager.setTotalShangZhuangCount(m_countshangZhuang);
	m_minzhuangmoney = json_object["zhuangminmoney"].asInt();
	m_ZhuangManager.setMinShangZhuangMoney(m_minzhuangmoney);

	m_carBeiLv[CARDTYPE_QINGLONG]=json_object["qinglong"].asInt();
	m_carBeiLv[CARDTYPE_BAIHU]=json_object["baihu"].asInt();
	m_carBeiLv[CARDTYPE_ZHUQUE]=json_object["zhuque"].asInt();
	m_carBeiLv[CARDTYPE_XUANWU]=json_object["xuanwu"].asInt();
	m_carBeiLv[CARDTYPE_XIAOBAILONG]=json_object["xiaoqinglong"].asInt();
	m_carBeiLv[CARDTYPE_XIAOLAOHU]=json_object["xiaobaihu"].asInt();
	m_carBeiLv[CARDTYPE_XIAOFENGHUANG]=json_object["xiaofenghuang"].asInt();
	m_carBeiLv[CARDTYPE_XIAOWUGUI]=json_object["xiaowugui"].asInt();

	m_timegamestart=json_object["gamestart"].asInt();
	m_timexiazhu=json_object["xiazhu"].asInt();
	m_timekaipai=json_object["kaipai"].asInt();
	m_timejiesuan=json_object["jiesuan"].asInt();
}

/// 统计各个区域的下注总输赢
int64 CServerLogicFrame::GetRectJettonTotal(tagCardType pCarType, bool isYouRobits)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		if (!isYouRobits)
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
			if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT)
				continue;
		}

		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			if((*iterTwo).first == pCarType)
				pTotal += ((*iterTwo).second * m_carBeiLv[(*iterTwo).first] - (*iterTwo).second);
			else
				pTotal -= (*iterTwo).second;
		}
	}

	return pTotal;
}

int CServerLogicFrame::masterwinorlost(bool iswin, bool isYouRobits, int64 pLastMoney)
{
	std::map<int64, std::vector<tagCardType> > tmpResult;

	for (int i = 0; i < CARDTYPE_NULL; i++)
	{
		int64 pTempMoney = GetRectJettonTotal((tagCardType)i, isYouRobits);
		tmpResult[pTempMoney].push_back((tagCardType)i);
	}

	if (tmpResult.empty())
		return rand() % 16;

	std::vector<tagCardType> pTempVector;

	std::map<int64, std::vector<tagCardType> >::iterator iter = tmpResult.begin();
	for (; iter != tmpResult.end(); ++iter)
	{
		if (iswin)
		{
			if ((*iter).first < 0)
			{
				//if (pLastMoney != 0)
				//{
				//	if (abs((*iter).first) < pLastMoney)
				//	{
				//		for (int i = 0; i < (int)(*iter).second.size(); i++)
				//			pTempVector.push_back((*iter).second[i]);
				//	}
				//}
				//else
				//{
					for (int i = 0; i < (int)(*iter).second.size();i++)
						pTempVector.push_back((*iter).second[i]);
				//}
			}
		}
		else
		{
			if ((*iter).first > 0)
			{
				//if (pLastMoney != 0)
				//{
				//	if (abs((*iter).first) < pLastMoney)
				//	{
				//		for (int i = 0; i < (int)(*iter).second.size(); i++)
				//			pTempVector.push_back((*iter).second[i]);
				//	}
				//}
				//else
				//{
					for (int i = 0; i < (int)(*iter).second.size(); i++)
						pTempVector.push_back((*iter).second[i]);
				//}
			}
		}
	}

	if (pTempVector.empty())
		return rand() % 16;

	return m_GameLogic.GetPostionByCarType(pTempVector[rand() % (int)pTempVector.size()]);
}

/// 统计指定玩家的下注总额
int64 CServerLogicFrame::GetUserJettonTotal(uint32 pUserId)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.find(pUserId);
	if(iter != m_xiazhus.end())
	{
		//Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		//if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT)
		//	return 0;

		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
			pTotal += (*iterTwo).second;
	}

	return pTotal;
}

int64 CServerLogicFrame::getusertotalresult(void)
{
	int64 pResult = 0;

	for (int i = 0; i < CARDTYPE_NULL; i++)
	{
		int64 pTempResult = 0;

		std::map<int32, std::map<tagCardType, int64> >::iterator iter = m_xiazhus.begin();
		for (; iter != m_xiazhus.end(); ++iter)
		{
			std::map<tagCardType, int64>::iterator iter2 = (*iter).second.begin();
			for (; iter2 != (*iter).second.end(); ++iter2)
			{
				int64 pTmpResult = 0;

				if ((*iter2).first == (tagCardType)i)
					pTmpResult = (*iter2).second * m_carBeiLv[(*iter2).first];
				else
					pTmpResult = -(*iter2).second;

				pTempResult += pTmpResult;
			}
		}

		if (pTempResult > pResult)
			pResult = pTempResult;
	}


	return pResult;
}

int64 CServerLogicFrame::getuserswinandlostresult(void)
{
	int64 pTotalResult = 0;

	for(int i=0;i<CARDTYPE_NULL;i++)
	{
		int64 pTempResult = 0;

		tagCardType pCardType = m_GameLogic.GetCarTypeByIndex((tagCardType)i);

		std::map<int32, std::map<tagCardType, int64> >::iterator iter = m_xiazhus.begin();
		for (; iter != m_xiazhus.end(); ++iter)
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
			if (pPlayer == NULL || pPlayer->GetType() != PLAYERTYPE_NORMAL)
				continue;

			int64 pResult = 0;

			std::map<tagCardType, int64>::iterator iter2 = (*iter).second.begin();
			for (; iter2 != (*iter).second.end(); ++iter2)
			{
				int64 pTmpResult = 0;

				if ((*iter2).first == pCardType)
					pTmpResult = (*iter2).second * m_carBeiLv[(*iter2).first];
				else
					pTmpResult = -(*iter2).second;

				pResult += pTmpResult;
			}

			pTempResult += pResult;
		}

		if(pTempResult > pTotalResult)
			pTotalResult = pTempResult;
	}

	return pTotalResult;
}

/// 得到结果值
int CServerLogicFrame::GetGamingResult(void)
{
	int64 pResult, pMax, pMin;
	pResult = pMax = pMin = 0;

	int pcurGamingResult = rand() % 16;

	int pstate = m_g_GameRoom->IsUserWin(&pResult, &pMax, &pMin);

	if (pstate == 2)
	{
		if (m_ZhuangManager.getCurrentZhuang() == -1)
		{
			pcurGamingResult = masterwinorlost(true);
		}
		else
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
			if (pPlayer)
			{
				if (pPlayer->GetType() == PLAYERTYPE_ROBOT)
					pcurGamingResult = masterwinorlost(true);
				else
					pcurGamingResult = masterwinorlost(false);
			}
		}
	}
	else
	{
		int64 pMasterResult = getuserswinandlostresult();

		if (m_ZhuangManager.getCurrentZhuang() == -1)
		{
			if (pMasterResult > pMax - pResult)
				pcurGamingResult = masterwinorlost(true, true, pMax - pResult);
		}
		else
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
			if (pPlayer)
			{
				if (pMasterResult > pMax - pResult)
				{
					if (pPlayer->GetType() == PLAYERTYPE_NORMAL)
					{
						pcurGamingResult = masterwinorlost(false ,true, pMax - pResult);
					}
					else
					{
						pcurGamingResult = masterwinorlost(true ,true, pMax - pResult);
					}
				}
			}
		}
	}

	return pcurGamingResult;
}

/// 游戏结束
void CServerLogicFrame::GameEnd(void)
{
	int64 pTotalResult = 0;
	int64 pOldTotalResult = 0;

	std::map<int32, std::map<tagCardType, int64> > m_userresultlist;
	std::map<tagCardType, int64> m_usertotalresultlist, m_usertotaljettonlist;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		int64 pResult = 0;
		std::string gametip;

		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			int64 pTmpResult = 0;

			if((*iterTwo).first == m_GameLogic.GetCarTypeByIndex(m_curGamingResult))
				pTmpResult = ((*iterTwo).second * m_carBeiLv[(*iterTwo).first]) - (*iterTwo).second;
			else
				pTmpResult = -(*iterTwo).second;

			m_userresultlist[(*iter).first][(*iterTwo).first] = pTmpResult;
			m_usertotalresultlist[(*iterTwo).first] += pTmpResult;
			m_usertotaljettonlist[(*iterTwo).first] += (*iterTwo).second;
			pResult += pTmpResult;

			char str[256];
			sprintf(str,"%s:%ld", getCardTypeName((*iterTwo).first).c_str(), (*iterTwo).second);
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
		if (pPlayer && pResult < 0 && -pResult > pPlayer->GetMoney())
			pResult = -pPlayer->GetMoney();

		/*if (pResult > 0)
		{
			float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
			lAgentRevenue = (int64)((float)pResult * pAgentRate);
			pResult -= lAgentRevenue;
		}*/

		enScoreKind ScoreKind = pResult > 0 ? enScoreKind_Win : enScoreKind_Lost;

		if (GetUserJettonTotal((*iter).first) > 0)
			m_g_GameRoom->WriteUserScore((*iter).first, pResult, lRevenue, ScoreKind, lAgentRevenue,true,0,gametip.c_str());

		if(pPlayer) m_g_GameRoom->UpdateUserScore(pPlayer);
	}

	pTotalResult = -pTotalResult;
	pOldTotalResult = pTotalResult;

	if (m_ZhuangManager.getCurrentZhuang() != -1)
	{
		int64 lRevenue = 0;
		int64 lAgentRevenue = 0;

		if (pTotalResult > 0)
		{
			lRevenue = (int64)((float)pTotalResult * m_g_GameRoom->GetChouShui());
			pTotalResult -= lRevenue;
		}

		Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
		if (pPlayer && pTotalResult < 0 && -pTotalResult > pPlayer->GetMoney())
			pTotalResult = -pPlayer->GetMoney();

		enScoreKind ScoreKind = pTotalResult > 0 ? enScoreKind_Win : enScoreKind_Lost;

		/*if (pTotalResult > 0)
		{
			float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
			lAgentRevenue = (int64)((float)pTotalResult * pAgentRate);
			pTotalResult -= lAgentRevenue;
		}*/

		std::string jettonstr2 = "闲家下注:";
		std::map<tagCardType, int64>::iterator iter = m_usertotaljettonlist.begin();
		for (; iter != m_usertotaljettonlist.end(); ++iter)
		{
			char str[256];
			sprintf(str,"%s:%ld", getCardTypeName((*iter).first).c_str(), (*iter).second);
			jettonstr2 += str;
		}

		m_g_GameRoom->WriteUserScore(m_ZhuangManager.getCurrentZhuang(), pTotalResult, lRevenue, ScoreKind, lAgentRevenue,true,0,jettonstr2.c_str());

		if (pPlayer)
		{
			m_ZhuangManager.SetZhuangTotalWinResult(m_ZhuangManager.getCurrentZhuang(), pOldTotalResult);
			m_g_GameRoom->UpdateUserScore(pPlayer);
		}

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
        out["curGamingResult"] = m_curGamingResult;
        out["OldTotalResult"] = (int)pOldTotalResult;
        out["state"] = 1;

		Json::Value arrayObj;
		std::map<tagCardType, int64>::iterator iter4 = m_usertotalresultlist.begin();
		for (int i=0; iter4 != m_usertotalresultlist.end(); ++iter4,i++)
		{
			arrayObj[i]["CardType"] = (int)(*iter4).first;
			arrayObj[i]["Money"] = -(int)(*iter4).second;
        }
        out["usertotalresultlist"] = arrayObj;

		m_g_GameRoom->SendTableMsg(m_ZhuangManager.getCurrentZhuang(), out);
	}

	int count = m_g_GameRoom->GetPlayerCount();

	for (int i = 0; i<m_g_GameRoom->GetMaxPlayer(); i++)
	{
		if (i == m_ZhuangManager.getCurrentZhuang()) continue;

		Player *pPlayer = m_g_GameRoom->GetPlayer(i);
		if (pPlayer == NULL) continue;

		std::map<int32, std::map<tagCardType, int64> >::iterator iter3 = m_userresultlist.find(pPlayer->GetChairIndex());
		if (iter3 != m_userresultlist.end())
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
            out["curGamingResult"] = m_curGamingResult;
            out["OldTotalResult"] = (int)pOldTotalResult;
            out["state"] = 1;

            Json::Value arrayObj;
            std::map<tagCardType, int64>::iterator iter4 = m_usertotalresultlist.begin();
            for (int i=0; iter4 != m_usertotalresultlist.end(); ++iter4,i++)
            {
                arrayObj[i]["CardType"] = (int)(*iter4).first;
                arrayObj[i]["Money"] = (int)(*iter4).second;
            }
            out["usertotalresultlist"] = arrayObj;

			m_g_GameRoom->SendTableMsg((*iter3).first, out);
		}
		else
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
            out["curGamingResult"] = m_curGamingResult;
            out["OldTotalResult"] = (int)pOldTotalResult;
            out["state"] = 0;

			m_g_GameRoom->SendTableMsg(pPlayer->GetChairIndex(), out);
		}
	}

	//if (m_ZhuangManager.getCurrentZhuang() != -1)
	//{
	//	CMolMessageOut out(IDD_MESSAGE_ROOM);
	//	out.write16(IDD_MESSAGE_ROOM_GAMEEND);
	//	out.write16(m_curGamingResult);
	//	out.write64(pOldTotalResult);
	//	out.write64(0);
	//	m_g_GameRoom->SendTableMsg(m_ZhuangManager.getCurrentZhuang(), out);
	//}

	m_ZhuangManager.OnProcessGameEnd();
}
