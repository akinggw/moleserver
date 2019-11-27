#include "cserverlogicframe.h"
#include <sstream>

static bool m_gamisrunning = false;

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL),m_GameState(STATE_NULL)
{
	m_timegamestart = m_timexiazhu = m_timekaipai = m_timejiesuan = 0;
	memset(m_curGamingCard,0,sizeof(m_curGamingCard));
	m_curGamingResult=CARTYPE_NULL;
	m_issupercontrol=false;
	m_curJvHao=0;
	m_curMonthDay=-1;
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
	case IDD_MESSAGE_ROOM_XIAZHU:
		{
			tagCardType pCarType = (tagCardType)mes["CardType"].asInt();
			int64 pTempMoney = mes["JettonMoney"].asInt();

			m_xiazhus[playerId][pCarType]+=pTempMoney;

            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_ROOM;
            out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
            out["playerid"] = playerId;
            out["CarType"] = (int)pCarType;
            out["Money"] = (int)pTempMoney;
            m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);
		}
		break;
	case IDD_MESSAGE_ROOM_XIAZHUCLEAR:
		{
			if(m_GameState == STATE_XIAZHU)
			{
                Json::Value out;
                out["MsgId"] = IDD_MESSAGE_ROOM;
                out["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUCLEAR;
                out["playerId"] = playerId;
                out["longtotaljetton"] = (int)GetUserJettonTotal(playerId,CARTYPE_LONG);
                out["hutotaljetton"] = (int)GetUserJettonTotal(playerId,CARTYPE_HU);
                out["hongtotaljetton"] = (int)GetUserJettonTotal(playerId,CARTYPE_HONG);
                m_g_GameRoom->SendTableMsg(playerId,out);

				m_xiazhus[playerId].clear();
			}
		}
		break;
	case IDD_MESSAGE_ROOM_SUPERCONTROL:
		{
			memset(m_curGamingCard,0,sizeof(m_curGamingCard));

			do
			{
				m_curGamingResult=GetTheGamingResult((tagCardType)mes["CardType"].asInt());
			}
			while(m_curGamingCard[0] == 0 || m_curGamingCard[1] == 0);

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
    out["curStrJvHao"] = m_curStrJvHao;

    Json::Value arrayObj;
	for(int i=0;i<(int)m_gamerecords.size();i++)
	{
		arrayObj[i]["cardtype"] = (int)m_gamerecords[i].cardtype;
		arrayObj[i]["strjvhao"] = m_gamerecords[i].strjvhao;
	}
	out["gamerecords"] = arrayObj;

	m_g_GameRoom->SendTableMsg(playerId,out);
}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.find(playerId);
	if(iter != m_xiazhus.end())
	{
	//	if(m_GameState != STATE_NULL)
	//	{
	//		int64 pResult = 0;

	//		std::map<tagCarType,int64>::iterator iterTwo = (*iter).second.begin();
	//		for(;iterTwo != (*iter).second.end();++iterTwo)
	//		{
	//			if(m_curGamingResult != -1)
	//			{
	//				if((*iterTwo).first == m_GameLogic.GetCarTypeByIndex(m_curGamingResult))
	//					pResult += ((*iterTwo).second * m_carBeiLv[(*iterTwo).first]);
	//				else
	//					pResult -= (*iterTwo).second;
	//			}
	//			else
	//				pResult -= (*iterTwo).second;
	//		}

	//		int64 lRevenue = 0;

	//		if(pResult > 0)
	//		{
	//			lRevenue = (int64)((float)pResult * m_g_GameRoom->GetChouShui());
	//			pResult -= lRevenue;
	//		}

	//		enScoreKind ScoreKind = pResult > 0 ? enScoreKind_Win : enScoreKind_Lost;
	//		m_g_GameRoom->WriteUserScore((*iter).first, pResult, lRevenue, ScoreKind);
	//	}

		m_xiazhus.erase(iter);
	}
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
    out["curStrJvHao"] = m_curStrJvHao;

    Json::Value arrayObj;
	for(int i=0;i<(int)m_gamerecords.size();i++)
	{
		arrayObj[i]["cardtype"] = (int)m_gamerecords[i].cardtype;
		arrayObj[i]["strjvhao"] = m_gamerecords[i].strjvhao;
	}
	out["gamerecords"] = arrayObj;

	Json::Value arrayObj2;
	for(int i=0;i<3;i++)
	{
		arrayObj2[i]["CardType"] = (tagCardType)i;
		arrayObj2[i]["RectJettonTotal"] = (int)GetRectJettonTotal2((tagCardType)i);
		arrayObj2[i]["UserJettonTotal"] = (int)GetUserJettonTotal(playerId,(tagCardType)i);
	}
	out["areajettons"] = arrayObj2;

	m_g_GameRoom->SendTableMsg(playerId,out);
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if(timerId == IDD_TIMER_GAME_STARTING && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_STARTING);

		std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.begin();
		for(;iter != m_xiazhus.end();++iter) (*iter).second.clear();
		memset(m_curGamingCard,0,sizeof(m_curGamingCard));
		m_curGamingResult=CARTYPE_NULL;
		m_issupercontrol=false;

		m_g_GameRoom->GameStart();
		m_GameState = STATE_XIAZHU;

		char str[256];
		sprintf(str,"%d%d",(uint32)time(NULL),++m_curJvHao);
		m_curStrJvHao=str;

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_STARTXIAZHU;
        out["GameState"] = m_GameState;
        out["curStrJvHao"] = m_curStrJvHao;
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_XIAZHU,m_timexiazhu);
	}
	else if(timerId == IDD_TIMER_GAME_XIAZHU && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_XIAZHU);

		if(!m_issupercontrol)
		{
			memset(m_curGamingCard,0,sizeof(m_curGamingCard));

			do
			{
				m_curGamingResult=GetGamingResult();
			}
			while(m_curGamingCard[0] == 0 || m_curGamingCard[1] == 0);
		}

		m_GameState=STATE_GAMING;

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_FAPAI;
        out["GameState"] = m_GameState;
        out["curGamingResult"] = (int)m_curGamingResult;
        out["cardone"] = m_curGamingCard[0];
        out["cardtwo"] = m_curGamingCard[1];
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,out);

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_KAIPAI,m_timekaipai);
	}
	else if(timerId == IDD_TIMER_GAME_KAIPAI && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_KAIPAI);

		if((int)m_gamerecords.size() >= 52)
			m_gamerecords.clear();

		m_gamerecords.push_back(tagGameRecord(m_curGamingResult,m_curStrJvHao));

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

    m_carBeiLv[CARTYPE_LONG] = json_object["long"].asFloat();
    m_carBeiLv[CARTYPE_HU] = json_object["hu"].asFloat();
    m_carBeiLv[CARTYPE_HONG] = json_object["he"].asFloat();

	m_timegamestart=json_object['gamestart'].asInt();
	m_timexiazhu=json_object['xiazhu'].asInt();
	m_timekaipai=json_object['kaipai'].asInt();
	m_timejiesuan=json_object['jiesuan'].asInt();
}

