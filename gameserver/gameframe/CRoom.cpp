#include "../../include/Common/defines.h"
#include "PlayerManager.h"
#include "DBOperator.h"

#include "../GameFrameManager.h"
#include "RoomManager.h"

#include <iosfwd>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>

std::wstring getCurrentTime(std::wstring str)
{
	time_t now;
	tm local;

	time(&now);

	local = *(localtime(&now));

	using namespace std;
	std::wostringstream os;
	/*os << "" << setw(2) << setfill('0') << local.tm_hour
		<< str << setw(2) << setfill('0') << local.tm_min
		<< str << setw(2) << setfill('0') << local.tm_sec
		<< "";*/

	return os.str();
}

unsigned int timeGetTime()
{
    unsigned int uptime = 0;
    struct timespec on;
    if(clock_gettime(CLOCK_MONOTONIC, &on) == 0)
             uptime = on.tv_sec*1000 + on.tv_nsec/1000000;
    return uptime;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CRoom::CRoom(RoomType type)
	: m_Id(0),m_MaxCount(0),m_RoomType(type),m_RoomState(ROOMSTATE_WAITING),
	  m_masterId(-1),m_curPlayerId(-1),m_RoomRevenue(0),m_RoomLastMoney(0),
	  m_RoomGameType(enStartMode_AllReady),m_ServerLogicFrame(NULL),m_Pielement(0),
	  m_GameType(0),m_RoomMarking(0),m_roomentermoneyfirst(0),m_roomentermoneysecond(0),m_gameid(0)
{
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		m_PlayerList[i] = NULL;
	}
}

CRoom::~CRoom()
{
	// 重置房间状态
	Clear();

	DeleteAllTimer();
	ClearAllPlayers();

	if(m_ServerLogicFrame)
		SafeDelete(m_ServerLogicFrame);
}

/**
 * 当前房间是否为空
 *
 * @return 如果当前房间为空的话返回真，否则返回假
 */
bool CRoom::IsEmpty(void)
{
	bool isok=true;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i])
		{
			isok = false;
			break;
		}
	}
	m_playersLock.Release();

	return isok;
}

/**
 * 清除房间中所有的玩家
 */
void CRoom::ClearAllPlayers(void)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] != NULL)
		{
			m_PlayerList[i]->SetRoomId(-1);
			m_PlayerList[i]->SetChairIndex(-1);
			m_PlayerList[i]->SetState(PLAYERSTATE_NORAML);
			m_PlayerList[i]->SetLookOn(false);
		}
		m_PlayerList[i] = NULL;
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.begin();
	for(;iter != m_LookonPlayerList.end();iter++)
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] != NULL)
			{
				(*iter).second[index]->SetRoomId(-1);
				(*iter).second[index]->SetChairIndex(-1);
				(*iter).second[index]->SetState(PLAYERSTATE_NORAML);
				(*iter).second[index]->SetLookOn(false);
			}
			(*iter).second[index] = NULL;
		}
		(*iter).second.clear();
	}
	m_lostPlayerList.clear();
	m_playersLock.Release();
}

/**
 * 得到当前房间指定状态的玩家个数
 *
 * @param state 要得到的玩家的状态
 *
 * @return 返回指定玩家玩家状态的玩家个数
 */
int CRoom::GetPlayerCount(PlayerState state)
{
	int count = 0;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] &&
			m_PlayerList[i]->GetState() == state)
			count+=1;
	}
	m_playersLock.Release();

	return count;
}

/**
 * 设置当前房间所有玩家的状态
 *
 * @param state 要设置的玩家的状态
 */
void CRoom::SetAllPlayerState(PlayerState state)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		m_PlayerList[i]->SetState(state);

		if(state == PLAYERSTATE_GAMING)
		{
			// 更新玩家游戏状态
			m_PlayerList[i]->setCurGamingState(true);

			//m_PlayerList[i]->SetTotalResult(0);
			m_PlayerList[i]->setCurrentGamingResult(0);
			ServerDBOperator.SetPlayerGameState(m_PlayerList[i]);
		}
	}
	m_playersLock.Release();
}

/**
 * 设置指定玩家的状态
 *
 * @param index 要设置的玩家的房间ID
 * @param state 要设置的玩家状态
 */
void CRoom::SetPlayerState(int index,PlayerState state)
{
	if(index < 0 || index >= ROOM_MAX_PLAYERS) return;

	m_playersLock.Acquire();
	if(m_PlayerList[index])
		m_PlayerList[index]->SetState(state);
	m_playersLock.Release();
}

/**
 * 添加一个玩家到当前房间中
 *
 * @param pPlayer 要添加的玩家
 * @param index 要把玩家添加到指定的位置上，如果设置成-1,那么表示随机添加到房间中
 *
 * @return 返回玩家在房间中的索引
 */
