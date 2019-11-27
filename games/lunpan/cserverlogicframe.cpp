#include "cserverlogicframe.h"
#include <sstream>

static bool m_gamisrunning = false;
int64  m_minzhuangmoney = 0;

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL),m_GameState(STATE_NULL)
{
	m_timegamestart = m_timexiazhu = m_timekaipai = m_timejiesuan = 0;
	m_maxtotaljettons=m_maxpeoplejettons=0;
	m_curGamingResult=0;
	m_timecurlost=0;
	m_issupercontrol=false;
	m_minzhuangmoney = 0;
	m_issuperZhuangWinControl = false;
	m_supercontroltype = 0;
	m_curMonthDay=m_curJvHao=0;
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
			if(m_GameState == STATE_XIAZHU)
			{
				YanZhuType pCarType = (YanZhuType)mes["CarType"].asInt();
				int pAreaType = mes["AreaType"].asInt();
				int64 pTempMoney = mes["TempMoney"].asInt();

				int64 pTemJettonMoney = m_UserXiaZhuManager[playerId].getTotalXiaZhu() + pTempMoney;

				Player *pPlayer = m_g_GameRoom->GetPlayer(playerId);
				if(pPlayer == NULL || pPlayer->GetMoney() < pTemJettonMoney)
				{
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_ROOM;
                    root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                    root["playerId"] = playerId;
                    root["CarType"] = (int)pCarType;
                    root["AreaType"] = pAreaType;
                    root["TempMoney"] = (int)pTempMoney;
                    root["state"] = 0;
                    m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

					return;
				}

				if(m_maxtotaljettons > 0 && (GetAllUserJettons() +  pTempMoney) > m_maxtotaljettons)
				{
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_ROOM;
                    root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                    root["playerId"] = playerId;
                    root["CarType"] = (int)pCarType;
                    root["AreaType"] = pAreaType;
                    root["TempMoney"] = (int)pTempMoney;
                    root["state"] = 0;
                    m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

					return;
				}

				if(m_maxpeoplejettons > 0 && pTemJettonMoney > m_maxpeoplejettons)
				{
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_ROOM;
                    root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                    root["playerId"] = playerId;
                    root["CarType"] = (int)pCarType;
                    root["AreaType"] = pAreaType;
                    root["TempMoney"] = (int)pTempMoney;
                    root["state"] = 0;
                    m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

					return;
				}

				if (pPlayer/* && pPlayer->GetMoney() >= getuserjettontotal(playerId) + pTempMoney*/)
				{
					m_UserXiaZhuManager[playerId].addXiaZhu(pCarType, pAreaType, pTempMoney);

					if (m_ZhuangManager.getCurrentZhuang() != -1)
					{
						Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
						if (pPlayer)
						{
							int64 pTmpMoney = -getusertotalresult();

							if (pTmpMoney < 0 && -pTmpMoney > pPlayer->GetMoney())
							{
                                Json::Value root;
                                root["MsgId"] = IDD_MESSAGE_ROOM;
                                root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                                root["playerId"] = playerId;
                                root["CarType"] = (int)pCarType;
                                root["AreaType"] = pAreaType;
                                root["TempMoney"] = (int)pTempMoney;
                                root["state"] = 0;
                                m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

								m_UserXiaZhuManager[playerId].decXiaZhu(pCarType, pAreaType, pTempMoney);

								return;
							}
						}
					}
				}

                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_ROOM;
                root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHU;
                root["playerId"] = playerId;
                root["CarType"] = (int)pCarType;
                root["AreaType"] = pAreaType;
                root["TempMoney"] = (int)pTempMoney;
                root["state"] = 1;
                m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_GETZHUANGLIST:
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_ROOM;
        root["MsgSubId"] = IDD_MESSAGE_ROOM_GETZHUANGLIST;
        root["minzhuangmone"] = (int)m_minzhuangmoney;
        Json::Value arrayObj;
        for(int i=0;i<(int)m_ZhuangManager.getShangZhuangList().size();i++)
        {
            arrayObj[i] = m_ZhuangManager.getShangZhuangList()[i];
        }
        root["ShangZhuangList"] = arrayObj;
        m_g_GameRoom->SendTableMsg(playerId, root);
	}
		break;
	case IDD_MESSAGE_ROOM_SHANGZHUANG:
	{
		if (m_ZhuangManager.addZhuang(playerId))
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_ROOM;
            root["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
            root["state"]=1;
            m_g_GameRoom->SendTableMsg(playerId, root);
		}
		else
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_ROOM;
            root["MsgSubId"] = IDD_MESSAGE_ROOM_SHANGZHUANG;
            root["state"]=0;
            m_g_GameRoom->SendTableMsg(playerId, root);
		}
	}
		break;
	case IDD_MESSAGE_ROOM_XIAZHUANG:
	{
		if (m_ZhuangManager.decZhuang(playerId, m_GameState == STATE_GAMING ? true : false))
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_ROOM;
            root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
            root["state"]=1;
            m_g_GameRoom->SendTableMsg(playerId, root);
		}
		else
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_ROOM;
            root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
            root["state"]=0;
            m_g_GameRoom->SendTableMsg(playerId, root);
		}
	}
		break;
	case IDD_MESSAGE_ROOM_SUPERCONTROL:
		{
			m_supercontroltype = mes["controltype"].asInt();

			if (m_supercontroltype == 0)
				m_curGamingResult = mes["GamingResult"].asInt();
			else
				m_issuperZhuangWinControl = mes["ZhuangWinControl"].asInt() > 0 ? true : false;

			m_issupercontrol=true;

		}
		break;
	case IDD_MESSAGE_ROOM_CHEXIAOJETTON:
		{
			if(m_GameState == STATE_XIAZHU)
			{
				YanZhuType pYanZhuType = (YanZhuType)mes["YanZhuType"].asInt();
				int pTypeIndex = mes["TypeIndex"].asInt();
				int64 pmoney = mes["money"].asInt();

                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_ROOM;
                root["MsgSubId"] = IDD_MESSAGE_ROOM_CHEXIAOJETTON;
                root["playerId"] = playerId;
                root["YanZhuType"] = pYanZhuType;
                root["TypeIndex"] = pTypeIndex;
                root["pmoney"] = (int)pmoney;

                m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

				m_UserXiaZhuManager[playerId].decXiaZhu(pYanZhuType,pTypeIndex,pmoney);
			}
		}
		break;
	case IDD_MESSAGE_ROOM_CLEARJETTONS:
		{
			if(m_GameState == STATE_XIAZHU)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_ROOM;
                root["MsgSubId"] = IDD_MESSAGE_ROOM_CLEARJETTONS;
                root["playerId"] = playerId;
                Json::Value arrayObj;
                int i=0;
				std::map<int,std::map<int,int64> >::iterator iter = m_UserXiaZhuManager[playerId].getXiaZhus().begin();
				for(;iter != m_UserXiaZhuManager[playerId].getXiaZhus().end();++iter)
				{
					std::map<int,int64>::iterator iterTwo = (*iter).second.begin();
					for(;iterTwo != (*iter).second.end();++iterTwo)
					{
						arrayObj[i]["jettonposx"] = (*iter).first;
						arrayObj[i]["jettonposy"] = (*iterTwo).first;
						arrayObj[i]["jettonmoney"] = (int)(*iterTwo).second;
						i+=1;
					}
				}
				root["jettons"] = arrayObj;

				m_g_GameRoom->SendTableMsg(INVALID_CHAIR,root);

				m_UserXiaZhuManager[playerId].clear();
			}
		}
		break;
	case IDD_MESSAGE_ROOM_SUPERUSERCONTROL:
		{
			int ptype = mes["type"].asInt();

			switch(ptype)
			{
			case 0:
				{
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_ROOM;
                    root["MsgSubId"] = IDD_MESSAGE_ROOM_SUPERUSERCONTROL;
                    root["ctlaccount"] = m_SuperUserControl.account;
                    root["ctlmoney"] = (int)m_SuperUserControl.controlmoney;
                    m_g_GameRoom->SendTableMsg(playerId,root);
				}
				break;
			case 1:
				{
					memset(&m_SuperUserControl,0,sizeof(m_SuperUserControl));
					m_SuperUserControl.userid=-1;
				}
				break;
			case 2:
				{
					strncpy(m_SuperUserControl.account,mes["account"].asString().c_str(),256);
					m_SuperUserControl.controlmoney = mes["money"].asInt();
				}
				break;
			default:
				break;
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

	if(m_gamisrunning == false)
	{
		LoadServerConfig();

		m_g_GameRoom->StartTimer(IDD_TIMER_GAME_STARTING,m_timegamestart);
		m_gamisrunning=true;
	}

	m_UserXiaZhuManager[playerId].clear();

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_ROOM;
    root["MsgSubId"] = IDD_MESSAGE_ROOM_ENTERGAME;
    root["GameState"] = m_GameState;
    root["timegamestart"] = m_timegamestart;
    root["timexiazhu"] = m_timexiazhu;
    root["timekaipai"] = m_timekaipai;
    root["timejiesuan"] = m_timejiesuan;
    Json::Value arrayObj;
    for(int i=0;i<(int)m_gamerecords.size();i++)
    {
        arrayObj[i] = (int)m_gamerecords[i].decGamingResult;
    }
    root["gamerecords"] = arrayObj;
    root["curStrJvHao"] = m_curStrJvHao;
    root["timecurlost"] = m_timecurlost;
    root["GamePielement"] = m_g_GameRoom->GetGamePielement();
    root["CurrentZhuang"] = m_ZhuangManager.getCurrentZhuang();
    root["TotalShangZhuangCount"] = m_ZhuangManager.getTotalShangZhuangCount();
    root["CurShangZhuangCount"] = m_ZhuangManager.GetCurShangZhuangCount();
    root["ZhuangTotalWinResult"] = (int)m_ZhuangManager.GetZhuangTotalWinResult();

    m_g_GameRoom->SendTableMsg(playerId,root);
}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	std::map<int,CXiaZhuManager>::iterator iter = m_UserXiaZhuManager.find(playerId);
	if(iter != m_UserXiaZhuManager.end())
		m_UserXiaZhuManager.erase(iter);

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

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_ROOM;
    root["MsgSubId"] = IDD_MESSAGE_ROOM_REENTERGAME;
    root["GameState"] = m_GameState;
    root["timegamestart"] = m_timegamestart;
    root["timexiazhu"] = m_timexiazhu;
    root["timekaipai"] = m_timekaipai;
    root["timejiesuan"] = m_timejiesuan;
    Json::Value arrayObj;
    for(int i=0;i<(int)m_gamerecords.size();i++)
    {
        arrayObj[i] = (int)m_gamerecords[i].decGamingResult;
    }
    root["gamerecords"] = arrayObj;
    root["curStrJvHao"] = m_curStrJvHao;
    root["timecurlost"] = m_timecurlost;
    root["TotalXiaZhu"] = (int)m_UserXiaZhuManager[playerId].getTotalXiaZhu();
    root["AllUserJettons"] = (int)GetAllUserJettons();
    root["GamePielement"] = m_g_GameRoom->GetGamePielement();
    root["CurrentZhuang"] = m_ZhuangManager.getCurrentZhuang();
    root["TotalShangZhuangCount"] = m_ZhuangManager.getTotalShangZhuangCount();
    root["CurShangZhuangCount"] = m_ZhuangManager.GetCurShangZhuangCount();
    root["ZhuangTotalWinResult"] = (int)m_ZhuangManager.GetZhuangTotalWinResult();

	m_g_GameRoom->SendTableMsg(playerId,root);
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

}

