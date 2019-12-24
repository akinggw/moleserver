#include "cserverlogicframe.h"
#include <sstream>

static bool m_gamisrunning = false;
tagCarType m_curGameResult = CARTYPE_NULL;

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{
	m_timegamestart = m_timexiazhu = m_timekaipai = m_timejiesuan = 0;
	m_curGamingResult = -1;
	m_timecurlost=0;
	m_issupercontrol=false;
	m_isEnableOpenRate=false;
	m_superzhuangstate=0;
	m_supercontroltype=CARTYPE_NULL;
	m_GameState=STATE_NULL;
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
			if(m_GameState == STATE_XIAZHU)
			{
				std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.find(playerId);
				if(iter != m_xiazhus.end())
				{
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_ROOM;
                    out["MsgSubId"] = IDD_MESSAGE_ROOM_CLEARJETTON;
                    out["playerId"] = playerId;
                    Json::Value arrayObj;
					std::map<tagCarType,int64>::iterator iter2 = (*iter).second.begin();
					for(int i=0;iter2 != (*iter).second.end();++iter2,i++)
					{
						arrayObj[i]["JettonArea"] = (*iter2).first;
						arrayObj[i]["JettonMoney"] = (Json::Int64)(*iter2).second;
					}
					out["Jettons"] = arrayObj;
					m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

					(*iter).second.clear();
				}
			}
		}
		break;
	case IDD_MESSAGE_ROOM_XIAZHU:
		{
			tagCarType pCarType = (tagCarType)mes["JettonArea"].asInt();
			int64 pTempMoney = mes["JettonMoney"].asInt64();

			Player *pPlayer = m_g_GameRoom->GetPlayer(playerId);
			if(pPlayer && pPlayer->GetMoney() >= GetUserJettonTotal(playerId) + pTempMoney)
			{
				m_xiazhus[playerId][pCarType]+=pTempMoney;

				if(m_ZhuangManager.getCurrentZhuang() != -1)
				{
					Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
					if(pPlayer)
					{
						int64 pTmpMoney = getusertotalresult();

						if(pTmpMoney > pPlayer->GetMoney())
						{
                            Json::Value out;
                            out["MsgId"] = IDD_MESSAGE_ROOM;
                            out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                            out["playerId"]	= playerId;
                            out["JettonArea"] = (int)pCarType;
                            out["JettonMoney"] = (Json::Int64)pTempMoney;
                            out["state"] = 0;
							m_g_GameRoom->SendTableMsg(playerId,out);

							m_xiazhus[playerId][pCarType]-=pTempMoney;

							return;
						}
					}
				}

                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                out["playerId"]	= playerId;
                out["JettonArea"] = (int)pCarType;
                out["JettonMoney"] = (Json::Int64)pTempMoney;
                out["state"] = 1;
				m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
			}
			else
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                out["playerId"]	= playerId;
                out["JettonArea"] = (int)pCarType;
                out["JettonMoney"] = (Json::Int64)pTempMoney;
                out["state"] = 0;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_GETZHUANGLIST:
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_GETZHUANGLIST;
            out["MinShangZhuangMoney"] = (Json::Int64)m_ZhuangManager.getMinShangZhuangMoney();
            Json::Value arrayObj;
            for(int i=0;i<(int)m_ZhuangManager.getShangZhuangList().size();i++)
            {
                arrayObj[i] = m_ZhuangManager.getShangZhuangList()[i];
            }
            out["ShangZhuangList"] = arrayObj;
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
                out["state"] = 1;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
			else
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
                out["state"] = 0;
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
                out["state"] = 1;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
			else
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
                out["state"] = 0;
				m_g_GameRoom->SendTableMsg(playerId,out);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_SUERCONTROL:
		{
			m_supercontroltype = (tagCarType)mes["JettonArea"].asInt();

			if(m_supercontroltype == CARTYPE_NULL)
			{
				m_superzhuangstate = mes["state"].asInt();
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
    out["ZhuangTotalWinResult"] = (Json::Int64)m_ZhuangManager.getShangZhuangGamingResult();

    Json::Value arrayObj;
    for(int i=0;i<(int)m_gamerecords.size();i++)
    {
        arrayObj[i]["decCardType"] = m_gamerecords[i].decCardType;
        arrayObj[i]["JvHaoStr"] = m_gamerecords[i].JvHaoStr;
    }
    out["GameRecords"] = arrayObj;

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
    out["ZhuangTotalWinResult"] = (Json::Int64)m_ZhuangManager.getShangZhuangGamingResult();

    Json::Value arrayObj;
    for(int i=0;i<(int)m_gamerecords.size();i++)
    {
        arrayObj[i]["decCardType"] = m_gamerecords[i].decCardType;
        arrayObj[i]["JvHaoStr"] = m_gamerecords[i].JvHaoStr;
    }
    out["GameRecords"] = arrayObj;

    Json::Value arrayObj2;
	for(int i=0;i<8;i++)
	{
		arrayObj2[i] = (Json::Int64)GetUserRectJetton(playerId,(tagCarType)i);
	}
	out["UserRectJetton"] = arrayObj2;

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

		std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.begin();
		for(;iter != m_xiazhus.end();++iter) (*iter).second.clear();
		m_curGamingResult = -1;

		m_g_GameRoom->GameStart();
		m_GameState = STATE_XIAZHU;
		m_xiazhus.clear();
		m_issupercontrol=false;
		m_supercontroltype = CARTYPE_NULL;

		m_ZhuangManager.OnProcessGameStart();

		char str[256];
		sprintf(str,"%d%d",(int32)time(NULL),++m_curJvHao);
		m_curStrJvHao=str;

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_STARTXIAZHU;
        out["GameState"] = m_GameState;
        out["curStrJvHao"] = m_curStrJvHao;
        out["CurrentZhuang"] = m_ZhuangManager.getCurrentZhuang();
        out["TotalShangZhuangCount"] = m_ZhuangManager.getTotalShangZhuangCount();
        out["CurShangZhuangCount"] = m_ZhuangManager.GetCurShangZhuangCount();
        out["ZhuangTotalWinResult"] = (Json::Int64)m_ZhuangManager.getShangZhuangGamingResult();
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_XIAZHU,m_timexiazhu);
	}
	else if(timerId == IDD_TIMER_GAME_XIAZHU && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_XIAZHU);

		if(!m_issupercontrol)
		{
			m_curGamingResult = GetGamingResult();
			//OnProcessOpenRate();
		}
		else
		{
			if(m_supercontroltype == CARTYPE_NULL)
			{
				if(m_superzhuangstate == 1)
				{
					m_curGamingResult=getLastMoneyResult2(0);
				}
				else
				{
					if(m_ZhuangManager.getCurrentZhuang() == -1)
					{
						m_curGamingResult=getLastMoneyResult2(-1,true);
					}
					else
					{
						Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
						if(pPlayer)
						{
							m_curGamingResult=getLastMoneyResult2(pPlayer->GetMoney(),true);
						}
					}
				}
			}
			else
			{
				m_curGamingResult = m_GameLogic.GetPostionByCarType(m_supercontroltype);
			}
		}
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

		m_GameState = STATE_NULL;
		GameEnd();

		if((int)m_gamerecords.size() >= 15)
			m_gamerecords.erase(m_gamerecords.begin());

		m_gamerecords.push_back(tagGamingRecord(m_GameLogic.GetCarTypeByIndex(m_curGamingResult),m_curStrJvHao));

		m_ZhuangManager.OnProcessGameEnd();
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

	int m_countshangZhuang = json_object["shangzhuangcount"].asInt();
	m_ZhuangManager.setTotalShangZhuangCount(m_countshangZhuang);
	int64 m_minzhuangmoney = json_object["zhuangminmoney"].asInt();
	m_ZhuangManager.setMinShangZhuangMoney(m_minzhuangmoney);

	m_carBeiLv[CARTYPE_FALALI]=json_object["falali"].asFloat();
	m_carBeiLv[CARTYPE_LANBOJILI]=json_object["lanbojili"].asFloat();
	m_carBeiLv[CARTYPE_MASAILADI]=json_object["masailadi"].asFloat();
	m_carBeiLv[CARTYPE_BAOSHIJIE]=json_object["baoshijie"].asFloat();
	m_carBeiLv[CARTYPE_BENCI]=json_object["benci"].asFloat();
	m_carBeiLv[CARTYPE_BAOMA]=json_object["baoma"].asFloat();
	m_carBeiLv[CARTYPE_AODI]=json_object["aodi"].asFloat();
	m_carBeiLv[CARTYPE_DAZONG]=json_object["dazong"].asFloat();

	m_timegamestart=json_object["gamestart"].asInt();
	m_timexiazhu=json_object["xiazhu"].asInt();
	m_timekaipai=json_object["kaipai"].asInt();
	m_timejiesuan=json_object["jiesuan"].asInt();

	m_ZhuangManager.setGameRoom(m_g_GameRoom);
}

