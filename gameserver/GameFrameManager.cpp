#include "GameFrameManager.h"
#include "../include/Common/defines.h"
#include "../../include/molnet/Network.h"
#include "gameframe/DBOperator.h"
#include "gameframe/RoomManager.h"
#include "gameframe/PlayerManager.h"

#include <map>
#include <sstream>

#define IDD_SEND_PLAYER_MAX_COUNT         30               // 一次最多发送多少个人的数据


initialiseSingleton(GameFrameManager);

/**
 * 构造函数
 */
GameFrameManager::GameFrameManager()
{
}

/**
 * 析构函数
 */
GameFrameManager::~GameFrameManager()
{
}

/**
 * 用于处理接收到的网络消息
 *
 * @param connId 要处理的客户端的网络ID
 * @param mes 接收到的客户端的消息
 */
void GameFrameManager::OnProcessNetMes(uint32 connId,CMolMessageIn *mes)
{
	if(mes == NULL) return;

    Json::Reader reader;
    Json::Value json_object;

    if (!reader.parse(mes->getData(), json_object))
    {
        LOG_ERROR("数据解析失败:%s",mes->getData());
        return;
    }

	switch(json_object["MsgId"].asInt())
	{
	case IDD_MESSAGE_FRAME:                            // 框架消息
		{
			OnProcessFrameMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_GAME_LOGIN:                       // 登录消息
		{
			OnProcessUserLoginMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_ROOM:                             // 房间消息
		{
			ServerRoomManager.OnProcessNetMes(connId,json_object);
		}
		break;
	default:
		break;
	}
}

/**
 * 用于处理接收网络连接消息
 *
 * @param connId 要处理的客户端的网络ID
 */
void GameFrameManager::OnProcessConnectedNetMes(uint32 connId)
{
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_CONNECT;
    root["MsgSubId"] = IDD_MESSAGE_CONNECT_SUCESS;

    Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
}

/**
 * 用于处理用于断开网络连接消息
 *
 * @param connId 要处理的客户端的网络ID
 */
void GameFrameManager::OnProcessDisconnectNetMes(uint32 connId)
{
    DelQueueList(connId);
	ServerRoomManager.OnProcessDisconnectNetMes(connId);
}

CPlayer* GameFrameManager::CreateNewPlayer(void)
{
	return new CPlayer(PLAYERTYPE_ROBOT);
}

/// 更新玩家信息
void GameFrameManager::UpdatePlayerInfo(Player *pPlayer)
{
	if(pPlayer == NULL) return;

	Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_UPDATE_USER_DATA;
    root["ID"] = pPlayer->GetID();
    root["State"] = pPlayer->GetState();
    root["Type"] = pPlayer->GetType();
    root["RoomId"] = pPlayer->GetRoomId();
    root["ChairIndex"] = pPlayer->GetChairIndex();
    root["Name"] = pPlayer->GetName();
    root["Level"] = pPlayer->GetLevel();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Revenue"] = (uint32)pPlayer->GetRevenue();
    root["TotalResult"] = (uint32)pPlayer->GetTotalResult();
    root["Experience"] = pPlayer->GetExperience();
    root["TotalBureau"] = pPlayer->GetTotalBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["FailBureau"] = pPlayer->GetFailBureau();
    root["RunawayBureau"] = pPlayer->GetRunawayBureau();
    root["SuccessRate"] = pPlayer->GetSuccessRate();
    root["Runawayrate"] = pPlayer->GetRunawayrate();

	ServerPlayerManager.SendMsgToEveryone(root);
}

/**
 * 处理用户登录系统消息
 *
 * @param connId 要处理的客户端
 * @param mes 要处理的客户端的消息
 */
void GameFrameManager::OnProcessUserLoginMes(uint32 connId,Json::Value &mes)
{
	std::string pUserName = mes["UserName"].asString();
	std::string pUserPW = mes["UserPW"].asString();
	int pDeviceType = mes["DeviceType"].asInt();

	uint32 pUserId = ServerDBOperator.IsExistUser(pUserName.c_str(),pUserPW.c_str());

	if(pUserId <= 0)
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	//检查当前人数是否超过指定在线人数
	if(ServerPlayerManager.GetPlayerCount() >= m_ServerSet.TableCount * m_ServerSet.PlayerCount)
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FULL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	UserDataStru pUserData;
	if(!ServerDBOperator.GetUserData(pUserId,pUserData))
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
        root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	// 首先查找当前玩家列表中是否存在这个用户，如果存在,并且用户处于掉线状态下，就处理用户的掉线重连
	CPlayer *pPlayer = ServerPlayerManager.GetLostPlayer(pUserId);
	if(pPlayer)
	{
		// 设置设备类型
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		if(pPlayer->GetState() == PLAYERSTATE_LOSTLINE)
		{
			pPlayer->SetConnectID(connId);

			// 先从掉线列表中清除这个玩家
			ServerPlayerManager.DeleteLostPlayer(pPlayer->GetID());

			// 然后重新加入到玩家列表中
			ServerPlayerManager.Reset(pPlayer);

			// 向玩家发送成功登录服务器消息
			SendPlayerLoginSuccess(pPlayer);
			return;
		}
	}

	pPlayer = ServerPlayerManager.GetPlayerById(pUserId);
	if(pPlayer)
	{
		// 设置设备类型
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		if(pPlayer->GetState() != PLAYERSTATE_LOSTLINE)
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_EXIST;
            root["ServerPort"] = m_ServerSet.m_iServerPort;
            root["GameType"] = m_ServerSet.m_GameType;
            root["GameState"] = 1;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			return;
		}
	}
	else
	{
		// 检测用户是否在另一个游戏服务器中
		uint32 pserverid,pgametype;
		int32 proomid,pchairid;
		pserverid=proomid=pchairid=pgametype=0;
		if(ServerDBOperator.IsExistUserGaming(pUserId,&pserverid,&proomid,&pchairid,&pgametype) &&
			(pserverid > 0 || proomid > -1 || pchairid > -1 || pgametype > 0))
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_EXIST;
            root["ServerPort"] = pserverid;
            root["GameType"] = pgametype;
            root["GameState"] = 2;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			return;
		}

		CPlayer *pPlayer = new CPlayer(pUserId,connId);
		pPlayer->SetName(pUserName.c_str());
		pPlayer->SetMoney(pUserData.Money);
		pPlayer->SetBankMoney(pUserData.BankMoney);
		pPlayer->SetRevenue(pUserData.Revenue);
		pPlayer->SetTotalResult(pUserData.TotalResult);
		pPlayer->SetLevel(pUserData.Level);
		pPlayer->SetExperience(pUserData.Experience);
//		pPlayer->SetUserAvatar(pUserData.UserAvatar);
		pPlayer->SetTotalBureau(pUserData.TotalBureau);
		pPlayer->SetSuccessBureau(pUserData.SBureau);
		pPlayer->SetFailBureau(pUserData.FailBureau);
		pPlayer->SetRunawayBureau(pUserData.RunawayBureau);
		pPlayer->SetSuccessRate(pUserData.SuccessRate);
		pPlayer->SetRunawayrate(pUserData.RunawayRate);
		pPlayer->SetSex(pUserData.sex);
		pPlayer->SetSysType(pUserData.gType);
		pPlayer->SetRealName(pUserData.realName);
		pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));
		pPlayer->SetDeviceType((PlayerDeviceType)pDeviceType);

		// 如果登录成功，将玩家加入到服务器中
		if(ServerPlayerManager.AddPlayer(pPlayer))
		{
			SendPlayerLoginSuccess(pPlayer);
		}
		else
		{
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
            root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_FAIL;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
	}
}

