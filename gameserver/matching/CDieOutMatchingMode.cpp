#include "CDieOutMatchingMode.h"

#include <algorithm>
#include <functional>
#include <time.h>

#include "../GameFrameManager.h"
#include "../gameframe/PlayerManager.h"
#include "../gameframe/RoomManager.h"

bool cmpList(const CPlayerPtr& x,const CPlayerPtr& y) { return x->GetMatchResult() > y->GetMatchResult(); }

class MatchCase {
public:
	MatchCase(CPlayer* n) : num(n) {}
	bool operator()(CPlayer* n) {
		return n == num;
	}
private:
	CPlayer* num;
};

/// 构造函数
CDieOutMatchingMode::CDieOutMatchingMode(matchMode pmatchMode)
	: BasicMatchingMode(pmatchMode),m_isAutoMatchingPlayerCount(false)
{

}

/**
 * 析构函数
 */
CDieOutMatchingMode::~CDieOutMatchingMode()
{

}

/// 清除所有数据
void CDieOutMatchingMode::Clear(void)
{
	std::vector<MatchStruct>::iterator iter = m_tableframePlayerList.begin();
	for(;iter != m_tableframePlayerList.end();++iter)
	{
		if((*iter).playerlist.empty()) continue;
		(*iter).playerlist.clear();
	}
	m_tableframePlayerList.clear();

	std::vector<std::vector<CRoom*> >::iterator iterTwo = m_tableframeroomlist.begin();
	for(;iterTwo != m_tableframeroomlist.end();++iterTwo)
	{
		if((*iterTwo).empty()) continue;

		(*iterTwo).clear();
	}
	m_tableframeroomlist.clear();
}

/// 检测指定的玩家是否在比赛中
bool CDieOutMatchingMode::IsExist(CPlayer *pPlayer)
{
	if(pPlayer == NULL || m_tableframePlayerList.empty() || pPlayer->GetMatchRoomIndex() < 0) return false;

	bool isok = false;

	LockTableFramePlayerList();
	MatchStruct *pMatchStruct = &m_tableframePlayerList[pPlayer->GetMatchRoomIndex()];
	if(pMatchStruct != NULL && !pMatchStruct->playerlist.empty())
	{
		//if(std::binary_search(pMatchStruct->playerlist.begin(),pMatchStruct->playerlist.end(),pPlayer))
		//	isok = true;
		std::vector<CPlayer*>::iterator iterTwo = std::find_if(pMatchStruct->playerlist.begin(),pMatchStruct->playerlist.end(),MatchCase(pPlayer));
		if(iterTwo != pMatchStruct->playerlist.end())
			isok = true;
	}
	UnLockTableFramePlayerList();

	return isok;
}

/// 从比赛场中删除玩家
bool CDieOutMatchingMode::DeletePlayer(CPlayer *pPlayer,bool isMatching)
{
	if(pPlayer == NULL || m_tableframePlayerList.empty() || pPlayer->GetMatchRoomIndex() < 0) return false;

	LockTableFramePlayerList();
	MatchStruct *pMatchStruct = &m_tableframePlayerList[pPlayer->GetMatchRoomIndex()];
	if(pMatchStruct != NULL && !pMatchStruct->playerlist.empty())
	{
		if(isMatching && pMatchStruct->isMatching)
		{
			UnLockTableFramePlayerList();
			return false;
		}

		std::vector<CPlayer*>::iterator iterTwo = std::find_if(pMatchStruct->playerlist.begin(),pMatchStruct->playerlist.end(),MatchCase(pPlayer));
		if(iterTwo != pMatchStruct->playerlist.end())
		{
			pPlayer->SetMatchRoomIndex(-1);

			if(pPlayer->GetState() != PLAYERSTATE_LOSTLINE)
				pPlayer->SetState(PLAYERSTATE_NORAML);

			pMatchStruct->playerlist.erase(iterTwo);

			// 如果这个场一个人都没有，就删掉这个场
			if(pMatchStruct->playerlist.empty())
				pMatchStruct->clear();

			UnLockTableFramePlayerList();
			return true;
		}
	}
	UnLockTableFramePlayerList();

	return false;
}

/// 添加比赛用的桌子
bool CDieOutMatchingMode::AddRoom(CRoom *pRoom)
{
	if(pRoom == NULL || m_ServerSet.m_MatchingMaxPalyer <= 0) return false;

	bool isOk=false;

	LockTableFramePlayerList();
	std::vector<std::vector<CRoom*> >::iterator iter = m_tableframeroomlist.begin();
	for(;iter != m_tableframeroomlist.end();++iter)
	{
		if((int)(*iter).size() < m_ServerSet.m_MatchingMaxPalyer/m_ServerSet.PlayerCount)
		{
			(*iter).push_back(pRoom);
			isOk = true;
			break;
		}
	}

	if(!isOk)
	{
		std::vector<CRoom*> tempList;
		tempList.push_back(pRoom);

		m_tableframeroomlist.push_back(tempList);
	}
	UnLockTableFramePlayerList();

	return true;
}