int CRoom::AddPlayer(CPlayer *pPlayer,int index)
{
	int playerIndex = -1;

	if(pPlayer == NULL ||
		GetPlayerCount() > m_MaxCount)
		return playerIndex;

	// 如果用户已经在房间中，就不加入了
	if(IsExist(pPlayer)) return playerIndex;

	// 检测玩家的金币数量是否满足当前房间要求
	//if(pPlayer->GetMoney() < m_RoomLastMoney) return playerIndex;

	m_playersLock.Acquire();
	if(index == playerIndex)
	{
		for(int i=0;i<ROOM_MAX_PLAYERS;i++)
		{
			if(m_PlayerList[i] == NULL)
			{
				m_PlayerList[i] = pPlayer;
				playerIndex = i;
				break;
			}
		}
	}
	else
	{
		if(index >= 0 && index < ROOM_MAX_PLAYERS)
		{
			if(m_PlayerList[index] == NULL)
			{
				m_PlayerList[index] = pPlayer;
				playerIndex = index;
			}
		}
	}
	m_playersLock.Release();

	// 更新玩家游戏状态
	pPlayer->setCurTableIndex(GetID());
	pPlayer->setCurChairIndex(playerIndex);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	return playerIndex;
}

/// 设置游戏逻辑处理接口
void CRoom::SetServerLogicFrame(ServerLogicFrame *frame)
{
	assert(frame != NULL);
	if(frame == NULL) return;

	m_ServerLogicFrame = frame;
	m_ServerLogicFrame->SetGameRoom(this);
}

/**
 * 添加一个旁观玩家到当前房间中
 *
 * @param pPlayer 要添加的玩家
 * @param index 要把玩家添加到指定的位置上
 *
 * @return 返回玩家在房间中的索引
 */
int CRoom::AddLookOnPlayer(CPlayer *pPlayer,int index)
{
	int playerIndex = -1;

	if(pPlayer == NULL ||
		(index < 0 || index > m_MaxCount))
		return playerIndex;

	// 如果用户已经在房间中，就不加入了
	if(IsExistLookOn(pPlayer)) return playerIndex;

	m_playersLock.Acquire();
	if(index >= 0 && index < ROOM_MAX_PLAYERS)
	{
		std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(index);
		if(iter != m_LookonPlayerList.end())
		{
			// 检测是否有这个用户，没有才添加进去
			bool isExistPlayer = true;
			for(int indexP = 0;indexP < (int)(*iter).second.size();indexP++)
			{
				if((*iter).second[indexP] == pPlayer)
				{
					isExistPlayer = false;
					break;
				}
			}

			if(isExistPlayer)
			{
				(*iter).second.push_back(pPlayer);
				playerIndex = index;
			}
		}
		else
		{
			std::vector<CPlayer*> tempPlayers;
			tempPlayers.push_back(pPlayer);

			m_LookonPlayerList.insert(std::pair<int,std::vector<CPlayer*> >(index,tempPlayers));
			playerIndex = index;
		}
	}
	m_playersLock.Release();

	// 更新玩家游戏状态
	pPlayer->setCurTableIndex(GetID());
	pPlayer->setCurChairIndex(playerIndex);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	return playerIndex;
}

/**
 * 设置当前房间的房主
 *
 * @param playerId 要设置成房主的玩家ID
 */
void CRoom::SetMaster(int playerId)
{
	m_masterId = playerId;
	m_curPlayerId = m_masterId;
}

/**
 * 得到当前房间中下一个玩家
 */
int CRoom::GetCurNextPlayer(void)
{
	m_curPlayerId += 1;

	if(m_curPlayerId >= GetPlayerCount())
		m_curPlayerId = 0;

	return m_curPlayerId;
}

/**
 * 得到当前房间中下一个出牌玩家
 */
int CRoom::GetCurNextWorkPlayer(void)
{
	int count = 0;

	while(true)
	{
		if(count >= GetPlayerCount()) break;

		int index = GetCurNextPlayer();
		Player *pPlayer = GetPlayer(index);
		if(pPlayer && pPlayer->GetState() == PLAYERSTATE_GAMING)
			return index;

		count+=1;
	}

	return -1;
}

/**
 * 开始一个定时器
 *
 * @param timerId 要开启的定时器ID
 * @param space 定时间隔
 *
 * @return 如果开启成功返回真，否则返回假
 */
bool CRoom::StartTimer(int timerId,int space)
{
	m_TimerLock.Acquire();

	//寻找子项
	CTimerItemArray::iterator iter = m_TimerItemActive.find(timerId);
	if(iter != m_TimerItemActive.end())
	{
		//获取时间
		tagSubTimerItem *pTimerItem=&(*iter).second;

		//设置判断
		if (pTimerItem->nTimerID==timerId)
		{
			pTimerItem->nTimerID=timerId;
			pTimerItem->nTimeLeave=space;
			pTimerItem->nIsEnable=true;

			m_TimerLock.Release();
			return true;
		}
	}

	tagSubTimerItem pTimerItemNew;

	//设置变量
	pTimerItemNew.nTimerID=timerId;
	pTimerItemNew.nTimeLeave=space;
	pTimerItemNew.nIsEnable=true;

	m_TimerItemActive.insert(std::pair<uint32,tagSubTimerItem>(timerId,pTimerItemNew));

	m_TimerLock.Release();

	return true;
}

/**
 * 关闭一个定时器
 *
 * @param id 要关闭的定时器ID
 */
