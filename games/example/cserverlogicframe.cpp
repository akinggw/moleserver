#include "cserverlogicframe.h"
#include <sstream>

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{
	m_resultCard = 0;
	m_GamePielement = 100;
	m_GameState = GAMESTATE_KONGXIAN;
	m_gamejvcount = 0;
	m_gamisrunning = false;

	for(int i=0;i<5;i++) m_colorrecordcount[i] = 0;

	m_fangkuai = 3.8f;
	m_meihua = 3.8f;
	m_heitao = 4.0f;
	m_hongtao = 4.0f;
	m_wang = 20.0f;

	m_timegamestart = 3;
	m_timexiazhu = 50;
	m_timekaipai = 5;
	m_timejiesuan = 5;
	m_unitmoney = 10;
	m_choushui = 0.0f;

	m_jettonTrad[0] = m_fangkuai;
	m_jettonTrad[1] = m_meihua;
	m_jettonTrad[2] = m_heitao;
	m_jettonTrad[3] = m_hongtao;
	m_jettonTrad[4] = m_wang;

	m_GamePielement = m_unitmoney;
}

/// 析构函数
CServerLogicFrame::~CServerLogicFrame()
{

}

/// 加载游戏配置
void CServerLogicFrame::LoadGameConfig(void)
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

	m_fangkuai = json_object["方块"].asFloat();
	m_meihua = json_object["梅花"].asFloat();
	m_heitao = json_object["黑桃"].asFloat();
	m_hongtao = json_object["红桃"].asFloat();
	m_wang = json_object["王"].asFloat();

	m_timegamestart = json_object["开始时间"].asInt();
	m_timexiazhu = json_object["下注时间"].asInt();
	m_timekaipai = json_object["开牌时间"].asInt();
	m_timejiesuan = json_object["结算时间"].asInt();
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

	Player *pPlayer = m_g_GameRoom->GetPlayer(playerId);

	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_ROOM_CLEARJETTON:
		{
			int64 pMoney[5];

			memset(pMoney,0,sizeof(pMoney));

			Json::Value arraydata = mes["jettons"];
			for(int i=0;i<5;i++)
			{
				pMoney[i] = arraydata[i].asUInt();
			}

			if(pPlayer &&
				m_GameState == GAMESTATE_XIAZHU)
			{
				for(int i=0;i<5;i++)
					m_userjettonresult[pPlayer->GetChairIndex()].clear();

				Json::Value root;
				root["MsgId"] = IDD_MESSAGE_ROOM;
				root["MsgSubId"] = IDD_MESSAGE_ROOM_CLEARJETTON;
				root["gamestate"] = 1;
				root["userid"] = pPlayer->GetID();
				Json::Value arrayObj;
				for(int i=0;i<5;i++)
				{
					arrayObj[i] = (int32)pMoney[i];
				}
				root["jettonmoney"] = arrayObj;

				m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_JETTON:
		{
			int64 pMoney[5];
			int64 pTotalMoney = 0;

			memset(pMoney,0,sizeof(pMoney));

			Json::Value arraydata = mes["jettonmoney"];
			for(int i=0;i<5;i++)
			{
				pMoney[i] = arraydata[i].asUInt();
				pTotalMoney += pMoney[i];
			}

			if(pPlayer &&
				ComputerUserTotalJetton(pPlayer->GetID()) + pTotalMoney <= pPlayer->GetMoney() &&
				m_GameState == GAMESTATE_XIAZHU)
			{
				for(int i=0;i<5;i++)
					m_userjettonresult[pPlayer->GetChairIndex()].jettons[i] += pMoney[i];

				Json::Value root;
				root["MsgId"] = IDD_MESSAGE_ROOM;
				root["MsgSubId"] = IDD_MESSAGE_ROOM_JETTON;
				root["gamestate"] = 1;
				root["userid"] = pPlayer->GetID();
				Json::Value arrayObj;
				for(int i=0;i<5;i++)
				{
					arrayObj[i] = (int32)pMoney[i];
				}
				root["jettonmoney"] = arrayObj;

				m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);
			}
			else
			{
				Json::Value root;
				root["MsgId"] = IDD_MESSAGE_ROOM;
				root["MsgSubId"] = IDD_MESSAGE_ROOM_JETTON;
				root["gamestate"] = 0;
				root["userid"] = pPlayer->GetID();
				Json::Value arrayObj;
				for(int i=0;i<5;i++)
				{
					arrayObj[i] = (int32)pMoney[i];
				}
				root["jettonmoney"] = arrayObj;

				m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);
			}
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

    std::stringstream ss;
    ss << "玩家" << playerId << "进入房间" << m_g_GameRoom->GetID() << "了." << (int)m_cardrecord.size();
	m_g_GameRoom->Room_Log(BASIC,ss.str());

	if(m_gamisrunning == false)
	{
	    LoadGameConfig();
		m_cardrecord.push_back(m_CGameLogic.GetCardByColor(rand()%5));

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_STARTING,3);
		m_gamisrunning=true;
	}

	Player *pPlayer = m_g_GameRoom->GetPlayer(playerId);

	std::map<uint32,tagJettons>::iterator iter = m_userjettonresult.find(pPlayer->GetChairIndex());
	if(iter == m_userjettonresult.end())
		m_userjettonresult[pPlayer->GetChairIndex()].clear();

	Json::Value root;
	root["MsgId"] = IDD_MESSAGE_ROOM;
	root["MsgSubId"] = IDD_MESSAGE_ROOM_ENTERGAME;
	root["gamestate"] = m_GameState;
	root["gamepielement"] = (int32)m_GamePielement;
	root["jvindex"] = m_gamejvcount;
	Json::Value arrayObj;
	for(int i=0;i<5;i++)
	{
		arrayObj[i] = (int)(m_jettonTrad[i]*10.0f);
	}
	root["GamePielement"] = arrayObj;
	Json::Value arrayObj2;
	for(int i=0;i<5;i++)
	{
		arrayObj2[i] = m_colorrecordcount[i];
	}
	root["colorrecordcount"] = arrayObj2;
	Json::Value arrayObj3;
	for(int i=0;i<(int)m_cardrecord.size();i++)
	{
		arrayObj3[i] = m_cardrecord[i];
	}
	root["cardrecourdcount"] = arrayObj3;

	root["timexiazhu"] = m_timexiazhu;
	root["timekaipai"] = m_timekaipai;
	root["timejiesuan"] = m_timejiesuan;

	//root["unitmoney"] = m_unitmoney;
	m_g_GameRoom->SendTableMsg(playerId,root);
}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    std::stringstream ss;
    ss << "玩家" << playerId << "离开房间了.";
	m_g_GameRoom->Room_Log(BASIC,ss.str());
}