/// 根据比赛场索引得到比赛场
MatchStruct* CDieOutMatchingMode::GetMatchStructById(int index)
{
	if(m_tableframePlayerList.empty()) return NULL;

	LockTableFramePlayerList();
	for(int i=0;i<(int)m_tableframePlayerList.size();i++)
	{
		if(m_tableframePlayerList[i].roomIndex == index)
		{
			UnLockTableFramePlayerList();
			return &m_tableframePlayerList[i];
		}
	}
	UnLockTableFramePlayerList();

	return NULL;
}

/// 添加玩家到比赛场中
bool CDieOutMatchingMode::AddPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL || m_ServerSet.m_MatchingMaxPalyer <= 0) return false;

	// 如果存在就不加入了
	if(IsExist(pPlayer)) return false;

	bool isOk=false;

	LockTableFramePlayerList();

	int pRoomIndex = GetEmptyMatchingRoom();
	if(pRoomIndex < 0)
	{
		UnLockTableFramePlayerList();
		return false;
	}

	std::map<int,int,std::greater<int> > pCurTableList;

	for(int i=0;i<(int)m_tableframePlayerList.size();i++)
	{
		int roomPlayerCount = (int)m_tableframePlayerList[i].playerlist.size();

		if(m_tableframePlayerList[i].isMatching == false &&
			//roomPlayerCount > 0 &&
			roomPlayerCount < m_ServerSet.m_MatchingMaxPalyer)
		{
			pCurTableList[roomPlayerCount] = i;
		}
	}

	if(!pCurTableList.empty())
	{
		int tableIndex = (*pCurTableList.begin()).second;

		int pMatchCount = (m_ServerSet.m_MatchingType == MATCHMODE_GROUPSMATCH) ? m_ServerSet.m_MatchingGroupCount : m_ServerSet.m_MatchingMaxPalyer/m_ServerSet.PlayerCount;

		if(m_tableframePlayerList[tableIndex].playerlist.empty())
		{
			m_tableframePlayerList[tableIndex].isMatching = false;
			m_tableframePlayerList[tableIndex].isEnable = true;
			m_tableframePlayerList[tableIndex].matchendcount = 0;
			m_tableframePlayerList[tableIndex].matchcount = pMatchCount;
			m_tableframePlayerList[tableIndex].roomIndex = pRoomIndex;
		}

		m_tableframePlayerList[tableIndex].playerlist.push_back(pPlayer);

		// 加入新的玩家后，先对玩家进行排序，以便于后面好用二分法进行搜索
		//std::sort(m_tableframePlayerList[tableIndex].playerlist.begin(),m_tableframePlayerList[tableIndex].playerlist.end());

		pPlayer->SetMatchRoomIndex(tableIndex);
		isOk = true;

		//// 发送当前排队人数
		//CMolMessageOut out(IDD_MESSAGE_FRAME);
		//out.write16(IDD_MESSAGE_FRAME_MATCH);
		//out.write16(IDD_MESSAGE_FRAME_MATCH_WAITINGCOUNT);
		//out.write16((int)m_tableframePlayerList[tableIndex].playerlist.size());

		//Send(pPlayer->GetConnectID(),out);
	}

	if(!isOk)
	{
		std::vector<CPlayer*> tempList;
		tempList.push_back(pPlayer);

		int pMatchCount = (m_ServerSet.m_MatchingType == MATCHMODE_GROUPSMATCH) ? m_ServerSet.m_MatchingGroupCount : m_ServerSet.m_MatchingMaxPalyer/m_ServerSet.PlayerCount;

		MatchStruct pMatchStruct = MatchStruct(false,
			tempList,
			pMatchCount,
			pRoomIndex,
			true,
			0);

		m_tableframePlayerList.push_back(pMatchStruct);
		pPlayer->SetMatchRoomIndex((int)m_tableframePlayerList.size()-1);

		//// 发送当前排队人数
		//CMolMessageOut out(IDD_MESSAGE_FRAME);
		//out.write16(IDD_MESSAGE_FRAME_MATCH);
		//out.write16(IDD_MESSAGE_FRAME_MATCH_WAITINGCOUNT);
		//out.write16((int)tempList.size());

		//Send(pPlayer->GetConnectID(),out);
	}
	UnLockTableFramePlayerList();

	// 设置状态为排队状态
	//pPlayer->SetState(PLAYERSTATE_QUEUE);

	return true;
}

/// 得到一个空的比赛房间
int CDieOutMatchingMode::GetEmptyMatchingRoom(void)
{
	int pRoomIndex = -1;

	if(m_tableframeroomlist.empty()) return pRoomIndex;

	LockTableFramePlayerList();
	for(int i=0;i<(int)m_tableframeroomlist.size();i++)
	{
		bool isok=true;

		for(int k=0;k<(int)m_tableframeroomlist[i].size();k++)
		{
			if(m_tableframeroomlist[i][k] == NULL ||
				m_tableframeroomlist[i][k]->IsEmpty() == false ||
				m_tableframeroomlist[i][k]->GetRoomState() == ROOMSTATE_GAMING)
			{
				isok=false;
				break;
			}
		}

		if(isok)
		{
			// 然后再检查这个比赛场是否满了，如果人满了，就不能进入这个比赛场了
			MatchStruct *pMatchStruct = GetMatchStructById(i);
			if(pMatchStruct == NULL || (int)pMatchStruct->playerlist.size() < m_ServerSet.m_MatchingMaxPalyer)
			{
				pRoomIndex = i;
				break;
			}
		}
	}
	UnLockTableFramePlayerList();

	return pRoomIndex;
}

