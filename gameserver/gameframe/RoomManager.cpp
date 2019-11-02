#include "RoomManager.h"
#include "PlayerManager.h"

#include "../../include/Common/defines.h"

#include "DBOperator.h"

#include <sstream>
#include <functional>

initialiseSingleton(RoomManager);

RoomManager::RoomManager()
{

}

RoomManager::~RoomManager()
{
	ClearAllRooms();
}

/**
 * 清除所有的房间
 */
void RoomManager::ClearAllRooms(void)
{
	if(m_RoomList.empty()) return;

	m_RoomLock.Acquire();
	for(int i=0;i<(int)m_RoomList.size();i++)
	{
		if(m_RoomList[i]) delete m_RoomList[i];
		m_RoomList[i] = NULL;
	}

	m_RoomList.clear();

	m_RoomLock.Release();
}

/**
 * 添加一个房间到房间管理器中
 *
 * @param pRoom 要添加的房间
 */
void RoomManager::AddRoom(CRoom *pRoom)
{
	if(pRoom == NULL) return;

	pRoom->SetID((int)m_RoomList.size());

	m_RoomLock.Acquire();
	m_RoomList.push_back(pRoom);
	m_RoomLock.Release();
}

/**
 * 改变一个玩家的游戏房间
 *
 * @param pPlayer 要改变游戏房间的玩家
 */
bool RoomManager::ChangeGameRoom(CPlayer *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	// 首先检测玩家当前的状态，如果玩家处于准备，等待，游戏中，掉线等状态时说明玩家已经在房间中，不能进入新的房间
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return false;

	// 第一遍搜索当前房间中已经存在玩家在那里等待的房间
	bool state = false;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter))
		{
			//(*iter)->UpdateAllPlayers();
			// 如果玩家不能满足房间要求的最少金币值，就不能进入房间
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble()) continue;

			if(!(*iter)->IsFull() &&
				(*iter)->GetPlayerCount() > 0 &&
				(*iter)->GetRoomState() != ROOMSTATE_GAMING)
			{
				// 找一个与原来不一样的房间
				if(pPlayer->GetRoomId() != (*iter)->GetID())
				{
					//PlayerState oldstate = pPlayer->GetState();

					// 设置玩家状态为改变房间状态，这样可以防止服务器将玩家强行掉线
					//pPlayer->SetState(PLAYERSTATE_CHANGEROOM);

					pPlayer->SetRoomId((*iter)->GetID());
					//pPlayer->SetState(oldstate);

					int playerIndex = (*iter)->AddPlayer(pPlayer);
					pPlayer->SetChairIndex(playerIndex);

					state = true;
					break;
				}
			}
		}
	}
	m_RoomLock.Release();

	if(state) return state;

	// 第二遍搜索当前房间中没有坐满的房间或者空房间
	state = false;

	m_RoomLock.Acquire();
	iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter))
		{
			//(*iter)->UpdateAllPlayers();
			// 如果玩家不能满足房间要求的最少金币值，就不能进入房间
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble()) continue;

			if(!(*iter)->IsFull() &&
				(*iter)->GetRoomState() != ROOMSTATE_GAMING)
			{
				// 找一个与原来不一样的房间
				if(pPlayer->GetRoomId() != (*iter)->GetID())
				{
					//PlayerState oldstate = pPlayer->GetState();

					// 设置玩家状态为改变房间状态，这样可以防止服务器将玩家强行掉线
					//pPlayer->SetState(PLAYERSTATE_CHANGEROOM);

					if(pPlayer == NULL)
					{
						state = false;
						break;
					}

					pPlayer->SetRoomId((*iter)->GetID());
					//pPlayer->SetState(oldstate);

					int playerIndex = (*iter)->AddPlayer(pPlayer);
					pPlayer->SetChairIndex(playerIndex);

					state = true;
					break;
				}
			}
		}
	}
	m_RoomLock.Release();

	return state;
}

/**
 * 添加一个旁观玩家到房间中
 *
 * @param pPlayer 要添加的玩家
 * @param roomIndex 玩家要进入的房间的索引
 * @param chairIndex 玩家要进入的房间的椅子的索引
 */