/// 发送指定玩家登陆成功的消息
void GameFrameManager::SendPlayerLoginSuccess(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	pPlayer->setCurGameID(m_ServerSet.m_GameType);
	pPlayer->setCurServerId(m_ServerSet.m_iServerPort);
	ServerDBOperator.SetPlayerGameState(pPlayer);

	// 向玩家发送成功登录服务器消息
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_GAME_LOGIN;
    root["MsgSubId"] = IDD_MESSAGE_GAME_LOGIN_SUCESS;
    root["ID"] = pPlayer->GetID();
    root["State"] = pPlayer->GetState();
    root["Type"] = pPlayer->GetType();
    root["RoomId"] = pPlayer->GetRoomId();
    root["ChairIndex"] = pPlayer->GetChairIndex();
    root["Name"] = pPlayer->GetName();
    //root["UserAvatar"] = pPlayer->GetUserAvatar();
    root["Level"] = pPlayer->GetLevel();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Revenue"] = (uint32)pPlayer->GetRevenue();
    root["TotalResult"] = (uint32)pPlayer->GetTotalResult();
    root["Experience"] = pPlayer->GetExperience();
    root["TotalBureau"] = pPlayer->GetTotalBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["FailBureau"] = pPlayer->GetFailBureau();
    root["SuccessBureau"] = pPlayer->GetSuccessBureau();
    root["SuccessRate"] = pPlayer->GetSuccessRate();
    root["Runawayrate"] = pPlayer->GetRunawayrate();
    root["Sex"] = pPlayer->GetSex();
    root["RealName"] = pPlayer->GetRealName();
    //root["LoginIP"] = pPlayer->GetLoginIP();
    root["DeviceType"] = pPlayer->GetDeviceType();
    root["CurGameID"] = pPlayer->getCurGameID();
    root["CurServerId"] = pPlayer->getCurServerId();

    in_addr inaddr;
    inaddr.s_addr=pPlayer->GetLoginIP();
    root["LoginIP"] = inet_ntoa(inaddr);

	ServerPlayerManager.SendMsgToEveryone(root);
}