/// 得到整个比赛人数
int CDieOutMatchingMode::GetMatchPlayerCount(void)
{
	if(m_tableframePlayerList.empty()) return 0;

	int count = 0;

	LockTableFramePlayerList();
	std::vector<MatchStruct>::iterator iter = m_tableframePlayerList.begin();
	for(;iter != m_tableframePlayerList.end();++iter)
	{
		if((*iter).isMatching)
			count+=m_ServerSet.m_MatchingMaxPalyer;
		else
			count+=(int)(*iter).playerlist.size();
	}
	UnLockTableFramePlayerList();

	return count;
}

/// 获取指定玩家比赛中排名
void CDieOutMatchingMode::GetPlayerRanking(CPlayer *pPlayer)
{
	if(pPlayer == NULL || m_tableframePlayerList.empty() || pPlayer->GetMatchRoomIndex() < 0) return;

	LockTableFramePlayerList();
	MatchStruct *pMatchStruct = &m_tableframePlayerList[pPlayer->GetMatchRoomIndex()];
	if(pMatchStruct != NULL && !pMatchStruct->playerlist.empty())
	{
		// 检测这一桌是否还有其它人在比赛中
		bool isMatching = false;
		for(int i=0;i<(int)pMatchStruct->playerlist.size();i++)
		{
			if(pMatchStruct->playerlist[i]->GetMatchCount() > 0)
			{
				isMatching = true;
				break;
			}
		}

		//if(std::binary_search(pMatchStruct->playerlist.begin(),pMatchStruct->playerlist.end(),pPlayer))
		std::vector<CPlayer*>::iterator iterTwo = std::find_if(pMatchStruct->playerlist.begin(),pMatchStruct->playerlist.end(),MatchCase(pPlayer));
		if(iterTwo != pMatchStruct->playerlist.end())
		{
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_FRAME;
            out["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
            out["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_GETRAKING;
            out["State"] = (pMatchStruct->matchcount > 0 && (*iterTwo)->GetMatchCount() == 0 && isMatching) ? 1 : 0;
            out["playerlist"] = (int)pMatchStruct->playerlist.size();

			std::vector<CPlayer*> pTempPlayerList = pMatchStruct->playerlist;

			// 先对玩家列表进行排名
			std::sort(pTempPlayerList.begin(),pTempPlayerList.end(),cmpList);

			std::vector<CPlayer*>::iterator iterTwo = pTempPlayerList.begin();
			for(int i=0;iterTwo != pTempPlayerList.end();++iterTwo,i++)
			{
				CPlayer *pPlayer = (*iterTwo);
				if(pPlayer == NULL) continue;

                out[i]["playerid"] = pPlayer->GetID();
                out[i]["MatchResult"] = (Json::Int64)pPlayer->GetMatchResult();
			}

			Sendhtml5(pPlayer->GetConnectID(),(const char*)out.toStyledString().c_str(),out.toStyledString().length());

			UnLockTableFramePlayerList();
			return;
		}
	}
	UnLockTableFramePlayerList();
}

/// 比赛场更新
void CDieOutMatchingMode::Update(void)
{
	if(m_tableframePlayerList.empty()) return;

	// 如果是比赛场，并且是定时比赛时，在开赛时如果发现人数不对时，而服务器又设置了比赛人数自适应的话，就自动适应人数
	/*if(m_ServerSet.GameType == ROOMTYPE_BISAI &&
		m_ServerSet.m_MatchingTimerPlayer &&
		m_ServerSet.m_MatchingShiPeiRenShu &&
		!m_isAutoMatchingPlayerCount)
	{
		CTime pCurTime,pTotalTime(m_ServerSet.m_MatchingTime);
		pCurTime=CTime::GetCurrentTime();

		if(pCurTime.GetHour() == pTotalTime.GetHour() &&
			pCurTime.GetMinute() == pTotalTime.GetMinute())
		{
			if(m_ServerSet.m_MatchingDate != 7 &&
				pCurTime.GetDayOfWeek()-1 != m_ServerSet.m_MatchingDate+1)
			{
				int pCurPlayerCount = GetMatchPlayerCount();
				int pPlayerCount = pCurPlayerCount % m_ServerSet.PlayerCount;

				if(pPlayerCount > 0)
					m_ServerSet.m_MatchingMaxPalyer = pCurPlayerCount + (m_ServerSet.PlayerCount - pPlayerCount);

				m_isAutoMatchingPlayerCount = true;
			}
		}
	}*/

	LockTableFramePlayerList();
	std::vector<MatchStruct>::iterator iter = m_tableframePlayerList.begin();
	for(;iter != m_tableframePlayerList.end();++iter)
	{
		if((*iter).isEnable == false) continue;

		// 如果比赛场处于排队状态，要发送排队信息
		if((*iter).isMatching == false && !(*iter).playerlist.empty())
			SendMatchingWaitingPlayerCount(&(*iter));

		OnProcessMatching(&(*iter));
	}
	UnLockTableFramePlayerList();
}

/// 给指定比赛场的玩家发送比赛排名
void CDieOutMatchingMode::SendMatchingRanking(MatchStruct *pMatchStruct,bool issort)
{
	if(pMatchStruct == NULL || pMatchStruct->playerlist.empty()) return;

	if(issort)
	{
		// 先对玩家列表进行排名
		std::sort(pMatchStruct->playerlist.begin(),pMatchStruct->playerlist.end(),cmpList);
	}

    Json::Value out;
    out["MsgId"] = IDD_MESSAGE_FRAME;
    out["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
    out["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_GETRAKING;
    out["State"] = 0;
    out["playerlist"] = (int)pMatchStruct->playerlist.size();

	std::vector<CPlayer*>::iterator iterTwo = pMatchStruct->playerlist.begin();
	for(int i=0;iterTwo != pMatchStruct->playerlist.end();++iterTwo,i++)
	{
		CPlayer *pPlayer = (*iterTwo);
		if(pPlayer == NULL) continue;

		out[i]["playerid"] = pPlayer->GetID();
		out[i]["MatchResult"] = (Json::Int64)pPlayer->GetMatchResult();
	}

	iterTwo = pMatchStruct->playerlist.begin();
	for(;iterTwo != pMatchStruct->playerlist.end();++iterTwo)
	{
		CPlayer *pPlayer = (*iterTwo);
		if(pPlayer == NULL) continue;

		Sendhtml5(pPlayer->GetConnectID(),(const char*)out.toStyledString().c_str(),out.toStyledString().length());
	}
}

/// 开始比赛
void CDieOutMatchingMode::StartMatching(MatchStruct *pMatchStruct,bool isConntinue)
{
	if(pMatchStruct == NULL || m_tableframeroomlist.empty() || pMatchStruct->playerlist.empty()) return;

	std::vector<CRoom*> pCurRoomList = m_tableframeroomlist[pMatchStruct->roomIndex];
	//std::vector<CPlayer*> pCurPlayerList = pMatchStruct->playerlist;

	int pPlayerCount = (int)pMatchStruct->playerlist.size()-1;

	for(int i=0;i<(int)pCurRoomList.size();i++)
	{
		CRoom *pRoom = pCurRoomList[i];
		if(pRoom == NULL) continue;

		//std::vector<CPlayer*> m_LostPlayerList;

		for(int k=0;k<pRoom->GetMaxPlayer();k++)
		{
			if(pPlayerCount < 0) return;

			// 清空玩家的比赛数据
			pMatchStruct->playerlist[pPlayerCount]->SetTotalMatchCount(m_ServerSet.m_MatchingLoopCount);
			pMatchStruct->playerlist[pPlayerCount]->SetMatchCount(m_ServerSet.m_MatchingLoopCount);
			pMatchStruct->playerlist[pPlayerCount]->SetMatching(true);

			if(!isConntinue)
				pMatchStruct->playerlist[pPlayerCount]->SetMatchResult(0);

			if(pMatchStruct->playerlist[pPlayerCount]->GetState() == PLAYERSTATE_LOSTLINE/* &&
				ServerPlayerManager.GetLostPlayer(pMatchStruct->playerlist[pPlayerCount]->GetID()) != NULL*/)
			{
				pMatchStruct->playerlist[pPlayerCount]->SetMatchingLostLine(true);
			}

			// 设置玩家状态为正常状态
			pMatchStruct->playerlist[pPlayerCount]->SetState(PLAYERSTATE_NORAML);

			if(ServerGameFrameManager.JoinPlayerToGameRoom(pMatchStruct->playerlist[pPlayerCount],pRoom->GetID(),k,false))
			{
				if(!isConntinue)
				{
					// 给所有发送比赛开始消息
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_FRAME;
                    out["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
                    out["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_START;
                    out["PlayerID"] = pMatchStruct->playerlist[pPlayerCount]->GetID();
                    out["MatchingLoopCount"] = m_ServerSet.m_MatchingLoopCount;

					Sendhtml5(pMatchStruct->playerlist[pPlayerCount]->GetConnectID(),(const char*)out.toStyledString().c_str(),out.toStyledString().length());
				}
				else
				{
					// 给所有发送比赛开始消息
                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_FRAME;
                    out["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
                    out["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_CONTINUE;
                    out["PlayerID"] = pMatchStruct->playerlist[pPlayerCount]->GetID();
                    out["MatchingLoopCount"] = m_ServerSet.m_MatchingLoopCount;

					Sendhtml5(pMatchStruct->playerlist[pPlayerCount]->GetConnectID(),(const char*)out.toStyledString().c_str(),out.toStyledString().length());
				}

				// 如果玩家还没有准备的话，帮玩家准备
				if(pMatchStruct->playerlist[pPlayerCount]->GetState() == PLAYERSTATE_NORAML)
				{
					ServerGameFrameManager.OnProcessGameReadyMatchingMes(pMatchStruct->playerlist[pPlayerCount]);
				}
			}
			else
			{
				char logstr[128];
				sprintf(logstr,"玩家%d在比赛中加入房间失败！",pMatchStruct->playerlist[pPlayerCount]->GetID());
				LOG_BASIC(logstr);

				// 清空玩家的比赛数据
				pMatchStruct->playerlist[pPlayerCount]->SetTotalMatchCount(0);
				pMatchStruct->playerlist[pPlayerCount]->SetMatchCount(0);
				pMatchStruct->playerlist[pPlayerCount]->SetMatching(false);
				pMatchStruct->playerlist[pPlayerCount]->SetMatchSignUp(false);

				// 如果玩家已经掉线的话，就替他准备开始游戏
				//if(isLostLine)
				//{
				//	pMatchStruct->playerlist[pPlayerCount]->SetState(PLAYERSTATE_LOSTLINE);
				//}
			}

			pPlayerCount-=1;
		}

		//// 如果这个房间已经开始游戏，那么原来那些掉线的就依然掉线状态
		//if(pRoom->GetRoomState() == ROOMSTATE_GAMING && !m_LostPlayerList.empty())
		//{
		//	for(int i=0;i<(int)m_LostPlayerList.size();i++)
		//	{
		//		ServerRoomManager.OnProcessMatchingDisconnectNetMes(m_LostPlayerList[i]);
		//	}
		//}
	}
}

/// 检测一轮比赛是否结束
bool CDieOutMatchingMode::IsOverOneLoopMatching(MatchStruct *pMatchStruct)
{
	if(pMatchStruct == NULL) return true;

	int count = 0;

	std::vector<CPlayer*>::iterator iterTwo = pMatchStruct->playerlist.begin();
	for(;iterTwo != pMatchStruct->playerlist.end();++iterTwo)
	{
		CPlayer *pPlayer = (*iterTwo);
		if(pPlayer == NULL) continue;

		pMatchStruct->curroomlist[pPlayer->GetRoomId()] = pPlayer->GetMatchCount();

		if(pPlayer->GetMatchCount() < 1)
		{
			count+=1;
		}
	}

	if(count != pMatchStruct->matchendcount && count != (int)pMatchStruct->playerlist.size())
	{
		pMatchStruct->matchendcount = count;

		//// 向已经结束游戏等待的玩家发送当前结束游戏的比赛玩家人数
		//iterTwo = pMatchStruct->playerlist.begin();
		//for(;iterTwo != pMatchStruct->playerlist.end();++iterTwo)
		//{
		//	CPlayer *pPlayer = (*iterTwo);
		//	if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT) continue;

		//	if(pPlayer->GetMatchCount() < 1)
		//	{
		//		CString tmpStr;
		//		tmpStr.Format(m_ServerOtherSet.MatchinEndTyp,pMatchStruct->matchendcount);

		//		// 给这个用户发送加金币消息
		//		CMolMessageOut out(IDD_MESSAGE_FRAME);
		//		out.write16(IDD_MESSAGE_FRAME_SUPER_BIG_MSG);
		//		out.write16(IDD_MESSAGE_TYPE_GAMESERVER_SYSTEM);
		//		out.writeString(WideCharConverToUtf8(tmpStr));

		//		Send(pPlayer->GetConnectID(),out);
		//	}
		//}
	}

	// 向已经完成比赛的用户发送其它没有完成比赛的房间进度
	if(!pMatchStruct->curroomlist.empty())
	{
		iterTwo = pMatchStruct->playerlist.begin();
		for(;iterTwo != pMatchStruct->playerlist.end();++iterTwo)
		{
			CPlayer *pPlayer = (*iterTwo);
			if(pPlayer == NULL || pPlayer->GetType() == PLAYERTYPE_ROBOT) continue;

			std::map<int,int>::iterator iterRL = pMatchStruct->curroomlist.begin();
			for(int i=0;i<rand()%(int)pMatchStruct->curroomlist.size();i++) ++iterRL;

			if(pMatchStruct->oldroomlisttimer[pPlayer->GetRoomId()] <= 0)
				pMatchStruct->oldroomlisttimer[pPlayer->GetRoomId()] = (DWORD)time(NULL);

			if(iterRL != pMatchStruct->curroomlist.end() &&
				(*iterRL).first != pPlayer->GetRoomId() &&
				(*iterRL).second < m_ServerSet.m_MatchingLoopCount &&
				(*iterRL).second != pMatchStruct->oldroomlist[pPlayer->GetRoomId()] &&
				(DWORD)time(NULL) > pMatchStruct->oldroomlisttimer[pPlayer->GetRoomId()]+10)
			{
				pMatchStruct->oldroomlist[pPlayer->GetRoomId()] = (*iterRL).second;
				pMatchStruct->oldroomlisttimer[pPlayer->GetRoomId()] = 0;

				/*CString tmpStr;

				if((*iterRL).second == 0)
					tmpStr.Format(m_ServerOtherSet.MatchimEndTypThree,(*iterRL).first+1);
				else
					tmpStr.Format(m_ServerOtherSet.MatchimEndTypTwo,(*iterRL).first+1,(*iterRL).second);

				if(pMatchStruct->oldroomlistString[tmpStr] <= 0)
				{
					pMatchStruct->oldroomlistString[tmpStr] = pPlayer->GetRoomId();

					CMolMessageOut out(IDD_MESSAGE_FRAME);
					out.write16(IDD_MESSAGE_FRAME_SUPER_BIG_MSG);
					out.write16(IDD_MESSAGE_TYPE_GAMESERVER_SYSTEM);
					out.writeString(WideCharConverToUtf8(tmpStr));

                    Json::Value out;
                    out["MsgId"] = IDD_MESSAGE_FRAME;
                    out["MsgSubId"] = IDD_MESSAGE_FRAME_SUPER_BIG_MSG;
                    out["MsgSubId2"] = IDD_MESSAGE_TYPE_GAMESERVER_SYSTEM;
                    out["String"] = pPlayer->GetID();
					Send(pPlayer->GetConnectID(),out);
				}*/
			}
		}
	}

	return (count == (int)pMatchStruct->playerlist.size() ? true : false);
}

/// 比赛场所有用户全部离开房间，准备进行下一轮
void CDieOutMatchingMode::AllPlayerLeaveMatchingToNext(MatchStruct *pMatchStruct)
{
	if(pMatchStruct == NULL) return;

	std::vector<CPlayer*>::iterator iterTwo = pMatchStruct->playerlist.begin();
	for(;iterTwo != pMatchStruct->playerlist.end();++iterTwo)
	{
		CPlayer *pPlayer = (*iterTwo);
		if(pPlayer == NULL) continue;

		Json::Value out;
		ServerGameFrameManager.OnProcessGameLeaveRoomMes(pPlayer->GetConnectID(),out,true);
	}
}

/// 结束比赛
void CDieOutMatchingMode::MatchingOver(MatchStruct *pMatchStruct)
{
	if(pMatchStruct == NULL) return;

	int count = (int)pMatchStruct->playerlist.size();

	std::vector<CPlayer*>::iterator iterTwo = pMatchStruct->playerlist.begin();
	for(int i=0;iterTwo != pMatchStruct->playerlist.end();i++)
	{
		CPlayer *pPlayer = (*iterTwo);
		if(pPlayer == NULL)
		{
			iterTwo = pMatchStruct->playerlist.erase(iterTwo);
			continue;
		}

		pPlayer->SetMatching(false);
		pPlayer->SetMatchSignUp(false);

		// 给所有发送比赛开始消息
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_FRAME;
        out["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
        out["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_OVER;
        out["PlayerID"] = pPlayer->GetID();
		Sendhtml5(pPlayer->GetConnectID(),(const char*)out.toStyledString().c_str(),out.toStyledString().length());

		if(pPlayer)
		{
			// 给玩家发比赛奖金
			pPlayer->SetMoney(pPlayer->GetMoney());

			// 统计玩家游戏局数
			pPlayer->SetSuccessBureau(pPlayer->GetSuccessBureau()+1);
			pPlayer->SetTotalBureau(pPlayer->GetFailBureau()+pPlayer->GetSuccessBureau());
			pPlayer->SetExperience(pPlayer->GetExperience()+1);

			// 计算等级
			uint32 plevel = uint32((pPlayer->GetLevel() + 1) * 10 + pPlayer->GetLevel() * 10);
			if(pPlayer->GetExperience() >= plevel)
				pPlayer->SetLevel(pPlayer->GetLevel()+1);

			ServerDBOperator.UpdateUserData(pPlayer);

			// 向当前服务器中所有玩家更新这个玩家的信息
			ServerGameFrameManager.UpdatePlayerMoney(pPlayer);

			// 写玩家的游戏记录
			//ServerDBOperator.InsertPlayerMatchingGameRecord(pPlayer->GetID(),(int64)pPlayer->GetMatchResult(),i+1,m_ServerSet.m_GameType,m_ServerSet.m_iServerPort,m_ServerSet.GameName);

			// 如果是模式比赛的话，要记录玩家当前积分等
			/*if(m_ServerSet.m_MatchingMode > 0)
			{
				ServerDBOperator.InsertPlayerModeMatchingGameRecord(pPlayer->GetID(),
																(int64)pPlayer->GetMatchResult(),
																i+1,
																m_ServerSet.m_MatchingMode,
																m_ServerSet.m_GameType,
																m_ServerSet.m_iServerPort,
																m_ServerSet.GameName);
			}*/

			//// 给获奖玩家发送金币奖励信息
			//CString tmpString;
			//tmpString.Format(m_ServerOtherSet.MatchingWinMoney,
			//	i+1,
			//	m_ServerOtherSet.MatchingRankMoney[i]);

			//// 给这个用户发送加金币消息
			//CMolMessageOut out(IDD_MESSAGE_FRAME);
			//out.write16(IDD_MESSAGE_FRAME_SUPER_BIG_MSG);
			//out.write16(IDD_MESSAGE_TYPE_SUPER_SMAILL_MSG);
			//out.writeString(WideCharConverToUtf8(tmpString));

			//Send(pPlayer->GetConnectID(),out);

			// 向所有游戏服务器发送这场比赛第一名
			if(i == 0)
			{
				/*CStringA tmpStr;
				tmpStr.Format(WideCharConverToUtf8(CString(m_ServerOtherSet.MatchingWinTip)).c_str(),
					pPlayer->GetName().c_str(),
					m_ServerSet.GameName,
					m_ServerOtherSet.MatchingRankMoney[i]);

				// 将这条最新消息插入数据库
				ServerDBOperator.insertlastgamingnews(tmpStr.GetBuffer());*/
				//if(m_ServerSet.m_MatchingMode > 0)
				//{
				//	CMolMessageOut out(IDD_MESSAGE_SUPER_BIG_MSG);
				//	out.write16(IDD_MESSAGE_TYPE_SUPER_BIG_MSG);
				//	out.writeString(tmpStr.GetBuffer());

				//	CCollector2Dlg::getSingleton().SendMsg(out);
				//}
				//else
				//{
				//	CMolMessageOut out(IDD_MESSAGE_FRAME);
				//	out.write16(IDD_MESSAGE_FRAME_SUPER_BIG_MSG);
				//	out.write16(IDD_MESSAGE_TYPE_SUPER_SMAILL_MSG);
				//	out.writeString(tmpStr.GetBuffer());

				//	ServerPlayerManager.SendMsgToEveryone(out);
				//}

				// 机器人开始抽奖
				//if(m_ServerSet.m_MatchingChouJiang && pPlayer->GetType() == PLAYERTYPE_ROBOT)
				//	ServerGameFrameManager.GetPrizeByUser(pPlayer,m_ServerSet.m_MatchingType);
			}
		}

		pPlayer->SetMatchRoomIndex(-1);
		iterTwo = pMatchStruct->playerlist.erase(iterTwo);

		if(pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
		{
			// 向房间所有玩家广播玩家离开服务器消息
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_FRAME;
            out["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
            out["PlayerID"] = pPlayer->GetID();
			ServerPlayerManager.SendMsgToEveryone(out);

			uint32 pConnID = pPlayer->GetConnectID();
			ServerPlayerManager.ClearPlayer(pPlayer);
			Disconnect(pConnID);
		}

		// 如果是比赛场，并且是定时比赛场，那么机器人在游戏结束之后必须离开比赛
		if(m_ServerSet.GameType == ROOMTYPE_BISAI &&
			m_ServerSet.m_MatchingTimerPlayer &&
			pPlayer->GetType() == PLAYERTYPE_ROBOT)
		{
			// 向房间所有玩家广播玩家离开服务器消息
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_FRAME;
            out["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
            out["PlayerID"] = pPlayer->GetID();
			ServerPlayerManager.SendMsgToEveryone(out);

			uint32 pConnID = pPlayer->GetConnectID();
			ServerPlayerManager.ClearPlayer(pPlayer);
			Disconnect(pConnID);
		}
	}

	// 清空比赛场
	pMatchStruct->clear();

	m_isAutoMatchingPlayerCount=false;

	//std::vector<MatchStruct>::iterator iter = m_tableframePlayerList.begin();
	//for(;iter != m_tableframePlayerList.end();++iter)
	//{
	//	if((*iter).roomIndex == (*pMatchStruct).roomIndex)
	//	{
	//		m_tableframePlayerList.erase(iter);
	//		break;
	//	}
	//}
}

/// 比赛继续
void CDieOutMatchingMode::MatchingContinue(MatchStruct *pMatchStruct)
{
	if(pMatchStruct == NULL || pMatchStruct->playerlist.empty()) return;

	std::vector<CPlayer*>::iterator iterTwo = pMatchStruct->playerlist.begin()+((pMatchStruct->matchcount+1) * m_ServerSet.PlayerCount);

	for(;iterTwo != pMatchStruct->playerlist.end();)
	{
		CPlayer *pPlayer = (*iterTwo);
		if(pPlayer == NULL)
		{
			iterTwo = pMatchStruct->playerlist.erase(iterTwo);
			continue;
		}

		// 给所有发送比赛开始消息
        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_FRAME;
        out["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
        out["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_OVER;
        out["PlayerID"] = (*iterTwo)->GetID();
		Sendhtml5((*iterTwo)->GetConnectID(),(const char*)out.toStyledString().c_str(),out.toStyledString().length());

		pPlayer->SetMatching(false);
		pPlayer->SetMatchSignUp(false);
		pPlayer->SetMatchRoomIndex(-1);

		// 统计玩家游戏局数
		pPlayer->SetFailBureau(pPlayer->GetFailBureau()+1);
		pPlayer->SetTotalBureau(pPlayer->GetFailBureau()+pPlayer->GetSuccessBureau());
		pPlayer->SetExperience(pPlayer->GetExperience()+1);

		// 计算等级
		uint32 plevel = uint32((pPlayer->GetLevel() + 1) * 10 + pPlayer->GetLevel() * 10);
		if(pPlayer->GetExperience() >= plevel)
			pPlayer->SetLevel(pPlayer->GetLevel()+1);

		ServerDBOperator.UpdateUserData(pPlayer);

		iterTwo = pMatchStruct->playerlist.erase(iterTwo);

		if(pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
		{
			// 向房间所有玩家广播玩家离开服务器消息
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_FRAME;
            out["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
            out["PlayerID"] = pPlayer->GetID();
			ServerPlayerManager.SendMsgToEveryone(out);

			uint32 pConnID = pPlayer->GetConnectID();
			ServerPlayerManager.ClearPlayer(pPlayer);
			Disconnect(pConnID);
		}

		// 如果是比赛场，并且是定时比赛场，那么机器人在游戏结束之后必须离开比赛
		if(m_ServerSet.GameType == ROOMTYPE_BISAI &&
			m_ServerSet.m_MatchingTimerPlayer &&
			pPlayer->GetType() == PLAYERTYPE_ROBOT)
		{
			// 向房间所有玩家广播玩家离开服务器消息
            Json::Value out;
            out["MsgId"] = IDD_MESSAGE_FRAME;
            out["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
            out["PlayerID"] = pPlayer->GetID();
			ServerPlayerManager.SendMsgToEveryone(out);

			uint32 pConnID = pPlayer->GetConnectID();
			ServerPlayerManager.ClearPlayer(pPlayer);
			Disconnect(pConnID);
		}
	}
}

/// 发送指定比赛场的等待人数
void CDieOutMatchingMode::SendMatchingWaitingPlayerCount(MatchStruct *pMatchStruct)
{
	if(pMatchStruct == NULL || pMatchStruct->isMatching == true || pMatchStruct->playerlist.empty()) return;

	//int pWaitingCount = (int)pMatchStruct->playerlist.size();

	//std::vector<CPlayer*>::iterator iter = pMatchStruct->playerlist.begin();
	//for(;iter != pMatchStruct->playerlist.end();++iter)
	//{
	//	CMolMessageOut out(IDD_MESSAGE_FRAME);
	//	out.write16(IDD_MESSAGE_FRAME_MATCH);
	//	out.write16(IDD_MESSAGE_FRAME_MATCH_WAITINGCOUNT);
	//	out.write16(pWaitingCount);

	//	Send((*iter)->GetConnectID(),out);
	//}
}

/// 开始处理比赛
void CDieOutMatchingMode::OnProcessMatching(MatchStruct *pMatchStruct)
{
	if(pMatchStruct == NULL) return;

	if(!pMatchStruct->isMatching)
	{
		int count = (int)pMatchStruct->playerlist.size();
		if(count < m_ServerSet.m_MatchingMaxPalyer) return;

		// 设置比赛开始
		pMatchStruct->isMatching = true;

		// 发送比赛消息
		StartMatching(pMatchStruct);

		// 发送比赛排名
		SendMatchingRanking(pMatchStruct);

		pMatchStruct->matchcount-=1;
	}
	else
	{
		if(pMatchStruct->matchcount > 0)
		{
			// 如果所有人一轮玩完，那么就要提人了，踢人后继续开始玩
			if(IsOverOneLoopMatching(pMatchStruct))
			{
				pMatchStruct->matchcount-=1;

				// 先发送比赛排名
				SendMatchingRanking(pMatchStruct);

				// 所有用户全部离开房间，准备进行下一轮
				AllPlayerLeaveMatchingToNext(pMatchStruct);

				// 如果当前房间人数小于最低人数，提前结束比赛
				if((int)pMatchStruct->playerlist.size() < m_ServerSet.PlayerCount)
				{
					// 比赛结束
					MatchingOver(pMatchStruct);

					pMatchStruct->isMatching=false;
					pMatchStruct->matchcount=0;
				}
				else
				{
					// 去掉多余的玩家
					MatchingContinue(pMatchStruct);

					// 然后开始下一轮
					StartMatching(pMatchStruct,true);
				}
			}
		}
		else                 // 全部比赛结束
		{
			// 如果所有人一轮玩完，那么就要提人了，踢人后继续开始玩
			if(IsOverOneLoopMatching(pMatchStruct))
			{
				// 先发送比赛排名
				SendMatchingRanking(pMatchStruct);

				// 所有用户全部离开房间
				AllPlayerLeaveMatchingToNext(pMatchStruct);

				// 比赛结束
				MatchingOver(pMatchStruct);

				pMatchStruct->isMatching=false;

				//// 比赛结束后，如果是定时房间的话，就要转到下一场定时赛中
				//if(m_ServerSet.GameType == ROOMTYPE_BISAI &&
				//	m_ServerSet.m_MatchingTimerPlayer)
				//{
				//	CCollector2Dlg::getSingleton().ChangeNextMatching();
				//}
			}
		}
	}
}
