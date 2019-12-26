#include "PlayerManager.h"
#include "CRoom.h"
#include "RoomManager.h"
#include "DBOperator.h"
#include "../GameFrameManager.h"

#include "../../include/Common/defines.h"

initialiseSingleton(PlayerManager);

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{
	ClearAllPlayers();
}

/**
 * 清除所有的玩家
 */
void PlayerManager::ClearAllPlayers(void)
{
	if(!m_PlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
		for(;iter != m_PlayerList.end();++iter)
		{
			if((*iter).second)
			{
				// 更新玩家游戏状态
				(*iter).second->setCurGameID(0);
				(*iter).second->setCurServerId(0);
				(*iter).second->setCurChairIndex(-1);
				(*iter).second->setCurTableIndex(-1);
				(*iter).second->setCurGamingState(false);
				ServerDBOperator.SetPlayerGameState((*iter).second);
			}

			if((*iter).second) delete (*iter).second;
			(*iter).second = NULL;
		}

		m_PlayerList.clear();
	}

	if(!m_RobotList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_RobotList.begin();
		for(;iter != m_RobotList.end();++iter)
		{
			if((*iter).second)
			{
				// 更新玩家游戏状态
				(*iter).second->setCurGameID(0);
				(*iter).second->setCurServerId(0);
				(*iter).second->setCurChairIndex(-1);
				(*iter).second->setCurTableIndex(-1);
				(*iter).second->setCurGamingState(false);
				ServerDBOperator.SetPlayerGameState((*iter).second);
			}

			if((*iter).second) delete (*iter).second;
			(*iter).second = NULL;
		}

		m_RobotList.clear();
	}

	if(!m_LostPlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.begin();
		for(;iter != m_LostPlayerList.end();++iter)
		{
			if((*iter).second)
			{
				// 更新玩家游戏状态
				(*iter).second->setCurGameID(0);
				(*iter).second->setCurServerId(0);
				(*iter).second->setCurChairIndex(-1);
				(*iter).second->setCurTableIndex(-1);
				(*iter).second->setCurGamingState(false);
				ServerDBOperator.SetPlayerGameState((*iter).second);
			}

			if((*iter).second) delete (*iter).second;
			(*iter).second = NULL;
		}

		m_LostPlayerList.clear();
	}
}

/**
 * 添加一个玩家到系统中
 *
 * @param pPlayer 要添加到系统中的玩家
 */
bool PlayerManager::AddPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	// 检测玩家是否已经存在于服务器中，如果存在就不加入了
	if(GetPlayer(pPlayer->GetConnectID()))
	{
		//mole2d::network::System_Log("添加玩家出现重号的情况了。");
		return false;
	}

	m_PlayersLock.Acquire();
	m_PlayerList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetConnectID(),pPlayer));
	m_PlayersLock.Release();

	return true;
}

/// 添加一个机器人到系统中
bool PlayerManager::AddRobot(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return false;

	// 检测玩家是否已经存在于服务器中，如果存在就不加入了
	if(GetPlayer(pPlayer->GetConnectID()))
	{
		//mole2d::network::System_Log("添加机器人出现重号的情况了。");
		return false;
	}

	m_PlayersLock.Acquire();
	m_RobotList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetConnectID(),pPlayer));
	m_PlayersLock.Release();

	return true;
}

///**
// * 从系统中删除指定网络ID的玩家
// *
// * @param connId 要删除的客户端的ID
// */
//void PlayerManager::ClearPlayerByconnId(uint32 connId)
//{
//	if(m_PlayerList.empty() || connId == NULL) return;
//
//	m_PlayersLock.Acquire();
//	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(connId);
//	if(iter != m_PlayerList.end())
//	{
//		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
//		if(pPlayer == NULL)
//		{
//			m_PlayersLock.Release();
//			return;
//		}
//
//		if(pPlayer->GetConnectID() == connId)
//		{
//			delete pPlayer;
//			pPlayer = NULL;
//
//			m_PlayerList.erase(iter);
//		}
//	}
//	m_PlayersLock.Release();
//}

/**
 * 通过客户端连接ID得到客户端
 *
 * @param connId 要得到的客户端的ID
 *
 * @return 如果这个客户端存在返回这个客户端，否则返回NULL
 */