/**
 * 先处理游戏框架消息
 *
 * @param connId 要处理的客户端
 * @param mes 要处理的客户端的消息
 */
void GameFrameManager::OnProcessFrameMes(uint32 connId,Json::Value &mes)
{
	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_LOOKON_ENTER_ROOM:              // 旁观进入游戏房间
		{
			CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
			if(pPlayer == NULL) return;

			// 检测用户是否在另一个游戏服务器中
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;
			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.m_iServerPort != pserverid || m_ServerSet.m_GameType != pgametype))
			{
				Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["gametype"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			int pRoomIndex = mes["RoomIndex"].asInt();
			int pChairIndex = mes["ChairIndex"].asInt();

			if(pRoomIndex >= 0 && pChairIndex >= 0)
			{
				if(ServerRoomManager.AddLookOnPlayer(pPlayer,pRoomIndex,pChairIndex))
				{
					// 先向服务器中所有的在线玩家通告玩家进入游戏房间的消息
                    Json::Value root;
                    root["MsgId"] = IDD_MESSAGE_FRAME;
                    root["MsgSubId"] = IDD_MESSAGE_LOOKON_ENTER_ROOM;
                    root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
                    root["ID"] = pPlayer->GetID();
                    root["RoomId"] = pPlayer->GetRoomId();
                    root["ChairIndex"] = pPlayer->GetChairIndex();

					ServerPlayerManager.SendMsgToEveryone(root);

					CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
					if(pRoom) pRoom->OnProcessEnterRoomMsg(pPlayer->GetChairIndex());

					return;
				}
			}

			// 向当前玩家发送进入游戏房间失败的消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LOOKON_ENTER_ROOM;
            root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
            root["ID"] = pPlayer->GetID();
            Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
		break;
	case IDD_MESSAGE_ENTER_ROOM:                     // 进入游戏房间
		{
			CPlayer *pPlayer = ServerPlayerManager.GetNewPlayer(connId);
			if(pPlayer == NULL)
				return;

			// 检测玩家是否已经在房间中了，如果在房间中，是不能进入房间的
			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom != NULL)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = 0;
                root["GameType"] = m_ServerSet.m_GameType;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			int pRoomIndex = mes["RoomIndex"].asInt();
			int pChairIndex = mes["ChairIndex"].asInt();
			std::string pEnterPWd = mes["EnterPWd"].asString();
			int64 pEnterfirst = mes["Enterfirst"].asInt();
			int64 pEntersecond = mes["Entersecond"].asInt();

			pPlayer->SetEnterPassword(pEnterPWd);
			pPlayer->SetEnterMoneyRect(pEnterfirst,pEntersecond);

			if(pRoomIndex < 0 || pRoomIndex >= 65535) pRoomIndex = -1;
			if(pChairIndex < 0 || pChairIndex >= 65535) pChairIndex = -1;

			// 检测用户是否在另一个游戏服务器中
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;
			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.m_iServerPort != pserverid || m_ServerSet.m_GameType != pgametype))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["GameType"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			// 更新用户身上的钱和其它东西
			OnProcessUserInfo(pPlayer);

			// 检查用户的金币是否满足房间要求
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_LASTMONEY;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			if(m_ServerSet.m_QueueGaming)
				AddQueueList(pPlayer);                // 排队进入
			else
                JoinPlayerToGameRoom(pPlayer,pRoomIndex,pChairIndex,m_ServerSet.m_QueueGaming);
		}
		break;
	case IDD_MESSAGE_CHANGER_ROOM:                  // 交换游戏房间
		{
			CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
			if(pPlayer == NULL) return;

			// 首先脚本处理了用户离开游戏房间
			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom == NULL) return;

			// 如果玩家在游戏中是不能换桌的
			if(pPlayer->GetState() == PLAYERSTATE_GAMING)
				return;

			// 如果用户不是旁观用户，那么就先结束游戏
			if(!pPlayer->IsLookOn())
			{
				pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());
			}

			// 向当前服务器中所有用户发送玩家离开房间消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
            root["UserID"] = pPlayer->GetID();

			ServerPlayerManager.SendMsgToEveryone(root);

			// 从房间中清除这个用户
			pRoom->ClearPlayer(pPlayer);

			// 设置玩家的状态为离开房间
			pPlayer->SetRoomId(-1);
			pPlayer->SetChairIndex(-1);

			// 准备好了的用户是否继续开始游戏
			pRoom->OnProcessContinueGaming();

			// 检测用户是否在另一个游戏服务器中
			uint32 pserverid,pgametype;
			int32 proomid,pchairid;

			pserverid=proomid=pchairid=pgametype=0;
			if(ServerDBOperator.IsExistUserGaming(pPlayer->GetID(),&pserverid,&proomid,&pchairid,&pgametype) &&
				(m_ServerSet.m_iServerPort != pserverid || m_ServerSet.m_GameType != pgametype))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_EXIST;
                root["serverid"] = pserverid;
                root["GameType"] = pgametype;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			// 更新用户身上的钱和其它东西
			OnProcessUserInfo(pPlayer);

			// 检查用户的金币是否满足房间要求
			if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
				pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_LASTMONEY;
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			if(m_ServerSet.m_QueueGaming)
				AddQueueList(pPlayer);                // 排队进入
			else
                JoinPlayerToGameRoom(pPlayer,-1,-1,m_ServerSet.m_QueueGaming);
		}
		break;
	case IDD_MESSAGE_USER_CHAT:                     // 用户聊天
		{
			int senduserID = mes["senduserID"].asInt();
			int receiverID = mes["receiverID"].asInt();
			std::string ChatMsg = mes["ChatMsg"].asString();

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_USER_CHAT;
            root["senduserID"] = senduserID;
            root["receiverID"] = receiverID;
            root["ChatMsg"] = ChatMsg;

			if(receiverID == -1)
				ServerPlayerManager.SendMsgToEveryone(root);
			else
			{
				ServerPlayerManager.SendMsgToPlayer(senduserID,root);
				ServerPlayerManager.SendMsgToPlayer(receiverID,root);
			}
		}
		break;
	case IDD_MESSAGE_UPDATE_USER_MONEY:             // 更新用户身上的钱
		{
		    uint32 pUserId = mes["UserId"].asInt();

			CPlayer *pPlayer = ServerPlayerManager.GetPlayerById(pUserId);
			if(pPlayer == NULL) return;

			// 更新用户的数据
			OnProcessUserInfo(pPlayer);
		}
		break;
	case IDD_MESSAGE_LEAVE_ROOM:                    // 离开游戏房间
		{
			// 处理用户离开房间
			OnProcessGameLeaveRoomMes(connId,mes);

			DelQueueList(connId);
		}
		break;
	case IDD_MESSAGE_GET_ONLINE_PLAYERS:             // 得到在线玩家
		{
			ServerPlayerManager.LockPlayerList();

			int pPlayerCount = 0;
			std::vector<std::vector<CPlayer*> > pPlayerList;
			std::vector<CPlayer*> pNewPlayers;

			std::map<uint32,CPlayer*>::iterator iter = ServerPlayerManager.GetPlayerList().begin();
			for(;iter != ServerPlayerManager.GetPlayerList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			iter = ServerPlayerManager.GetRobotList().begin();
			for(;iter != ServerPlayerManager.GetRobotList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			iter = ServerPlayerManager.GetLostPlayerList().begin();
			for(;iter != ServerPlayerManager.GetLostPlayerList().end();++iter)
			{
				if((*iter).second == NULL) continue;

				pPlayerCount+=1;
				pNewPlayers.push_back((*iter).second);

				if(pPlayerCount >= IDD_SEND_PLAYER_MAX_COUNT)
				{
					pPlayerList.push_back(pNewPlayers);
					pPlayerCount=0;
					pNewPlayers.clear();
				}
			}

			if(pPlayerCount > 0 && !pNewPlayers.empty())
				pPlayerList.push_back(pNewPlayers);

			for(int i=0;i<(int)pPlayerList.size();i++)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_GET_ONLINE_PLAYERS;
                root["PlayerCount"] = (int)pPlayerList[i].size();

				for(int k=0;k<(int)pPlayerList[i].size();k++)
				{
				    Json::Value root2;
					root2["ID"] = pPlayerList[i][k]->GetID();
					root2["State"] = pPlayerList[i][k]->GetState();
					root2["Type"] = pPlayerList[i][k]->GetType();
					root2["RoomId"] = pPlayerList[i][k]->GetRoomId();
					root2["ChairIndex"] = pPlayerList[i][k]->GetChairIndex();
					root2["Name"] = pPlayerList[i][k]->GetName();
					root2["Level"] = pPlayerList[i][k]->GetLevel();
					root2["Money"] = (uint32)pPlayerList[i][k]->GetMoney();
					root2["BankMoney"] = (uint32)pPlayerList[i][k]->GetBankMoney();
					root2["Revenue"] = (uint32)pPlayerList[i][k]->GetRevenue();
					root2["TotalResult"] = (uint32)pPlayerList[i][k]->GetTotalResult();
					root2["Experience"] = pPlayerList[i][k]->GetExperience();
					root2["TotalBureau"] = pPlayerList[i][k]->GetTotalBureau();
					root2["SuccessBureau"] = pPlayerList[i][k]->GetSuccessBureau();
					root2["FailBureau"] = pPlayerList[i][k]->GetFailBureau();
					root2["RunawayBureau"] = pPlayerList[i][k]->GetRunawayBureau();
					root2["SuccessRate"] = pPlayerList[i][k]->GetSuccessRate();
					root2["Runawayrate"] = pPlayerList[i][k]->GetRunawayrate();
					root2["Sex"] = pPlayerList[i][k]->GetSex();
					root2["RealName"] = pPlayerList[i][k]->GetRealName();
					//root2["LoginIP"] = pPlayerList[i][k]->GetLoginIP();
					root2["DeviceType"] = pPlayerList[i][k]->GetDeviceType();

                    in_addr inaddr;
                    inaddr.s_addr=pPlayerList[i][k]->GetLoginIP();
                    root2["LoginIP"] = inet_ntoa(inaddr);

                    std::stringstream ss;
                    ss << "player" << i;
                    root[ss.str()] = root2;
				}

				Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_GET_ONLINE_PLAYERS_END;
            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

			ServerPlayerManager.UnlockPlayerList();
		}
		break;
	case IDD_MESSAGE_GET_ROOM_LIST:                  // 得到当前服务器中房间列表
		{
			ServerRoomManager.LockRoomList();

			std::vector<CRoom*> pRoomList = ServerRoomManager.GetRoomList();
			if(pRoomList.empty())
			{
				ServerRoomManager.UnlockRoomList();
				return;
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_GET_ROOM_LIST;

			// 首先写服务器中房间的数量
			root["RoomCount"] = (int)pRoomList.size();
			root["GameName"] = m_ServerSet.GameName;
			root["GameType"] = m_ServerSet.GameType;
			root["PlayerCount"] = m_ServerSet.PlayerCount;

			std::vector<CRoom*>::iterator iter = pRoomList.begin();
			for(int index=0;iter != pRoomList.end();iter++,index++)
			{
				if((*iter) == NULL) continue;

				int64 pfirst,psecond;
				pfirst=psecond = 0;
				(*iter)->GetEnterMoneyRect(&pfirst,&psecond);

				Json::Value root2;
				// 发送房间状态
				root2["RoomState"] = (int)(*iter)->GetRoomState();
				root2["EnterPassword"] = (*iter)->getEnterPassword();
				root2["first"] = (uint32)pfirst;
				root2["second"] = (uint32)psecond;
				root2["PlayerCount"] = (*iter)->GetPlayerCount();

				// 开始发送房间中玩家信息
				for(int i=0;i<(*iter)->GetMaxPlayer();i++)
				{
					Player *pPlayer = (*iter)->GetPlayer(i);
					if(pPlayer == NULL) continue;

					Json::Value root3;
					root3["ID"] = pPlayer->GetID();
					root3["ChairIndex"] = pPlayer->GetChairIndex();

                    std::stringstream ss;
                    ss << "player" << i;
                    root2[ss.str()] = root3;
				}

                std::stringstream ss;
                ss << "room" << index;
				root[ss.str()] = root2;
			}

			Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

			ServerRoomManager.UnlockRoomList();
		}
		break;
	case IDD_MESSAGE_READY_START:                     // 玩家准备开始游戏
		{
			OnProcessGameReadyMes(connId,mes);
		}
		break;
	case IDD_MESSAGE_REENTER_ROOM:                    // 重新回到游戏
		{
			CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
			if(pPlayer == NULL) return;

			CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
			if(pRoom == NULL || pRoom->GetRoomState() != ROOMSTATE_GAMING)
			{
				// 向当前玩家发送进入游戏房间失败的消息
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
                root["ID"] = pPlayer->GetID();
                Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());

				return;
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_REENTER_ROOM;
            root["ID"] = pPlayer->GetID();
			ServerPlayerManager.SendMsgToEveryone(root);

			pPlayer->SetState(PLAYERSTATE_GAMING);

			pRoom->OnProcessReEnterRoomMes(pPlayer->GetChairIndex());
		}
		break;
	default:
		break;
	}
}

/// 更新玩家信息
void GameFrameManager::OnProcessUserInfo(CPlayer *pPlayer)
{
	if(pPlayer == NULL) return;

	UserDataStru pUserData;
	if(ServerDBOperator.GetUserData(pPlayer->GetID(),pUserData))
	{
		bool isSendUpdateMsg = false;

		if(pPlayer->GetMoney() != pUserData.Money || pPlayer->GetBankMoney() != pUserData.BankMoney ||
			pPlayer->GetSex() != pUserData.sex || pPlayer->GetRealName() != pUserData.realName)
			isSendUpdateMsg = true;

		pPlayer->SetMoney(pUserData.Money);
		pPlayer->SetBankMoney(pUserData.BankMoney);
		pPlayer->SetSex(pUserData.sex);
		pPlayer->SetRealName(pUserData.realName);
		pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));

		if(isSendUpdateMsg)
		{
			UpdatePlayerMoney(pPlayer);
		}
	}
}