int64 CServerLogicFrame::GetAllUserJettons(void)
{
	int64 pTotal = 0;

	std::map<int,CXiaZhuManager>::iterator iter =  m_UserXiaZhuManager.begin();
	for(;iter != m_UserXiaZhuManager.end();++iter)
		pTotal += (*iter).second.getTotalXiaZhu();

	return pTotal;
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

	int m_countshangZhuang = json_object['shangzhuangcount'].asInt();
	m_ZhuangManager.setTotalShangZhuangCount(m_countshangZhuang);
	m_minzhuangmoney = json_object['zhuangminmoney'].asInt();
	m_ZhuangManager.setMinShangZhuangMoney(m_minzhuangmoney);

	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_ZHIJIEZHU,json_object['zhijiezhu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_FENZHU,json_object['fenzhu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_JIEZHU,json_object['jiezhu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_SANSHU,json_object['sanshu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_JIAOZHU,json_object['jiaozhu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_SIGEHAOMA,json_object['sigehaoma'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_XIANZHU,json_object['xianzhu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_LIEZHU,json_object['liezhu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_XIAZHUYIDASHU,json_object['xiazhuyidashu'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_HONGHEI,json_object['honghei'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_DANSHUANG,json_object['danshuang'].asInt());
	m_BeiLvXiaZhuManager.setXiaZhuBeilv(TYPE_DAXIAO,json_object['daxiao'].asInt());

	m_maxtotaljettons=json_object['maxtotaljettons'].asInt();
	m_maxpeoplejettons=json_object['maxpeoplejettons'].asInt();

	m_timegamestart=json_object['gamestart'].asInt();
	m_timexiazhu=json_object['xiazhu'].asInt();
	m_timekaipai=json_object['kaipai'].asInt();
	m_timejiesuan=json_object['jiesuan'].asInt();
}

int64 CServerLogicFrame::getusertotalresult(void)
{
	int64 pTempDecMoney = 0;

	for (int i = 0; i < 37; i++)
	{
		m_curGamingResult = i;

		int64 ptempmoney = getWinMoney();

		if (ptempmoney > pTempDecMoney)
			pTempDecMoney = ptempmoney;
	}

	return pTempDecMoney;
}

int CServerLogicFrame::GetGamingResultByUserId(int userid,int64 pMoney)
{
	std::vector<int> tmpResults2;
	int64 lRevenue = 0;

	for(int i=0;i<37;i++)
	{
		m_curGamingResult = i;

		int64 pResultMoney = getWinMoneyByUserId(userid,&lRevenue);

		//if(pResultMoney <= pMoney)
		//{
			if(pMoney > 0 && pResultMoney > 0 && pResultMoney <= pMoney)
				tmpResults2.push_back(i);
			else if(pMoney < 0 && pResultMoney < 0)
				tmpResults2.push_back(i);
		//}
	}

	if(!tmpResults2.empty())
		return tmpResults2[rand()%(int)tmpResults2.size()];

	return rand()%37;
}

int CServerLogicFrame::masterwinorlost(bool iswin, int64 pMaxMoney)
{
	std::vector<int> tmpResults;

	for (int i = 0; i < 37; i++)
	{
		m_curGamingResult = i;

		int64 ptempmoney = getWinMoney();

		if (iswin)
		{
			if (ptempmoney < 0)
				tmpResults.push_back(i);
		}
		else
		{
			if (ptempmoney > 0)
				tmpResults.push_back(i);
		}
	}

	if (!tmpResults.empty())
		return tmpResults[rand() % (int)tmpResults.size()];

	return rand() % 37;
}

/// 得到结果值
void CServerLogicFrame::GetGamingResult(void)
{
	int64 pResult, pMax, pMin;
	pResult = pMax = pMin = 0;

	m_curGamingResult = rand() % 37;

	int pstate = m_g_GameRoom->IsUserWin(&pResult, &pMax, &pMin);

	if (pstate == 2)
	{
		if (m_ZhuangManager.getCurrentZhuang() == -1)
		{
			m_curGamingResult = masterwinorlost(true);
		}
		else
		{
			Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
			if (pPlayer)
			{
				if (pPlayer->GetType() == PLAYERTYPE_ROBOT)
					m_curGamingResult = masterwinorlost(true);
				else
					m_curGamingResult = masterwinorlost(false);
			}
		}
	}
	else
	{
		int64 pMasterResult = getWinMoney();

		if (m_ZhuangManager.getCurrentZhuang() == -1)
		{
			if (pMasterResult > pMax - pResult)
				m_curGamingResult = masterwinorlost(true);
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
						m_curGamingResult = masterwinorlost(false);
					}
					else
					{
						m_curGamingResult = masterwinorlost(true);
					}
				}
			}
		}
	}
}


/// 计算所有玩家的输赢值
int64 CServerLogicFrame::getWinMoney(void)
{
	int64 pTotal = 0;
	int64 lRevenue = 0;

	std::map<int,CXiaZhuManager>::iterator iter = m_UserXiaZhuManager.begin();
	for(;iter != m_UserXiaZhuManager.end();++iter)
	{
		//Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		//if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT)
		//	continue;

		pTotal += getWinMoneyByUserId((*iter).first,&lRevenue);
	}

	return pTotal;
}

//计算指定玩家赢了多少钱
int64 CServerLogicFrame::getWinMoneyByUserId(int userid,int64 *lRevenue)
{
	int64 pTotal = 0;

	std::map<int,std::map<int,int64> > pXiaZhuManager = m_UserXiaZhuManager[userid].getXiaZhus();
	std::map<int,std::map<int,int64> >::iterator iter = pXiaZhuManager.begin();
	for(;iter != pXiaZhuManager.end();++iter)
	{
		pTotal += getWinMoneyByUserIdAndArea(userid,(YanZhuType)(*iter).first,lRevenue);
	}

	return pTotal;
}

//得到指定玩家指定区域的输赢值
int64 CServerLogicFrame::getWinMoneyByUserIdAndArea(int userid,YanZhuType type,int64 *lRevenue)
{
	int64 pTotal = 0;

	std::map<int,std::map<int,int64> >::iterator iter = m_UserXiaZhuManager[userid].getXiaZhus().find((int)type);
	if(iter != m_UserXiaZhuManager[userid].getXiaZhus().end())
	{
		std::map<int,int64>::iterator iter2 = (*iter).second.begin();
		for(;iter2 != (*iter).second.end();++iter2)
		{
			int paaary[24];
			int pcount = getXiaZhuNumbers((YanZhuType)(*iter).first,(*iter2).first,paaary);

			bool isfinded = false;

			for(int i=0;i<pcount;i++)
			{
				if(paaary[i] == m_curGamingResult)
				{
					isfinded = true;
					break;
				}
			}

			if(isfinded)
			{
				int64 lUserRevenue = 0;
				int64 pTempMoney = ((*iter2).second * m_BeiLvXiaZhuManager.getXiaZhuBeilv((YanZhuType)(*iter).first));

				if(pTempMoney>0)
				{
					lUserRevenue=(int64)((float)pTempMoney * m_g_GameRoom->GetChouShui());
					pTempMoney -= lUserRevenue;

					*lRevenue += lUserRevenue;
				}

				pTotal += pTempMoney;
			}
			else
			{
				pTotal -= (*iter2).second;
			}
		}
	}

	return pTotal;
}

/// 游戏结束
void CServerLogicFrame::GameEnd(void)
{
	int64 pTotalResult = 0;
	int64 pOldTotalResult = 0;

	std::map<int32, int64> m_userresultlist;

	std::map<int,CXiaZhuManager>::iterator iter = m_UserXiaZhuManager.begin();
	for (; iter != m_UserXiaZhuManager.end(); ++iter)
	{
		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer == NULL) continue;

		int64 pResult = 0;
		int64 lRevenue = 0;
		int64 lAgentRevenue = 0;

		pResult = getWinMoneyByUserId((*iter).first,&lRevenue);

		m_userresultlist[(*iter).first] = pResult;
		pTotalResult += pResult;

//		Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if (pPlayer && pResult < 0 && -pResult > pPlayer->GetMoney())
			pResult = -pPlayer->GetMoney();

		//if (pResult > 0)
		//{
		//	float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
		//	lAgentRevenue = (int64)((float)pResult * pAgentRate);
		//	pResult -= lAgentRevenue;
		//}
		//else if(pResult < 0)
		//{
		//	float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
		//	lAgentRevenue = (int64)((float)(-pResult) * pAgentRate);
		//}

		char str[256];
		sprintf(str,"%s:%ld",m_curStrJvHao.c_str(),(*iter).second.getTotalXiaZhu());

		enScoreKind ScoreKind = pResult > 0 ? enScoreKind_Win : enScoreKind_Lost;
		if ((*iter).second.getTotalXiaZhu() > 0)
			m_g_GameRoom->WriteUserScore((*iter).first, pResult, lRevenue, ScoreKind,lAgentRevenue,true,(*iter).second.getTotalXiaZhu(),str);

		//Player *pPlayer = m_g_GameRoom->GetPlayer((*iter).first);
		if(pPlayer) m_g_GameRoom->UpdateUserScore(pPlayer);
	}

	pTotalResult = -pTotalResult;
	pOldTotalResult = pTotalResult;

	if (m_ZhuangManager.getCurrentZhuang() != -1)
	{
		int64 lRevenue = 0;
		int64 lAgentRevenue = 0;

		Player *pPlayer = m_g_GameRoom->GetPlayer(m_ZhuangManager.getCurrentZhuang());
		if (pPlayer && pTotalResult < 0 && -pTotalResult > pPlayer->GetMoney())
			pTotalResult = -pPlayer->GetMoney();

		enScoreKind ScoreKind = pTotalResult > 0 ? enScoreKind_Win : enScoreKind_Lost;

		//if (pTotalResult > 0)
		//{
		//	float pAgentRate = (float)m_g_GameRoom->GetPlayerAgentMoneyRate(pPlayer) / 100.0f;
		//	lAgentRevenue = (int64)((float)pTotalResult * pAgentRate);
		//	pTotalResult -= lAgentRevenue;
		//}

		char str[256];
		sprintf(str, "%s:%ld", m_curStrJvHao.c_str(), GetAllUserJettons());

		if (GetAllUserJettons() > 0)
			m_g_GameRoom->WriteUserScore(m_ZhuangManager.getCurrentZhuang(), pTotalResult, lRevenue, ScoreKind,  lAgentRevenue,str);

		if (pPlayer)
		{
			m_ZhuangManager.SetZhuangTotalWinResult(m_ZhuangManager.getCurrentZhuang(), pOldTotalResult);
			m_g_GameRoom->UpdateUserScore(pPlayer);
		}

        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_ROOM;
        root["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
        root["GameState"] = m_GameState;
        root["GamingResult"] = m_curGamingResult;
        root["OldTotalResult"] = (int)pOldTotalResult;
        root["OldTotalResult2"] = (int)pOldTotalResult;

		m_g_GameRoom->SendTableMsg(m_ZhuangManager.getCurrentZhuang(), root);
	}

	for (int i = 0; i < m_g_GameRoom->GetMaxPlayer(); i++)
	{
		if (i == m_ZhuangManager.getCurrentZhuang()) continue;

		Player *pPlayer = m_g_GameRoom->GetPlayer(i);
		if (pPlayer == NULL) continue;

		std::map<int32, int64>::iterator iter3 = m_userresultlist.find(pPlayer->GetChairIndex());
		if (iter3 != m_userresultlist.end())
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_ROOM;
            root["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
            root["GameState"] = m_GameState;
            root["GamingResult"] = m_curGamingResult;
            root["OldTotalResult"] = (int)pOldTotalResult;
            root["OldTotalResult2"] = (int)(*iter3).second;

			m_g_GameRoom->SendTableMsg((*iter3).first, root);
		}
		else
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_ROOM;
            root["MsgSubId"] = IDD_MESSAGE_ROOM_GAMEEND;
            root["GameState"] = m_GameState;
            root["GamingResult"] = m_curGamingResult;
            root["OldTotalResult"] = (int)pOldTotalResult;
            root["OldTotalResult2"] = 0;

			m_g_GameRoom->SendTableMsg(pPlayer->GetChairIndex(), root);
		}
	}

	m_ZhuangManager.OnProcessGameEnd();
}