CPlayer* PlayerManager::GetPlayer(uint32 connId)
{
	if(connId <= 0) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();
	bool isFinded=false;

	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(connId);
	if(iter != m_PlayerList.end())
	{
		if((*iter).second &&
			(*iter).second->GetConnectID() == connId)
		{
			pPlayer = (*iter).second;
			isFinded=true;
		}
	}

	if(isFinded == false)
	{
		//搜索所有机器人
		iter = m_RobotList.find(connId);
		if(iter != m_RobotList.end())
		{
			if((*iter).second &&
				(*iter).second->GetConnectID() == connId)
			{
				pPlayer = (*iter).second;
				isFinded=true;
			}
		}
	}

	if(isFinded == false && !m_LostPlayerList.empty())
	{
		//搜索所有掉线玩家
		std::map<uint32,CPlayer*>::iterator iterLost = m_LostPlayerList.begin();
		for(;iterLost != m_LostPlayerList.end();++iterLost)
		{
			if((*iterLost).second &&
				(*iterLost).second->GetConnectID() == connId)
			{
				pPlayer = (*iterLost).second;
				isFinded=true;
			}
		}
	}
	m_PlayersLock.Release();

	return pPlayer;
}

/// 得到空闲机器人个数
int PlayerManager::GetFreeRobotCount(void)
{
	if(m_RobotList.empty()) return 0;

	int pcount = 0;

	m_PlayersLock.Acquire();

	std::map<uint32,CPlayer*>::iterator freerobotiter = m_RobotList.begin();      /**< 用于随机抽取空闲机器人 */
	for(;freerobotiter != m_RobotList.end();++freerobotiter)
	{
		if((*freerobotiter).second &&
			(*freerobotiter).second->GetState() == PLAYERSTATE_NORAML &&
			(*freerobotiter).second->GetRoomId() == -1)
		{
			pcount+=1;
		}
	}

	m_PlayersLock.Release();

	return pcount;
}

/**
 * 得到空闲的机器人
 */
CPlayer* PlayerManager::GetFreeRobot(void)
{
	if(m_RobotList.empty()) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();

	std::vector<CPlayer*> probotList;

	std::map<uint32,CPlayer*>::iterator freerobotiter = m_RobotList.begin();      /**< 用于随机抽取空闲机器人 */
	for(;freerobotiter != m_RobotList.end();++freerobotiter)
	{
		if((*freerobotiter).second &&
			(*freerobotiter).second->GetState() == PLAYERSTATE_NORAML &&
			(*freerobotiter).second->GetRoomId() == -1)
		{
			probotList.push_back((*freerobotiter).second);
		}
	}

	if(!probotList.empty())
	{
		pPlayer = probotList[rand()%(int)probotList.size()];
	}

	m_PlayersLock.Release();

	return pPlayer;
}

/**
 * 通过客户端连接ID得到刚登陆的客户端
 *
 * @param connId 要得到的客户端的ID
 *
 * @return 如果这个客户端存在返回这个客户端，否则返回NULL
 */
CPlayer* PlayerManager::GetNewPlayer(uint32 connId)
{
	if(connId <= 0) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();
	bool isFinded = false;
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(connId);
	if(iter != m_PlayerList.end())
	{
		CPlayer *tmpPlayer = (*iter).second;
		if( tmpPlayer &&
			tmpPlayer->GetConnectID() == connId &&
			tmpPlayer->GetState() == PLAYERSTATE_NORAML)
		{
			pPlayer = (*iter).second;
			isFinded = true;
		}
	}

	// 查找机器人
	if(isFinded == false)
	{
		iter = m_RobotList.find(connId);
		if(iter != m_RobotList.end())
		{
			CPlayer *tmpPlayer = (*iter).second;
			if( tmpPlayer &&
				tmpPlayer->GetConnectID() == connId &&
				tmpPlayer->GetState() == PLAYERSTATE_NORAML)
			{
				pPlayer = (*iter).second;
				isFinded = true;
			}
		}
	}
	m_PlayersLock.Release();

	return pPlayer;
}

/**
 * 通过玩家ID得到客户端
 *
 * @param id 要得到的用户的ID
 *
 * @return 如果这个指定ID的用户存在返回这个客户端，否则返回NULL
 */
CPlayer* PlayerManager::GetPlayerById(uint32 id)
{
	if(id <= 0) return NULL;

	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();
	bool isFinded = false;
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		if((*iter).second &&
			(*iter).second->GetID() == id)
		{
			pPlayer = (*iter).second;
			isFinded = true;
			break;
		}
	}

	// 查找机器人
	if(isFinded == false)
	{
		iter = m_RobotList.begin();
		for(;iter != m_RobotList.end();++iter)
		{
			if((*iter).second &&
				(*iter).second->GetID() == id)
			{
				pPlayer = (*iter).second;
				isFinded = true;
				break;
			}
		}
	}
	m_PlayersLock.Release();

	return pPlayer;
}