void CRoom::StopTimer(int id)
{
	m_TimerLock.Acquire();
	//删除时间
	if (id!=0)
	{
		//寻找子项
		CTimerItemArray::iterator iter = m_TimerItemActive.find(id);
		if(iter != m_TimerItemActive.end())
		{
			//获取时间
			tagSubTimerItem *pTimerItem=&(*iter).second;

			//删除判断
			if (pTimerItem->nTimerID==id)
			{
				pTimerItem->nIsEnable=false;
			}
		}
	}
	else
	{
		CTimerItemArray::iterator iter = m_TimerItemActive.begin();
		for(;iter != m_TimerItemActive.end();++iter)
		{
			(*iter).second.nIsEnable=false;
		}
	}
	m_TimerLock.Release();
}

/**
 * 关闭所有的定时器
 */
void CRoom::StopAllTimer(void)
{
	m_TimerLock.Acquire();
	CTimerItemArray::iterator iter = m_TimerItemActive.begin();
	for(;iter != m_TimerItemActive.end();++iter)
	{
		(*iter).second.nIsEnable=false;
	}
	m_TimerLock.Release();
}

/// 删除所有的定时器
void CRoom::DeleteAllTimer(void)
{
	m_TimerItemActive.clear();
}

///时间事件
bool CRoom::OnEventTimer(DWORD dwTimerID, DWORD wBindParam)
{
	m_TimerLock.Acquire();
	//寻找子项
	CTimerItemArray::iterator iter = m_TimerItemActive.begin();
	for(;iter != m_TimerItemActive.end();++iter)
	{
		if((*iter).second.nIsEnable == false)
			continue;

		//变量定义
		tagSubTimerItem *pTimerItem=&(*iter).second;
		if(pTimerItem == NULL) continue;

		//时间处理
		if (pTimerItem->nTimeLeave<1L)
		{
			pTimerItem->nIsEnable=false;
		}
		else
		{
			pTimerItem->nTimeLeave--;
		}

		if(GetServerLogicFrame())
		{
			GetServerLogicFrame()->OnProcessTimerMsg(pTimerItem->nTimerID,pTimerItem->nTimeLeave);
		}
	}
	m_TimerLock.Release();

	return true;
}

/**
 * 从当前房间中清除指定的玩家
 *
 * @param pPlayer 要清除的玩家
 */
void CRoom::ClearPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL)
		return;

	// 更新玩家游戏状态
	pPlayer->setCurChairIndex(-1);
	pPlayer->setCurTableIndex(-1);
	pPlayer->SetEnterMoneyRect(0,0);
	pPlayer->SetEnterPassword("");
	ServerDBOperator.SetPlayerGameState(pPlayer);

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] &&
			m_PlayerList[i] == pPlayer)
		{
			//pPlayer->SetState(PLAYERSTATE_NORAML);
			//pPlayer->SetRoomId(-1);
			//pPlayer->SetChairIndex(-1);
			//pPlayer->SetLookOn(false);

			ClearLostPlayerById(i);

			m_PlayerList[i] = NULL;

			break;
		}
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(pPlayer->GetChairIndex());
	if(iter != m_LookonPlayerList.end())
	{
		std::vector<CPlayer*>::iterator iter2 = (*iter).second.begin();
		for(;iter2 != (*iter).second.end();iter2++)
		{
			if((*iter2) == pPlayer)
			{
				//pPlayer->SetState(PLAYERSTATE_NORAML);
				//pPlayer->SetRoomId(-1);
				//pPlayer->SetChairIndex(-1);
				//pPlayer->SetLookOn(false);

				(*iter2) = NULL;
				(*iter).second.erase(iter2);
				break;
			}
		}
	}
	m_playersLock.Release();

	// 如果房间中没人的话，就重设房间
	if(IsEmpty())
	{
		bool isChangeRoomState = false;

		if(m_roomenterpwd != "" ||
			m_roomentermoneyfirst != 0 ||
			m_roomentermoneysecond != 0)
			isChangeRoomState = true;

		m_roomenterpwd="";
		m_roomentermoneyfirst=m_roomentermoneysecond=0;

		if(isChangeRoomState)
		{
			// 先向服务器中所有的在线玩家通告玩家进入游戏房间的消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_FRAME_ROOM_JOINSET;
            root["UserID"] = GetID();
            root["roomenterpwd"] = m_roomenterpwd;
            root["roomentermoneyfirst"] = (uint32)m_roomentermoneyfirst;
            root["roomentermoneysecond"] = (uint32)m_roomentermoneysecond;

			ServerPlayerManager.SendMsgToEveryone(root);
		}
	}
}

/**
 * 从当前房间中删除指定的玩家
 *
 * @param pPlayer 要删除的玩家
 */
void CRoom::DeletePlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL)
		return;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == pPlayer)
		{
			if(!IsExistLostPlayer(i))
				m_lostPlayerList.push_back(i);

			delete m_PlayerList[i];
			m_PlayerList[i] = NULL;
			break;
		}
	}
	m_playersLock.Release();
}

/**
 * 根据用户连接ID得到用户在房间中的ID
 *
 * @param connId 要得到的用户的连接ID
 *
 * @return 如果这个用户存在于这个房间，返回这个用户在这个房间中的ID，否则返回-1
 */