int64 CServerLogicFrame::GetUserRectJetton(uint32 pUserId,tagCarType pCarType)
{
	std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.find(pUserId);
	if(iter != m_xiazhus.end())
	{
		std::map<tagCarType,int64>::iterator iterTwo = (*iter).second.find(pCarType);
		if(iterTwo != (*iter).second.end())
			return (*iterTwo).second;
	}

	return 0;
}

int64 CServerLogicFrame::GetRectJettonTotal2(tagCarType pCarType)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL)
			continue;

		std::map<tagCarType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			if((*iterTwo).first == pCarType)
			{
				pTotal += ((*iterTwo).second * m_carBeiLv[(*iterTwo).first]);
				pTotal -= (*iterTwo).second;
			}
			else
				pTotal -= (*iterTwo).second;
		}
	}

	return pTotal;
}

/// 统计各个区域的下注总输赢
int64 CServerLogicFrame::GetRectJettonTotal(tagCarType pCarType,bool isRobot)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL)
			continue;

		if(isRobot)
		{
			if(pPlayer->GetType() == PLAYERTYPE_NORMAL)
				continue;
		}
		else
		{
			if(pPlayer->GetType() == PLAYERTYPE_ROBOT)
				continue;
		}

		std::map<tagCarType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			if((*iterTwo).first == pCarType)
			{
				pTotal += ((*iterTwo).second * m_carBeiLv[(*iterTwo).first]);
				pTotal -= (*iterTwo).second;
			}
			else
				pTotal -= (*iterTwo).second;
		}
	}

	return pTotal;
}