/// 更新玩家身上的钱
void GameFrameManager::UpdatePlayerMoney(Player *pPlayer)
{
	if(pPlayer == NULL) return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_UPDATE_USER_MONEY;
    root["ID"] = pPlayer->GetID();
    root["Money"] = (uint32)pPlayer->GetMoney();
    root["BankMoney"] = (uint32)pPlayer->GetBankMoney();
    root["Sex"] = pPlayer->GetSex();
    root["RealName"] = pPlayer->GetRealName();

	ServerPlayerManager.SendMsgToEveryone(root);
}

/// 用于处理用户准备消息
void GameFrameManager::OnProcessGameReadyMes(uint32 connId,Json::Value &mes)
{
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	// 机器人用户不处理，这里只处理普通用户
	if(pPlayer->GetState() != PLAYERSTATE_NORAML)
		return;

	// 如果玩家在游戏中也不处理
	if(pRoom->GetRoomState() == ROOMSTATE_GAMING)
		return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_READY_START;
    root["ID"] = pPlayer->GetID();
	ServerPlayerManager.SendMsgToEveryone(root);

	pPlayer->SetState(PLAYERSTATE_READY);
	pPlayer->SetReadyTime((DWORD)time(NULL));

	if(pRoom->GetMaster() < 0)
		pRoom->SetMaster(pPlayer->GetChairIndex());

	pRoom->OnProcessPlayerReadyMes(pPlayer->GetChairIndex());
}