int CRoom::GetPlayerRoomId(uint32 connId)
{
	int state = -1;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetConnectID() == connId)
		{
			state = i;
			break;
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * 检测指定的索引是否在掉线玩家列表中
 *
 * @param index 要检测的玩家在房间中的索引
 *
 * @return 如果玩家在掉线玩家列表中返回真，否则返回假
 */
bool CRoom::IsExistLostPlayer(int index)
{
	if(m_lostPlayerList.empty()) return false;

	bool isok = false;
	m_playersLock.Acquire();
	for(int i=0;i<(int)m_lostPlayerList.size();i++)
	{
		if(m_lostPlayerList[i] == index)
		{
			isok = true;
			break;
		}
	}
	m_playersLock.Release();

	return isok;
}

/**
 * 清除掉线玩家列表中玩家
 *
 * @param id 要清除的玩家的ID
 */
void CRoom::ClearLostPlayerById(int id)
{
	if(m_lostPlayerList.empty()) return;

	m_playersLock.Acquire();
	std::vector<int>::iterator iter = m_lostPlayerList.begin();
	for(int i=0;iter != m_lostPlayerList.end();i++)
	{
		if(i == id)
		{
			iter = m_lostPlayerList.erase(iter);
			break;
		}
		else
		{
			++iter;
		}
	}
	m_playersLock.Release();
}

/**
 * 从当前房间中清除指定房间ID的玩家
 *
 * @param Id 要清除的玩家在房间中的ID
 */
void CRoom::ClearPlayerById(int Id)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] && m_PlayerList[i]->GetChairIndex() == Id)
		{
			//m_PlayerList[i]->SetRoomId(-1);
			//m_PlayerList[i]->SetChairIndex(-1);
			//m_PlayerList[i]->SetLookOn(false);

			ClearLostPlayerById(Id);

			m_PlayerList[i] = NULL;

			break;
		}
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(Id);
	if(iter != m_LookonPlayerList.end())
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] != NULL)
			{
				//(*iter).second[index]->SetRoomId(-1);
				//(*iter).second[index]->SetChairIndex(-1);
				//(*iter).second[index]->SetState(PLAYERSTATE_NORAML);
				//(*iter).second[index]->SetLookOn(false);
			}
			(*iter).second[index] = NULL;
		}
		(*iter).second.clear();
	}
	m_playersLock.Release();
}

/**
 * 得到当前房间人数
 */
int CRoom::GetPlayerCount(void)
{
	int count = 0;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetRoomId() != -1 &&
			m_PlayerList[i]->GetChairIndex() != -1)
			count+=1;
	}
	m_playersLock.Release();

	return count;
}

/**
 * 得到准备好状态下玩家的人数
 *
 * @return 返回当前房间中准备好的玩家的个数
 */
int CRoom::GetReadyPlayerCount(void)
{
	int count = 0;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetState() == PLAYERSTATE_READY)
		{
			count += 1;
		}
	}
	m_playersLock.Release();

	return count;
}

/**
 * 清空当前房间数据
 */
void CRoom::Clear(void)
{
	// 重置玩家状态
	//SetAllPlayerState(PLAYERSTATE_NORAML);

	m_lostPlayerList.clear();
	m_RoomState = ROOMSTATE_WAITING;
	m_masterId = -1;
	m_curPlayerId = -1;

	StopAllTimer();
}

/**
 * 检测指定玩家是否存在当前房间
 *
 * @param pPlayer 要检测的玩家
 *
 * @return 如果要检测的玩家存在返回真，否则返回假
 */
bool CRoom::IsExist(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	bool state = false;

	if(pPlayer == NULL)
		return state;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] &&
			m_PlayerList[i]->GetID() == pPlayer->GetID())
		{
			state = true;
			break;
		}
	}
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(pPlayer->GetChairIndex());
	if(iter != m_LookonPlayerList.end())
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] == pPlayer)
			{
				state = true;
				break;
			}
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * 检测指定的旁观玩家是否存在当前房间中
 *
 * @param pPlayer 要检测的玩家
 *
 * @return 如果要检测的玩家存在返回真，否则返回假
 */
bool CRoom::IsExistLookOn(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	bool state = false;

	if(pPlayer == NULL)
		return state;

	m_playersLock.Acquire();
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(pPlayer->GetChairIndex());
	if(iter != m_LookonPlayerList.end())
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] == pPlayer)
			{
				state = true;
				break;
			}
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * 检测房间是否已经满员
 *
 * @return 如果房间已经满员返回真，否则返回假
 */
bool CRoom::IsFull(void)
{
	return GetPlayerCount() == m_MaxCount ? true : false;
}

/**
 * 检测房间是否全部准备完毕
 *
 * @param 如果当前房间满员并且全部处于准备状态下返回真，否则返回假
 */
bool CRoom::IsReadyed(void)
{
	if(/*m_RoomState == ROOMSTATE_WAITING &&*/
		IsFull())
	{
		bool isReady = true;

		m_playersLock.Acquire();
		for(int i=0;i<ROOM_MAX_PLAYERS;i++)
		{
			if(m_PlayerList[i] == NULL) continue;

			if(m_PlayerList[i]->GetState() != PLAYERSTATE_READY)
			{
				isReady = false;
				break;
			}
		}
		m_playersLock.Release();

		return isReady;
	}

	return false;
}

/**
 * 向指定的玩家发送数据
 *
 * @param index 要发送的玩家的索引,如果为-1的话，表示发送给所有人，否则发送给指定的人
 * @param msg 要发送的数据
 */