bool RoomManager::AddLookOnPlayer(CPlayer *pPlayer,int roomIndex,int chairIndex)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	// 首先检测玩家当前的状态，如果玩家处于准备，等待，游戏中，掉线等状态时说明玩家已经在房间中，不能进入新的房间
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return false;

	// 玩家要进入指定房间，指定的座位
	CRoom *pRoom = GetRoomById(roomIndex);
	if(pRoom == NULL) return false;

	// 如果房间已满或者房间状态处于游戏中的话，是进不了房间的
	if(/*pRoom->IsFull() ||*/
		pRoom->GetRoomState() != ROOMSTATE_GAMING)
		return false;

	// 检测房间中指定索引的玩家是否存在，如果存在是不能加入房间的
	if(!pRoom->GetPlayer(chairIndex))
		return false;

	pPlayer->SetRoomId(pRoom->GetID());
	pPlayer->SetChairIndex(pRoom->AddLookOnPlayer(pPlayer,chairIndex));
	pPlayer->SetLookOn(true);

	return true;
}

/**
 * 添加一个玩家到房间中
 *
 * @param pPlayer 要添加的玩家
 * @param roomIndex 玩家要进入的房间的索引
 * @param chairIndex 玩家要进入的房间的椅子的索引
 */
bool RoomManager::AddPlayer(CPlayer *pPlayer,int roomIndex,int chairIndex,bool isQueue,bool isGaming)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	// 首先检测玩家当前的状态，如果玩家处于准备，等待，游戏中，掉线等状态时说明玩家已经在房间中，不能进入新的房间
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return false;

	// 如果roomIndex等于-1,chairIndex等于-1表示当前玩家不需要排队进入房间
	if(roomIndex == -1 && chairIndex == -1)
	{
		//// 第一遍搜索当前房间中已经存在玩家在那里等待的房间
		std::map<int,CRoom*,std::greater<int> > pCurRoomList;

		m_RoomLock.Acquire();
		for(int index=(int)m_RoomList.size()-1;index >= 0;--index)
		{
			if(m_RoomList[index] == NULL) continue;
			if(isGaming && m_RoomList[index]->GetRoomState() == ROOMSTATE_GAMING) continue;

			int64 pEnterMinMoney,pEnterMaxMoney;
			pEnterMinMoney=pEnterMaxMoney=0;
			m_RoomList[index]->GetEnterMoneyRect(&pEnterMinMoney,&pEnterMaxMoney);

			// 如果玩家是机器人的话，必须进入普通的桌子，私人密码桌子是没法进入的
			if(/*pPlayer->GetType() == PLAYERTYPE_ROBOT && */
				!m_RoomList[index]->getEnterPassword().empty() &&
				pPlayer->getEnterPassword() != m_RoomList[index]->getEnterPassword())
				continue;

			// 如果玩家是机器人的话，进入设置范围的私人桌子时，玩家的钱必须满足设置的范围
			if(pEnterMinMoney > 0 &&
				pEnterMaxMoney > 0 &&
				/*pPlayer->GetType() == PLAYERTYPE_ROBOT && */
				(pPlayer->GetMoney() < pEnterMinMoney || pPlayer->GetMoney() > pEnterMaxMoney))
				continue;

			if(!m_RoomList[index]->IsFull())
				pCurRoomList[m_RoomList[index]->GetPlayerCount()]  = m_RoomList[index];
		}
		m_RoomLock.Release();

		if(pCurRoomList.empty()) return false;

		std::map<int,CRoom*,std::greater<int> >::iterator iterTwo = pCurRoomList.begin();
		if(iterTwo != pCurRoomList.end())
		{
			// 如果是排队状态下，必须选择一个有人的房间
			if(isQueue)
			{
				if((*iterTwo).second->GetPlayerCount() > 0)
				{
					pPlayer->SetRoomId(((*iterTwo).second)->GetID());
					int playerIndex = ((*iterTwo).second)->AddPlayer(pPlayer);
					pPlayer->SetChairIndex(playerIndex);

					return true;
				}

				return false;
			}

			pPlayer->SetRoomId(((*iterTwo).second)->GetID());
			int playerIndex = ((*iterTwo).second)->AddPlayer(pPlayer);
			pPlayer->SetChairIndex(playerIndex);

			return true;
		}

		return false;
	}
	else
	{
		// 玩家要进入指定房间，指定的座位
		CRoom *pRoom = GetRoomById(roomIndex);
		if(pRoom == NULL) return false;

		// 如果房间已满或者房间状态处于游戏中的话，是进不了房间的
		if(pRoom->IsFull())
			return false;

		if(isGaming && pRoom->GetRoomState() == ROOMSTATE_GAMING)
			return false;

		// 检测房间中指定索引的玩家是否存在，如果存在是不能加入房间的
		if(pRoom->GetPlayer(chairIndex))
			return false;

		// 如果用户带密码进入，如果房间不为空并且密码不对的话，不能进入房间
		if(!pPlayer->getEnterPassword().empty())
		{
			if(!pRoom->IsEmpty() && pRoom->getEnterPassword() != pPlayer->getEnterPassword())
				return false;
		}

		// 如果房间设置了金币限制，如果玩家金币不能满足限制也没法进入房间
		int64 pfirst,psecond;
		pfirst=psecond=0;
		pRoom->GetEnterMoneyRect(&pfirst,&psecond);

		if(pfirst > 0 && psecond > 0 && pfirst < psecond)
		{
			if(pPlayer->GetMoney() < pfirst ||
				pPlayer->GetMoney() > psecond)
				return false;
		}

		pPlayer->SetRoomId(pRoom->GetID());
		int playerIndex = pRoom->AddPlayer(pPlayer,chairIndex);
		if(playerIndex == -1)
		{
			LOG_ERROR("玩家%d在进入房间的时候失败了，这是个大问题！",pPlayer->GetID());
			return false;
		}
		pPlayer->SetChairIndex(playerIndex);

		// 如果是第一进入房间的就设置房间
		if(pRoom->GetPlayerCount() <= 1)
		{
			pRoom->SetEnterPassword(pPlayer->getEnterPassword());

			int64 pfirst,psecond;
			pfirst=psecond=0;
			pPlayer->GetEnterMoneyRect(&pfirst,&psecond);

			pRoom->SetEnterMoneyRect(pfirst,psecond);

			// 先向服务器中所有的在线玩家通告玩家进入游戏房间的消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_FRAME_ROOM_JOINSET;
            root["UserID"] = pRoom->GetID();
            root["EnterPassword"] = pRoom->getEnterPassword();
            root["first"] = (uint32)pfirst;
            root["second"] = (uint32)psecond;
			ServerPlayerManager.SendMsgToEveryone(root);
		}

		return true;
	}

	return false;
}