/// 用于处理用户离开消息
void GameFrameManager::OnProcessGameLeaveRoomMes(uint32 connId,Json::Value &mes)
{
	CPlayer *pPlayer = ServerPlayerManager.GetPlayer(connId);
	if(pPlayer == NULL) return;

	CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
	if(pRoom == NULL) return;

	if(!pPlayer->IsLookOn())
	{
		pRoom->OnProcessLeaveRoomMsg(pPlayer->GetChairIndex());

		//pPlayer->SetState(PLAYERSTATE_NORAML);
	}

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_LEAVE_ROOM;
    root["ID"] = pPlayer->GetID();

	ServerPlayerManager.SendMsgToEveryone(root);

	pRoom->ClearPlayer(pPlayer);

	pPlayer->SetRoomId(-1);
	pPlayer->SetChairIndex(-1);
}

bool GameFrameManager::JoinPlayerToGameRoom(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue)
{
	if(pPlayer == NULL)
		return false;

	if(AddPlayerInServer(pPlayer,pRoomIndex,pChairIndex,isQueue,true)) return true;

	if(!isQueue)
	{
		if(m_ServerSet.m_GameStartMode == enStartMode_FullReady ||
			m_ServerSet.m_GameStartMode == enStartMode_TimeControl)
		{
			if(!AddPlayerInServer(pPlayer,pRoomIndex,pChairIndex,isQueue,false))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;

				if(!pPlayer->getEnterPassword().empty())
					root["MsgSubId2"] = IDD_MESSAGE_PERSONROOM_ENTER_ROOM_FAIL;
				else
					root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;

				root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
		}
		else
		{
			CRoom *pRoom = ServerRoomManager.GetFreeRoom();
			if(pRoom != NULL)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FAIL;
                root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
			else
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_FRAME;
                root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
                root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_FULL;
                root["ID"] = pPlayer->GetID();

				Sendhtml5(pPlayer->GetConnectID(),(const char*)root.toStyledString().c_str(),root.toStyledString().length());
			}
		}
	}

	pPlayer->SetEnterMoneyRect(0,0);
	pPlayer->SetEnterPassword("");

	return false;
}