/// 处理用户断线消息
void CServerLogicFrame::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    std::stringstream ss;
    ss << "玩家" << playerId << "从房间掉线了.";
	m_g_GameRoom->Room_Log(BASIC,ss.str());
}

/// 处理用户断线重连消息
void CServerLogicFrame::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

    std::stringstream ss;
    ss << "玩家" << playerId << "重回房间了.";
	m_g_GameRoom->Room_Log(BASIC,ss.str());

	Json::Value root;
	root["MsgId"] = IDD_MESSAGE_ROOM;
	root["MsgSubId"] = IDD_MESSAGE_ROOM_REENTERGAME;
	root["gamestate"] = m_GameState;
	root["gamepielement"] = (int32)m_GamePielement;
	root["jvindex"] = m_gamejvcount;
	Json::Value arrayObj;
	for(int i=0;i<5;i++)
	{
		arrayObj[i] = (int)(m_jettonTrad[i]*10.0f);
	}
	root["GamePielement"] = arrayObj;
	Json::Value arrayObj2;
	for(int i=0;i<5;i++)
	{
		arrayObj2[i] = m_colorrecordcount[i];
	}
	root["colorrecordcount"] = arrayObj2;
	Json::Value arrayObj3;
	for(int i=0;i<(int)m_cardrecord.size();i++)
	{
		arrayObj3[i] = m_cardrecord[i];
	}
	root["cardrecourdcount"] = arrayObj3;

	root["timexiazhu"] = m_timexiazhu;
	root["timekaipai"] = m_timekaipai;
	root["timejiesuan"] = m_timejiesuan;

	//root["unitmoney"] = m_unitmoney;
	m_g_GameRoom->SendTableMsg(playerId,root);
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if(timerId == IDD_TIMER_GAME_STARTING && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_STARTING);

		ClearJettonRecord();
		m_resultCard = 0;
		m_GameState = GAMESTATE_XIAZHU;

		m_g_GameRoom->GameStart();

		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_ROOM;
		root["MsgSubId"] = IDD_MESSAGE_ROOM_STARTJETTON;
		root["gamestate"] = m_GameState;
		root["jvindex"] = m_gamejvcount;

		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_XIAZHU, m_timexiazhu);
	}
	else if(timerId == IDD_TIMER_GAME_XIAZHU && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_XIAZHU);

		m_resultCard = GetResultCard();

		m_colorrecordcount[m_CGameLogic.GetCardColor(m_resultCard)] += 1;
		m_GameState = GAMESTATE_KAIPAI;

		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_ROOM;
		root["MsgSubId"] = IDD_MESSAGE_ROOM_OPENCARD;
		root["gamestate"] = m_GameState;
		root["resultcard"] = m_resultCard;

		m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_KAIPAI, m_timekaipai);
	}
	else if(timerId == IDD_TIMER_GAME_KAIPAI && curTimer <= 0)
	{
		m_g_GameRoom->StopTimer(IDD_TIMER_GAME_KAIPAI);

		m_GameState = GAMESTATE_KONGXIAN;

		TradGame();

		m_cardrecord.push_back(m_resultCard);

		if((int)m_cardrecord.size() >= 65)
		{
			m_cardrecord.clear();
			m_gamejvcount+=1;
			std::map<uint8,int>::iterator iter = m_colorrecordcount.begin();
			for(;iter != m_colorrecordcount.end();++iter) (*iter).second = 0;

			m_cardrecord.push_back(m_resultCard);
		}

		m_g_GameRoom->GameEnd();

        m_g_GameRoom->StartTimer(IDD_TIMER_GAME_STARTING, m_timejiesuan);
	}
}