void CRoom::SendTableMsg(int index,Json::Value &msg)
{
	if(index != INVALID_CHAIR)
	{
		m_playersLock.Acquire();
		Player *pPlayer = dynamic_cast<Player*>(m_PlayerList[index]);
		if(pPlayer != NULL)
		{
			// 如果玩家是机器人的话，就处理机器人
			if(pPlayer->GetType() == PLAYERTYPE_ROBOT)
			{
				if(m_PlayerList[index]->GetRobotLogicFrame() != NULL)
                {
                   // CMolMessageIn pInData = CMolMessageIn(msg.getData(),msg.getLength());
					m_PlayerList[index]->GetRobotLogicFrame()->OnProcessPlayerRoomMes(msg);
                }
			}
			else
			{
				if(pPlayer->GetConnectID() == NULL ||
					IsConnected(pPlayer->GetConnectID()) == false)
				{
					m_playersLock.Release();
					return;
				}

				Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
			}
		}
		m_playersLock.Release();
	}
	else
	{
		m_playersLock.Acquire();
		// 处理当前房间中的机器人
		for(int i=0;i<GetMaxPlayer();i++)
		{
			if(m_PlayerList[i] == NULL) continue;
			if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
			if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

			//CMolMessageIn pInData = CMolMessageIn(msg.getData(),msg.getLength());
			m_PlayerList[i]->GetRobotLogicFrame()->OnProcessPlayerRoomMes(msg);
		}
		m_playersLock.Release();

		SendAllPlayerMsg(msg);
	}
}

/**
 * 向指定的玩家发送旁观数据
 *
 * @param index 要发送的玩家的索引,如果为-1的话，表示发送给所有人，否则发送给指定的人
 * @param msg 要发送的数据
 */
void CRoom::SendLookOnMes(int index,Json::Value &msg)
{
	if(index >= 0 && index < ROOM_MAX_PLAYERS)
	{
		m_playersLock.Acquire();
		std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.find(index);
		if(iter != m_LookonPlayerList.end())
		{
			for(int index = 0;index < (int)(*iter).second.size();index++)
			{
				if((*iter).second[index] == NULL) continue;

				if((*iter).second[index]->GetConnectID() == NULL ||
					IsConnected((*iter).second[index]->GetConnectID()) == false)
					continue;

				Sendhtml5((*iter).second[index]->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
			}
		}
		m_playersLock.Release();
	}
	else if(index == -1)                    // 发送给所有人
	{
		SendAllLookOnPlayerMsg(msg);
	}
}

/**
 * 向所有玩家发送数据
 *
 * @param msg 要发送的数据
 */
void CRoom::SendAllPlayerMsg(Json::Value &msg)
{
	m_playersLock.Acquire();
	for(int i=0;i<GetMaxPlayer();i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetConnectID() == NULL ||
			IsConnected(m_PlayerList[i]->GetConnectID()) == false)
			continue;

		Sendhtml5(m_PlayerList[i]->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	}
	m_playersLock.Release();
}

/**
 * 向所有旁观玩家发送数据
 *
 * @param msg 要发送的数据
 */
void CRoom::SendAllLookOnPlayerMsg(Json::Value &msg)
{
	m_playersLock.Acquire();
	std::map<int,std::vector<CPlayer*> >::iterator iter = m_LookonPlayerList.begin();
	for(;iter != m_LookonPlayerList.end();iter++)
	{
		for(int index = 0;index < (int)(*iter).second.size();index++)
		{
			if((*iter).second[index] == NULL) continue;

			if((*iter).second[index]->GetConnectID() == NULL ||
				IsConnected((*iter).second[index]->GetConnectID()) == false)
				continue;

			Sendhtml5((*iter).second[index]->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
		}
	}
	m_playersLock.Release();
}

/**
 * 得到指定索引的玩家
 *
 * @param index 要得到的玩家的索引
 *
 * @return 如果这个指定索引的玩家存在返回这个玩家，否则返回NULL
 */
Player* CRoom::GetPlayer(int index)
{
	Player *pPlayer = NULL;

	if(index < 0 || index >= ROOM_MAX_PLAYERS)
		return pPlayer;

	m_playersLock.Acquire();
	pPlayer = dynamic_cast<Player*>(m_PlayerList[index]);
	m_playersLock.Release();

	return pPlayer;
}

/**
 * 根据玩家连接id得到玩家在当前房间中的索引
 *
 * @param connId 要得到的客户端连接ID
 *
 * @return 返回这个连接在房间中的索引
 */
int CRoom::GetPlayerIndex(uint32 connId)
{
	int state = -1;

	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		if(m_PlayerList[i] == NULL) continue;

		if(m_PlayerList[i]->GetConnectID() == connId)
		{
			state = m_PlayerList[i]->GetChairIndex();
			break;
		}
	}
	m_playersLock.Release();

	return state;
}

/**
 * 用于处理用户准备后的消息
 *
 * @param playerId 要处理的客户端的房间ID
 */
void CRoom::OnProcessPlayerReadyMes(int playerId)
{
	m_playersLock.Acquire();
	// 根据房间类型确定是否可以开始游戏
	if(GetRoomGameType() == enStartMode_FullReady)
	{
		// 如果准备状态的玩家达到2个就可以开始游戏了
		int pReadyCount = GetReadyPlayerCount();

		if(pReadyCount >= 2 && pReadyCount == GetPlayerCount())
			GameStart();

		m_playersLock.Release();
		return;
	}
	else if(GetRoomGameType() == enStartMode_AllReady)
	{
		if(IsReadyed())
		{
			GameStart();
		}

		m_playersLock.Release();
		return;
	}

	// 处理当前房间中的机器人
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
		//if(m_PlayerList[playerId] == NULL) continue;
		//if(m_PlayerList[playerId]->GetType() != PLAYERTYPE_ROBOT) continue;
		//if(m_PlayerList[playerId]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessPlayerReadyMes(playerId);
	//}
	m_playersLock.Release();
}

/**
 * 用于处理用户进入游戏房间后的消息
 *
 * @param playerId 要处理的客户端的房间ID
 * @param mes 接收的客户端发送的消息
 */
void CRoom::OnProcessPlayerRoomMes(int playerId,Json::Value &mes)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessPlayerRoomMes(playerId,mes);
}

