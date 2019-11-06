#include "CRobotManager.h"
#include "../../include/Common/defines.h"
#include "DBOperator.h"
#include "RoomManager.h"
#include "PlayerManager.h"
#include "../GameFrameManager.h"

initialiseSingleton(CRobotManager);

CRobotManager::CRobotManager():m_CurRobotUserCount(0)
{

}

CRobotManager::~CRobotManager()
{

}

// 导入机器人
void CRobotManager::RobotsEnterGameRoom(void)
{
	if(m_RobotUserList.empty()) return;

	//如果当前服务器已经满员就不加入机器人了
	if(ServerPlayerManager.GetTotalCount() >= m_ServerSet.PlayerCount * m_ServerSet.TableCount)
	{
		return;
	}

	//如果有离开的机器人，就先从离开的机器人中加入
	m_LeaveRobotLock.Acquire();
	if(!m_LeaveRobotList.empty())
	{
		uint32 pPlayerConnId = *(m_LeaveRobotList.begin());
		int pRobotUserListIndex = pPlayerConnId - IDD_ROBOT_SOCKET_START;

		UserDataStru pUserData;
		if(ServerDBOperator.GetUserData(m_RobotUserList[pRobotUserListIndex],pUserData))
		{
			CPlayer *pPlayer = ServerGameFrameManager.CreateNewPlayer();
			pPlayer->SetConnectID(IDD_ROBOT_SOCKET_START+pRobotUserListIndex);
			pPlayer->SetID(pUserData.UserId);
			pPlayer->SetDeviceType((PlayerDeviceType)(rand()%2));
			pPlayer->SetName(pUserData.UserName);
			pPlayer->SetMoney(pUserData.Money);
			pPlayer->SetBankMoney(pUserData.BankMoney);
			pPlayer->SetRevenue(pUserData.Revenue);
			pPlayer->SetTotalResult(pUserData.TotalResult);
			pPlayer->SetLevel(pUserData.Level);
			pPlayer->SetExperience(pUserData.Experience);
			pPlayer->SetTotalBureau(pUserData.TotalBureau);
			pPlayer->SetSuccessBureau(pUserData.SBureau);
			pPlayer->SetFailBureau(pUserData.FailBureau);
			pPlayer->SetRunawayBureau(pUserData.RunawayBureau);
			pPlayer->SetSuccessRate(pUserData.SuccessRate);
			pPlayer->SetRunawayrate(pUserData.RunawayRate);
			pPlayer->SetSex(pUserData.sex);
			pPlayer->SetRealName(pUserData.realName);
			pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));

			// 导入机器人逻辑接口
			pPlayer->SetRobotLogicFrame((RobotLogicFrame*)m_g_ServerServiceManager->CreateAndroidUserItemSink());

			if(ServerPlayerManager.AddRobot(pPlayer))
			{
				// 发送登录成功消息
				ServerGameFrameManager.SendPlayerLoginSuccess(pPlayer);
			}

			m_LeaveRobotList.erase(m_LeaveRobotList.begin());

			m_LeaveRobotLock.Release();
			return;
		}
	}
	m_LeaveRobotLock.Release();

	if(m_CurRobotUserCount >= IDD_ROBOT_MAX_COUNT || m_CurRobotUserCount >= (int)m_RobotUserList.size())
	{
		return;
	}

	UserDataStru pUserData;
	if(ServerDBOperator.GetUserData(m_RobotUserList[m_CurRobotUserCount],pUserData))
	{
		CPlayer *pPlayer = ServerGameFrameManager.CreateNewPlayer();
		pPlayer->SetConnectID(IDD_ROBOT_SOCKET_START+m_CurRobotUserCount);
		pPlayer->SetID(pUserData.UserId);
		pPlayer->SetDeviceType((PlayerDeviceType)(rand()%2));
		pPlayer->SetName(pUserData.UserName);
		pPlayer->SetMoney(pUserData.Money);
		pPlayer->SetBankMoney(pUserData.BankMoney);
		pPlayer->SetRevenue(pUserData.Revenue);
		pPlayer->SetTotalResult(pUserData.TotalResult);
		pPlayer->SetLevel(pUserData.Level);
		pPlayer->SetExperience(pUserData.Experience);
		pPlayer->SetTotalBureau(pUserData.TotalBureau);
		pPlayer->SetSuccessBureau(pUserData.SBureau);
		pPlayer->SetFailBureau(pUserData.FailBureau);
		pPlayer->SetRunawayBureau(pUserData.RunawayBureau);
		pPlayer->SetSuccessRate(pUserData.SuccessRate);
		pPlayer->SetRunawayrate(pUserData.RunawayRate);
		pPlayer->SetSex(pUserData.sex);
		pPlayer->SetRealName(pUserData.realName);
		pPlayer->SetLoginIP((uint32)inet_addr(pUserData.UserIP));

		pPlayer->SetRobotLogicFrame((RobotLogicFrame*)m_g_ServerServiceManager->CreateAndroidUserItemSink());

		if(ServerPlayerManager.AddRobot(pPlayer))
		{
			ServerGameFrameManager.SendPlayerLoginSuccess(pPlayer);
		}
	}

	m_CurRobotUserCount+=1;
}