/// 清空所有人下注记录
void CServerLogicFrame::ClearJettonRecord(void)
{
	if(m_userjettonresult.empty())
		return;

	std::map<uint32,tagJettons>::iterator iter = m_userjettonresult.begin();
	for(;iter != m_userjettonresult.end();++iter)
	{
		(*iter).second.clear();
	}
}

/// 结算游戏
void CServerLogicFrame::TradGame(void)
{
	if(m_userjettonresult.empty())
		return;

	std::map<uint32,tagJettons>::iterator iter = m_userjettonresult.begin();
	for(;iter != m_userjettonresult.end();)
	{
		int64 pResult = 0;

		if((*iter).second.getTotal())
		{
			for(int i=0;i<5;i++)
			{
				if(i == m_CGameLogic.GetCardColor(m_resultCard))
				{
					pResult += (int64)((float)(*iter).second.jettons[i] * m_jettonTrad[i]);
					pResult -= (*iter).second.jettons[i];
				}
				else
				{
					pResult -= (*iter).second.jettons[i];
				}
			}

			int64 lRevenue = 0;

			if (pResult > 0)
			{
				lRevenue = (int64)((float)pResult * m_choushui);
				pResult -= lRevenue;
			}

			enScoreKind ScoreKind = pResult > 0 ? enScoreKind_Win : enScoreKind_Lost;
			m_g_GameRoom->WriteUserScore((*iter).first, pResult, lRevenue, ScoreKind);

			(*iter).second.clear();
		}

		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer != NULL)
		{
			int64 pUserMoney,pUserBankMoney;
			pUserMoney = pUserBankMoney = 0;

			pUserMoney = pPlayer->GetMoney();
			pUserBankMoney = pPlayer->GetBankMoney();

			Json::Value root;
			root["MsgId"] = IDD_MESSAGE_ROOM;
			root["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEOVER;
			root["gamestate"] = m_GameState;
			root["resultnum"] = (int32)pResult;
			root["usermoney"] = (int32)pUserMoney;

			m_g_GameRoom->SendTableMsg(pPlayer->GetChairIndex(),root);

			++iter;
		}
		else
		{
			iter = m_userjettonresult.erase(iter);
		}
	}
}

/// 根据玩家下注得到最终牌
uint8 CServerLogicFrame::GetResultCard(void)
{
	int64 pResult = 0;
	int64 pSysMax, pSysMin;
	pSysMax = 0;
	pSysMin = 0;
	int pstate = 0;
	int pCardColor = rand() % 4;

	if (pstate == 2)
	{
		pCardColor = ComputerCardColorFailTotalResult(&pResult);
	}
	else /*if(pstate == 1)*/
	{
		pCardColor = ComputerCardColorTotalResult(pSysMax - pResult);

		if (pCardColor == 4 && rand() % 100 > 5)
			pCardColor = rand() % 4;
	}

	return m_CGameLogic.GetCardByColor(pCardColor);
}