bool GameFrameManager::AddPlayerInServer(CPlayer *pPlayer,int pRoomIndex,int pChairIndex,bool isQueue,bool isGaming)
{
	if(ServerRoomManager.AddPlayer(pPlayer,pRoomIndex,pChairIndex,isQueue,isGaming))
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
        root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
        root["ID"] = pPlayer->GetID();
        root["RoomId"] = pPlayer->GetRoomId();
        root["ChairIndex"] = pPlayer->GetChairIndex();

		ServerPlayerManager.SendMsgToEveryone(root);

		CRoom *pRoom = ServerRoomManager.GetRoomById(pPlayer->GetRoomId());
		if(pRoom)
		{
			pRoom->OnProcessEnterRoomMsg(pPlayer->GetChairIndex());
		}

		pPlayer->SetEnterMoneyRect(0,0);
		pPlayer->SetEnterPassword("");

		return true;
	}

	return false;
}

/// 更新房间信息
void GameFrameManager::UpdateGameRoomInfo(void)
{
    ServerDBOperator.UpdateGameRoomOnlinePlayerCount(m_ServerSet.RoomId,
                                                     ServerPlayerManager.GetPlayerCount(),
                                                     ServerPlayerManager.GetRobotCount());
}

/// 得到当前排队人数
int GameFrameManager::GetQueueRealPlayerCount(void)
{
    int count = 0;

    m_PlayerQueueListLock.Acquire();
    std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
    for(;iter != m_PlayerQueueList.end();++iter)
    {
        if((*iter).second != NULL && (*iter).second->GetType() == PLAYERTYPE_NORMAL) count+=1;
    }
    m_PlayerQueueListLock.Release();

    return count;
}
/// 得到当前排队机器人数
int GameFrameManager::GetQueueRobotPlayerCount(void)
{
    int count = 0;

    m_PlayerQueueListLock.Acquire();
    std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
    for(;iter != m_PlayerQueueList.end();++iter)
    {
        if((*iter).second != NULL && (*iter).second->GetType() == PLAYERTYPE_ROBOT) count+=1;
    }
    m_PlayerQueueListLock.Release();

    return count;
}
/// 清空排队列表
void GameFrameManager::ClearQueueList(void)
{
    m_PlayerQueueListLock.Acquire();
    std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
    for(;iter != m_PlayerQueueList.end();++iter)
    {
        if((*iter).second == NULL) continue;

        (*iter).second->SetState(PLAYERSTATE_NORAML);
    }
    m_PlayerQueueList.clear();
    m_PlayerQueueListLock.Release();
}