int64 CServerLogicFrame::getusertotalresultbytype(tagCarType pCarType)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		std::map<tagCarType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			if((*iterTwo).first == pCarType)
			{
				pTotal += ((*iterTwo).second * m_carBeiLv[(*iterTwo).first]);
				pTotal -= (*iterTwo).second;
			}
			else
			{
				pTotal -= (*iterTwo).second;
			}
		}
	}

	return pTotal;
}

int64 CServerLogicFrame::getusertotalresult(void)
{
	int64 pTotal = 0;

	for(int i=0;i<CARTYPE_NULL;i++)
	{
		int64 ptmpTotal = getusertotalresultbytype((tagCarType)i);

		if(ptmpTotal > pTotal)
			pTotal = ptmpTotal;
	}

	return pTotal;
}

/// 统计指定玩家的下注总额
int64 CServerLogicFrame::GetUserJettonTotal(uint32 pUserId)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.find(pUserId);
	if(iter != m_xiazhus.end())
	{
		//Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		//if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT)
		//	return 0;

		std::map<tagCarType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
			pTotal += (*iterTwo).second;
	}

	return pTotal;
}

/// 得到结果值
int CServerLogicFrame::GetGamingResult(void)
{
	int64 pResult = 0;
	int64 pMax,pMin;
	pMax=pMin=0;
	int pState = m_g_GameRoom->IsUserWin(&pResult,&pMax,&pMin);

	if(pState == 2)
	{
		// 如果没有庄家，而且玩家赢的情况下，玩家必须输
		if(m_ZhuangManager.getCurrentZhuang() == -1)
		{
			return getLastMoneyResult(0,false);
		}
		else
		{
			// 如果有庄家，而且庄家是机器人，在玩家赢的情况下，玩家必须输
			Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
			if(pPlayer)
			{
				if(pPlayer->GetType() == PLAYERTYPE_ROBOT)
					return getLastMoneyResult(0,false);
				else
					return getLastMoneyResult(pPlayer->GetMoney(),true,true);
			}
		}
	}
	else
	{
		if(m_ZhuangManager.getCurrentZhuang() == -1)
		{
			return getLastMoneyResult(pMax-pResult,false);
		}
		else
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
			if(pPlayer)
			{
				if(pPlayer->GetType() == PLAYERTYPE_ROBOT)
				{
					int64 pDecMoney = pMax-pResult;
					pDecMoney = pDecMoney > pPlayer->GetMoney() ? pPlayer->GetMoney() : pDecMoney;

					return getLastMoneyResult(pDecMoney,false);
				}
				else
				{
					return getLastRobtolMoneyResult(-(pMax-pResult),pPlayer->GetMoney());
				}
			}
		}
	}

	return rand()%32;
}