/// 计算某种花色总的下注量
int64 CServerLogicFrame::ComputerCardColorTotalJetton(uint8 cardcolor)
{
	if(m_userjettonresult.empty())
		return 0;

	int64 pTotal = 0;
	std::map<uint32,tagJettons>::iterator iter = m_userjettonresult.begin();
	for(;iter != m_userjettonresult.end();++iter)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT)
			continue;

		pTotal += (*iter).second.jettons[cardcolor] * m_jettonTrad[cardcolor];
	}

	return pTotal;
}

/// 得到指定玩家总的下注
int64 CServerLogicFrame::ComputerUserTotalJetton(uint32 pUserId)
{
	if(m_userjettonresult.empty())
		return 0;

	int64 pTotal = 0;
	std::map<uint32,tagJettons>::iterator iter = m_userjettonresult.find(pUserId);
	if(iter != m_userjettonresult.end())
	{
		for(int i=0;i<5;i++)
			pTotal += (*iter).second.jettons[i];
	}

	return pTotal;
}

/// 得到指定玩家指定花色的下注
int64 CServerLogicFrame::ComputerUserCardColorTotalJetton(uint32 pUserId,uint8 cardcolor)
{
	if(m_userjettonresult.empty())
		return 0;

	int64 pTotal = 0;
	std::map<uint32,tagJettons>::iterator iter = m_userjettonresult.find(pUserId);
	if(iter != m_userjettonresult.end())
	{
		pTotal += (*iter).second.jettons[cardcolor] * m_jettonTrad[cardcolor];
	}

	return pTotal;
}

/// 得到当前最亏的花色
int CServerLogicFrame::ComputerCardColorFailTotalResult(int64 *presult)
{
	int64 pResult[5];

	for(int i=0;i<5;i++)
	{
		pResult[i] = ComputerCardColorTotalJetton(i);
	}

	std::map<int64,int> mTmpVector;

	for(int i=0;i<5;i++)
	{
		int64 pTmpResult = 0;
		for(int k=0;k<5;k++)
		{
			if(k != i)
				pTmpResult += pResult[k];
		}

		int64 tmpMoney = pResult[i]-pTmpResult;

		if(pResult[i] < pTmpResult)
			mTmpVector[tmpMoney] = i;
	}

	if(!mTmpVector.empty())
	{
		if((int)mTmpVector.size() == 1)
		{
			*presult = (*mTmpVector.begin()).first;
			return (*mTmpVector.begin()).second;
		}
		else
		{
			std::map<int64,int>::iterator iterTwo = mTmpVector.begin();

			for(int i=0;i<rand()%(int)mTmpVector.size();i++)
				++iterTwo;

			if(iterTwo != mTmpVector.end())
			{
				*presult = (*iterTwo).first;
				return (*iterTwo).second;
			}
		}
	}

	return rand()%5;
}

/// 得到当前赚钱最少的花色
int CServerLogicFrame::ComputerCardColorTotalResult(int64 presult)
{
	int64 pResult[5];

	for(int i=0;i<5;i++)
	{
		pResult[i] = ComputerCardColorTotalJetton(i);
	}

	std::map<int64,int> mTmpVector;

	for(int i=0;i<5;i++)
	{
		int64 pTmpResult = 0;
		for(int k=0;k<5;k++)
		{
			if(k != i)
				pTmpResult += pResult[k];
		}

		int64 tmpMoney = pResult[i]-pTmpResult;

		if(pResult[i] > pTmpResult &&
			tmpMoney > 0 &&
			tmpMoney <= presult)
			mTmpVector[tmpMoney] = i;
	}

	if(!mTmpVector.empty())
	{
		if((int)mTmpVector.size() == 1)
		{
			return (*mTmpVector.begin()).second;
		}
		else
		{
			std::map<int64,int>::iterator iterTwo = mTmpVector.begin();

			for(int i=0;i<rand()%(int)mTmpVector.size();i++)
				++iterTwo;

			if(iterTwo != mTmpVector.end())
				return (*iterTwo).second;
		}
	}

	return rand()%5;
}