/// 从系统中摘除指定的玩家
void PlayerManager::DeletePlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL || m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(pPlayer->GetConnectID());
	if(iter != m_PlayerList.end())
	{
		m_PlayerList.erase(iter);
	}
	m_PlayersLock.Release();
}

/**
 * 从系统中删除指定的玩家
 *
 * @param pPlayer 要删除的玩家
 */
void PlayerManager::ClearPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	// 更新玩家游戏状态
	pPlayer->setCurGameID(0);
	pPlayer->setCurServerId(0);
	pPlayer->setCurChairIndex(-1);
	pPlayer->setCurTableIndex(-1);
	pPlayer->setCurGamingState(false);
	pPlayer->SetState(PLAYERSTATE_NORAML);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	m_PlayersLock.Acquire();

	if(!m_LostPlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(pPlayer->GetID());
		if(iter != m_LostPlayerList.end())
		{
			CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
			if(pPlayer != NULL)
			{
				SafeDelete(pPlayer);
				pPlayer = NULL;
			}

			m_LostPlayerList.erase(iter);

			m_PlayersLock.Release();

            //更新服务器在线人数
            ServerGameFrameManager.UpdateGameRoomInfo();

			return;
		}
	}

	if(!m_PlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.find(pPlayer->GetConnectID());
		if(iter != m_PlayerList.end())
		{
			CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
			if(pPlayer != NULL)
			{
				SafeDelete(pPlayer);
				pPlayer = NULL;
			}

			m_PlayerList.erase(iter);

			m_PlayersLock.Release();

			//更新服务器在线人数
            ServerGameFrameManager.UpdateGameRoomInfo();

			return;
		}
	}

	if(!m_RobotList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_RobotList.find(pPlayer->GetConnectID());
		if(iter != m_RobotList.end())
		{
			CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
			if(pPlayer != NULL)
			{
				SafeDelete(pPlayer);
				pPlayer = NULL;
			}

			m_RobotList.erase(iter);

			m_PlayersLock.Release();

			//更新服务器在线人数
            ServerGameFrameManager.UpdateGameRoomInfo();

			return;
		}
	}

	m_PlayersLock.Release();
}

/**
 * 根据玩家的连接ID设置玩家
 *
 * @param pPlayer 要重新设置的玩家数据
 */
void PlayerManager::Reset(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator newiter = m_PlayerList.find(pPlayer->GetConnectID());
	if(newiter == m_PlayerList.end())
	{
		m_PlayerList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetConnectID(),pPlayer));
	}
	m_PlayersLock.Release();
}

/**
 * 发送消息给当前在线的所有玩家
 *
 * @param msg 要发送的消息
 */
void PlayerManager::SendMsgToEveryone(Json::Value &msg)
{
	if(m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
		if(pPlayer == NULL)
			continue;

		if(IsConnected(pPlayer->GetConnectID()) == false)
			continue;

		Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	}
	m_PlayersLock.Release();
}

/**
 * 发送消息给除了指定玩家的所有在线玩家
 *
 * @param connId 要排除在外的玩家的连接ID
 * @param msg 要发送的消息
 */
void PlayerManager::SendMsgToOtherPlayer(uint32 connId,Json::Value &msg)
{
	if(m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
		if(pPlayer == NULL || pPlayer->GetConnectID() == connId) continue;

		if(IsConnected(pPlayer->GetConnectID()) == false)
			continue;

		Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	}
	m_PlayersLock.Release();
}

/// 发送消息给指定ID的玩家
void PlayerManager::SendMsgToPlayer(uint32 UserID,Json::Value &msg)
{
	if(m_PlayerList.empty() || UserID <= 0) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		CPlayer *pPlayer = dynamic_cast<CPlayer*>((*iter).second);
		if(pPlayer == NULL || pPlayer->GetID() != UserID) continue;

		if(IsConnected(pPlayer->GetConnectID()) == false)
			continue;

		Sendhtml5(pPlayer->GetConnectID(),(const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
		break;
	}
	m_PlayersLock.Release();
}

/// 得到玩家总的输赢
int64 PlayerManager::GetPlayerTotalResult(void)
{
	int64 totalResult = 0;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		totalResult+=(*iter).second->GetTotalResult();
	}
	m_PlayersLock.Release();

	return totalResult;
}

/// 得到机器人总的输赢
int64 PlayerManager::GetRobotTotalResult(void)
{
	int64 totalResult = 0;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_RobotList.begin();
	for(;iter != m_RobotList.end();++iter)
	{
		totalResult+=(*iter).second->GetTotalResult();
	}
	m_PlayersLock.Release();

	return totalResult;
}