/// 处理用户进入房间消息
void CRoom::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessEnterRoomMsg(playerId);

	m_playersLock.Acquire();

	// 处理当前房间中的机器人
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->SetCurGameRoom(this);
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessEnterRoomMsg(playerId);
		}
	//}
	m_playersLock.Release();
}

/// 处理用户离开房间消息
void CRoom::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL || playerId < 0 || playerId >= GetMaxPlayer()) return;

	m_ServerLogicFrame->OnProcessLeaveRoomMsg(playerId);

	m_playersLock.Acquire();

	// 处理当前房间中的机器人
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessLeaveRoomMsg(playerId);
		}
	//}

	if(m_PlayerList[playerId])
		m_PlayerList[playerId]->SetState(PLAYERSTATE_NORAML);
	m_playersLock.Release();
}

/// 处理用户断线消息
void CRoom::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessOfflineRoomMes(playerId);

	m_playersLock.Acquire();

	// 处理当前房间中的机器人
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessOfflineRoomMes(playerId);
		}
	//}
	m_playersLock.Release();
}

/// 处理用户断线重连消息
void CRoom::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_ServerLogicFrame->OnProcessReEnterRoomMes(playerId);

	m_playersLock.Acquire();

	// 处理当前房间中的机器人
	//for(int i=0;i<GetMaxPlayer();i++)
	//{
	//	if(m_PlayerList[i] == NULL) continue;
	//	if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
	//	if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		if(m_PlayerList[playerId] &&
			m_PlayerList[playerId]->GetType() == PLAYERTYPE_ROBOT &&
			m_PlayerList[playerId]->GetRobotLogicFrame() != NULL)
		{
			m_PlayerList[playerId]->GetRobotLogicFrame()->OnProcessReEnterRoomMes(playerId);
		}
	//}
	m_playersLock.Release();
}

/**
 * 用于处理接收到的网络消息
 *
 * @param connId 要处理的客户端
 * @param mes 要处理的客户端发送的网络消息
 */
void CRoom::OnProcessNetMes(uint32 connId,Json::Value &mes)
{
	int playerId = GetPlayerIndex(connId);
	if(playerId == -1) return;

	m_playersLock.Acquire();
	OnProcessPlayerRoomMes(playerId,mes);
	m_playersLock.Release();
}

/**
 * 更新指定玩家的数据
 *
 * @param ChairID 要更新的玩家的ID
 */
void CRoom::UpdateUserScore(Player *pPlayer)
{
	if(pPlayer == NULL) return;

	ServerGameFrameManager.UpdatePlayerInfo(pPlayer);
}

/// 写入用户积分
bool CRoom::WriteUserScore(int wChairID, int64 lScore, int64 lRevenue, enScoreKind ScoreKind,int64 pAgentmoney,bool isCumulativeResult,int64 pcurJetton,const char* pgametip)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return false;

	//if(lScore == 0) return false;

	CPlayer *pPlayer = static_cast<CPlayer*>(GetPlayer(wChairID));
	if(pPlayer == NULL) return false;

	bool isok=true;

	int64 pTempMoney = (lScore + lRevenue) + pAgentmoney;

	m_playersLock.Acquire();
	pPlayer->SetMoney(pPlayer->GetMoney() + lScore);
	pPlayer->SetTotalBureau(pPlayer->GetTotalBureau()+1);
	pPlayer->SetExperience(pPlayer->GetExperience()+1);
	pPlayer->SetTotalResult(pPlayer->GetTotalResult() + pTempMoney);
	if(isCumulativeResult) pPlayer->setCurrentGamingResult(pTempMoney);

	switch(ScoreKind)
	{
	case enScoreKind_Win:
		{
			pPlayer->SetSuccessBureau(pPlayer->GetSuccessBureau()+1);
		}
		break;
	case enScoreKind_Lost:
		{
			pPlayer->SetFailBureau(pPlayer->GetFailBureau()+1);
		}
		break;
	case enScoreKind_Flee:
		{
			pPlayer->SetRunawayBureau(pPlayer->GetRunawayBureau()+1);
		}
		break;
	default:
		break;
	}

	// 计算等级
	uint32 plevel = uint32((pPlayer->GetLevel() + 1) * 10 + pPlayer->GetLevel() * 10);
	if(pPlayer->GetExperience() >= plevel)
		pPlayer->SetLevel(pPlayer->GetLevel()+1);

	// 计算胜率
	pPlayer->SetSuccessRate((float)pPlayer->GetSuccessBureau() / (float)pPlayer->GetTotalBureau());

	// 计算逃跑率
	pPlayer->SetRunawayrate((float)pPlayer->GetRunawayBureau() / (float)pPlayer->GetTotalBureau());

	// 计算税收
	if(lRevenue > 0)
	{
		pPlayer->SetRevenue(pPlayer->GetRevenue()+lRevenue);
	}

	// 写玩家的游戏记录
	ServerDBOperator.InsertPlayerGameRecord(pPlayer->GetID(),
											lScore,
											lRevenue,
											GetGameId(),
											GetRoomMarking(),
											m_ServerSet.GameName,
											pPlayer->GetRoomId(),
											pPlayer->GetChairIndex(),
											pPlayer->GetMoney(),
											"",
											pAgentmoney,
											pcurJetton);

	isok = ServerDBOperator.UpdateUserData(pPlayer);

	m_playersLock.Release();

	return isok;
}