/// 得到当前排队人数
int GameFrameManager::GetCurrentQueuePlayerCount(void)
{
	int pWaitingCount = 0;

	m_PlayerQueueListLock.Acquire();
	pWaitingCount = (int)m_PlayerQueueList.size();
	m_PlayerQueueListLock.Release();

	return pWaitingCount;
}

/// 发送当前排队人数
void GameFrameManager::SendQueuingCount(void)
{
	// 先向所有在线玩家发送排队信息
	if(!m_ServerSet.m_QueueGaming/* || m_PlayerQueueList.empty()*/)
		return;

	int pWaitingCount = 0;

	m_PlayerQueueListLock.Acquire();
	pWaitingCount = (int)m_PlayerQueueList.size();
	m_PlayerQueueListLock.Release();

	//// 如果人数相同，就不更新了
	//if(m_oldWaitingCount == pWaitingCount)
	//	return;

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_FRAME;
    root["MsgSubId"] = IDD_MESSAGE_FRAME_MATCH;
    root["MsgSubId2"] = IDD_MESSAGE_FRAME_MATCH_WAITINGCOUNT;
    root["WaitingCount"] = pWaitingCount;

	ServerPlayerManager.SendMsgToEveryone(root);

	//m_oldWaitingCount = pWaitingCount;
}