int CServerLogicFrame::getLastRobtolMoneyResult(int64 pMoneystart,int64 pMoneyTwo)
{
	std::vector<tagCarType> tmpResult;

	for(int i=0;i<8;i++)
	{
		int64 pTmpResult = GetRectJettonTotal((tagCarType)i,true);

		if(pTmpResult >= pMoneystart && pTmpResult < pMoneyTwo)
			tmpResult.push_back((tagCarType)i);
	}

	if(!tmpResult.empty())
		return m_GameLogic.GetPostionByCarType(tmpResult[rand()%(int)tmpResult.size()]);

	return rand()%32;
}

int CServerLogicFrame::getLastMoneyResult2(int64 pMoney,bool isWinOk)
{
	std::vector<tagCarType> tmpResult;

	for(int i=0;i<8;i++)
	{
		int64 pTmpResult = GetRectJettonTotal2((tagCarType)i);

		if(isWinOk)
		{
			if(pMoney == -1)
			{
				if(pTmpResult > 0)
					tmpResult.push_back((tagCarType)i);
			}
			else
			{
				if(pTmpResult > 0 && pTmpResult <= pMoney)
					tmpResult.push_back((tagCarType)i);
			}
		}
		else
		{
			if(pTmpResult <= pMoney)
				tmpResult.push_back((tagCarType)i);
		}
	}

	if(!tmpResult.empty())
		return m_GameLogic.GetPostionByCarType(tmpResult[rand()%(int)tmpResult.size()]);

	return rand()%32;
}

int CServerLogicFrame::getLastMoneyResult(int64 pMoney,bool isRobot,bool isWinOk)
{
	std::vector<tagCarType> tmpResult;

	for(int i=0;i<8;i++)
	{
		int64 pTmpResult = GetRectJettonTotal((tagCarType)i,isRobot);

		if(isWinOk)
		{
			if(pTmpResult > 0 && pTmpResult <= pMoney)
				tmpResult.push_back((tagCarType)i);
		}
		else
		{
			if(pTmpResult <= pMoney)
				tmpResult.push_back((tagCarType)i);
		}
	}

	if(!tmpResult.empty())
		return m_GameLogic.GetPostionByCarType(tmpResult[rand()%(int)tmpResult.size()]);

	return rand()%32;
}

