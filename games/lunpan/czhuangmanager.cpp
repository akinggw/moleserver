#include "czhuangmanager.h"

initialiseSingleton(CZhuangManager);

CZhuangManager::CZhuangManager()
	:m_curZhuangId(-1),m_minShangZhuangMoney(10),m_g_myselfRoom(NULL),m_totalShangZhuangCount(10),m_curShangZhuangCount(0),m_zhuangtotalwinresult(0)
{

}

CZhuangManager::CZhuangManager(Room *proom)
	:m_curZhuangId(-1),m_minShangZhuangMoney(10),m_g_myselfRoom(proom),m_totalShangZhuangCount(10),m_curShangZhuangCount(0),m_zhuangtotalwinresult(0)
{

}

CZhuangManager::~CZhuangManager()
{

}

bool CZhuangManager::addZhuang(int32 pplayerId)
{
	if(findZhuang(pplayerId) || m_curZhuangId == pplayerId) return false;

	Player *pplayer = m_g_myselfRoom->GetPlayer(pplayerId);
	if(pplayer == NULL || pplayer->GetMoney() < m_minShangZhuangMoney)
		return false;

	m_zhuanglist.push_back(pplayerId);

	return true;
}

bool CZhuangManager::decZhuang(int32 pplayerId,bool isGaming)
{
	if(m_curZhuangId == pplayerId)
	{
		if(isGaming) return false;

		m_curZhuangId=-1;
		m_curShangZhuangCount=0;
		m_zhuangtotalwinresult=0;

		return true;
	}

	bool isFinded = false;

	std::vector<int32>::iterator iter = m_zhuanglist.begin();
	for(;iter != m_zhuanglist.end();++iter)
	{
		if((*iter) == pplayerId)
		{
			m_zhuanglist.erase(iter);
			isFinded=true;
			break;
		}
	}

	return isFinded;
}

bool CZhuangManager::findZhuang(int32 pplayerId)
{
	for(int i=0;i<(int)m_zhuanglist.size();i++)
		if(m_zhuanglist[i] == pplayerId)
			return true;

	return false;
}

int CZhuangManager::OnProcessGameStart(void)
{
	if(m_curZhuangId != -1 &&
		m_curShangZhuangCount == m_totalShangZhuangCount)
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_ROOM;
        root["MsgSubId"] = IDD_MESSAGE_ROOM_XIAZHUANG;
        root["state"] = 1;
        m_g_myselfRoom->SendTableMsg(m_curZhuangId,root);
	}

	if(m_curZhuangId == -1 ||
		m_curShangZhuangCount >= m_totalShangZhuangCount)
	{
		if(m_zhuanglist.empty())
		{
			m_curZhuangId=-1;
			m_curShangZhuangCount=0;
			m_zhuangtotalwinresult=0;
			return -1;
		}

		m_curShangZhuangCount = 0;
		m_zhuangtotalwinresult=0;
		m_curZhuangId = (*m_zhuanglist.begin());
		m_zhuanglist.erase(m_zhuanglist.begin());
	}
	else
	{
		m_curShangZhuangCount+=1;
	}

	Player *pplayer = m_g_myselfRoom->GetPlayer(m_curZhuangId);
	if(pplayer == NULL || pplayer->GetMoney() < m_minShangZhuangMoney)
		return -1;

	return m_curZhuangId;
}

void CZhuangManager::OnProcessGameEnd(void)
{
	std::vector<int32>::iterator iter = m_zhuanglist.begin();
	for(;iter != m_zhuanglist.end();)
	{
		Player *pplayer = m_g_myselfRoom->GetPlayer((*iter));
		if(pplayer == NULL ||
			pplayer->GetState() == PLAYERSTATE_LOSTLINE ||
			pplayer->GetMoney() < m_minShangZhuangMoney)
			iter = m_zhuanglist.erase(iter);
		else
			++iter;
	}

	if(m_curZhuangId != -1)
	{
		Player *pplayer = m_g_myselfRoom->GetPlayer(m_curZhuangId);
		if(pplayer == NULL ||
			pplayer->GetState() == PLAYERSTATE_LOSTLINE ||
			pplayer->GetMoney() < m_minShangZhuangMoney)
		{
			m_curZhuangId=-1;
			m_curShangZhuangCount=0;
			m_zhuangtotalwinresult=0;
		}
	}
}

void CZhuangManager::OnProcessUserLeave(int32 pplayerId)
{
	decZhuang(pplayerId,false);
}