/// 统计指定玩家的下注总额
int64 CServerLogicFrame::GetUserJettonTotal(uint32 pUserId,tagCardType pCarType)
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
		{
			if((*iterTwo).second == pCarType)
				pTotal += (*iterTwo).second;
		}
	}

	return pTotal;
}

/// 统计各个区域的下注总和
int64 CServerLogicFrame::GetRectJettonTotal2(tagCardType pCarType)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			if((*iterTwo).first == pCarType)
				pTotal += (*iterTwo).second;
		}
	}

	return pTotal;
}

/// 统计各个区域的下注总输赢
int64 CServerLogicFrame::GetRectJettonTotal(tagCardType pCarType)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();++iter)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT)
			continue;

		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			if((*iterTwo).first == pCarType)
				pTotal += (int64)((float)(*iterTwo).second * m_carBeiLv[(*iterTwo).first]);
			else
			{
				if(pCarType != CARTYPE_HONG)
					pTotal -= (*iterTwo).second;
			}
		}
	}

	return pTotal;
}

/// 统计指定玩家的下注总额
int64 CServerLogicFrame::GetUserJettonTotal(uint32 pUserId)
{
	int64 pTotal = 0;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.find(pUserId);
	if(iter != m_xiazhus.end())
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT)
			return 0;

		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
			pTotal += (*iterTwo).second;
	}

	return pTotal;
}

/// 得到指定值的游戏结果
tagCardType CServerLogicFrame::GetTheGamingResult(tagCardType pCardType)
{
	return m_GameLogic.GetCardByResultType(pCardType,&m_curGamingCard[0],&m_curGamingCard[1]);
}