/**
 * 从房间中清除指定的玩家
 *
 * @param pPlayer 要清除的玩家
 */
void RoomManager::ClearPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter) &&
			(*iter)->IsExist(pPlayer))
		{
			(*iter)->ClearPlayer(pPlayer);
			break;
		}
	}
	m_RoomLock.Release();
}

/**
 * 从当前房间中删除指定的玩家
 *
 * @param pPlayer 要删除的玩家
 *
 * @return  如果玩家清除成功返回真，否则返回假
 */
bool RoomManager::DeletePlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL ||
		m_RoomList.empty())
		return false;

	bool state = false;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter) &&
			(*iter)->IsExist(pPlayer))
		{
			(*iter)->DeletePlayer(pPlayer);
			state = true;
			break;
		}
	}
	m_RoomLock.Release();

	return state;
}

/**
 * 根据房间ID号得到房间
 *
 * @param id 要得到的房间的ID
 *
 * @return 如果根据这个ID能得到这个房间，返回真个房间，否则返回NULL
 */
CRoom* RoomManager::GetRoomById(int id)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter)->GetID() == id)
		{
			pRoom = (*iter);
			break;
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// 得到一个空闲的房间
CRoom* RoomManager::GetFreeRoom(void)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();++iter)
	{
		if(!(*iter)->IsFull() &&
			(*iter)->GetRoomState() != ROOMSTATE_GAMING)
		{
			pRoom = (*iter);
			break;
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// 得到一个未满但不为空的房间
CRoom* RoomManager::GetUnderFullRoom(void)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	// 在所有房间中找寻人数最多的那个房间
	m_RoomLock.Acquire();
	int pPlayerCount = 0;
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();++iter)
	{
		if(!(*iter)->IsFull() &&
			(*iter)->GetPlayerCount() > 0 &&
			(*iter)->GetRoomState() != ROOMSTATE_GAMING)
		{
			if((*iter)->GetPlayerCount() > pPlayerCount)
			{
				pRoom = (*iter);
				pPlayerCount = (*iter)->GetPlayerCount();
			}
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// 得到一个空房间
CRoom* RoomManager::GetEmptyRoom(void)
{
	if(m_RoomList.empty()) return NULL;

	CRoom *pRoom = NULL;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter)->IsEmpty())
		{
			pRoom = (*iter);
			break;
		}
	}
	m_RoomLock.Release();

	return pRoom;
}

/// 得到当前房间中还有多少人
int RoomManager::GetRoomPlayerCount(void)
{
	if(m_RoomList.empty()) return 0;

	int Count = 0;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		Count += (*iter)->GetPlayerCount();
	}
	m_RoomLock.Release();

	return Count;
}

/**
 * 重置所有的游戏房间
 */
void RoomManager::ResetAllGameRooms(void)
{
	if(m_RoomList.empty())
		return;

	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();iter++)
	{
		if((*iter))
			(*iter)->ClearAllPlayers();
	}
	m_RoomLock.Release();
}