// 机器人离开
void CRobotManager::AddLeaveRobot(uint32 connId)
{
    m_LeaveRobotLock.Acquire();
    m_LeaveRobotList.push_back(connId);
    m_LeaveRobotLock.Release();
}

// 更新机器人
void CRobotManager::LoadGameRobot(void)
{
	//导入机器人配置
	ServerDBOperator.GetRobotsOfGameServer(m_ServerSet.m_GameId,m_ServerSet.RoomId,m_RobotUserList);
}

// 机器人进入房间
void CRobotManager::UpdateRobot(void)
{
	CPlayer *pPlayer = ServerPlayerManager.GetFreeRobot();
	if(pPlayer == NULL) return;

	// 添加一个限制条件，只允许玩家在规定时间段进入游戏
	if(!ServerDBOperator.GetRobotEnterRoomTimes())
	{
		// 如果机器人没有在时间段时，有60的可能性离开服务器
		if(rand()%100 < 60)
		{
			// 向房间所有玩家广播玩家离开服务器消息
            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_FRAME;
            root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
            root["UserID"] = pPlayer->GetID();

			ServerPlayerManager.SendMsgToEveryone(root);

			pPlayer->SetRoomId(-1);
			pPlayer->SetChairIndex(-1);
			pPlayer->SetState(PLAYERSTATE_NORAML);

			uint32 pConnId = pPlayer->GetConnectID();
			ServerPlayerManager.ClearPlayer(pPlayer);
			//Disconnect(pConnId);

			AddLeaveRobot(pConnId);

			return;
		}
	}

	// 更新用户身上的钱
	ServerGameFrameManager.OnProcessUserInfo(pPlayer);

	int prealqueuingcount = ServerGameFrameManager.GetQueueRealPlayerCount();

	bool isEnterOk = false;

	// 检查用户的金币是否满足房间要求，如果不能满足房间要求就退出服务器,
	// 还有一种情况就是50%的几率离开房间
    if(pPlayer->GetMoney() < m_ServerSet.lastMoney ||
        pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble())
        isEnterOk = true;

    if(!isEnterOk)
        isEnterOk = (rand()%100 < 5 ? true : false);

	if(isEnterOk)
	{
		//// 如果玩家已经输光了所有的钱，就不再进入游戏了
		//bool isReLoad = (pPlayer->GetMoney() < m_ServerSet.lastMoney ||
		//	pPlayer->GetMoney() < m_ServerSet.m_Pielement * m_g_ServerServiceManager->GetRoomLastDouble()) ? false : true;

		// 向房间所有玩家广播玩家离开服务器消息
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_FRAME;
        root["MsgSubId"] = IDD_MESSAGE_LEAVE_SERVER;
        root["UserID"] = pPlayer->GetID();

		ServerPlayerManager.SendMsgToEveryone(root);

		pPlayer->SetRoomId(-1);
		pPlayer->SetChairIndex(-1);
		pPlayer->SetState(PLAYERSTATE_NORAML);

		uint32 pConnId = pPlayer->GetConnectID();
		ServerPlayerManager.ClearPlayer(pPlayer);
		//Disconnect(pConnId);

		AddLeaveRobot(pConnId);

		return;
	}

	if(m_ServerSet.m_QueueGaming)
		ServerGameFrameManager.AddQueueList(pPlayer);                // 排队进入
	else
		ServerGameFrameManager.JoinPlayerToGameRoom(pPlayer,-1,-1,m_ServerSet.m_QueueGaming);
}