/// 得到结果值
tagCardType CServerLogicFrame::GetGamingResult(void)
{
	//m_curGamingCard[0] = 0x05;
	//m_curGamingCard[1] = 0x14;
	//return CARTYPE_LONG;
	int64 pResult = 0;
	int64 pMax,pMin;
	pMax=pMin=0;
	int pState = m_g_GameRoom->IsUserWin(&pResult,&pMax,&pMin);

	if(pState == 2)
	{
		std::map<int64,std::vector<tagCardType> > tmpResult;

		for(int i=0;i<3;i++)
			tmpResult[GetRectJettonTotal((tagCardType)i)].push_back((tagCardType)i);

		if(!tmpResult.empty())
			return m_GameLogic.GetCardByResultType((tagCardType)((*(tmpResult.begin())).second[rand()%(int)(*(tmpResult.begin())).second.size()]),&m_curGamingCard[0],&m_curGamingCard[1]);
	}
	else /*if(pState == 1)*/
	{
		std::map<int64,std::vector<tagCardType> > tmpResult;
		std::map<int64,std::vector<tagCardType> > tmpResult2;

		for(int i=0;i<3;i++)
		{
			int64 pTmpResult = GetRectJettonTotal((tagCardType)i);
			tmpResult2[pTmpResult].push_back((tagCardType)i);

			if(pTmpResult > 0 && pTmpResult <= pMax-pResult)
				tmpResult[pTmpResult].push_back((tagCardType)i);
		}

		if(!tmpResult.empty() && rand()%100 < 50)
			return m_GameLogic.GetCardByResultType((tagCardType)((*(tmpResult.begin())).second[rand()%(int)(*(tmpResult.begin())).second.size()]),&m_curGamingCard[0],&m_curGamingCard[1]);

		if(!tmpResult2.empty())
			return m_GameLogic.GetCardByResultType((tagCardType)((*(tmpResult2.begin())).second[rand()%(int)(*(tmpResult2.begin())).second.size()]),&m_curGamingCard[0],&m_curGamingCard[1]);
	}

	return m_GameLogic.GetCardByResultType((tagCardType)(rand()%3),&m_curGamingCard[0],&m_curGamingCard[1]);
}

/// 游戏结束
void CServerLogicFrame::GameEnd(void)
{
	if(m_xiazhus.empty())
		return;

	std::map<int32,std::map<tagCardType,int64> >::iterator iter = m_xiazhus.begin();
	for(;iter != m_xiazhus.end();)
	{
		int64 pResult = 0;
		char pStrGameTip[256];

		sprintf(pStrGameTip,"局号:%s",m_curStrJvHao.c_str());

		std::map<tagCardType,int64>::iterator iterTwo = (*iter).second.begin();
		for(;iterTwo != (*iter).second.end();++iterTwo)
		{
			char tmpStr[256];

			if((*iterTwo).first == CARTYPE_LONG)
				sprintf(tmpStr,"%s龙:%ld",pStrGameTip,(*iterTwo).second);
			else if((*iterTwo).first == CARTYPE_HU)
				sprintf(tmpStr,"%s虎:%ld",pStrGameTip,(*iterTwo).second);
			else
				sprintf(tmpStr,"%s和:%ld",pStrGameTip,(*iterTwo).second);

			if((*iterTwo).first == m_curGamingResult)
			{
				pResult += (int64)((float)(*iterTwo).second * m_carBeiLv[(*iterTwo).first]);
			}
			else
			{
				if(m_curGamingResult != CARTYPE_HONG)
					pResult -= (*iterTwo).second;
			}
		}

		int64 lRevenue = 0;

		if(pResult > 0)
		{
			lRevenue = (int64)((float)pResult * m_g_GameRoom->GetChouShui());
			pResult -= lRevenue;
		}

		enScoreKind ScoreKind = pResult > 0 ? enScoreKind_Win : enScoreKind_Lost;
		m_g_GameRoom->WriteUserScore((*iter).first, pResult, lRevenue, ScoreKind,0,true,0,pStrGameTip);

		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer) m_g_GameRoom->UpdateUserScore(pPlayer);

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_ROOM;
        out["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
        out["GameState"] = m_GameState;
        out["Result"] = (int)pResult;
		m_g_GameRoom->SendTableMsg((*iter).first,out);

		if(pPlayer == NULL || pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
			iter = m_xiazhus.erase(iter);
		else
			++iter;
	}
}