/**
 * 用于处理玩家改变游戏房间
 *
 * @param connId 要处理的网络连接的客户端连接ID
 */
void RoomManager::OnProcessChangeGameRoomMes(uint32 connId)
{
	CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
	if(pPlayer != NULL)
	{
		if(ChangeGameRoom(pPlayer))
		{
			// 先向服务器中所有的在线玩家通告玩家进入游戏房间的消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
            root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
            root["UserID"] = pPlayer->GetID();
            root["RoomId"] = pPlayer->GetRoomId();
            root["ChairIndex"] = pPlayer->GetChairIndex();
			ServerPlayerManager.SendMsgToEveryone(root);
		}
		else
		{
			// 向当前玩家发送进入游戏房间失败的消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
            root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
            root["UserID"] = pPlayer->GetID();
			Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
	}
}

/**
 * 用于处理用于断开网络连接消息
 *
 * @param connId 要断开网络连接的客户端连接ID
 */
void RoomManager::OnProcessDisconnectNetMes(uint32 connId)
{
	// 首先根据用户连接ID得到用户，然后根据用户信息得到房间信息，最后交给房间处理
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL)
	{
		// 如果用户没有进入任何房间的话，删除这个玩家
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
        root["UserID"] = pPlayer->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);

		uint32 pConnID = pPlayer->GetConnectID();
		ServerPlayerManager.ClearPlayer(pPlayer);
		Disconnect(pConnID);
		return;
	}

	bool isGaming = (pPlayer->GetState() == PLAYERSTATE_GAMING);

	if(isGaming)
	{
		pPlayer->SetState(PLAYERSTATE_LOSTLINE);

		//更新玩家状态
		ServerDBOperator.SetPlayerGameState(pPlayer);

		// 先从玩家列表中摘除这个玩家
		ServerPlayerManager.DeletePlayer(pPlayer);
		// 然后加入到掉线列表里
		ServerPlayerManager.AddLostPlayer(pPlayer);

		// 向房间所有玩家广播玩家离开房间消息
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_FALLLINE_ROOM;
        root["UserID"] = pPlayer->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);
	}
	else
	{
		// 如果是断线状态，直接退出
		if(pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
		{
			//mole2d::network::System_Log("断开连接的时候，玩家不能处于断线状态！");

			// 先从玩家列表中摘除这个玩家
			ServerPlayerManager.DeletePlayer(pPlayer);
			// 然后加入到掉线列表里
			ServerPlayerManager.AddLostPlayer(pPlayer);

			return;
		}

		// 向房间所有玩家广播玩家离开服务器消息
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
        root["UserID"] = pPlayer->GetID();
		ServerPlayerManager.SendMsgToEveryone(root);

		m_RoomLock.Acquire();

		pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());
		pRoom->ClearPlayer(pPlayer);

		uint32 pConnID = pPlayer->GetConnectID();
		ServerPlayerManager.ClearPlayer(pPlayer);
		Disconnect(pConnID);

        // 准备好了的用户是否继续开始游戏
        pRoom->OnProcessContinueGaming();

		m_RoomLock.Release();
		return;
	}

	m_RoomLock.Acquire();
	pRoom->OnProcessOfflineRoomMes(pPlayer->GetChairIndex());

	//if(pPlayer->GetState() != PLAYERSTATE_LOSTLINE)
	//	pRoom->ClearPlayer(pPlayer);

	//if(m_ServerSet.GameType != ROOMTYPE_BISAI)
	//{
	//	// 如果房间中的用户全部掉线，就清空房间状态
	//	if(pRoom->GetPlayerCount(PLAYERSTATE_LOSTLINE) == pRoom->GetMaxPlayer())
	//	{
	//		pRoom->GameEnd();
	//		//pRoom->AllPlayerLeveaServer();
	//	}
	//}
	m_RoomLock.Release();
}

/**
 * 用于处理接收到的网络消息
 *
 * @param connId 要处理的客户端
 * @param mes 要处理的客户端的消息
 */
void RoomManager::OnProcessNetMes(uint32 connId,Json::Value &mes)
{
	if(m_RoomList.empty()) return;

	Player *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	m_RoomLock.Acquire();
	pRoom->OnProcessNetMes(connId,mes);
	m_RoomLock.Release();
}

///时间事件
bool RoomManager::OnEventTimer(uint32 dwTimerID, uint32 wBindParam)
{
	m_RoomLock.Acquire();
	std::vector<CRoom*>::iterator iter = m_RoomList.begin();
	for(;iter != m_RoomList.end();++iter)
	{
		if((*iter) == NULL) continue;

		(*iter)->OnEventTimer(dwTimerID,wBindParam);
	}
	m_RoomLock.Release();

	return true;
}