/**
 * 向当前房间所有玩家发送用户离开服务器消息
 */
void CRoom::AllPlayerLeveaServer(void)
{
	m_playersLock.Acquire();
	for(int i=0;i<ROOM_MAX_PLAYERS;i++)
	{
		CPlayer *pPlayer = m_PlayerList[i];
		if(pPlayer == NULL) continue;

		// 向房间所有玩家广播玩家离开服务器消息
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
        root["UserID"] = m_PlayerList[i]->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);

		m_PlayerList[i]->SetRoomId(-1);
		m_PlayerList[i]->SetChairIndex(-1);
		m_PlayerList[i]->SetState(PLAYERSTATE_NORAML);

		ClearPlayer(m_PlayerList[i]);

		uint32 pConnID = pPlayer->GetConnectID();
		ServerPlayerManager.ClearPlayer(pPlayer);
		Disconnect(pConnID);
	}
	m_playersLock.Release();
}

/// 是否继续游戏
void CRoom::OnProcessContinueGaming(void)
{
	// 如果游戏模式为满人开始，那么在游戏房间不处于游戏状态的情况下，
	// 如果有两个以上的人准备的情况下，开始游戏
	m_playersLock.Acquire();
	if(/*m_ServerSet.GameType != ROOMTYPE_BISAI && */
		GetRoomGameType() == enStartMode_FullReady &&
		GetRoomState() != ROOMSTATE_GAMING)
	{
		// 如果准备状态的玩家达到2个就可以开始游戏了
		int pReadyCount = GetReadyPlayerCount();

		if(pReadyCount >= 2 && pReadyCount == GetPlayerCount())
			GameStart();
	}
	m_playersLock.Release();
}

/**
 * 游戏开始是调用
 */
void CRoom::GameStart(void)
{
	// 向当前服务器中所有用户广播当前这桌已经开始了游戏
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_GAME_START;
    root["UserID"] = GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	// 设置房间状态为开始状态，所有用户状态为开始状态
	SetRoomState(ROOMSTATE_GAMING);
	SetAllPlayerState(PLAYERSTATE_GAMING);

	srand((uint32)time(0)+rand()%10000);

	// 游戏开始了
	OnProcessPlayerGameStartMes();
}

/// 用于处理用户开始游戏开始消息
void CRoom::OnProcessPlayerGameStartMes(void)
{
	assert(m_ServerLogicFrame!=NULL);
	if(m_ServerLogicFrame == NULL) return;

	m_playersLock.Acquire();
	// 处理当前房间中的机器人
	for(int i=0;i<GetMaxPlayer();i++)
	{
		if(m_PlayerList[i] == NULL) continue;
		if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
		if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		m_PlayerList[i]->GetRobotLogicFrame()->OnProcessPlayerGameStartMes();
	}

	m_playersLock.Release();

	m_ServerLogicFrame->OnProcessPlayerGameStartMes();
}

/**
 * 游戏结束时调用
 */