/// 游戏结束
void CServerLogicFrame::GameEnd(void)
{
	int64 pTotalResult = 0;
	int64 pOldTotalResult = 0;
	std::map<int,int64> pUserResult;

	std::map<int32,std::map<tagCarType,int64> > m_userresultlist;
	std::map<tagCarType,int64> m_usertotalresultlist,m_usertotaljettonlist;

	std::map<int32,std::map<tagCarType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL) continue;

		int64 pResult = 0;
		std::string jettonstr;

		std::map<tagCarType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			int64 pTmpResult = 0;

			if((*iterTwo).first == m_GameLogic.GetCarTypeByIndex(m_curGamingResult))
			{
				pTmpResult = ((*iterTwo).second * m_carBeiLv[(*iterTwo).first]);
				pTmpResult -= (*iterTwo).second;
			}
			else
				pTmpResult = -(*iterTwo).second;

			m_userresultlist[(*iter).first][(*iterTwo).first] = pTmpResult;
			m_usertotalresultlist[(*iterTwo).first] += pTmpResult;
			m_usertotaljettonlist[(*iterTwo).first] += (*iterTwo).second;
			pResult += pTmpResult;

			char str[256];
			sprintf(str,"%s:%ld",m_GameLogic.GetCarTypeName((*iterTwo).first).c_str(),(*iterTwo).second);
			jettonstr+=str;
		}

		pTotalResult += pResult;

		int64 lRevenue = 0;
		int64 lAgentRevenue = 0;

		if(pResult > 0)
		{
			lRevenue = (int64)((float)pResult * m_g_GameRoom->GetChouShui());
			pResult -= lRevenue;
		}

		//if(pResult > 0)
		//{
		//	float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
		//	lAgentRevenue = (int64)((float)pResult * pAgentRate);
		//	pResult -= lAgentRevenue;
		//}

		pUserResult[(*iter).first] = pResult;

	//	Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer && pResult < 0 && -pResult > pPlayer->GetMoney())
			pResult = -pPlayer->GetMoney();

		enScoreKind ScoreKind = pResult > 0 ? enScoreKind_Win : enScoreKind_Lost;
		m_g_GameRoom->WriteUserScore((*iter).first, pResult, lRevenue, ScoreKind,lAgentRevenue,true,0,jettonstr.c_str());

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

		//if(pTotalResult > 0 && pPlayer)
		//{
		//	float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
		//	lAgentRevenue = (int64)((float)pTotalResult * pAgentRate);
		//	pTotalResult -= lAgentRevenue;
		//}

		if(pPlayer && pTotalResult < 0 && -pTotalResult > pPlayer->GetMoney())
			pTotalResult = -pPlayer->GetMoney();

		char jettonstr[256];
		sprintf(jettonstr,"结果:%s",m_GameLogic.GetCarTypeName(m_GameLogic.GetCarTypeByIndex(m_curGamingResult)).c_str());
		std::map<tagCarType,int64>::iterator iter = m_usertotaljettonlist.begin();
		for(;iter != m_usertotaljettonlist.end();++iter)
		{
			char str[256];
			sprintf(str,"%s%s:%ld",jettonstr,m_GameLogic.GetCarTypeName((*iter).first).c_str(),(*iter).second);
		}

		enScoreKind ScoreKind = pTotalResult > 0 ? enScoreKind_Win : enScoreKind_Lost;
		m_g_GameRoom->WriteUserScore(m_ZhuangManager.getCurrentZhuang(), pTotalResult, lRevenue, ScoreKind,lAgentRevenue,true,0,jettonstr);

		if(pPlayer) m_g_GameRoom->UpdateUserScore(pPlayer);

		pOldTotalResult = pTotalResult;

		m_ZhuangManager.setShangZhuangGamingResult(m_ZhuangManager.getShangZhuangGamingResult()+pOldTotalResult);

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
        out["curGamingResult"] = m_GameLogic.GetCarTypeByIndex(m_curGamingResult);
        out["OldTotalResult"] = (Json::Int64)pOldTotalResult;
        out["OldTotalResult2"] = (Json::Int64)-pOldTotalResult;
		m_g_GameRoom->SendTableMsg(m_ZhuangManager.getCurrentZhuang(),out);
	}

	int count = m_g_GameRoom->GetPlayerCount();

	for(int i=0;i<m_g_GameRoom->GetMaxPlayer();i++)
	{
		if(i == m_ZhuangManager.getCurrentZhuang()) continue;

		Player *pPlayer = m_g_GameRoom->GetPlayer(i);
		if(pPlayer == NULL) continue;

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
        out["curGamingResult"] = m_GameLogic.GetCarTypeByIndex(m_curGamingResult);
        out["OldTotalResult"] = (Json::Int64)pOldTotalResult;
        out["OldTotalResult2"] = (Json::Int64)pUserResult[i];
		m_g_GameRoom->SendTableMsg(i,out);
	}
}