///时间事件
bool PlayerManager::OnEventTimer(uint32 dwTimerID, uint32 wBindParam)
{
	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_RobotList.begin();
	for(;iter != m_RobotList.end();++iter)
	{
		if((*iter).second == NULL) continue;

		(*iter).second->OnEventTimer(dwTimerID,wBindParam);
	}
	m_PlayersLock.Release();

	return true;
}

/// 给所有在线玩家提示座位信息
void PlayerManager::UpdatePlayersTableInfo(void)
{
	if(m_PlayerList.empty()) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		if((*iter).second == NULL) continue;
		if((*iter).second->GetRoomId() < 0 || (*iter).second->GetChairIndex() < 0 || (*iter).second->GetState() != PLAYERSTATE_READY)
			continue;

		// 准备时间必须超过20秒
		if((DWORD)time(NULL) - (*iter).second->GetReadyTime() < 20)
			continue;

		CRoom *pRoom = ServerRoomManager.GetUnderFullRoom();
		if(pRoom == NULL) break;

        Json::Value out;
        out["MsgId"] = IDD_MESSAGE_FRAME;
        out["MsgSubId"] = IDD_MESSAGE_FRAME_ROOM_UNDERFULL;
        out["RoomID"] = pRoom->GetID();
        out["ChairCount"] = pRoom->GetMaxPlayer() - pRoom->GetPlayerCount();
		Sendhtml5((*iter).second->GetConnectID(),(const char*)out.toStyledString().c_str(),out.toStyledString().length());
	}
	m_PlayersLock.Release();
}

/// 添加一个玩家到掉线列表中
void PlayerManager::AddLostPlayer(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(pPlayer->GetID());
	if(iter == m_LostPlayerList.end())
	{
		// 先保存原来老的连接ID
		pPlayer->SetOffLineConnectId(pPlayer->GetConnectID());

		// 玩家连接ID至零
		pPlayer->SetConnectID(IDD_LOSTONLINE_SOCKET_START+pPlayer->GetID());

		m_LostPlayerList.insert(std::pair<uint32,CPlayer*>(pPlayer->GetID(),pPlayer));
	}
	m_PlayersLock.Release();
}

/// 根据ID得到掉线玩家
CPlayer* PlayerManager::GetLostPlayer(uint32 userId)
{
	CPlayer *pPlayer = NULL;

	m_PlayersLock.Acquire();

	bool isfinded = false;

	if(!m_LostPlayerList.empty())
	{
		std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(userId);
		if(iter != m_LostPlayerList.end())
		{
			pPlayer = (*iter).second;
			isfinded = true;
		}
	}

	//if(!isfinded)
	//{
	//	// 如果掉线列表中没有找到，就要去在线玩家列表中找了，因为有可能出现
	//	// 玩家由于网络慢的情况下，玩家卡死在在线玩家列表中，而没有出现在掉线列表中
	//	if(!m_PlayerList.empty())
	//	{
	//		std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	//		for(;iter != m_PlayerList.end();++iter)
	//		{
	//			if((*iter).second == NULL || (*iter).second->GetID() != userId)
	//				continue;

	//			if((*iter).second->GetState() == PLAYERSTATE_LOSTLINE)
	//			{
	//				pPlayer = (*iter).second;
	//				isfinded = true;
	//				break;
	//			}
	//		}
	//	}

	//}

	m_PlayersLock.Release();

	return pPlayer;
}

/// 根据ID删除掉线玩家
void PlayerManager::DeleteLostPlayer(uint32 userId)
{
	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_LostPlayerList.find(userId);
	if(iter != m_LostPlayerList.end())
	{
		m_LostPlayerList.erase(iter);
	}
	m_PlayersLock.Release();
}

/// 得到检测到的已经掉线的玩家列表
void PlayerManager::GetRealLostPlayerList(void)
{
	std::vector<uint32> pTmpLostPlayers;

	m_PlayersLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerList.begin();
	for(;iter != m_PlayerList.end();++iter)
	{
		if((*iter).second == NULL) continue;
		if(IsConnected((*iter).second->GetConnectID())) continue;

		pTmpLostPlayers.push_back((*iter).second->GetConnectID());
	}

	for(int i=0;i<(int)pTmpLostPlayers.size();i++)
	{
		ServerGameFrameManager.OnProcessDisconnectNetMes(pTmpLostPlayers[i]);
	}
	m_PlayersLock.Release();
}