void CRoom::GameEnd(bool isupdateuserdata)
{
	// 先更新用户总的结果值
	//UpdateUserTotalResult();

	// 重置房间状态
	Clear();

	if(m_ServerSet.GameType == ROOMTYPE_BISAI)
	{
		// 从房间中剔除所有的掉线用户,并且更新所有玩家的数据
		m_playersLock.Acquire();
		for(int index = 0;index < GetMaxPlayer();index++)
		{
			CPlayer *pPlayer = m_PlayerList[index];
			if(pPlayer == NULL) continue;

			if(m_PlayerList[index]->GetState() != PLAYERSTATE_LOSTLINE)          // 如果一局结束，但用户已经掉线而且没有回来，先离开房间，然后离开服务器，他需要重新登录才能进入游戏
			{
				// 重新将状态设置成正常状态
				m_PlayerList[index]->SetState(PLAYERSTATE_NORAML);
			}

			// 更新用户信息到客户端
			UpdateUserScore(m_PlayerList[index]);

			// 从房间中清除掉这些玩家
			ClearPlayer(m_PlayerList[index]);

			//// 设置玩家的状态为离开房间
			//pPlayer->SetRoomId(-1);
			//pPlayer->SetChairIndex(-1);

			// 更新玩家游戏状态
			pPlayer->setCurGamingState(false);
			ServerDBOperator.SetPlayerGameState(pPlayer);
		}
		m_playersLock.Release();

		// 向当前服务器中所有用户广播当前这桌已经结束了游戏
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_GAME_END;
        root["UserID"] = GetID();
		ServerPlayerManager.SendMsgToEveryone(root);

		return;
	}

	// 从房间中剔除所有的掉线用户,并且更新所有玩家的数据
	m_playersLock.Acquire();
	for(int index = 0;index < GetMaxPlayer();index++)
	{
		CPlayer *pPlayer = m_PlayerList[index];
		if(pPlayer == NULL) continue;

		if(m_PlayerList[index]->GetState() == PLAYERSTATE_LOSTLINE)          // 如果一局结束，但用户已经掉线而且没有回来，先离开房间，然后离开服务器，他需要重新登录才能进入游戏
		{
			// 向房间所有玩家广播玩家离开服务器消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
            root["UserID"] = m_PlayerList[index]->GetID();
			ServerPlayerManager.SendMsgToEveryone(root);

			//m_PlayerList[index]->SetRoomId(-1);
			//m_PlayerList[index]->SetChairIndex(-1);
			//m_PlayerList[index]->SetState(PLAYERSTATE_NORAML);

			ClearPlayer(m_PlayerList[index]);

			uint32 pConnID = pPlayer->GetConnectID();
			ServerPlayerManager.ClearPlayer(pPlayer);
			Disconnect(pConnID);

			continue;
		}

		// 重新将状态设置成正常状态
		m_PlayerList[index]->SetState(PLAYERSTATE_NORAML);

		// 更新用户信息到客户端
		UpdateUserScore(m_PlayerList[index]);

		// 如果一局结束，用户是机器人，就离开房间
		if(m_PlayerList[index]->GetType() == PLAYERTYPE_ROBOT)
		{
			if(rand()%100 < 50)
			{
				// 先调用用户离开房间接口
				OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());

				// 向当前服务器中所有用户发送玩家离开房间消息
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
                root["UserID"] = m_PlayerList[index]->GetID();
				ServerPlayerManager.SendMsgToEveryone(root);

				ClearPlayer(m_PlayerList[index]);

				pPlayer->SetRoomId(-1);
				pPlayer->SetChairIndex(-1);
				continue;
			}
		}

		// 如果玩家金币少于最低标准，必须要离开房间
		if(m_PlayerList[index]->GetMoney() < m_ServerSet.lastMoney ||
			pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
		{
			// 先调用用户离开房间接口
			OnProcessLeaveRoomMsg(m_PlayerList[index]->GetChairIndex());

			// 向当前服务器中所有用户发送玩家离开房间消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
            root["UserID"] = m_PlayerList[index]->GetID();
			ServerPlayerManager.SendMsgToEveryone(root);

			ClearPlayer(m_PlayerList[index]);

			// 设置玩家的状态为离开房间
			pPlayer->SetRoomId(-1);
			pPlayer->SetChairIndex(-1);
			continue;
		}

		// 更新玩家游戏状态
		pPlayer->setCurGamingState(false);
		ServerDBOperator.SetPlayerGameState(pPlayer);
	}
	m_playersLock.Release();

	// 向当前服务器中所有用户广播当前这桌已经结束了游戏
	Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_GAME_END;
    root["UserID"] = GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	m_playersLock.Acquire();
	// 处理当前房间中的机器人
	for(int i=0;i<GetMaxPlayer();i++)
	{
		if(m_PlayerList[i] == NULL) continue;
		if(m_PlayerList[i]->GetType() != PLAYERTYPE_ROBOT) continue;
		if(m_PlayerList[i]->GetRobotLogicFrame() == NULL) continue;

		m_PlayerList[i]->GetRobotLogicFrame()->OnProcessPlayerGameOverMes();
	}
	m_playersLock.Release();
}

/// 剔除指定的玩家，必须在游戏空闲时间才能剔除
bool CRoom::EliminatePlayer(Player *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomState == ROOMSTATE_GAMING ||
		pPlayer->GetRoomId() != this->GetID())
		return false;

	// 先调用用户离开房间接口
	OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());

	// 向当前服务器中所有用户发送玩家离开房间消息
	Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
    root["UserID"] = pPlayer->GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	ClearPlayer(static_cast<CPlayer*>(pPlayer));

	pPlayer->SetRoomId(-1);
	pPlayer->SetChairIndex(-1);

	return true;
}

/// 打印日志
void CRoom::Room_Log(enLogLevel loglevel,std::string log)
{
    switch(loglevel)
    {
    case BASIC:
        {
            LOG_BASIC(log.c_str());
        }
        break;
     case DETAIL:
        {
            LOG_DETAIL(log.c_str());
        }
        break;
    case ERROR:
        {
            LOG_ERROR(log.c_str());
        }
        break;
     case DEBUG:
        {
            LOG_DEBUG(log.c_str());
        }
        break;
    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