/// 加入一个玩家到排队列表中
void GameFrameManager::AddQueueList(CPlayer *connId)
{
	if(connId == NULL) return;

	m_PlayerQueueListLock.Acquire();

	std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.find(connId->GetID());
	if(iter == m_PlayerQueueList.end())
	{
		connId->SetState(PLAYERSTATE_QUEUE);

		m_PlayerQueueList.insert(std::pair<uint32,CPlayer*>(connId->GetID(),connId));
	}

	m_PlayerQueueListLock.Release();
	//CString tmpStr;
	//tmpStr.Format("进来排队人数:%d\n",(int)m_PlayerQueueList.size());
	//::OutputDebugString(tmpStr);

	// 先更新在线排队人数
	SendQueuingCount();

	// 如果排队人数满足比赛要求，就立刻开始比赛
	UpdateQueueList();
}

/// 删除一个玩家从排队列表中
void GameFrameManager::DelQueueList(uint32 connId)
{
	m_PlayerQueueListLock.Acquire();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
	for(;iter != m_PlayerQueueList.end();)
	{
		if((*iter).second->GetConnectID() == connId)
		{
			// 只有在用户处于排队状态时，才有权改变它的状态
			if((*iter).second->GetState() == PLAYERSTATE_QUEUE)
			{
				(*iter).second->SetState(PLAYERSTATE_NORAML);
			}

			iter = m_PlayerQueueList.erase(iter);

			break;
		}
		else
		{
			++iter;
		}
	}
	m_PlayerQueueListLock.Release();

	//CString tmpStr;
	//tmpStr.Format("离开排队人数:%d\n",(int)m_PlayerQueueList.size());
	//::OutputDebugString(tmpStr);

	// 先更新在线排队人数
	SendQueuingCount();
}

/// 更新排队玩家列表
void GameFrameManager::UpdateQueueList(void)
{
	if((int)m_PlayerQueueList.size() < m_ServerSet.PlayerCount)
	{
		if(m_PlayerQueueList.empty()) return;

		if(!m_PlayerQueueList.empty())
		{
			m_PlayerQueueListLock.Acquire();
			std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
			for(;iter != m_PlayerQueueList.end();)
			{
				(*iter).second->SetState(PLAYERSTATE_NORAML);
				if(JoinPlayerToGameRoom((*iter).second) == false)
				{
					(*iter).second->SetState(PLAYERSTATE_QUEUE);
					break;
				}

				iter = m_PlayerQueueList.erase(iter);
			}
			m_PlayerQueueListLock.Release();
		}

		return;
	}

	CRoom *pRoom = ServerRoomManager.GetEmptyRoom();
	if(pRoom == NULL) return;

	m_PlayerQueueListLock.Acquire();
	int realCount = (int)m_PlayerQueueList.size();
	std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
	for(int index=0;iter != m_PlayerQueueList.end();index++)
	{
		if(index >= (realCount/m_ServerSet.PlayerCount) * m_ServerSet.PlayerCount)
			break;

		if(pRoom->IsFull())
			pRoom = ServerRoomManager.GetEmptyRoom();

		if(pRoom == NULL || pRoom->GetRoomState() == ROOMSTATE_GAMING) break;

		(*iter).second->SetRoomId(pRoom->GetID());
		(*iter).second->SetState(PLAYERSTATE_NORAML);

		int playerIndex = pRoom->AddPlayer((*iter).second);

		(*iter).second->SetChairIndex(playerIndex);

		// 先向服务器中所有的在线玩家通告玩家进入游戏房间的消息
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_ENTER_ROOM;
        root["MsgSubId2"] = IDD_MESSAGE_ENTER_ROOM_SUCC;
        root["ID"] = (*iter).second->GetID();
        root["RoomId"] = (*iter).second->GetRoomId();
        root["ChairIndex"] = (*iter).second->GetChairIndex();

		ServerPlayerManager.SendMsgToEveryone(root);

		if(pRoom) pRoom->OnProcessEnterRoomMsg((*iter).second->GetChairIndex());

		iter = m_PlayerQueueList.erase(iter);
	}
	m_PlayerQueueListLock.Release();

	if(!m_PlayerQueueList.empty())
	{
		m_PlayerQueueListLock.Acquire();
		std::map<uint32,CPlayer*>::iterator iter = m_PlayerQueueList.begin();
		for(;iter != m_PlayerQueueList.end();)
		{
			(*iter).second->SetState(PLAYERSTATE_NORAML);
			if(JoinPlayerToGameRoom((*iter).second) == false)
			{
				(*iter).second->SetState(PLAYERSTATE_QUEUE);
				break;
			}

			iter = m_PlayerQueueList.erase(iter);
		}
		m_PlayerQueueListLock.Release();
	}
}
