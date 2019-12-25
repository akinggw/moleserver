#include "cserverlogicframe.h"
#include <sstream>

#define TIMER_TYPE(value)				((value)>>2)			//时钟的类型掩码
#define TIMER_CHAIRID(value)			((value)&0x0003)		//时钟的座位掩码,2位
#define TIMER(type,chairId)				(((type)<<2)+chairId)

const MJBYTE CServerLogicFrame::bOriginalMJ[MAX_ORG_MJ] =
{
		0x01, 0x02,	0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,		//万 1~9
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,		//条 1~9
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,		//筒 1~9
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,       //万 1~9
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,       //条 1~9
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,       //筒 1~9
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,       //万 1~9
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,       //条 1~9
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,       //筒 1~9
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,       //万 1~9
		0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,       //条 1~9
		0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,       //筒 1~9
};

/// 构造函数
CServerLogicFrame::CServerLogicFrame():m_g_GameRoom(NULL)
{
	//初始化算法库
	CMahJongAlg *alg = new CMahJongAlg(MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing|MahJongColor_Word);
	m_pGameLogic = new CMahJongLogic();
	m_pGameLogic->SetMahJongAlg(alg);
	m_pGameLogic->SetRequiredCheck();

	m_iIsReRandomBanker = 0;
	m_iBankerUser = 0;
	Reset();
}

/// 析构函数
CServerLogicFrame::~CServerLogicFrame()
{
	if (m_pGameLogic)
	{
		delete m_pGameLogic;
		m_pGameLogic = NULL;
	}
}

/// 用于处理用户开始游戏开始消息
void CServerLogicFrame::OnProcessPlayerGameStartMes()
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	Reset();

	bool bHasRobot = false;
	bool bHasRealPlayer = false;
	int	iRobotChaitID = ChairLoc_Invalid;
	//游戏基础积分
	if (m_g_GameRoom->GetRoomType() == ROOMTYPE_BISAI)
	{
		m_iBisicCore = 1;
	}
	else
	{
		m_iBisicCore = m_g_GameRoom->GetGamePielement();
	}


	//判断是不是真人和机器人混玩
	for (int i=0; i<GAME_PLAYER; ++i)
	{
		Player *myPlayer = m_g_GameRoom->GetPlayer(i);
		if (myPlayer->GetType() == PLAYERTYPE_ROBOT)
		{
			bHasRobot = true;
			iRobotChaitID = i;
			if (m_g_GameRoom->GetRoomType() == ROOMTYPE_BISAI)
			{
				m_UserMoney[i] = 102400;
			}
			else
			{
				m_UserMoney[i] = myPlayer->GetMoney();
			}
		}
		else if (myPlayer->GetType() == PLAYERTYPE_NORMAL)
		{
			bHasRealPlayer = true;
			if (m_g_GameRoom->GetRoomType() == ROOMTYPE_BISAI)
			{
				m_UserMoney[i] = 1024*m_iBisicCore;
			}
			else
			{
				m_UserMoney[i] = myPlayer->GetMoney();
			}
		}
	}

	//投色子，确定摸牌位置。马牌和杠牌从最后拿
	m_iDiceNum01 = m_pGameLogic->GetRandomNum(1,6);
	m_iDiceNum02 = m_pGameLogic->GetRandomNum(1,6);
	m_iDiceNum11 = m_pGameLogic->GetRandomNum(1,6);
	m_iDiceNum12 = m_pGameLogic->GetRandomNum(1,6);

	m_iIsReRandomBanker ++;

	memcpy(m_bAllMJ, bOriginalMJ, sizeof(bOriginalMJ));

	int64 isRobotWin = m_g_GameRoom->IsUserWin();
	if (isRobotWin == 2)	//如果机器人输了，那么就看看本局是否是机器人和真人混玩，如果是，给机器人发好牌
	{
		if (bHasRobot && bHasRealPlayer)
		{
			GoodMJToRobot(m_bAllMJ, MAX_ORG_MJ, iRobotChaitID);
		}
	}
	else					//否则随机发牌
	{
		m_pGameLogic->RandomMJ(m_bAllMJ, MAX_ORG_MJ);
	}

	if (m_iIsReRandomBanker == 1)
	{
		m_iOperateUer = m_iBankerUser = (m_iDiceNum01+m_iDiceNum02)%GAME_PLAYER;
	}
	else
	{
		//庄家待定
		m_iOperateUer = m_iBankerUser;
	}

	//开始录制游戏， 默认录制庄家
	/*Player *defaultPlayer = m_g_GameRoom->GetPlayer(m_iBankerUser);
	if (defaultPlayer)
	{
		m_g_GameRoom->StartVideoTape(defaultPlayer, m_g_GameRoom);
	}*/


	//设置玩家状态
	for (int i=0; i<GAME_PLAYER; ++i)
	{
		enUserStatu[i] = PlayerStatu_Normal;
	}

#ifdef ENABLE_DEBUG_TOOL
	m_IsDebug = ForTest();
#endif

	//起牌，按正常操作，每人抓4张3次
	for (int i = 0; i<3; ++i)
	{
		for (int j=0; j<GAME_PLAYER; j++)
		{
			m_bHandMJ[j][m_iHandMJCount[j]++] = m_bAllMJ[m_iTouchPos++];
			m_bHandMJ[j][m_iHandMJCount[j]++] = m_bAllMJ[m_iTouchPos++];
			m_bHandMJ[j][m_iHandMJCount[j]++] = m_bAllMJ[m_iTouchPos++];
			m_bHandMJ[j][m_iHandMJCount[j]++] = m_bAllMJ[m_iTouchPos++];
		}
	}

	for (int i=0; i<GAME_PLAYER; ++i)
	{
		int iAllPlayer = (m_iBankerUser + i) % GAME_PLAYER;
		m_bHandMJ[iAllPlayer][m_iHandMJCount[iAllPlayer]++] = m_bAllMJ[m_iTouchPos++];
	}

	//////////////////////////////////////////////////////////////////////////
	//清除操作信息
	//这时吧14张牌拿来判断庄家是否有操作，判断完再把手牌+1
	//开局就判断庄家可能有的操作，可能天胡和暗杠
	//////////////////////////////////////////////////////////////////////////
	bool isHu = this->isHu(m_iBankerUser, m_bHUMJ[m_iBankerUser], m_iHUMJCount[m_iBankerUser], m_bHandMJ[m_iBankerUser], m_iHandMJCount[m_iBankerUser], m_bAllMJ[m_iTouchPos]);
	if (isHu)
	{
		if (m_iTouchPos <= MAX_ORG_MJ)
		{
			m_OperationInfo[m_iBankerUser].bOperation |= Operation_Hu;
		}
	}

	//最后判断是不是有暗杠
	bool isAnGang = m_pGameLogic->IsAnGang(m_bHUMJ[m_iBankerUser], m_iHUMJCount[m_iBankerUser], m_bHandMJ[m_iBankerUser], m_iHandMJCount[m_iBankerUser], m_bAllMJ[m_iTouchPos], m_OperationInfo[m_iBankerUser].bMJGang, m_OperationInfo[m_iBankerUser].iGangCount);
	if (isAnGang)
	{
		m_OperationInfo[m_iBankerUser].bOperation |= Operation_Gang;
		m_OperationInfo[m_iBankerUser].eGangType = Operation_Gang_Type_An;
		m_OperPriority[m_iBankerUser] = Operation_Gang;
	}

	//庄家第14张牌
	m_bHandMJ[m_iBankerUser][m_iHandMJCount[m_iBankerUser]++] = m_bAllMJ[m_iTouchPos++];
	m_UserTouchFirst[m_iBankerUser]++;

	//开始给客户端发送信息
	tagCMD_S_DealMJ cmdDeal[GAME_PLAYER];
	for (int i=0; i<GAME_PLAYER; i++)
	{
		memset(&cmdDeal[i], 0, sizeof(tagCMD_S_DealMJ));

		cmdDeal[i].iBankerUser = m_iBankerUser;
		cmdDeal[i].iBaseScore = m_g_GameRoom->GetGamePielement();
		if (m_iIsReRandomBanker == 1)
		{
			cmdDeal[i].iIsRandomBanker = 1;
		}
		else
		{
			cmdDeal[i].iIsRandomBanker = 0;
		}
		cmdDeal[i].iDiceNum01 = m_iDiceNum01;
		cmdDeal[i].iDiceNum02 = m_iDiceNum02;
		cmdDeal[i].iDiceNum11 = m_iDiceNum11;
		cmdDeal[i].iDiceNum12 = m_iDiceNum12;
		cmdDeal[i].iUserChairID = i;

		m_pGameLogic->SortHandMJ(m_bHandMJ[i], m_iHandMJCount[i]);
		cmdDeal[i].iHandMJCount = m_iHandMJCount[i];
		memcpy(cmdDeal[i].bHandMJ, m_bHandMJ[i], sizeof(m_bHandMJ[i]));
		cmdDeal[i].iStatMJPos = m_iTouchPos;
	}

	cmdDeal[m_iBankerUser].bOperation = m_OperationInfo[m_iBankerUser].bOperation;
	if (m_OperationInfo[m_iBankerUser].bOperation & Operation_Hu)
	{
		m_OperPriority[m_iBankerUser] = Operation_Hu;
	}
	if (m_OperationInfo[m_iBankerUser].bOperation & Operation_Gang)
	{
		memcpy(cmdDeal[m_iBankerUser].bMJGang, m_OperationInfo[m_iBankerUser].bMJGang, sizeof(m_OperationInfo[m_iBankerUser].bMJGang));
		cmdDeal[m_iBankerUser].iGangCount = m_OperationInfo[m_iBankerUser].iGangCount;
	}

	for (int i=0;i<GAME_PLAYER;i++)
	{
        Json::Value outMsg;
        outMsg["MsgId"] = IDD_MESSAGE_ROOM;
        outMsg["MsgSubId"] = ID_S_GAMEMSG_DEALMJ;
		Player *pPlayer = m_g_GameRoom->GetPlayer(i);
		if (pPlayer)
		{
			CmdHelper::CodeMessage(cmdDeal[i], outMsg);
			m_g_GameRoom->SendTableMsg(i, outMsg);
		}
	}
	//开启定时器
	int itime = (TIME_MaxOut+TIME_MaxOut - 8000)/1000;//TIME_Dice*2+TIME_BuildWall+TIME_Deal+TIME_ServerDelay
	int itime1 = (TIME_Dice*2+TIME_BuildWall+TIME_Deal+TIME_ServerDelay)/1000;
	m_g_GameRoom->StartTimer(TIMER_START, itime);
	m_g_GameRoom->StartTimer(TIMER_QUIT, itime1);
	m_g_GameRoom->StartTimer(TIMER_STRATDROP, itime1);
}

/// 用于处理用户进入游戏房间后的消息
void CServerLogicFrame::OnProcessPlayerRoomMes(int playerId,Json::Value &mes)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	int iMsgID = mes["MsgSubId"].asInt();
	switch(iMsgID)
	{
	case ID_C_GAMEMSG_READY:
		{
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (enUserStatu[i] == PlayerStatu_Dropline && m_StartUserID[i] <= 0)
				{
					m_StartUserID[i]++;
					m_ReadyCount++;
				}
			}
			if (m_StartUserID[playerId] <= 0)
			{
				m_StartUserID[playerId]++;
				m_ReadyCount++;
			}
			if (m_ReadyCount >= 4)
			{
				m_g_GameRoom->StopTimer(TIMER_START);
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
                    Json::Value msgOut;
                    msgOut["MsgId"] = IDD_MESSAGE_ROOM;
                    msgOut["MsgSubId"] = ID_S_GAMEMSG_START;
					m_g_GameRoom->SendTableMsg(i, msgOut);
				}
				int itime = (TIME_Dice*2+TIME_BuildWall+TIME_Deal+TIME_ServerDelay)/1000;
				m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
			}
		}
		break;
	case ID_C_GAMEMSG_OUT:
		{
			//代打次数置空
			m_iOperateUer = playerId;
			m_iAUTO_OutCount[m_iOperateUer] = 0;
			CMD_C_OutMJ cmdOut;
			memset(&cmdOut, 0, sizeof(CMD_C_OutMJ));
			CmdHelper::DeCodeMessage(cmdOut, mes);
			OperOut(playerId, cmdOut);
		}
		break;
	case ID_C_GAMEMSG_PENG:
		{
			//代打次数置空
			m_iOperateUer = playerId;
			m_iAUTO_OutCount[m_iOperateUer] = 0;
			OperPeng(playerId);
		}
		break;
	case ID_C_GAMEMSG_GANG:
		{
			m_iOperateUer = playerId;
			//代打次数置空
			m_iAUTO_OutCount[m_iOperateUer] = 0;
			CMD_C_GangMJ cmdGang;
			memset(&cmdGang, 0, sizeof(CMD_C_GangMJ));
			CmdHelper::DeCodeMessage(cmdGang, mes);
			OperGang(playerId, cmdGang);
		}
		break;
	case ID_C_GAMEMSG_HU:
		{
			enHuType enhuType = HuType_Null;
			if (m_iHandMJCount[playerId]%3 == 2)
			{
				enhuType = HuType_ZiMo;
				OperHu(playerId, m_iOldOperateUer, enhuType);
			}
			else if (m_bQiangGangMJ[playerId] != 0)
			{
				enhuType = HuType_QiangGang;
				OperHu(playerId, m_BeiQiangUser, enhuType);
			}
			else
			{
				enhuType = HuType_PingHu;
				OperHu(playerId, m_iOldOperateUer, enhuType);
			}
		}
		break;
	case ID_C_GAMEMSG_GIVEUP:
		{
			if (m_OperPriority[playerId] & Operation_Hu)
			{
				m_iOperateUer = playerId;
			}
			OperGiveUp(playerId);
		}
		break;
	case ID_C_GAMEMSG_TRUSTEE:
		{
			OperTrustee(playerId);
		}
		break;
	case ID_C_GAMEMSG_MJ:
		{
			CMD_S_MJ cmdsMJ;
			memset(&cmdsMJ, 0, sizeof(CMD_S_MJ));
			for (int i=0; i<GAME_PLAYER; i++)
			{
				cmdsMJ.bHandMjCount[i] = m_iHandMJCount[i];
				cmdsMJ.bHUMjCount[i] = m_iHUMJCount[i];
				cmdsMJ.bOutMjCount[i] = m_iOutMJCount[i];
				memcpy(cmdsMJ.bHandMj[i], m_bHandMJ[i], sizeof(MJBYTE)*MAX_HAND_MJ);
				memcpy(cmdsMJ.bHUMj[i], m_bHUMJ[i], sizeof(MJBlock)*MAX_BLOCK_COUNT);
				memcpy(cmdsMJ.bOutMj[i], m_bOutMJ[i], sizeof(MJBYTE)*MAX_OUT_MJ);
			}

            Json::Value msgOut;
            msgOut["MsgId"] = IDD_MESSAGE_ROOM;
            msgOut["MsgSubId"] = ID_S_GAMEMSG_MJ;
			CmdHelper::CodeMessage(cmdsMJ, msgOut);
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgOut);
		}
		break;
	case ID_C_GAMEMSG_CHAT:
		{
			CMD_S_Chat cmdsChat;
			memset(&cmdsChat, 0, sizeof(CMD_S_Chat));
			CMD_C_Chat cmdChat;
			memset(&cmdChat, 0, sizeof(CMD_C_Chat));
			CmdHelper::DeCodeMessage(cmdChat, mes);
			cmdsChat.bType = cmdChat.bType;
			cmdsChat.iFaceID = cmdChat.iFaceID;
			cmdsChat.iTalkID = cmdChat.iTalkID;
			cmdsChat.iChatUser = playerId;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
                Json::Value msgOut;
                msgOut["MsgId"] = IDD_MESSAGE_ROOM;
                msgOut["MsgSubId"] = ID_S_GAMEMSG_CHAT;
				CmdHelper::CodeMessage(cmdsChat, msgOut);
				m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgOut);
			}
		}
		break;
	}
}

/// 处理用户进入房间消息
void CServerLogicFrame::OnProcessEnterRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

}

/// 处理用户离开房间消息
void CServerLogicFrame::OnProcessLeaveRoomMsg(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	RoomState rs = m_g_GameRoom->GetRoomState();
	if (rs == ROOMSTATE_GAMING)
	{
		m_iIsReRandomBanker = 0;
		if (enUserStatu[playerId] != PlayerStatu_ForceQuit)
		{
			OperHu(playerId, INVALID_CHAIR, HuType_QiangTui);
		}
	}
	else if(rs == ROOMSTATE_WAITING)
	{
		for (int i=0; i<GAME_PLAYER; i++)
		{
            Json::Value msgOut;
            msgOut["MsgId"] = IDD_MESSAGE_ROOM;

			Player *myPlayer = m_g_GameRoom->GetPlayer(i);
			if (myPlayer && myPlayer->GetState() == PLAYERSTATE_NORAML)
			{
				msgOut["MsgSubId"] = ID_S_GAMEMSG_CHANGETABLE;
				m_g_GameRoom->SendTableMsg(i, msgOut);
			}
		}
		m_iIsReRandomBanker = 0;
	}
}

/// 处理用户断线消息
void CServerLogicFrame::OnProcessOfflineRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	//保存掉线前的状态
	int iCount = 0;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (enUserStatu[i] != PlayerStatu_Dropline)
		{
			enUserStatuOld[i] = enUserStatu[i];
		}
	}

	if (enUserStatu[playerId] >= PlayerStatu_ZiMo)
	{

	}
	else
	{
		enUserStatu[playerId] = PlayerStatu_Dropline;
	}

	if (m_g_GameRoom->GetRoomType() == ROOMTYPE_BISAI && m_CmdsGameOver.enEndType > 0)
	{
		return;
	}

	//如果所有的玩家都掉线了，那么结束游戏，玩家不计得分。
	/*int iOffLineUserCount = 0;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		if (enUserStatu[i] == PlayerStatu_Dropline || enUserStatu[i] >= PlayerStatu_ZiMo)
		{
			iOffLineUserCount ++;
		}
	}
	if (iOffLineUserCount>=4)
	{
		m_g_GameRoom->StopAllTimer();
		m_g_GameRoom->GameEnd(true);
		return;
	}*/

	//如果刚好轮到掉线玩家出牌，那么定时器。等待
	if (m_iOperateUer == playerId && enUserStatu[playerId] == PlayerStatu_Dropline)
	{
		m_g_GameRoom->StopTimer(TIMER_Nomal);
		m_g_GameRoom->StopTimer(TIMER(TIMER_DHUNomal, playerId));
	}
	else if (enUserStatu[playerId] == PlayerStatu_Dropline)
	{
		m_g_GameRoom->StopTimer(TIMER(TIMER_DHUNomal, playerId));
	}

	//等待30秒
	if (enUserStatu[playerId] == PlayerStatu_Dropline)
	{
		int itime = 30;
		int iTimeID = TIMER_USER_OFFLINE + playerId;
		m_g_GameRoom->StartTimer(iTimeID, itime);
	}
}

/// 处理用户断线重连消息
void CServerLogicFrame::OnProcessReEnterRoomMes(int playerId)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	RoomState rs = m_g_GameRoom->GetRoomState();

	if (rs == ROOMSTATE_GAMING )
	{
		m_g_GameRoom->StopTimer(TIMER_USER_OFFLINE);
		m_g_GameRoom->StopTimer(TIMER_USER_OFFLINE+1);
		m_g_GameRoom->StopTimer(TIMER_USER_OFFLINE+2);
		m_g_GameRoom->StopTimer(TIMER_USER_OFFLINE+3);

		CMD_S_ReEnterRoom cmdsReEnTerRoom;
		memset(&cmdsReEnTerRoom, 0, sizeof(CMD_S_ReEnterRoom));

		cmdsReEnTerRoom.iBankerUser = m_iBankerUser;
		cmdsReEnTerRoom.iBaseScore = m_iBisicCore;
		cmdsReEnTerRoom.iIsRandomBanker = m_iIsReRandomBanker;
		cmdsReEnTerRoom.iDiceNum01 = m_iDiceNum01;
		cmdsReEnTerRoom.iDiceNum02 = m_iDiceNum02;
		cmdsReEnTerRoom.iDiceNum11 = m_iDiceNum11;
		cmdsReEnTerRoom.iDiceNum12 = m_iDiceNum12;

		cmdsReEnTerRoom.iCurrentUser = m_iOperateUer;
		cmdsReEnTerRoom.iOldOperUser = m_iOldCurrentUer;
		cmdsReEnTerRoom.iReadyPlayer = m_ReadyCount;
		std::sort(m_huRanderUser, m_huRanderUser+4);
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			if (m_huRanderUser[i] > 0)
			{
				for (int j = 0; j < GAME_PLAYER; ++j)
				{
					if (i == j)
					{
						continue;
					}
					if (m_huRanderUser[i] == m_huRanderUser[j])
					{
						cmdsReEnTerRoom.isYiPaoDuoXiang = 1;
						break;
					}
				}
				if (cmdsReEnTerRoom.isYiPaoDuoXiang == 1)
				{
					break;
				}
			}
		}
		if (m_ForceUserID[playerId] > 0)
		{
			cmdsReEnTerRoom.isForceQuit = 1;
		}
		if (m_iOperateUer != playerId)
		{
			cmdsReEnTerRoom.isOtherPriority = 1;
		}
		cmdsReEnTerRoom.iUserChairID = playerId;

		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			cmdsReEnTerRoom.bDianPaoMJ[i] = m_HuInfo[i].bHuMJ;
		}
		if (m_HuInfo[playerId].iHuInfoType >= HuType_ZiMo)
		{
			cmdsReEnTerRoom.enhuType = m_HuInfo[playerId].iHuInfoType;
			for (int i = 0; i < m_iHUMJCount[playerId]; ++i)
			{
				if (m_bHUMJ[playerId][i].style >= BlockStyle_Gang_Ming)
				{
					cmdsReEnTerRoom.iGangFan++;
				}
			}
			cmdsReEnTerRoom.iGangScore = m_AllGangSorce[playerId];
			/*for (int i = 0; i < GuaFengMaxCount; ++i)
			{
				if (m_GuaFengInfo[i].eGangType == Operation_Gang_Type_Null)
				{
					break;
				}
				else if (m_GuaFengInfo[i].eGangPlayer == playerId)
				{
					cmdsReEnTerRoom.iGangScore += m_GuaFengInfo[i].eGangSorce;
				}
			}*/
			cmdsReEnTerRoom.iHuFan = m_HuInfo[playerId].iHuFan;
			cmdsReEnTerRoom.iHuScore = int64(pow(2.0f, (int32)(cmdsReEnTerRoom.iHuFan-1)))*m_iBisicCore;
			cmdsReEnTerRoom.iDianPaoUser = m_HuInfo[playerId].iDianPaoUser;
			memcpy(cmdsReEnTerRoom.iFanStyle, m_HuInfo[playerId].iFanStyle, sizeof(int)*MAX_FAN_STYLE);
		}

		for (int i=0; i<GAME_PLAYER; i++)
		{
			cmdsReEnTerRoom.iHandMJCount[i] = m_iHandMJCount[i];
			cmdsReEnTerRoom.iHuMJCount[i] = m_iHUMJCount[i];
			cmdsReEnTerRoom.iOutMJCount[i] = m_iOutMJCount[i];

			if (i == playerId)
			{
				memcpy(cmdsReEnTerRoom.bHandMJ[i], m_bHandMJ[i], sizeof(MJBYTE)*MAX_HAND_MJ);
				//还原以前的状态
				enUserStatu[i] = enUserStatuOld[i];
			}
			else
			{
				if (enUserStatu[i] == PlayerStatu_Dropline)
				{
					//以前是掉线状态，那么仍旧是掉线状态
					enUserStatu[i] = PlayerStatu_Dropline;
				}
			}
			cmdsReEnTerRoom.enUserStatu[i] = enUserStatu[i];
			memcpy(cmdsReEnTerRoom.bHuMJ[i], m_bHUMJ[i], sizeof(MJBlock)*MAX_BLOCK_COUNT);
			memcpy(cmdsReEnTerRoom.bOutMJ[i], m_bOutMJ[i], sizeof(MJBYTE)*MAX_OUT_MJ);
		}
		for (int i=0; i<MAX_BLOCK_COUNT; i++)
		{
			for (int j=0; j<GAME_PLAYER; j++)
			{
				if (j!=playerId)
				{
					if (cmdsReEnTerRoom.bHuMJ[j][i].style == BlockStyle_Gang_An)
					{
						cmdsReEnTerRoom.bHuMJ[j][i].first = CONST_MJ_HIDE;
					}
				}
			}
		}

        Json::Value msgOut;
        msgOut["MsgId"] = IDD_MESSAGE_ROOM;
        msgOut["MsgSubId"] = ID_S_GAMEMSG_USERREBACK;
		CmdHelper::CodeMessage(cmdsReEnTerRoom, msgOut);
		m_g_GameRoom->SendTableMsg(playerId, msgOut);

		//隐藏消息
		enPlayerStatu tmpStatu[GAME_PLAYER];
		memset(tmpStatu, 0, sizeof(enPlayerStatu)*GAME_PLAYER);
		memcpy(tmpStatu, cmdsReEnTerRoom.enUserStatu, sizeof(enPlayerStatu)*GAME_PLAYER);
		cmdsReEnTerRoom.iUserChairID = ChairLoc_Invalid;
		for (int i=0; i<GAME_PLAYER; i++)
		{
			if (i != playerId)
			{
                Json::Value msgOut;
                msgOut["MsgId"] = IDD_MESSAGE_ROOM;
                msgOut["MsgSubId"] = ID_S_GAMEMSG_USERREBACK;
				CmdHelper::CodeMessage(cmdsReEnTerRoom, msgOut);
				m_g_GameRoom->SendTableMsg(i, msgOut);
			}
		}
		if (m_iOperateUer == playerId && m_CmdsGameOver.enEndType == 0)
		{
			if (m_StartDrop)
			{
				m_g_GameRoom->StartTimer(TIMER_Nomal, 12);
			}
			else
			{
				OperAUTO(m_iOperateUer, true);
			}
		}
		else if (m_OperPriority[playerId] & Operation_Hu)
		{
			m_iOperateUer = playerId;
			OperAUTO(playerId, true);
		}
		if (m_g_GameRoom->GetRoomType() == ROOMTYPE_BISAI && m_CmdsGameOver.enEndType > 0)
		{
            Json::Value msgGameOver;
            msgGameOver["MsgId"] = IDD_MESSAGE_ROOM;
            msgGameOver["MsgSubId"] = ID_S_GAMEMSG_GAMEOVER;
			CmdHelper::CodeMessage(m_CmdsGameOver, msgGameOver);
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgGameOver);
		}
	}
}

/// 处理用户定时器消息
void CServerLogicFrame::OnProcessTimerMsg(int timerId,int curTimer)
{
	assert(m_g_GameRoom != NULL);
	if(m_g_GameRoom == NULL) return;

	if (curTimer)
	{
		return;
	}
	int isOtherHu = 0;int TimerID = timerId;
	if (timerId > 12)
	{
		isOtherHu = TIMER_CHAIRID(timerId);
		timerId = TIMER_TYPE(timerId);
	}

	switch(timerId)
	{
	case TIMER_START:
		{
			m_g_GameRoom->StopTimer(TIMER_START);
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
                Json::Value msgOut;
                msgOut["MsgId"] = IDD_MESSAGE_ROOM;
                msgOut["MsgSubId"] = ID_S_GAMEMSG_START;
				m_g_GameRoom->SendTableMsg(i, msgOut);
				m_ReadyCount = 4;
			}
			int itime = (TIME_Dice*2+TIME_BuildWall+TIME_Deal+TIME_ServerDelay)/1000;
			m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
		}
		break;
	case TIMER_STRATDROP:
		{
			m_g_GameRoom->StopTimer(TIMER_STRATDROP);
			m_StartDrop = false;
		}
		break;
	case TIMER_QUIT:
		{
			m_g_GameRoom->StopTimer(TIMER_QUIT);
			m_StartQuit = false;
		}
		break;
	case TIMER_DHUNomal:
		{
			m_g_GameRoom->StopTimer(TimerID);
			if (enUserStatu[isOtherHu] != PlayerStatu_Trustee)
			{
				m_iAUTO_OutCount[isOtherHu]++;
				if (m_iAUTO_OutCount[isOtherHu]>2)
				{
					OperTrustee(isOtherHu, true);
				}
			}
			if (m_iAUTO_OutCount[isOtherHu] <= 2)
			{
				m_iOperateUer = isOtherHu;
				OperAUTO(isOtherHu, true);
			}
		}
		break;
	case TIMER_Nomal:
		{
			m_g_GameRoom->StopTimer(TIMER_Nomal);
			int iCurrent = 0;
			iCurrent = m_iOperateUer;
			if (enUserStatu[m_iOperateUer] != PlayerStatu_Trustee)
			{
				m_iAUTO_OutCount[m_iOperateUer]++;
				if (m_iAUTO_OutCount[m_iOperateUer]>2)
				{
					OperTrustee(m_iOperateUer, true);
				}
			}
			if (iCurrent == m_iOperateUer)
			{
				OperAUTO(m_iOperateUer, true);
			}
		}
		break;
	case TIMER_USER_OFFLINE + 0:
		{
			int iTimeID = timerId;
			m_g_GameRoom->StopTimer(iTimeID);

			enUserStatu[0] = PlayerStatu_Trustee;

			CMD_S_Trustee cmdsTrustee;
			memset(&cmdsTrustee, 0, sizeof(CMD_S_Trustee));

			cmdsTrustee.bUserTrustee = 1;
			cmdsTrustee.iUserChairID = 0;

            Json::Value msgTrustee;
            msgTrustee["MsgId"] = IDD_MESSAGE_ROOM;
            msgTrustee["MsgSubId"] = ID_S_GAMEMSG_TRUSTEE;
			CmdHelper::CodeMessage(cmdsTrustee, msgTrustee);
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgTrustee);

			if (m_iOperateUer == 0)
			{
				OperAUTO(m_iOperateUer, true);
			}
			else if (m_OperPriority[0] & Operation_Hu)
			{
				OperAUTO(m_iOperateUer, true);
			}
		}
		break;
	case TIMER_USER_OFFLINE + 1:
		{
			int iTimeID = timerId;
			m_g_GameRoom->StopTimer(iTimeID);

			enUserStatu[1] = PlayerStatu_Trustee;

			CMD_S_Trustee cmdsTrustee;
			memset(&cmdsTrustee, 0, sizeof(CMD_S_Trustee));

			cmdsTrustee.bUserTrustee = 1;
			cmdsTrustee.iUserChairID = 1;

            Json::Value msgTrustee;
            msgTrustee["MsgId"] = IDD_MESSAGE_ROOM;
            msgTrustee["MsgSubId"] = ID_S_GAMEMSG_TRUSTEE;
			CmdHelper::CodeMessage(cmdsTrustee, msgTrustee);
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgTrustee);

			if (m_iOperateUer == 1)
			{
				OperAUTO(m_iOperateUer, true);
			}
			else if (m_OperPriority[1] & Operation_Hu)
			{
				OperAUTO(m_iOperateUer, true);
			}
		}
		break;
	case TIMER_USER_OFFLINE + 2:
		{
			int iTimeID = timerId;
			m_g_GameRoom->StopTimer(iTimeID);

			enUserStatu[2] = PlayerStatu_Trustee;

			CMD_S_Trustee cmdsTrustee;
			memset(&cmdsTrustee, 0, sizeof(CMD_S_Trustee));

			cmdsTrustee.bUserTrustee = 1;
			cmdsTrustee.iUserChairID = 2;

            Json::Value msgTrustee;
            msgTrustee["MsgId"] = IDD_MESSAGE_ROOM;
            msgTrustee["MsgSubId"] = ID_S_GAMEMSG_TRUSTEE;
			CmdHelper::CodeMessage(cmdsTrustee, msgTrustee);
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgTrustee);

			if (m_iOperateUer == 2)
			{
				OperAUTO(m_iOperateUer, true);
			}
			else if (m_OperPriority[2] & Operation_Hu)
			{
				OperAUTO(m_iOperateUer, true);
			}
		}
		break;
	case TIMER_USER_OFFLINE + 3:
		{
			int iTimeID = timerId;
			m_g_GameRoom->StopTimer(iTimeID);

			enUserStatu[3] = PlayerStatu_Trustee;

			CMD_S_Trustee cmdsTrustee;
			memset(&cmdsTrustee, 0, sizeof(CMD_S_Trustee));

			cmdsTrustee.bUserTrustee = 1;
			cmdsTrustee.iUserChairID = 3;

            Json::Value msgTrustee;
            msgTrustee["MsgId"] = IDD_MESSAGE_ROOM;
            msgTrustee["MsgSubId"] = ID_S_GAMEMSG_TRUSTEE;
			CmdHelper::CodeMessage(cmdsTrustee, msgTrustee);
			m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgTrustee);

			if (m_iOperateUer == 3)
			{
				OperAUTO(m_iOperateUer, true);
			}
			else if (m_OperPriority[3] & Operation_Hu)
			{
				OperAUTO(m_iOperateUer, true);
			}
		}
		break;
	case TIMER_BUFFER:
		{
			m_g_GameRoom->StopTimer(TIMER_BUFFER);
			if (enUserStatu[m_iOperateUer] != PlayerStatu_Dropline)
			{
				OperAUTO(m_iOperateUer, true);
			}
		}
		break;
	case TIMER_DHU:
		{
			m_g_GameRoom->StopTimer(TimerID);
			if (enUserStatu[isOtherHu] != PlayerStatu_Dropline)
			{
				m_iOperateUer = isOtherHu;
				OperAUTO(isOtherHu, true);
			}
		}
		break;
	case TIMER_DELAY_READY:	//延时发准备消息
		{
			if (curTimer==0)
			{
				m_g_GameRoom->StopTimer(TIMER_DELAY_READY);
				for(int i=0;i<GAME_PLAYER;i++)
				{
					Player *pPlayer = m_g_GameRoom->GetPlayer(i);
					if(pPlayer == NULL) continue;

					//if(pPlayer->GetMatchCount() > 0)
					//{
						pPlayer->SendReadyMsg();
					//}
				}
			}
			return ;
		}
		break;
	}
}

bool CServerLogicFrame::isHu(int &iChairID, MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfHandMJ[], int iSelfHandMJCount, MJBYTE bMJ, bool iWalkType)
{
	bool isHu = false;

	//过水不胡判定
	if (m_iTouchPos <= MAX_ORG_MJ  - 4)
	{
		if (iWalkType)
		{
			if (m_GuoShuiInfo[iChairID].bGuoShuiMJ == bMJ)
			{
				return isHu;
			}
		}
		else if (m_GuoShuiInfo[iChairID].bGangPao == 1)
		{
			return isHu;
		}
	}

	//3色的判定
	int iHuaSeCount = 0;
	bool bHasWan = false;bool bHasTiao = false;bool bHasBing = false;
	for (int i=0; i<iSelfHUMJCount; ++i)
	{
		if (MASK_MAHJONGKIND(bSelfHUMJ[i].first) == MahJong_Wan)bHasWan = true;
		else if (MASK_MAHJONGKIND(bSelfHUMJ[i].first) == MahJong_Tiao)bHasTiao = true;
		else if (MASK_MAHJONGKIND(bSelfHUMJ[i].first) == MahJong_Bing)bHasBing = true;
	}
	for (int i=0; i<iSelfHandMJCount; ++i)
	{
		if (MASK_MAHJONGKIND(bSelfHandMJ[i]) == MahJong_Wan)bHasWan = true;
		else if (MASK_MAHJONGKIND(bSelfHandMJ[i]) == MahJong_Tiao)bHasTiao = true;
		else if (MASK_MAHJONGKIND(bSelfHandMJ[i]) == MahJong_Bing)bHasBing = true;
	}
	if (bHasWan) iHuaSeCount++;
	if (bHasTiao)iHuaSeCount++;
	if (bHasBing)iHuaSeCount++;
	if (iHuaSeCount >= 3)
	{
		return isHu;
	}

	isHu = m_pGameLogic->IsHu(bSelfHUMJ, iSelfHUMJCount, bSelfHandMJ, iSelfHandMJCount, bMJ);
	if (isHu)
	{
		return isHu;
	}

	//剩下  看七对的时候能不能胡，麻将库里没有写这方面的算法，特殊麻将特殊对待，加上
	MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
	memset(btmpHandMJ, 0, sizeof(MJBYTE)*MAX_HAND_MJ);
	memcpy(btmpHandMJ, m_bHandMJ[iChairID], sizeof(MJBYTE)*MAX_HAND_MJ);
	itmpHandCount = m_iHandMJCount[iChairID];
	btmpHandMJ[itmpHandCount] = bMJ;
	itmpHandCount++;
	m_pGameLogic->SortHandMJ(btmpHandMJ, itmpHandCount);
	int iDuiCount = 0;
	for (int i = 0; i < itmpHandCount;)
	{
		if (btmpHandMJ[i] == btmpHandMJ[i+1])
		{
			i += 2;
			iDuiCount++;
		}
		else
		{
			i++;
		}
	}
	if (iDuiCount == 7)
	{
		isHu = true;
		return isHu;
	}
	return isHu;
}

void CServerLogicFrame::OperOut(int iChairID, CMD_C_OutMJ &cmdOut, bool isTimeOut)
{
	if (iChairID != m_iOperateUer)
	{
		return;
	}
	if (m_iOperateUer == iChairID)
	{
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			m_iOldHandCount[i] = m_iHandMJCount[i];
		}
		bool isSuccess = m_pGameLogic->OutMJ(m_bHandMJ[iChairID], m_iHandMJCount[iChairID], m_bOutMJ[iChairID], m_iOutMJCount[iChairID], cmdOut.bOutMJ);
		if (isSuccess)
		{
			m_g_GameRoom->StopTimer(TIMER_Nomal);
			m_g_GameRoom->StopTimer(TIMER_BUFFER);
			//m_g_GameRoom->StopTimer(TIMER(TIMER_DHU,iChairID));
			//m_g_GameRoom->StopTimer(TIMER(TIMER_DHUNomal, iChairID));

			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				m_iHandMJCount[i] = m_iOldHandCount[i];
			}
			m_iHandMJCount[iChairID]--;
			memset(m_iOldHandCount, 0, sizeof(int)*GAME_PLAYER);
			memset(m_OperPriority, 0, sizeof(int)*GAME_PLAYER);
			memset(m_operationGaveUp, Operation_Hu, sizeof(int)*GAME_PLAYER);
			memset(m_bQiangGangMJ, 0, sizeof(int)*GAME_PLAYER);
			//打出一张牌后给手牌排序
			m_pGameLogic->SortHandMJ(m_bHandMJ[iChairID], m_iHandMJCount[iChairID]);
			m_isAlreadyHu = 0;
			m_huMJ = 0;
			//上次出牌玩家
			m_iOldCurrentUer = iChairID;
			m_iOldOperateUer = m_iOperateUer;
			//下一个操作玩家
			int itmpNexter = 0;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				itmpNexter = (i+iChairID) % GAME_PLAYER;
				if (itmpNexter == iChairID || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo)
				{
					continue;
				}
				m_iOperateUer = itmpNexter;
				if (m_iOperateUer == itmpNexter)
				{
					break;
				}
			}
			/////m_TouchPlayer = (m_iOldCurrentUer+1) % GAME_PLAYER;这句用上面for循环代替
			 m_TouchPlayer = m_iOperateUer;

			CMD_S_OutMJ cmdsOut[GAME_PLAYER];
			for (int i=0; i<GAME_PLAYER; i++)
			{
				memset(&cmdsOut[i], 0, sizeof(CMD_S_OutMJ));
				cmdsOut[i].bOutMJ = cmdOut.bOutMJ;
				cmdsOut[i].wOutMJUser = iChairID;
				cmdsOut[i].isTimeOut = isTimeOut;
			}

			int itmpChairID = 0;
			for (int i=0; i<GAME_PLAYER; ++i)
			{
				itmpChairID = (i+iChairID) % GAME_PLAYER;
				if (itmpChairID == iChairID || enUserStatu[itmpChairID] >= PlayerStatu_ZiMo)
				{
					continue;
				}
				if (m_bGangTouchMJ != 0)
				{
					UpdateOperation(itmpChairID, cmdOut.bOutMJ, m_bHUMJ[itmpChairID], m_iHUMJCount[itmpChairID], m_bHandMJ[itmpChairID], m_iHandMJCount[itmpChairID], m_bOutMJ[itmpChairID], m_iOutMJCount[itmpChairID], m_OperPriority[itmpChairID]);
				}
				else
				{
					UpdateOperation(itmpChairID, cmdOut.bOutMJ, m_bHUMJ[itmpChairID], m_iHUMJCount[itmpChairID], m_bHandMJ[itmpChairID], m_iHandMJCount[itmpChairID], m_bOutMJ[itmpChairID], m_iOutMJCount[itmpChairID], m_OperPriority[itmpChairID], true);
				}
			}
			m_iOperateUer = UpdatePriority(m_iOperateUer, m_iOldCurrentUer);

			for (int i=0; i<GAME_PLAYER; ++i)
			{
				//如果有操作，则其他玩家的为一
				if (m_OperPriority[m_iOperateUer] & Operation_Hu)
				{
					//如果下个玩家有胡操作，则检测其他玩家有没有胡操作,都给客户端发过去
					if (m_OperPriority[i] & Operation_Hu)
					{
						cmdsOut[i].bOperation = Operation_Hu;
						cmdsOut[i].isOtherPriority = 0;
					}
					else
					{
						cmdsOut[i].bOperation = 0;
						cmdsOut[i].isOtherPriority = 1;
					}
				}
				else if (m_OperPriority[m_iOperateUer])
				{
					if (i == m_iOperateUer)
					{
						cmdsOut[i].bOperation = m_OperPriority[m_iOperateUer];
						cmdsOut[i].isOtherPriority = 0;
					}
					else
					{
						cmdsOut[i].bOperation = 0;
						cmdsOut[i].isOtherPriority = 1;
					}
				}
				else
				{
					cmdsOut[i].bOperation = 0;
					cmdsOut[i].isOtherPriority = 0;
				}

                Json::Value msgOut;
                msgOut["MsgId"] = IDD_MESSAGE_ROOM;
                msgOut["MsgSubId"] = ID_S_GAMEMSG_OUT;
				CmdHelper::CodeMessage(cmdsOut[i], msgOut);
				m_g_GameRoom->SendTableMsg(i, msgOut);
			}

			int isDoubleHu = 0;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (m_OperPriority[i] & Operation_Hu)
				{
					isDoubleHu++;
				}
			}

			//没有操作，那么摸牌
			if (!m_OperPriority[m_iOperateUer])
			{
				if (m_iTouchPos >= MAX_ORG_MJ)
				{
					//流局
					OperGameOver(EndGameType_LiuJu);
					return;
				}
				else
				{
					OperTouch(m_TouchPlayer);
				}
			}
			else if (isDoubleHu >= 2)
			{
				//最后4张如果有胡牌的，先自动胡牌
				bool hasAutoHu = false;
				if (MAX_ORG_MJ - m_iTouchPos < 4)
				{
					for (int i=0;i<GAME_PLAYER;i++)
					{
						if (m_OperPriority[i] & Operation_Hu)
						{
							hasAutoHu = true;
							break;
						}
					}
					if (hasAutoHu)
					{
						//最后4张有能胡的自动胡牌
						int iNextHu = 0;
						for (int i=0;i<GAME_PLAYER;i++)
						{
							iNextHu = (i + iChairID) % GAME_PLAYER;
							if (iNextHu == iChairID || enUserStatu[iNextHu] >= PlayerStatu_ZiMo)
							{
								continue;
							}
							if (m_OperPriority[iNextHu] & Operation_Hu)
							{
								if (m_bGangTouchMJ != 0)
								{
									m_isLiuLieUser[iNextHu] =  m_bGangTouchMJ;
								}
								OperHu(iNextHu, iChairID, HuType_PingHu);
							}
						}
					}
				}
				else
				{
					m_bTouchMJ = cmdOut.bOutMJ;
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						if(enUserStatu[i] == PlayerStatu_Trustee && (m_OperPriority[i] & Operation_Hu))
						{
							m_g_GameRoom->StartTimer(TIMER(TIMER_DHU,i), 3);
						}
						else if (m_OperPriority[i] & Operation_Hu)
						{
							//开启定时器
							if (m_bGangTouchMJ != 0)
							{
								m_isLiuLieUser[i] =  m_bGangTouchMJ;
							}
							int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
							m_g_GameRoom->StartTimer(TIMER(TIMER_DHUNomal, i), itime);
						}
					}
				}
			}
			//否则有操作，那么做相关处理
			else
			{
				m_bTouchMJ = cmdOut.bOutMJ;
				if(enUserStatu[m_iOperateUer] == PlayerStatu_Trustee)
				{
					m_g_GameRoom->StartTimer(TIMER_BUFFER, 3);
				}
				else if (m_OperPriority[m_iOperateUer] & Operation_Hu)
				{
					//如果有杠摸了牌，在打出牌,别人能胡，则流泪
					if (m_bGangTouchMJ != 0)
					{
						m_bGangTouchMJ = 0;
						m_isLiuLieUser[m_iOperateUer] = true;
					}
					else if (m_bGangTouchMJ == 0)
					{
						m_isLiuLieUser[m_iOperateUer] = false;
					}
					//最后4张如果有胡牌的，先自动胡牌
					bool hasAutoHu = false;
					if (MAX_ORG_MJ - m_iTouchPos < 4)
					{
						OperHu(m_iOperateUer, iChairID, HuType_PingHu);
					}
					else
					{
						//开启定时器
						int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
						m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
					}
				}
				else
				{
					//开启定时器
					int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
					m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
				}
			}
		}
		//出牌就把杠开,杠后摸牌标志清空
		m_bGangTouchMJ = 0;
	}
	return;
}

void CServerLogicFrame::OperAUTO(int iChairID, bool isTimeOut)
{
	CMD_S_Trustee cmdsTrustee;
	memset(&cmdsTrustee, 0, sizeof(CMD_S_Trustee));
	cmdsTrustee.iUserChairID = iChairID;

	if (m_iOperateUer == iChairID)
	{
		if (m_OperPriority[m_iOperateUer])
		{
			OperGiveUp(iChairID);
		}
		//如果没有操作，那么就出牌
		else if (m_iHandMJCount[iChairID]%3==2)
		{
			CMD_C_OutMJ cmdOut;
			memset(&cmdOut, 0, sizeof(CMD_C_OutMJ));
			cmdOut.bOutMJ = m_bHandMJ[iChairID][m_iHandMJCount[iChairID]-1];
			OperOut(iChairID, cmdOut, isTimeOut);
		}
	}
	//如果不是当前玩家
	else
	{
		//如果有操作，那么放弃
		if (m_OperPriority[iChairID])
		{
			m_iOperateUer = iChairID;
			OperGiveUp(iChairID);
		}
	}
	return;
}

void CServerLogicFrame::OperTouch(int iChairID, int iTouchTpye)
{
	if (iChairID == m_iOperateUer)
	{
		MJBYTE mjTouch = m_bAllMJ[m_iTouchPos++];
		MJBYTE mjTmpTouch = mjTouch;
		m_bHandMJ[iChairID][m_iHandMJCount[iChairID]] = mjTouch;
		if (m_UserTouchFirst[iChairID] <= 2)
		{
			m_UserTouchFirst[iChairID]++;
		}
		//清空过水不胡状态 吧mj置为0就行
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			if (m_GuoShuiInfo[i].bWhoHuUser == iChairID)
			{
				m_GuoShuiInfo[i].bGuoShuiMJ = 0;
				m_GuoShuiInfo[i].bGangPao = 0;
			}
		}

		//摸完牌就要把一炮多响的下家摸牌标志清空
		m_HasHuTouch = -1;
		m_QiangHuCount = 0;
		m_GiveQiangHuCount = 0;
		m_BeiQiangUser = -1;
		//摸完牌就要把一炮多响的下家摸牌标志清空
		m_HasHuTouch = -1;

		CMD_S_TouchMJ cmdTouch;
		memset(&cmdTouch,0,sizeof(CMD_S_TouchMJ));
		cmdTouch.wCurrentUser = iChairID;
		cmdTouch.bTouchType = iTouchTpye;

		//是不是杠后摸牌
		m_bGangTouchMJ = 0;
		if (iTouchTpye)
		{
			m_bGangTouchMJ = mjTouch;
		}

		bool bIsHu = this->isHu(iChairID, m_bHUMJ[iChairID], m_iHUMJCount[iChairID], m_bHandMJ[iChairID], m_iHandMJCount[iChairID], mjTouch);
		if (bIsHu)
		{
			if (iTouchTpye)
			{
				m_bGangKaiMJ[iChairID] = mjTouch;
			}
			cmdTouch.bOperation |= Operation_Hu;
		}
		bool bIsZiMoGang = m_pGameLogic->IsSelfMingGang(m_bHUMJ[iChairID], m_iHUMJCount[iChairID], mjTmpTouch);
		if (bIsZiMoGang && (m_iHandMJCount[iChairID]>=1))
		{
			MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
			MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
			MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
			memcpy(btmpHuMJ, m_bHUMJ[iChairID], sizeof(MJBlock)*MAX_BLOCK_COUNT);
			itmpHuCount = m_iHUMJCount[iChairID];
			memcpy(btmpHandMJ, m_bHandMJ[iChairID], sizeof(MJBYTE)*MAX_HAND_MJ);
			itmpHandCount = m_iHandMJCount[iChairID];
			memcpy(btmpOutMJ, m_bOutMJ[iChairID], sizeof(MJBYTE)*MAX_OUT_MJ);
			itmpOutCount = m_iOutMJCount[iChairID];

			btmpHandMJ[itmpHandCount] = mjTmpTouch;
			itmpHandCount++;

			bIsZiMoGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, mjTmpTouch, Operation_Gang_Type_ZiMoMing);
			if (bIsZiMoGang)
			{
				cmdTouch.iGangCount = 1;
				cmdTouch.bMJGang[0] = mjTmpTouch;
				cmdTouch.bGangType = Operation_Gang_Type_ZiMoMing;
				cmdTouch.bOperation |= Operation_Gang;
			}
		}
		else
		{
			bool bIsAnGang = m_pGameLogic->IsAnGang(m_bHUMJ[iChairID], m_iHUMJCount[iChairID], m_bHandMJ[iChairID], m_iHandMJCount[iChairID], mjTouch, cmdTouch.bMJGang, cmdTouch.iGangCount);
			if (bIsAnGang && (m_iHandMJCount[iChairID]>=4))
			{
				for (int iCount=0; iCount<cmdTouch.iGangCount; iCount++)
				{
					MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
					MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
					MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
					memcpy(btmpHuMJ, m_bHUMJ[iChairID], sizeof(MJBlock)*MAX_BLOCK_COUNT);
					itmpHuCount = m_iHUMJCount[iChairID];
					memcpy(btmpHandMJ, m_bHandMJ[iChairID], sizeof(MJBYTE)*MAX_HAND_MJ);
					itmpHandCount = m_iHandMJCount[iChairID];
					memcpy(btmpOutMJ, m_bOutMJ[iChairID], sizeof(MJBYTE)*MAX_OUT_MJ);
					itmpOutCount = m_iOutMJCount[iChairID];

					btmpHandMJ[itmpHandCount] = mjTouch;
					itmpHandCount++;

					bIsAnGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, cmdTouch.bMJGang[iCount], Operation_Gang_Type_An);
					if (bIsAnGang)
					{
						cmdTouch.bGangType |= Operation_Gang_Type_An;
						cmdTouch.bOperation |= Operation_Gang;
					}
				}
			}
		}

		bool HasOperMal = false;

		if (cmdTouch.bOperation)
		{
			m_OperPriority[m_iOperateUer] = cmdTouch.bOperation;
			HasOperMal = true;
		}
		else
		{
			m_OperPriority[m_iOperateUer] = 0;
		}

		//判断完可能的操作，摸牌数量加一
		m_iHandMJCount[iChairID]++;

		cmdTouch.bTouchMJ = mjTouch;

        Json::Value msgOut;
        msgOut["MsgId"] = IDD_MESSAGE_ROOM;
        msgOut["MsgSubId"] = ID_S_GAMEMSG_TOUCH;
		CmdHelper::CodeMessage(cmdTouch, msgOut);
		m_g_GameRoom->SendTableMsg(cmdTouch.wCurrentUser, msgOut);

		//隐藏信息
		cmdTouch.bTouchMJ = CONST_MJ_HIDE;
		cmdTouch.bOperation = Operation_Null;
		cmdTouch.bGangType = 0;
		memset(cmdTouch.bMJGang, 0, sizeof(MJBYTE)*MAX_SELECT_GANG);
		cmdTouch.iGangCount = 0;

		for (int i=0;i<GAME_PLAYER;i++)
		{
			if (i != cmdTouch.wCurrentUser)
			{
                Json::Value msgOut;
                msgOut["MsgId"] = IDD_MESSAGE_ROOM;
                msgOut["MsgSubId"] = ID_S_GAMEMSG_TOUCH;
				CmdHelper::CodeMessage(cmdTouch, msgOut);
				m_g_GameRoom->SendTableMsg(i, msgOut);
			}
		}


		if (MAX_ORG_MJ - m_iTouchPos < 4 && m_OperPriority[iChairID] & Operation_Hu)
		{
			//最后4张能胡必胡
			OperHu(iChairID, -1, HuType_ZiMo);
		}
		//托管3秒自动打出
		//摸牌后启动计时器
		else if (enUserStatu[iChairID] == PlayerStatu_Trustee)
		{
			m_bTouchMJ = mjTouch;
			m_g_GameRoom->StartTimer(TIMER_BUFFER, 3);
		}
		else
		{
			int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
			m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
		}
	}
	return;
}

void CServerLogicFrame::OperPeng(int playerId)
{
	if (playerId != m_iOperateUer)
	{
		return;
	}
	MJBYTE bPengMJ = m_bOutMJ[m_iOldOperateUer][m_iOutMJCount[m_iOldOperateUer]-1];
	bool bScuuess = m_pGameLogic->Peng(m_bHUMJ[playerId], m_iHUMJCount[playerId], m_bHandMJ[playerId], m_iHandMJCount[playerId], m_bOutMJ[m_iOldOperateUer], m_iOutMJCount[m_iOldOperateUer], bPengMJ);
	if (bScuuess)
	{
		m_g_GameRoom->StopTimer(TIMER_Nomal);
		m_g_GameRoom->StopTimer(TIMER_BUFFER);
		memset(m_OperPriority, 0, sizeof(int)*GAME_PLAYER);
		m_iOperateUer = playerId;
		//碰完后给手牌排序
		m_pGameLogic->SortHandMJ(m_bHandMJ[playerId], m_iHandMJCount[playerId]);
		CMD_S_PengMJ cmdsPeng;
		memset(&cmdsPeng, 0, sizeof(CMD_S_PengMJ));
		cmdsPeng.wCurrentUser = playerId;
		cmdsPeng.wLastOutUser = m_iOldOperateUer;
		cmdsPeng.bMJPeng = bPengMJ;

        Json::Value msgOut;
        msgOut["MsgId"] = IDD_MESSAGE_ROOM;
        msgOut["MsgSubId"] = ID_S_GAMEMSG_PENG;
		CmdHelper::CodeMessage(cmdsPeng, msgOut);
		m_g_GameRoom->SendTableMsg(playerId, msgOut);

		//隐藏信息
		cmdsPeng.bOperation = 0;

		for (int i=0; i<GAME_PLAYER; i++)
		{
			if (i!=playerId)
			{
                Json::Value msgOut;
                msgOut["MsgId"] = IDD_MESSAGE_ROOM;
                msgOut["MsgSubId"] = ID_S_GAMEMSG_PENG;
				CmdHelper::CodeMessage(cmdsPeng, msgOut);
				m_g_GameRoom->SendTableMsg(i, msgOut);
			}
		}
		//开启定时器
		int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
		m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
	}
	return;
}

void CServerLogicFrame::OperGang(int iChairID, CMD_C_GangMJ &cmdGang)
{
	if (iChairID != m_iOperateUer)
	{
		return;
	}

	bool bSuccess = m_pGameLogic->Gang(m_bHUMJ[m_iOperateUer], m_iHUMJCount[m_iOperateUer], m_bHandMJ[m_iOperateUer], m_iHandMJCount[m_iOperateUer], m_bOutMJ[m_iOldOperateUer], m_iOutMJCount[m_iOldOperateUer], cmdGang.bGangMJ, cmdGang.bGangType);

	if (bSuccess)
	{
		m_g_GameRoom->StopTimer(TIMER_Nomal);
		m_g_GameRoom->StopTimer(TIMER_BUFFER);
		memset(m_OperPriority, 0, sizeof(int)*GAME_PLAYER);
		//杠完后给手牌排序
		m_pGameLogic->SortHandMJ(m_bHandMJ[m_iOperateUer], m_iHandMJCount[m_iOperateUer]);

		CMD_S_GangMJ cmdsGang;
		memset(&cmdsGang, 0, sizeof(CMD_S_GangMJ));

		//杠牌后可能有操作  抢杠胡 (抢杠胡算自摸)
		int iHuUser = INVALID_CHAIR;int iTmpChairID = INVALID_CHAIR;
		if (cmdGang.bGangType & Operation_Gang_Type_ZiMoMing)
		{
			memset(m_OperPriority, 0, sizeof(int)*GAME_PLAYER);
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				iTmpChairID = (i+iChairID) % GAME_PLAYER;
				if (iTmpChairID == iChairID || enUserStatu[iTmpChairID] >= PlayerStatu_ZiMo)
				{
					continue;
				}
				bSuccess = this->isHu(iTmpChairID, m_bHUMJ[iTmpChairID], m_iHUMJCount[iTmpChairID], m_bHandMJ[iTmpChairID], m_iHandMJCount[iTmpChairID], cmdGang.bGangMJ);
				if (bSuccess)
				{
					cmdsGang.bOperation |= Operation_Hu;
					if (iHuUser == INVALID_CHAIR)
					{
						iHuUser = iTmpChairID;
						//保存有操作的玩家
						m_iOperateUer = iTmpChairID;
						m_BeiQiangUser = iChairID;
					}
					m_bQiangGangMJ[iTmpChairID] = cmdGang.bGangMJ;
					m_OperPriority[iTmpChairID] = Operation_Hu;
					m_QiangHuCount++;
					//这里的break不能要，不然就最多只能检测出一家能有胡操作，
					//因为血战麻将能一炮多响，所以必须每个玩家都要检测到
					//break;
				}
			}
		}

		//准备发送消息
        Json::Value msgGang;
        msgGang["MsgId"] = IDD_MESSAGE_ROOM;
        msgGang["MsgSubId"] = ID_S_GAMEMSG_GANG;

		cmdsGang.wOutMJUser = m_iOldCurrentUer;
		cmdsGang.wCurrentUser = iChairID;
		cmdsGang.bMJGang = cmdGang.bGangMJ;
		cmdsGang.bGangType = cmdGang.bGangType;

		//找一个位置插入刮风下雨相关信息
		for (int i = 0; i < GuaFengMaxCount; ++i)
		{
			if (m_GuaFengInfo[i].eGangType == Operation_Gang_Type_Null)
			{
				m_GuaFengInfo[i].eGangPlayer = iChairID;
				m_GuaFengInfo[i].eGangType = cmdGang.bGangType;
				memset(m_GuaFengInfo[i].eYinGanger, INVALID_CHAIR, sizeof(int)*GAME_PLAYER);
				if (cmdGang.bGangType & Operation_Gang_Type_An)
				{
					//实时记录杠的最后得分
					int GuaFengSocre = 0;
					GuaFengSocre = GuaFeng_An_ZiMoMing;

					for (int j = 0; j < GAME_PLAYER; ++j)
					{
						if (j != iChairID && enUserStatu[j] < PlayerStatu_ZiMo)
						{
							m_GuaFengInfo[i].eYinGanger[j] = j;
							//判断刮风下雨是否有玩家强制退出
							if (-GuaFengSocre*m_iBisicCore + m_UserMoney[j] <= 0)
							{
								m_isForceQuit[j] = 1;
								m_GuaFengInfo[i].eGangSorce[j] = m_UserMoney[j];
								m_GuaFengInfo[i].eGangSorce[iChairID] += m_UserMoney[j];

								assert(m_UserMoney[j] >= 0);
								m_AllGangSorce[j] += -m_UserMoney[j];
								m_AllGangSorce[iChairID] += m_UserMoney[j];

								//实时更新总分
								m_AllTotal[iChairID] += m_UserMoney[j];
								m_AllTotal[j] += -m_UserMoney[j];
								//实时更新数据库总分
								m_UserMoney[iChairID] += m_UserMoney[j];
								m_UserMoney[j] = 0;
							}
							else
							{
								m_GuaFengInfo[i].eGangSorce[j] += GuaFengSocre*m_iBisicCore;
								m_GuaFengInfo[i].eGangSorce[iChairID] += GuaFengSocre*m_iBisicCore;

								m_AllGangSorce[iChairID] += GuaFengSocre*m_iBisicCore;
								m_AllGangSorce[j] -= GuaFengSocre*m_iBisicCore;
								//实时更新总分
								m_AllTotal[iChairID] += GuaFengSocre*m_iBisicCore;
								m_AllTotal[j] -= GuaFengSocre*m_iBisicCore;
								//实时更新数据库总分
								m_UserMoney[iChairID] += GuaFengSocre*m_iBisicCore;
								m_UserMoney[j] -= GuaFengSocre*m_iBisicCore;
							}
						}
					}
					cmdsGang.iGangScore = GuaFengSocre*m_iBisicCore;
					//m_GuaFengInfo[i].eGangSorce = cmdsGang.iGangScore;
				}
				else if (cmdGang.bGangType & Operation_Gang_Type_Ming)
				{
					m_GuaFengInfo[i].eYinGanger[cmdsGang.wOutMJUser] = cmdsGang.wOutMJUser;

					cmdsGang.iGangScore = GuaFeng_Ming*m_iBisicCore;
					//m_GuaFengInfo[i].eGangSorce = cmdsGang.iGangScore;

					//判断刮风下雨是否有玩家强制退出
					if (-GuaFeng_Ming*m_iBisicCore + m_UserMoney[cmdsGang.wOutMJUser] <= 0)
					{
						m_GuaFengInfo[i].eGangSorce[cmdsGang.wOutMJUser] = m_UserMoney[cmdsGang.wOutMJUser];
						m_GuaFengInfo[i].eGangSorce[iChairID] += m_UserMoney[cmdsGang.wOutMJUser];

						assert(m_UserMoney[cmdsGang.wOutMJUser] >= 0);
						m_AllGangSorce[cmdsGang.wOutMJUser] += m_UserMoney[cmdsGang.wOutMJUser];
						m_AllGangSorce[iChairID] += m_UserMoney[cmdsGang.wOutMJUser];

						m_isForceQuit[cmdsGang.wOutMJUser] = 1;
						//实时更新总分
						m_AllTotal[iChairID] += m_UserMoney[cmdsGang.wOutMJUser];
						m_AllTotal[cmdsGang.wOutMJUser] -= m_UserMoney[cmdsGang.wOutMJUser];

						m_UserMoney[iChairID] += m_UserMoney[cmdsGang.wOutMJUser];
						m_UserMoney[cmdsGang.wOutMJUser] = 0;
					}
					else
					{
						m_GuaFengInfo[i].eGangSorce[cmdsGang.wOutMJUser] += GuaFeng_Ming*m_iBisicCore;
						m_GuaFengInfo[i].eGangSorce[iChairID] += GuaFeng_Ming*m_iBisicCore;
						//实时更新总分
						m_AllTotal[iChairID] += GuaFeng_Ming*m_iBisicCore;
						m_AllTotal[cmdsGang.wOutMJUser] -= GuaFeng_Ming*m_iBisicCore;
						//实时记录杠的最后得分
						m_AllGangSorce[iChairID] += GuaFeng_Ming*m_iBisicCore;
						m_AllGangSorce[cmdsGang.wOutMJUser] -= GuaFeng_Ming*m_iBisicCore;

						m_UserMoney[iChairID] += GuaFeng_Ming*m_iBisicCore;
						m_UserMoney[cmdsGang.wOutMJUser] -= GuaFeng_Ming*m_iBisicCore;
					}
				}
				else if (cmdGang.bGangType & Operation_Gang_Type_ZiMoMing)
				{
					cmdsGang.iGangScore = GuaFeng_Ming*m_iBisicCore;
				}
				break;
			}
		}

		//如果是抢杠胡
		if (cmdsGang.bOperation & Operation_Hu)
		{
			cmdsGang.bMJGang = cmdGang.bGangMJ;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (m_OperPriority[i] & Operation_Hu)
				{
					cmdsGang.bOperation = Operation_Hu;
					cmdsGang.isOtherPriority = 0;
				}
				else
				{
					cmdsGang.bOperation = 0;
					cmdsGang.isOtherPriority = 1;
				}
                Json::Value msgGang;
                msgGang["MsgId"] = IDD_MESSAGE_ROOM;
                msgGang["MsgSubId"] = ID_S_GAMEMSG_GANG;
				CmdHelper::CodeMessage(cmdsGang, msgGang);
				m_g_GameRoom->SendTableMsg(i, msgGang);
			}
			if (MAX_ORG_MJ - m_iTouchPos < 4)
			{
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (i != iChairID && (m_OperPriority[i] & Operation_Hu))
					{
						OperHu(i, m_BeiQiangUser, HuType_QiangGang);
					}
				}
			}
			else
			{
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (m_OperPriority[i] & Operation_Hu)
					{
						//开启定时器
						if (enUserStatu[i] == PlayerStatu_Trustee)
						{
							m_g_GameRoom->StartTimer(TIMER(TIMER_DHU, i), 3);
						}
						else
						{
							int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
							m_g_GameRoom->StartTimer(TIMER(TIMER_DHUNomal, i), itime);
						}
					}
				}
			}
		}
		else
		{
			//如果是自摸明杠+上刮风下雨
			if (cmdsGang.bGangType & Operation_Gang_Type_ZiMoMing)
			{
				for (int i = 0; i < GuaFengMaxCount; ++i)
				{
					if (m_GuaFengInfo[i].eGangType == Operation_Gang_Type_Null)
					{
						m_GuaFengInfo[i].eGangPlayer = iChairID;
						m_GuaFengInfo[i].eGangType = cmdGang.bGangType;
						memset(m_GuaFengInfo[i].eYinGanger, INVALID_CHAIR, sizeof(int)*GAME_PLAYER);

						//实时记录杠的最后得分
						int GuaFengSocre = 0;
						GuaFengSocre = GuaFeng_Ming;

						for (int j = 0; j < GAME_PLAYER; ++j)
						{
							if (j != iChairID && enUserStatu[j] < PlayerStatu_ZiMo)
							{
								m_GuaFengInfo[i].eYinGanger[j] = j;

								//判断刮风下雨是否有玩家强制退出
								if (-GuaFengSocre*m_iBisicCore+ m_UserMoney[j] <= 0)
								{
									m_GuaFengInfo[i].eGangSorce[j] += m_UserMoney[j];
									m_GuaFengInfo[i].eGangSorce[iChairID] += m_UserMoney[j];

									m_AllGangSorce[j] += -m_UserMoney[j];
									m_AllGangSorce[iChairID] += m_UserMoney[j];

									m_isForceQuit[j] = 1;

									//实时更新总分
									m_AllTotal[iChairID] += m_UserMoney[j];
									m_AllTotal[j] -= m_UserMoney[j];

									m_UserMoney[iChairID] += m_UserMoney[j];
									m_UserMoney[j] = 0;
								}
								else
								{
									m_GuaFengInfo[i].eGangSorce[j] += GuaFengSocre*m_iBisicCore;
									m_GuaFengInfo[i].eGangSorce[iChairID] += GuaFengSocre*m_iBisicCore;

									m_AllGangSorce[j] += -GuaFengSocre*m_iBisicCore;
									m_AllGangSorce[iChairID] += GuaFengSocre*m_iBisicCore;

									//实时更新总分
									m_AllTotal[iChairID] += GuaFengSocre*m_iBisicCore;
									m_AllTotal[j] -= GuaFengSocre*m_iBisicCore;

									m_UserMoney[iChairID] += GuaFengSocre*m_iBisicCore;
									m_UserMoney[j] -= GuaFengSocre*m_iBisicCore;
								}
							}
						}
						cmdsGang.iGangScore = GuaFengSocre*m_iBisicCore;
						//m_GuaFengInfo[i].eGangSorce = cmdsGang.iGangScore;
						break;
					}

				}
			}

			//发送消息
			CmdHelper::CodeMessage(cmdsGang, msgGang);
			m_g_GameRoom->SendTableMsg(iChairID, msgGang);
			m_iOperateUer = iChairID;

			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (i == iChairID)
				{
					continue;
				}
				if (cmdsGang.bGangType & Operation_Gang_Type_An)
				{
					cmdsGang.bMJGang = 0;
				}
				else
				{
					cmdsGang.bMJGang = cmdGang.bGangMJ;
				}

                Json::Value msgGang;
                msgGang["MsgId"] = IDD_MESSAGE_ROOM;
                msgGang["MsgSubId"] = ID_S_GAMEMSG_GANG;
				CmdHelper::CodeMessage(cmdsGang, msgGang);
				m_g_GameRoom->SendTableMsg(i, msgGang);
			}

			if (m_iTouchPos >= MAX_ORG_MJ)
			{
				//流局
				OperGameOver(EndGameType_LiuJu);
				return;
			}
			else
			{
				OperTouch(m_iOperateUer, true);
			}
		}



		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			if (i != iChairID && m_isForceQuit[i] == 1 )
			{
				m_ForceUserID[i]++;
				m_isForceQuit[i] = 0;
				Player *pPlayer = m_g_GameRoom->GetPlayer(i);
				if(pPlayer != NULL && m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
				{
					int64 pPlayerMoney = int64((-1)*pPlayer->GetMoney());
					m_g_GameRoom->WriteUserScore(i, pPlayerMoney, 0, enScoreKind_Lost);
				}
				OperHu(i, INVALID_CHAIR, HuType_ForceQuit);
			}
		}

	}
	return;
}

void  CServerLogicFrame::OperGiveUp(int playId)
{
	if (playId != m_iOperateUer)
	{
		return;
	}
	RoomState rs = m_g_GameRoom->GetRoomState();
	if (rs!=  ROOMSTATE_GAMING)return;

	CMD_S_GiveUp cmdsGiveUp;
	memset(&cmdsGiveUp, 0, sizeof(CMD_S_GiveUp));

	m_g_GameRoom->StopTimer(TIMER_Nomal);
	m_g_GameRoom->StopTimer(TIMER_BUFFER);
	m_g_GameRoom->StopTimer(TIMER(TIMER_DHU,playId));
	m_g_GameRoom->StopTimer(TIMER(TIMER_DHUNomal, playId));

	//如果是自己摸牌后有操作而放弃
	if (m_iHandMJCount[playId]%3 == 2)
	{
		//吧自己的操作清0,不然可能开启两次普通定时间
		//吧杠开的牌请0;
		m_bGangKaiMJ[playId] = 0;
		m_OperPriority[playId] = 0;
		cmdsGiveUp.isOtherPriority = 0;
		cmdsGiveUp.iOperUser = playId;
		cmdsGiveUp.iLastOutUser = m_iOldCurrentUer;

        Json::Value msgGiveUp;
        msgGiveUp["MsgId"] = IDD_MESSAGE_ROOM;
        msgGiveUp["MsgSubId"] = ID_S_GAMEMSG_GIVEUP;
		CmdHelper::CodeMessage(cmdsGiveUp, msgGiveUp);
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgGiveUp);

		//在托管的状态下，就开启一个3秒定时器，帮其打出一张牌
		if (enUserStatu[playId] == PlayerStatu_Trustee)
		{
			m_bTouchMJ = m_bHandMJ[playId][m_iHandMJCount[playId]-1];
			m_g_GameRoom->StartTimer(TIMER_BUFFER, 3);
		}
		else
		{
			//开启一个普通定时器
			int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
			m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
		}
	}
	//如果是别人打出一张牌后有操作而放弃，那么就判断从放弃的这个玩家开始到打出牌玩家之间玩家是否有操作
	else
	{
		bool bIsHu = false;
		MJBYTE bLastOutMJ = m_bOutMJ[m_iOldCurrentUer][m_iOutMJCount[m_iOldCurrentUer]-1];

		if (enUserStatu[playId] == PlayerStatu_Escape)
		{
			m_OperPriority[playId] = 0;
		}
		//////////////////////////////////////////////////////////////////////////
		//int iNextUser = (m_iOldCurrentUer+1)%GAME_PLAYER;
		//这句被下面的for循环取代
		bool isZiMoMingTouch = false;
		if (m_OperPriority[playId] & Operation_Hu)
		{
			//过水不胡信息添加 要排除抢杠胡放弃
			if (m_bQiangGangMJ[playId] == 0)
			{
				m_GuoShuiInfo[playId].bGSStartUser = m_iOldCurrentUer;
				m_GuoShuiInfo[playId].bWhoHuUser = playId;
				m_GuoShuiInfo[playId].bGuoShuiMJ = bLastOutMJ;
				if (m_isLiuLieUser[playId] != 0)
				{
					m_GuoShuiInfo[playId].bGangPao = 1;
				}
			}
			else
			{
				m_GiveQiangHuCount++;
				m_bQiangGangMJ[playId] = 0;
				if (m_GiveQiangHuCount == m_QiangHuCount)
				{
					isZiMoMingTouch = true;
					m_GiveQiangHuCount = 0; m_QiangHuCount = 0;
					//吧自摸明杠的分加上
					for (int i = 0; i < GuaFengMaxCount; ++i)
					{
						if (m_GuaFengInfo[i].eGangType == Operation_Gang_Type_Null)
						{
							m_GuaFengInfo[i].eGangPlayer = m_BeiQiangUser;
							m_GuaFengInfo[i].eGangType = Operation_Gang_Type_ZiMoMing;
							memset(m_GuaFengInfo[i].eYinGanger, INVALID_CHAIR, sizeof(int)*GAME_PLAYER);
							//实时记录杠的最后得分
							for (int j = 0; j < GAME_PLAYER; ++j)
							{
								if (j != m_BeiQiangUser && enUserStatu[j] < PlayerStatu_ZiMo)
								{
									m_GuaFengInfo[i].eYinGanger[j] = j;
									//判断刮风下雨是否有玩家强制退出
									if (GuaFeng_Ming*m_iBisicCore + m_UserMoney[j] <= 0)
									{
										m_GuaFengInfo[i].eGangSorce[j] += m_UserMoney[j];
										m_GuaFengInfo[i].eGangSorce[m_BeiQiangUser] += m_UserMoney[j];

										m_AllGangSorce[j] += -m_UserMoney[j];
										m_AllGangSorce[m_BeiQiangUser] += m_UserMoney[j];

										m_isForceQuit[j] = 1;

										//实时更新总分
										m_AllTotal[m_BeiQiangUser] += m_UserMoney[j];
										m_AllTotal[j] -= m_UserMoney[j];

										m_UserMoney[m_BeiQiangUser] += m_UserMoney[j];
										m_UserMoney[j] = 0;
									}
									else
									{
										m_GuaFengInfo[i].eGangSorce[j] += GuaFeng_Ming*m_iBisicCore;
										m_GuaFengInfo[i].eGangSorce[m_BeiQiangUser] += GuaFeng_Ming*m_iBisicCore;

										m_AllGangSorce[j] += -GuaFeng_Ming*m_iBisicCore;
										m_AllGangSorce[m_BeiQiangUser] += GuaFeng_Ming*m_iBisicCore;
										//实时更新总分
										m_AllTotal[m_BeiQiangUser] += GuaFeng_Ming*m_iBisicCore;
										m_AllTotal[j] -= GuaFeng_Ming*m_iBisicCore;

										m_UserMoney[m_BeiQiangUser] += GuaFeng_Ming*m_iBisicCore;
										m_UserMoney[j] -= GuaFeng_Ming*m_iBisicCore;
									}
								}
							}
							//m_GuaFengInfo[i].eGangSorce = GuaFeng_Ming*m_iBisicCore;
							break;
						}
					}
				}
			}
			m_OperPriority[playId] ^= Operation_Hu;
			m_isLiuLieUser[playId] = 0;
		}
		else if (m_OperPriority[playId])
		{
			m_OperPriority[playId] = 0;
		}

		//放弃之后服务器索要进行操作
		//首先判断一炮多响问题，如果有玩家放弃，还存在一炮多响，那么就给客户端发送信息
		//其次不是胡操作，放弃当前操作，检测其他玩家有没有操作，虽然这里其他玩家不可能有操作，但是测试的时候要+上
		//最后放弃的是最后一个操作，即所有人都没有操作，那么给出牌的下家摸牌，并且如果是一炮多响的时候给胡的下家摸牌

		cmdsGiveUp.iLastOutUser = m_iOldCurrentUer;
		int isHuCount = 0;int isOtherMal = 0;
		for (int i =0; i < GAME_PLAYER; ++i)
		{
			if (m_OperPriority[i] & Operation_Hu)
			{
				isHuCount++;
			}
			else if (m_OperPriority[i])
			{
				isOtherMal++;
			}
		}
		//首先判断一炮多响问题，如果有玩家放弃，还存在一炮多响，那么就给客户端发送信息
		if (isHuCount > 0)
		{
			m_operationGaveUp[playId] = 0;
			cmdsGiveUp.iOperUser = playId;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (m_OperPriority[i] & Operation_Hu)
				{
					//抢杠胡
					if (m_bQiangGangMJ[i] != 0)
					{
						cmdsGiveUp.bOperation |= Operation_Hu;
						cmdsGiveUp.bOutMJ = m_bQiangGangMJ[i];
						cmdsGiveUp.isOtherPriority = 0;
						cmdsGiveUp.isOtherHu = 1;
					}
					//点炮胡
					else
					{
						cmdsGiveUp.bOperation |= Operation_Hu;
						cmdsGiveUp.bOutMJ = bLastOutMJ;
						cmdsGiveUp.isOtherPriority = 0;
						cmdsGiveUp.isOtherHu = 1;
					}
				}
				else
				{
					cmdsGiveUp.bOperation = 0;
					cmdsGiveUp.isOtherPriority = 1;
					cmdsGiveUp.isOtherHu = 1;
				}
                Json::Value msgGiveUp;
                msgGiveUp["MsgId"] = IDD_MESSAGE_ROOM;
                msgGiveUp["MsgSubId"] = ID_S_GAMEMSG_GIVEUP;
				CmdHelper::CodeMessage(cmdsGiveUp, msgGiveUp);
				m_g_GameRoom->SendTableMsg(i, msgGiveUp);
			}
		}
		//其次不是胡操作，放弃当前操作，检测其他玩家有没有操作，虽然这里其他玩家不可能有操作，但是测试的时候要+上
		else if (isOtherMal > 0)
		{
			//这个变量主要用于一炮多响处理的ioperuser
			cmdsGiveUp.iOperUser = playId;
			int iNextMal = 0;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				iNextMal = (i + m_iOldCurrentUer) % GAME_PLAYER;
				if (iNextMal == m_iOldCurrentUer || enUserStatu[iNextMal] >= PlayerStatu_ZiMo)
				{
					continue;
				}
				if (m_OperPriority[iNextMal])
				{
					assert(iNextMal != m_iOldOperateUer);
					m_iOperateUer = iNextMal;
					break;
				}
			}
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (i == iNextMal)
				{
					cmdsGiveUp.bOperation = m_OperPriority[i];
					cmdsGiveUp.bOutMJ = bLastOutMJ;
					cmdsGiveUp.isOtherPriority = 0;
				}
				else
				{
					cmdsGiveUp.bOperation = 0;
					cmdsGiveUp.bOutMJ = bLastOutMJ;
					cmdsGiveUp.isOtherPriority = 1;
				}
                Json::Value msgGiveUp;
                msgGiveUp["MsgId"] = IDD_MESSAGE_ROOM;
                msgGiveUp["MsgSubId"] = ID_S_GAMEMSG_GIVEUP;
				CmdHelper::CodeMessage(cmdsGiveUp, msgGiveUp);
				m_g_GameRoom->SendTableMsg(i, msgGiveUp);
			}
		}
		else if ((isHuCount == 0) && (isOtherMal == 0))
		{
			//这个变量主要用于一炮多响处理的ioperuser
			cmdsGiveUp.iOperUser = playId;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				cmdsGiveUp.bOperation = 0;
				cmdsGiveUp.bOutMJ = bLastOutMJ;
				cmdsGiveUp.isOtherPriority = 0;

                Json::Value msgGiveUp;
                msgGiveUp["MsgId"] = IDD_MESSAGE_ROOM;
                msgGiveUp["MsgSubId"] = ID_S_GAMEMSG_GIVEUP;
				CmdHelper::CodeMessage(cmdsGiveUp, msgGiveUp);
				m_g_GameRoom->SendTableMsg(i, msgGiveUp);
			}
			//发送完信息，该哪家摸牌就摸牌吧
			if (m_iTouchPos >= MAX_ORG_MJ)
			{
				//流局
				OperGameOver(EndGameType_LiuJu);
				return;
			}
			else
			{
				if (m_HasHuTouch >= 0 && enUserStatu[m_HasHuTouch] < PlayerStatu_ZiMo)
				{
					m_iOperateUer = m_HasHuTouch;
					m_TouchPlayer = m_HasHuTouch;
					m_HasHuTouch = -1;
				}
				else
				{
					int iNextToucher = 0;
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						iNextToucher = (i + m_iOldOperateUer) % GAME_PLAYER;
						if (iNextToucher == m_iOldOperateUer || enUserStatu[iNextToucher] >= PlayerStatu_ZiMo)
						{
							continue;
						}
						m_TouchPlayer = iNextToucher;
						if (m_TouchPlayer == iNextToucher)
						{
							m_iOperateUer = iNextToucher;
							break;
						}
					}
				}
				if (isZiMoMingTouch)
				{
					if (m_BeiQiangUser >= 0 && enUserStatu[m_BeiQiangUser] < PlayerStatu_ZiMo)
					{
						m_iOperateUer = m_BeiQiangUser;
						m_TouchPlayer = m_BeiQiangUser;
						m_BeiQiangUser = -1;
					}
					OperTouch(m_TouchPlayer, true);
				}
				else
				{
					OperTouch(m_TouchPlayer);
				}
			}
		}

		//开定时器
		if (isOtherMal)
		{
			if (enUserStatu[m_iOperateUer] == PlayerStatu_Trustee)
			{
				m_g_GameRoom->StartTimer(TIMER_BUFFER, 3);
			}
			else
			{
				int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
				m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
			}
		}
	}
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (i != playId && m_isForceQuit[i] == 1)
		{
			m_ForceUserID[i]++;
			m_isForceQuit[i] = 0;
			Player *pPlayer = m_g_GameRoom->GetPlayer(i);
			if(pPlayer != NULL && m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
			{
				int64 pPlayerMoney = int64((-1)*pPlayer->GetMoney());
				m_g_GameRoom->WriteUserScore(i, pPlayerMoney, 0, enScoreKind_Lost);
			}
			OperHu(i, INVALID_CHAIR, HuType_ForceQuit);
		}
	}
	return;
}

void CServerLogicFrame::OperTrustee(int iChairID, bool isTimeOut)
{
	CMD_S_Trustee cmdsTrustee;
	memset(&cmdsTrustee, 0, sizeof(CMD_S_Trustee));
	cmdsTrustee.iUserChairID = iChairID;

	if (enUserStatu[iChairID] == PlayerStatu_Trustee)
	{
		enUserStatu[iChairID] = PlayerStatu_Normal;
		cmdsTrustee.bUserTrustee = 0;
		m_iAUTO_OutCount[iChairID] = 0;
		m_TruBuffer[iChairID] = false;
		//发送托管消息
        Json::Value msgTrustee;
        msgTrustee["MsgId"] = IDD_MESSAGE_ROOM;
        msgTrustee["MsgSubId"] = ID_S_GAMEMSG_TRUSTEE;
		CmdHelper::CodeMessage(cmdsTrustee, msgTrustee);
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgTrustee);
	}
	else
	{
		enUserStatu[iChairID] = PlayerStatu_Trustee;
		cmdsTrustee.bUserTrustee = 1;
		m_TruBuffer[iChairID] = true;
		//发送托管消息
        Json::Value msgTrustee;
        msgTrustee["MsgId"] = IDD_MESSAGE_ROOM;
        msgTrustee["MsgSubId"] = ID_S_GAMEMSG_TRUSTEE;
		CmdHelper::CodeMessage(cmdsTrustee, msgTrustee);
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgTrustee);
		//一炮多响的时候第一个人
		if (m_iOperateUer == iChairID)
		{
			OperAUTO(iChairID, true);
		}
		//一炮多响的时候第二，三人
		else if (m_OperPriority[iChairID] & Operation_Hu)
		{
			OperAUTO(iChairID, true);
		}
	}
	return;

}
void CServerLogicFrame::LookHuaZhuDaJiao(CMD_S_GameOver &cmdsGameOver)
{
	//先判断每个人的状态，是花猪还是大叫
	int64 iMaxPayCount[GAME_PLAYER];
	int64 iNoramlFan[GAME_PLAYER];
	int64 iExtraFan[GAME_PLAYER];
	memset(iMaxPayCount, 0, sizeof(iMaxPayCount));
	memset(iNoramlFan, 0, sizeof(iNoramlFan));
	memset(iExtraFan, 0, sizeof(iExtraFan));

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (enUserStatu[i] < PlayerStatu_ZiMo)
		{
			//3色的判定
			int iHuaSeCount = 0;
			bool bHasWan = false;bool bHasTiao = false;bool bHasBing = false;
			for (int j = 0; j < m_iHandMJCount[i]; ++j)
			{
				if (MASK_MAHJONGKIND(m_bHandMJ[i][j]) == MahJong_Wan)bHasWan = true;
				else if (MASK_MAHJONGKIND(m_bHandMJ[i][j]) == MahJong_Tiao)bHasTiao = true;
				else if (MASK_MAHJONGKIND(m_bHandMJ[i][j]) == MahJong_Bing)bHasBing = true;
			}
			for (int j = 0; j < m_iHUMJCount[i]; ++j)
			{
				if (MASK_MAHJONGKIND(m_bHUMJ[i][j].first) == MahJong_Wan)bHasWan = true;
				else if (MASK_MAHJONGKIND(m_bHUMJ[i][j].first) == MahJong_Tiao)bHasTiao = true;
				else if (MASK_MAHJONGKIND(m_bHUMJ[i][j].first) == MahJong_Bing)bHasBing = true;
			}
			if (bHasWan) iHuaSeCount++;
			if (bHasTiao)iHuaSeCount++;
			if (bHasBing)iHuaSeCount++;

			//花猪
			if (iHuaSeCount >= 3)
			{
				enUserStatu[i] = PlayerStatu_HuaZhu;
				continue;
			}
			//大叫
			//int iNoramlFan = 0; int iExtraFan = 0;
			int FanStyle[MAX_FAN_STYLE];
			memset(FanStyle, 0, sizeof(int)*MAX_FAN_STYLE);
			for (int j = 0; j < MAX_ORG_MJ/4; ++j)
			{
				bool huSuccess = this->isHu(i, m_bHUMJ[i], m_iHUMJCount[i], m_bHandMJ[i], m_iHandMJCount[i], bOriginalMJ[j]);
				if (huSuccess)
				{
					enUserStatu[i] = PlayerStatu_TingPai;
					FanCount(i, bOriginalMJ[j], FanStyle, HuType_PingHu, iNoramlFan[i], iExtraFan[i]);
					if (iMaxPayCount[i] < iNoramlFan[i] + iExtraFan[i])
					{
						iMaxPayCount[i] = iNoramlFan[i] + iExtraFan[i];
					}
					iNoramlFan[i] = 0;iExtraFan[i] = 0;
				}
			}
			if (enUserStatu[i] != PlayerStatu_TingPai)
			{
				enUserStatu[i] = PlayerStatu_WeiTing;
			}
		}
	}

	//先把胡牌人的分算好
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		//////////////////////////////////////////////////////////////////////////
		//两大实时胡，杠更新运用
		cmdsGameOver.iFanScore[i] = m_AllHuSorce[i];
		cmdsGameOver.iGangScore[i] = m_AllGangSorce[i];
		cmdsGameOver.iTotalScore[i] += m_AllTotal[i];
		//////////////////////////////////////////////////////////////////////////
		if (m_HuInfo[i].iHuInfoType == HuType_PingHu || m_HuInfo[i].iHuInfoType == HuType_QiangGang)
		{
			//胡的人所有信息
			int iPHuUser = m_HuInfo[i].iHuUser;
			cmdsGameOver.iDianPaoUser[iPHuUser] = m_HuInfo[i].iDianPaoUser;
			cmdsGameOver.iNomalFan[iPHuUser] = m_HuInfo[i].iHuFan;
			cmdsGameOver.iExtraFan[iPHuUser] = m_HuInfo[i].iExtraFan;
			cmdsGameOver.enhuType[iPHuUser] = m_HuInfo[i].iHuInfoType;
			cmdsGameOver.enUSEndType[iPHuUser] = UserEndType_PingHu;
			for (int j = 0; j < MAX_FAN_STYLE; ++j)
			{
				if (m_HuInfo[i].iFanStyle[j] > 0)
				{
					cmdsGameOver.iFanStyle[iPHuUser] = j;
					break;
				}
			}
		}
		else if (m_HuInfo[i].iHuInfoType == HuType_ZiMo)
		{
			cmdsGameOver.enhuType[i] = m_HuInfo[i].iHuInfoType;
			cmdsGameOver.iNomalFan[i] = m_HuInfo[i].iHuFan;
			cmdsGameOver.iExtraFan[i] = m_HuInfo[i].iExtraFan;
			cmdsGameOver.enUSEndType[i] = UserEndType_ZiMo;
			for (int j = 0; j < MAX_FAN_STYLE; ++j)
			{
				if (m_HuInfo[i].iFanStyle[j] > 0)
				{
					cmdsGameOver.iFanStyle[i] = j;
					break;
				}
			}
		}
	}

	//在把花猪大叫强退加上
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		cmdsGameOver.iFanScore[i] += m_QuitSorce[i];
		if (enUserStatu[i] == PlayerStatu_Escape)
		{
			cmdsGameOver.enhuType[i] = HuType_QiangTui;
			cmdsGameOver.iTotalScore[i] = m_AllTotal[i];
			//返回刮风下雨得分
			/*for (int j = 0; j < GuaFengMaxCount; ++j)
			{
				if (m_GuaFengInfo[j].eGangType == Operation_Gang_Type_Null)
				{
					break;
				}
				if (m_GuaFengInfo[j].eGangPlayer == i)
				{
					int tmpYinGanger = INVALID_CHAIR;
					int iGangBackCount = 0;
					for (int z = 0; z < GAME_PLAYER; ++z)
					{
						tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
						if ((tmpYinGanger != -1) && (enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu))
						{
							iGangBackCount++;
						}
					}
					if (m_GuaFengInfo[j].eGangSorce[i] >= m_UserMoney[i])
					{
						m_GuaFengInfo[j].eGangSorce[i] = m_UserMoney[i];
					}
					cmdsGameOver.iGangScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
					cmdsGameOver.iTotalScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
					m_UserMoney[i] -= m_GuaFengInfo[j].eGangSorce[i];
					ASSERT(m_UserMoney[i] >= 0);
					for (int z = 0; z < GAME_PLAYER; ++z)
					{
						tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
						if ((tmpYinGanger != -1) && (enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu))
						{
							m_UserMoney[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
							cmdsGameOver.iGangScore[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
							cmdsGameOver.iTotalScore[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
						}
					}
				}
			}*/
			cmdsGameOver.enUSEndType[i] = UserEndType_Escape;
		}
		else if (enUserStatu[i] == PlayerStatu_HuaZhu)
		{
			int iWuHuCount = 0;int64 iAllHuaZhu = 0;
			for (int j = 0; j < GAME_PLAYER; ++j)
			{
				if (enUserStatu[j] == PlayerStatu_TingPai || enUserStatu[j] == PlayerStatu_WeiTing)
				{
					iWuHuCount++;
				}
			}
			if (-16*m_iBisicCore*iWuHuCount + m_UserMoney[i] <= 0)
			{
				iAllHuaZhu = -m_UserMoney[i];
				cmdsGameOver.iTotalScore[i] += iAllHuaZhu;
				cmdsGameOver.iFanScore[i] += iAllHuaZhu;
				m_UserMoney[i] = 0;
			}
			else
			{
				iAllHuaZhu = -16*m_iBisicCore*iWuHuCount;
				cmdsGameOver.iTotalScore[i] += iAllHuaZhu;
				cmdsGameOver.iFanScore[i] += iAllHuaZhu;
				m_UserMoney[i] += -16*m_iBisicCore*iWuHuCount;
			}
			assert(iAllHuaZhu <= 0);
			for (int j = 0; j < GAME_PLAYER; ++j)
			{
				if (enUserStatu[j] == PlayerStatu_TingPai || enUserStatu[j] == PlayerStatu_WeiTing)
				{
					//花猪扣除玩家16倍的积分给未胡玩家
					cmdsGameOver.iTotalScore[j] += (-1)*iAllHuaZhu/iWuHuCount;
					cmdsGameOver.iFanScore[j] += (-1)*iAllHuaZhu/iWuHuCount;
					m_UserMoney[j] += (-1)*iAllHuaZhu/iWuHuCount;
				}
			}
			//返回刮风下雨得分
			for (int j = 0; j < GuaFengMaxCount; ++j)
			{
				if (m_GuaFengInfo[j].eGangType == Operation_Gang_Type_Null)
				{
					break;
				}
				if (m_GuaFengInfo[j].eGangPlayer == i)
				{
					int tmpYinGanger = INVALID_CHAIR;
					int iGangBackCount = 0;
					for (int z = 0; z < GAME_PLAYER; ++z)
					{
						tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
						if ((tmpYinGanger != -1) && (enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu))
						{
							iGangBackCount++;
						}
					}
					if (m_GuaFengInfo[j].eGangSorce[i] >= m_UserMoney[i])
					{
						m_GuaFengInfo[j].eGangSorce[i] = m_UserMoney[i];
					}
					cmdsGameOver.iGangScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
					cmdsGameOver.iTotalScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
					m_UserMoney[i] -= m_GuaFengInfo[j].eGangSorce[i];
					assert(m_UserMoney[i] >= 0);

					for (int z = 0; z < GAME_PLAYER; ++z)
					{
						tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
						if ((tmpYinGanger != -1) && (enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu))
						{
							cmdsGameOver.iGangScore[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
							cmdsGameOver.iTotalScore[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
							m_UserMoney[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
						}
					}
				}
			}
			cmdsGameOver.enUSEndType[i] = UserEndType_HuaZhu;
		}
		else if (enUserStatu[i] == PlayerStatu_TingPai)
		{
			//未听玩家赔给最大可能番数给听玩家
			cmdsGameOver.enUSEndType[i] = UserEndType_Ting;
			int iWuTingCount = 0;
			for (int j = 0; j < GAME_PLAYER; ++j)
			{
				if (enUserStatu[j] == PlayerStatu_WeiTing)
				{
					cmdsGameOver.enUSEndType[j] = UserEndType_NotTing;

					cmdsGameOver.iNomalFan[i] = iMaxPayCount[i];
					int64 iMaxPaySorce = int64(pow(2.0f, (int32)(iMaxPayCount[i]-1)))*m_iBisicCore;
					if (-iMaxPaySorce + m_UserMoney[j] <= 0)
					{
						iMaxPaySorce = m_UserMoney[j];
						m_UserMoney[j] = 0;
					}
					cmdsGameOver.iFanScore[i] += iMaxPaySorce;
					cmdsGameOver.iFanScore[j] -= iMaxPaySorce;

					cmdsGameOver.iTotalScore[i] += iMaxPaySorce;
					cmdsGameOver.iTotalScore[j] -= iMaxPaySorce;
					m_UserMoney[i] += iMaxPaySorce;
				}
			}
		}
		else if (enUserStatu[i] == PlayerStatu_WeiTing)
		{
			//返回刮风下雨得分
			for (int j = 0; j < GuaFengMaxCount; ++j)
			{
				if (m_GuaFengInfo[j].eGangType == Operation_Gang_Type_Null)
				{
					break;
				}
				if (m_GuaFengInfo[j].eGangPlayer == i)
				{
					int tmpYinGanger = INVALID_CHAIR;
					int iGangBackCount = 0;
					for (int z = 0; z < GAME_PLAYER; ++z)
					{
						tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
						if ((tmpYinGanger != -1) && (enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu))
						{
							iGangBackCount++;
						}
					}
					if (m_GuaFengInfo[j].eGangSorce[i] >= m_UserMoney[i])
					{
						m_GuaFengInfo[j].eGangSorce[i] = m_UserMoney[i];
					}
					cmdsGameOver.iGangScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
					cmdsGameOver.iTotalScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
					m_UserMoney[i] -= m_GuaFengInfo[j].eGangSorce[i];
					assert(m_UserMoney[i] >= 0);
					for (int z = 0; z < GAME_PLAYER; ++z)
					{
						tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
						if ((tmpYinGanger != -1) && (enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu))
						{
							cmdsGameOver.iGangScore[tmpYinGanger] +=  m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
							cmdsGameOver.iTotalScore[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
							m_UserMoney[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
						}
					}
				}
			}
			cmdsGameOver.enUSEndType[i] = UserEndType_NotTing;
		}
	}

	return;
}

void CServerLogicFrame::FanCount(int &iChairID, MJBYTE bMJ, int FanStyle[], enHuType enhuType, int64 &iHuPaiFan, int64 &iExtraFan)
{
	MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount = 0;
	MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount = 0;
	memset(btmpHuMJ, 0, sizeof(MJBlock)*MAX_BLOCK_COUNT);
	memset(btmpHandMJ, 0, sizeof(MJBYTE)*MAX_HAND_MJ);
	memcpy(btmpHuMJ, m_bHUMJ[iChairID], sizeof(MJBlock)*MAX_BLOCK_COUNT);
	itmpHuCount = m_iHUMJCount[iChairID];
	memcpy(btmpHandMJ, m_bHandMJ[iChairID], sizeof(MJBYTE)*MAX_HAND_MJ);
	itmpHandCount = m_iHandMJCount[iChairID];

	int tmpKaiMenCount = 0;
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_iHUMJCount[i] > 0)
		{
			tmpKaiMenCount++;
		}
	}

	int iGengCount = 0;
	if (enhuType == HuType_ZiMo)
	{
		MJBYTE bGenMJ[MAX_ARRAY_MJ] = {0};
		for (int i = 0; i < itmpHandCount; ++i)
		{
			bGenMJ[MASK_MAHJONGKIND(btmpHandMJ[i])*10 + MASK_MAHJONGNUM(btmpHandMJ[i])]++;
		}
		for (int i = 0; i < itmpHuCount; ++i)
		{
			if (btmpHuMJ[i].style == BlockStyle_Peng)
			{
				bGenMJ[MASK_MAHJONGKIND(btmpHuMJ[i].first)*10 + MASK_MAHJONGNUM(btmpHuMJ[i].first)]++;
				bGenMJ[MASK_MAHJONGKIND(btmpHuMJ[i].first)*10 + MASK_MAHJONGNUM(btmpHuMJ[i].first)]++;
				bGenMJ[MASK_MAHJONGKIND(btmpHuMJ[i].first)*10 + MASK_MAHJONGNUM(btmpHuMJ[i].first)]++;
			}
		}
		for(int i=0; i<MAX_ARRAY_MJ; ++i)
		{
			if (bGenMJ[i] >= 4)
			{
				iExtraFan++;
				iGengCount++;
				FanStyle[FanStyle_Gen]++;
			}
		}
		//天地胡
		if (m_UserTouchFirst[iChairID] == 1 && iChairID == m_iBankerUser)
		{
			FanStyle[FanStyle_TianHu] = 1;
			iHuPaiFan = 6;
			return;
		}
		else if (tmpKaiMenCount == 0 && m_UserTouchFirst[iChairID] == 1)
		{
			FanStyle[FanStyle_DiHu] = 1;
			iHuPaiFan = 6;
			return;
		}
		if (m_bGangKaiMJ[iChairID] != 0)
		{
			m_bGangKaiMJ[iChairID] = 0;
			FanStyle[FanStyle_GangShangHua] = 1;
			iExtraFan++;
		}
	}
	else
	{
		if (m_isLiuLieUser[iChairID])
		{
			m_isLiuLieUser[iChairID] = 0;
			FanStyle[FanStyle_GangShangPao] = 1;
			iExtraFan++;
		}
		btmpHandMJ[itmpHandCount] = bMJ;
		itmpHandCount++;

		MJBYTE bGenMJ[MAX_ARRAY_MJ] = {0};
		for (int i = 0; i < itmpHandCount; ++i)
		{
			bGenMJ[MASK_MAHJONGKIND(btmpHandMJ[i])*10 + MASK_MAHJONGNUM(btmpHandMJ[i])]++;
		}
		for (int i = 0; i < itmpHuCount; ++i)
		{
			if (btmpHuMJ[i].style == BlockStyle_Peng)
			{
				bGenMJ[MASK_MAHJONGKIND(btmpHuMJ[i].first)*10 + MASK_MAHJONGNUM(btmpHuMJ[i].first)]++;
				bGenMJ[MASK_MAHJONGKIND(btmpHuMJ[i].first)*10 + MASK_MAHJONGNUM(btmpHuMJ[i].first)]++;
				bGenMJ[MASK_MAHJONGKIND(btmpHuMJ[i].first)*10 + MASK_MAHJONGNUM(btmpHuMJ[i].first)]++;
			}
		}
		for(int i=0; i<MAX_ARRAY_MJ; ++i)
		{
			if (bGenMJ[i] >= 4)
			{
				iExtraFan++;
				iGengCount++;
				FanStyle[FanStyle_Gen]++;
			}
		}
	}

	m_pGameLogic->SortHandMJ(btmpHandMJ, itmpHandCount);

	for (int i = 0; i < m_iHUMJCount[iChairID]; ++i)
	{
		if (m_bHUMJ[iChairID][i].style >= BlockStyle_Gang_Ming)
		{
			FanStyle[FanStyle_Gang]++;
			iExtraFan++;
		}
	}
	//3色的判定
	int iHuaSeCount = 0;
	bool bHasWan = false;bool bHasTiao = false;bool bHasBing = false;
	for (int i=0; i<itmpHuCount; ++i)
	{
		if (MASK_MAHJONGKIND(btmpHuMJ[i].first) == MahJong_Wan)bHasWan = true;
		else if (MASK_MAHJONGKIND(btmpHuMJ[i].first) == MahJong_Tiao)bHasTiao = true;
		else if (MASK_MAHJONGKIND(btmpHuMJ[i].first) == MahJong_Bing)bHasBing = true;
	}
	for (int i=0; i<itmpHandCount; ++i)
	{
		if (MASK_MAHJONGKIND(btmpHandMJ[i]) == MahJong_Wan)bHasWan = true;
		else if (MASK_MAHJONGKIND(btmpHandMJ[i]) == MahJong_Tiao)bHasTiao = true;
		else if (MASK_MAHJONGKIND(btmpHandMJ[i]) == MahJong_Bing)bHasBing = true;
	}
	if (bHasWan) iHuaSeCount++;
	if (bHasTiao)iHuaSeCount++;
	if (bHasBing)iHuaSeCount++;


	int iDuiCount = 0;
	for (int i = 0; i < itmpHandCount;)
	{
		if (btmpHandMJ[i] == btmpHandMJ[i+1])
		{
			i += 2;
			iDuiCount++;
		}
		else
		{
			i++;
		}
	}
	//七对
	if (iDuiCount == 7)
	{
		bool HasQingQiDui = false; bool HasLongQiDui = false;
		FanStyle[FanStyle_QiDui] = 1;
		iHuPaiFan = 3;
		//清七对
		if (iHuaSeCount == 1)
		{
			HasQingQiDui = true;
			FanStyle[FanStyle_QingQiDui] = 1;
			FanStyle[FanStyle_QiDui] = 0;
			iHuPaiFan = 5;
		}
		//龙七对
		if (iGengCount >= 1)
		{
			HasLongQiDui = true;
			FanStyle[FanStyle_LongQiDui] = 1;
			FanStyle[FanStyle_QiDui] = 0;
			FanStyle[FanStyle_Gen] = iGengCount - 1;
			iExtraFan--;
			iHuPaiFan = 5;
		}
		if (HasLongQiDui && HasQingQiDui)
		{
			FanStyle[FanStyle_QingLongQiDui] = 1;
			FanStyle[FanStyle_QingQiDui] = 0;
			FanStyle[FanStyle_LongQiDui] = 0;
			iHuPaiFan = 6;
		}
		return;
	}

	//-----------------------------------------------2)
	MJBlock mjData[10][MAX_BLOCK_COUNT];
	int iHuZhuHeCount = 0;
	memset(mjData, 0, sizeof(MJBlock)*10*MAX_BLOCK_COUNT);
	MJBYTE bTmpHandMJ[MAX_HAND_MJ];
	memset(bTmpHandMJ, 0, sizeof(MJBYTE)*MAX_HAND_MJ);
	memcpy(bTmpHandMJ, btmpHandMJ, sizeof(MJBYTE)*MAX_HAND_MJ);
	int bTmpHandCount = itmpHandCount;
	m_pGameLogic->GetMahJongAlg()->GetHuCombination(bTmpHandMJ, bTmpHandCount, mjData, iHuZhuHeCount);
	//-----------------------------------------------2)
	bool HasHandYaoJiu = false;bool HasHuYaoJiu = false;bool HasShun = false;
	bool HasHandKe = false;bool HasHuJiangKe = false;
	int iCmpHandCount = 0;int iCmpKeHandCount = 0;//前面用于幺九 后面用于对对
	for (int i=0; i<iHuZhuHeCount; ++i)
	{
		iCmpHandCount = bTmpHandCount;iCmpKeHandCount = bTmpHandCount;HasShun = true;
		for (int j=0; mjData[i][j].style!=BlockStyle_Null; j++)
		{
			if (j < 5)
			{
				if (mjData[i][j].style== BlockStyle_Ke)
				{
					iCmpHandCount -= 3;iCmpKeHandCount -= 3;
					if (MASK_MAHJONGNUM(mjData[i][j].first) == 0x01 || MASK_MAHJONGNUM(mjData[i][j].first) == 0x09)
					{
					}
					else
					{
						if (HasShun)
						{
							HasShun = false;
						}
					}

					if (iCmpHandCount == 0 && HasShun)
					{
						HasHandYaoJiu = true;
					}
					if (iCmpKeHandCount == 0)
					{
						HasHandKe = true;
					}
					continue;
				}
				else if(mjData[i][j].style == BlockStyle_Jiang)
				{
					iCmpHandCount -= 2;iCmpKeHandCount -= 2;
					if (MASK_MAHJONGNUM(mjData[i][j].first) == 0x01 || MASK_MAHJONGNUM(mjData[i][j].first) == 0x09)
					{
					}
					else
					{
						if (HasShun)
						{
							HasShun = false;
						}
					}
					if (iCmpHandCount == 0 && HasShun)
					{
						HasHandYaoJiu = true;
					}
					if (iCmpKeHandCount == 0)
					{
						HasHandKe = true;
					}
					continue;
				}
				else if (mjData[i][j].style == BlockStyle_Shun)
				{
					if (MASK_MAHJONGNUM(mjData[i][j].first) == 0x01 || MASK_MAHJONGNUM(mjData[i][j].first) == 0x07)
					{
					}
					else
					{
						if (HasShun)
						{
							HasShun = false;
						}
					}
					iCmpHandCount -= 3;
					if (iCmpHandCount == 0 && HasShun)
					{
						HasHandYaoJiu = true;
					}
					continue;
				}
			}
		}
	}
	int iCmpHuYaoJiuCount = 0;int iCmpHuJiangDui = 0;
	for (int i = 0; i < itmpHuCount; ++i)
	{
		if (MASK_MAHJONGNUM(btmpHuMJ[i].first) == 0x01 || MASK_MAHJONGNUM(btmpHuMJ[i].first) == 0x09)
		{
			iCmpHuYaoJiuCount++;
		}
		else if (MASK_MAHJONGNUM(btmpHuMJ[i].first) == 0x02 || MASK_MAHJONGNUM(btmpHuMJ[i].first) == 0x05 || MASK_MAHJONGNUM(btmpHuMJ[i].first) == 0x08)
		{
			iCmpHuJiangDui++;
		}
	}
	if (iCmpHuYaoJiuCount == itmpHuCount)
	{
		HasHuYaoJiu = true;
	}
	else if (iCmpHuJiangDui == itmpHuCount)
	{
		HasHuJiangKe = true;
	}
	//带幺九
	if (HasHandYaoJiu && HasHuYaoJiu)
	{
		FanStyle[FanStyle_DaiYaoJiu] = 1;
		iHuPaiFan = 3;
		if (iHuaSeCount == 1)
		{
			//清幺九
			FanStyle[FanStyle_QingJiuYao] = 1;
			FanStyle[FanStyle_DaiYaoJiu] = 0;
			iHuPaiFan = 5;
		}
		return;
	}
	//对对胡
	else if (HasHandKe)
	{
		FanStyle[FanStyle_DuiDuiHu] = 1;
		iHuPaiFan = 2;
		//清对
		if (iHuaSeCount == 1)
		{
			FanStyle[FanStyle_QingDui] = 1;
			FanStyle[FanStyle_DuiDuiHu] = 0;
			iHuPaiFan = 4;
		}
		//将对
		else if (HasHuJiangKe)
		{
			bool HasJiangDui = true;
			for(int i = 0; i < itmpHandCount; ++i)
			{
				if (MASK_MAHJONGNUM(btmpHandMJ[i]) == 0x02 || MASK_MAHJONGNUM(btmpHandMJ[i]) == 0x05 || MASK_MAHJONGNUM(btmpHandMJ[i]) == 0x08)
				{
					continue;
				}
				else
				{
					HasJiangDui = false;
				}
			}
			if (HasJiangDui)
			{
				FanStyle[FanStyle_JiangDui] = 1;
				FanStyle[FanStyle_DuiDuiHu] = 0;
				iHuPaiFan = 4;
			}
		}
		return;
	}
	else if (iHuaSeCount == 1)
	{
		FanStyle[FanStyle_QingYiSe] = 1;
		iHuPaiFan = 3;
		return;
	}
	else
	{
		FanStyle[FanStyle_PingHu] = 1;
		iHuPaiFan = 1;
		return;
	}
	return;
}

void CServerLogicFrame::OperHu(int playerId, int iDianPaoPlayer, enHuType enhuType)
{
	MJBYTE bLastMJ = INVALID_BYTE;

	RoomState rs = m_g_GameRoom->GetRoomState();
	if (rs!=  ROOMSTATE_GAMING)return;

	if (playerId == m_iOperateUer)
	{
		m_g_GameRoom->StopTimer(TIMER_Nomal);
		m_g_GameRoom->StopTimer(TIMER_BUFFER);
	}
	m_g_GameRoom->StopTimer(TIMER(TIMER_DHU,playerId));
	m_g_GameRoom->StopTimer(TIMER(TIMER_DHUNomal, playerId));

	CMD_S_HU cmdsHu;
	memset(&cmdsHu, 0, sizeof(CMD_S_HU));

	cmdsHu.iHuUser = playerId;
	cmdsHu.enhuType = enhuType;
	//自摸
	if (enhuType == HuType_ZiMo)
	{
		m_OperPriority[playerId] = 0;
		MJBYTE tmpTouchMJ = m_bHandMJ[playerId][m_iHandMJCount[playerId]-1];
		bLastMJ = tmpTouchMJ;
		cmdsHu.iDianPaoUser = -1;
		enUserStatu[playerId] = PlayerStatu_ZiMo;

		//保存胡玩家信息，留用OperGameOver使用
		m_HuInfo[playerId].iHuUser = playerId;
		m_HuInfo[playerId].iDianPaoUser = -1;
		m_HuInfo[playerId].iHuInfoType = HuType_ZiMo;
	}
	else if (enhuType == HuType_QiangGang)
	{
		bLastMJ = m_bQiangGangMJ[playerId];
		m_OperPriority[playerId] = 0;
		if (m_isAlreadyHu == 0)
		{
			m_HuPos++;
			m_huRanderUser[playerId] = m_HuPos;
			m_huMJ = bLastMJ;
		}
		else
		{
			m_huRanderUser[playerId] = m_HuPos;
		}
		m_bQiangGangMJ[playerId] = 0;
		cmdsHu.iDianPaoUser = iDianPaoPlayer;
		enUserStatu[playerId] = PlayerStatu_PingHu;
		cmdsHu.iAlreadyHu = m_isAlreadyHu;
		m_isAlreadyHu++;
		bLastMJ = m_huMJ;
		//保存胡玩家信息，留用OperGameOver使用
		m_HuInfo[playerId].bHuMJ = bLastMJ;
		m_HuInfo[playerId].iHuUser = playerId;
		m_HuInfo[playerId].iDianPaoUser = iDianPaoPlayer;
		m_HuInfo[playerId].iHuInfoType = HuType_QiangGang;
		//吧自摸明杠变成碰
		for (int i = 0; i < m_iHandMJCount[iDianPaoPlayer]; ++i)
		{
			if (m_bHUMJ[iDianPaoPlayer][i].first == bLastMJ && m_bHUMJ[iDianPaoPlayer][i].style == BlockStyle_Gang_ZiMoMing)
			{
				m_bHUMJ[iDianPaoPlayer][i].style = BlockStyle_Peng;
			}
		}
	}
	else if(enhuType == HuType_PingHu)
	{
		m_OperPriority[playerId] = 0;
		bLastMJ = m_bOutMJ[m_iOldOperateUer][m_iOutMJCount[m_iOldOperateUer]-1];
		//m_iHandMJCount++;
		//m_bHandMJ[playerId][m_iHandMJCount[playerId]] = bLastMJ;
		if (m_isAlreadyHu == 0)
		{
			m_HuPos++;
			m_huRanderUser[playerId] = m_HuPos;
			m_iOutMJCount[m_iOldOperateUer]--;
			m_huMJ = bLastMJ;
		}
		else
		{
			m_huRanderUser[playerId] = m_HuPos;
		}
		cmdsHu.iDianPaoUser = iDianPaoPlayer;
		enUserStatu[playerId] = PlayerStatu_PingHu;
		cmdsHu.iAlreadyHu = m_isAlreadyHu;
		m_isAlreadyHu++;
		bLastMJ = m_huMJ;
		//保存胡玩家信息，留用OperGameOver使用
		m_HuInfo[playerId].bHuMJ = bLastMJ;//断线重连使用
		m_HuInfo[playerId].iHuUser = playerId;
		m_HuInfo[playerId].iDianPaoUser = iDianPaoPlayer;
		m_HuInfo[playerId].iHuInfoType = HuType_PingHu;
	}
	else if (enhuType == HuType_QiangTui)
	{
		if (enUserStatu[playerId] >= PlayerStatu_ZiMo)
		{
			//保存胡玩家信息，留用OperGameOver使用
			if (enUserStatu[playerId] == PlayerStatu_ZiMo)
			{
				m_HuInfo[playerId].iHuInfoType = HuType_ZiMo;
			}
			else if (enUserStatu[playerId] == PlayerStatu_PingHu)
			{
				m_HuInfo[playerId].iHuInfoType = HuType_PingHu;
			}
			//胡的玩家强退不返回刮风下雨得分
			enUserStatu[playerId] = PlayerStatu_GameOver;
			enhuType = HuType_GameOver;
			//重新设置胡的类型，为安全退出类型
			cmdsHu.enhuType = enhuType;
		}
		else
		{
			enUserStatu[playerId] = PlayerStatu_Escape;
			//保存胡玩家信息，留用OperGameOver使用
			m_HuInfo[playerId].iHuUser = playerId;
			m_HuInfo[playerId].iHuInfoType = HuType_QiangTui;
			if (m_StartQuit)
			{
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (enUserStatu[i] == PlayerStatu_Escape)
					{
						m_StartQuit = false;

						int64 MaxQiangTuiSorce = -3*16*m_iBisicCore;
						//对于不够输分玩家之间强制退出
						if (m_UserMoney[playerId] + MaxQiangTuiSorce <= 0)
						{
							assert(m_UserMoney[playerId] >=0);
							MaxQiangTuiSorce = (-1)*m_UserMoney[playerId];
						}

						m_AllTotal[playerId] += MaxQiangTuiSorce;
						m_QuitSorce[playerId] += MaxQiangTuiSorce;
						m_UserMoney[playerId] += MaxQiangTuiSorce;

						if (m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
						{
							m_g_GameRoom->WriteUserScore(playerId, m_AllTotal[playerId], 0, enScoreKind_Lost);
						}
						//Player *pPlayer = m_g_GameRoom->GetPlayer(i);
						//if(pPlayer != NULL)
						//{
						//	int pPlayerMoney = pPlayer->GetMoney();
						//	m_AllTotal[i] = (-3)*16*m_iBisicCore;
						//	if (m_AllTotal[i] + pPlayerMoney <= 0)
						//	{
						//		m_AllTotal[i] = (-1)*pPlayerMoney;
						//	}
						//	m_QuitSorce[i] = m_AllTotal[i];
						//}
						for (int j = 0; j < GAME_PLAYER; ++j)
						{
							if (j != i)
							{
								m_AllTotal[j] = MaxQiangTuiSorce/(-3);
								m_QuitSorce[j] += MaxQiangTuiSorce/(-3);
								m_UserMoney[j] += MaxQiangTuiSorce/(-3);
							}
						}
						OperGameOver(EndGameType_QiangTui);
						return;
					}
				}
			}
			//itmpNum为强退是有多少人还在比赛
			int iTmpNum = 0;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (enUserStatu[i] < PlayerStatu_Escape)
				{
					iTmpNum--;
				}
			}
			int64 MaxQiangTuiSorce = iTmpNum*16*m_iBisicCore;
			//对于不够输分玩家之间强制退出
			if (m_UserMoney[playerId] + MaxQiangTuiSorce <= 0)
			{
				assert(m_UserMoney[playerId] >=0);
				MaxQiangTuiSorce = (-1)*m_UserMoney[playerId];
			}

			m_AllTotal[playerId] += MaxQiangTuiSorce;
			m_QuitSorce[playerId] += MaxQiangTuiSorce;
			m_UserMoney[playerId] += MaxQiangTuiSorce;
			if (m_AllTotal[playerId] > 0)
			{
				double iTmpShui = m_AllTotal[playerId]*m_g_GameRoom->GetChouShui();
				int iChouShui = 0;
				if (iTmpShui >= 1)
				{
					iChouShui = (int)ceil(iTmpShui);
				}
				else
				{
					iChouShui = int(iTmpShui);
				}
				m_AllTotal[playerId] -= iChouShui;
				if (m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
				{
					m_g_GameRoom->WriteUserScore(playerId, m_AllTotal[playerId], iChouShui, enScoreKind_Win);
				}
			}
			else
			{
				if (m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
				{
					m_g_GameRoom->WriteUserScore(playerId, m_AllTotal[playerId], 0, enScoreKind_Lost);
				}
			}
			//给在场玩家平分得分
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (i != playerId && enUserStatu[i] < PlayerStatu_Escape)
				{
					m_AllTotal[i] += MaxQiangTuiSorce/iTmpNum;
					m_QuitSorce[i] += MaxQiangTuiSorce/iTmpNum;
					m_UserMoney[i] += MaxQiangTuiSorce/iTmpNum;
				}
			}
		}
	}
	else if (enhuType == HuType_ForceQuit)
	{
		enUserStatu[playerId] = PlayerStatu_ForceQuit;
	}
	if (m_huMJ == 0)
	{
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			if (m_bQiangGangMJ[playerId] != 0)
			{
				cmdsHu.bUserHuMJ = m_bQiangGangMJ[playerId];
				break;
			}
		}
		if (cmdsHu.bUserHuMJ == 0)
		{
			cmdsHu.bUserHuMJ = m_bOutMJ[m_iOldCurrentUer][m_iOutMJCount[m_iOldCurrentUer]-1];
		}
	}
	else
	{
		cmdsHu.bUserHuMJ = bLastMJ;
	}

	if (m_zuozhuang == -1 && (enhuType == HuType_ZiMo|| enhuType == HuType_QiangGang || EndGameType_Hu))
	{
		m_zuozhuang = playerId;
	}
	/*if (m_twice-- >= 0)
	{
		m_zuozhuang = playerId;
		if (m_twice == 0 && cmdsHu.iAlreadyHu == 1)
		{
			m_zuozhuang = iDianPaoPlayer;
		}
	}*/

	//强退不需要番数 和 分数 等opergameover时一起结算
	if (enhuType == HuType_ZiMo || enhuType == HuType_PingHu || enhuType == HuType_QiangGang)
	{
		FanCount(playerId, bLastMJ, cmdsHu.iFanStyle, enhuType, cmdsHu.iNomalFan, cmdsHu.iExtraFan);
		if (enhuType == HuType_QiangGang)
		{
			cmdsHu.iExtraFan++;
			cmdsHu.iFanStyle[FanStyle_QiangGang] = 1;
		}
		cmdsHu.iHuScore = int(pow(2.0f, (int32)(cmdsHu.iNomalFan + cmdsHu.iExtraFan - 1)))*m_iBisicCore;
		//保存胡玩家信息，留用OperGameOver使用
		m_HuInfo[playerId].iHuFan = cmdsHu.iNomalFan;
		m_HuInfo[playerId].iExtraFan = cmdsHu.iExtraFan;
		memcpy(m_HuInfo[playerId].iFanStyle, cmdsHu.iFanStyle, sizeof(int)*MAX_FAN_STYLE);

		cmdsHu.iDianPaoScore = m_AllHuSorce[playerId];
		//实时保存胡番得分,这样gameover就能直接赋值了
		if (enhuType == HuType_ZiMo)
		{
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (enUserStatu[i] < PlayerStatu_ZiMo)
				{
					int64 tmpHuSocre = m_AllHuSorce[i];
					//m_AllHuSorce[i] -= cmdsHu.iHuScore;
					//m_AllHuSorce[playerId] += cmdsHu.iHuScore;
					//对于不够输分玩家之间强制退出
					if (-cmdsHu.iHuScore + m_UserMoney[i] <= 0)
					{
						assert(m_UserMoney[i] >= 0);
						m_isForceQuit[i] = 1;

						m_AllHuSorce[i] += -m_UserMoney[i];
						m_AllHuSorce[playerId] += m_UserMoney[i];

						//实时更新总分
						m_AllTotal[playerId] += m_UserMoney[i];
						m_AllTotal[i] += -m_UserMoney[i];

						m_UserMoney[playerId] += m_UserMoney[i];
						m_UserMoney[i] = 0;
					}
					else
					{
						m_AllHuSorce[i] += -cmdsHu.iHuScore;
						m_AllHuSorce[playerId] += cmdsHu.iHuScore;

						//实时更新总分
						m_AllTotal[playerId] += cmdsHu.iHuScore;
						m_AllTotal[i] += -cmdsHu.iHuScore;

						m_UserMoney[playerId] += cmdsHu.iHuScore;
						m_UserMoney[i] -= cmdsHu.iHuScore;
					}
				}
			}
			cmdsHu.iAllScore = m_AllTotal[playerId];
		}
		else
		{
			int64 tmpHuSocre = m_AllHuSorce[iDianPaoPlayer];
			//m_AllHuSorce[iDianPaoPlayer] -= cmdsHu.iHuScore;
			//m_AllHuSorce[playerId] += cmdsHu.iHuScore;
			//对于不够输分玩家之间强制退出
			assert(m_UserMoney[iDianPaoPlayer] >= 0);
			if (-cmdsHu.iHuScore + m_UserMoney[iDianPaoPlayer] <= 0)
			{
				m_isForceQuit[iDianPaoPlayer] = 1;

				m_AllHuSorce[iDianPaoPlayer] += -m_UserMoney[iDianPaoPlayer];
				m_AllHuSorce[playerId] += m_UserMoney[iDianPaoPlayer];

				//实时更新总分
				m_AllTotal[playerId] += m_UserMoney[iDianPaoPlayer];
				m_AllTotal[iDianPaoPlayer] += -m_UserMoney[iDianPaoPlayer];

				m_UserMoney[playerId] += m_UserMoney[iDianPaoPlayer];
				m_UserMoney[iDianPaoPlayer] = 0;
			}
			else
			{
				m_AllHuSorce[iDianPaoPlayer] += -cmdsHu.iHuScore;
				m_AllHuSorce[playerId] += cmdsHu.iHuScore;

				//实时更新总分
				m_AllTotal[playerId] += cmdsHu.iHuScore;
				m_AllTotal[iDianPaoPlayer] -= cmdsHu.iHuScore;

				m_UserMoney[playerId] += cmdsHu.iHuScore;
				m_UserMoney[iDianPaoPlayer] -= cmdsHu.iHuScore;
			}
			cmdsHu.iAllScore = m_AllTotal[playerId];
		}

		//刮风下雨的分数
		cmdsHu.iGangScore = m_AllGangSorce[playerId];
	}

	//胡或逃跑 都要优先检测是不是满足游戏结束条件
	int iHuTuiCount = 0;
	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (enUserStatu[i] >= PlayerStatu_ZiMo)
		{
			iHuTuiCount++;
		}
	}
	if (iHuTuiCount >= 3)
	{
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
            Json::Value msgHu;
            msgHu["MsgId"] = IDD_MESSAGE_ROOM;
            msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
			CmdHelper::CodeMessage(cmdsHu, msgHu);
			m_g_GameRoom->SendTableMsg(i, msgHu);
		}
		OperGameOver(EndGameType_Hu);
		return;
	}



	if (enhuType == HuType_QiangTui)
	{
		//首先判断是不是强退玩家操作
		if (m_iOperateUer == playerId)
		{
			//判断是摸牌操作，还是别人打出一张牌强退玩家有操作
			if (m_iHandMJCount[playerId] % 3 == 2)
			{
				//首先给每个玩家发送强退消息
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					m_OperPriority[i] = 0;
					cmdsHu.isNowOper = 1;
					cmdsHu.bOperation = 0;
					cmdsHu.isOtherPriority = 0;

                    Json::Value msgHu;
                    msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                    msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
					CmdHelper::CodeMessage(cmdsHu, msgHu);
					m_g_GameRoom->SendTableMsg(i, msgHu);
				}
				//其次让强退的下个玩家摸牌
				int itmpNexter = 0;
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					itmpNexter = (i+playerId) % GAME_PLAYER;
					if (itmpNexter == playerId || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo || m_isForceQuit[itmpNexter] == 1)
					{
						continue;
					}
					m_iOperateUer = itmpNexter;
					if (m_iOperateUer == itmpNexter)
					{
						break;
					}
				}
				if (m_iHandMJCount[m_iOperateUer] % 3 != 2)
				{
					if (m_iTouchPos >= MAX_ORG_MJ)
					{
						//流局
						OperGameOver(EndGameType_LiuJu);
						return;
					}
					else
					{
						OperTouch(m_iOperateUer);
					}
				}
			}
			else if (m_OperPriority[playerId])
			{
				//首先看看是不是胡操作
				if (m_OperPriority[playerId] & Operation_Hu)
				{
					//首先判断完是什么操作之后，就要把强退玩家操作清空
					m_OperPriority[playerId] = 0;
					cmdsHu.isNowOper = 1;
					bool HasMal = false;
					//胡操作，那么检测还有没有人胡
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						//有人胡，isotherHu标志先赋值
						if (m_OperPriority[i] & Operation_Hu)
						{
							cmdsHu.isOtherHu = 1;
							break;
						}
					}

					//都放弃操作，那么检测其他人还有没有操作
					int iMalNextUser = 0;
					if (cmdsHu.isOtherHu == 0)
					{
						for (int i = 0; i < GAME_PLAYER; ++i)
						{
							iMalNextUser = (i + m_iOldOperateUer) % GAME_PLAYER;
							if (m_OperPriority[iMalNextUser])
							{
								m_iOperateUer = iMalNextUser;
								HasMal = true;
								break;
							}
						}
					}
					if (cmdsHu.isOtherHu)
					{
						for (int i = 0; i < GAME_PLAYER; ++i)
						{
							if (m_OperPriority[i] & Operation_Hu)
							{
								cmdsHu.bOperation = Operation_Hu;
								cmdsHu.isOtherPriority = 0;
							}
							else
							{
								cmdsHu.bOperation = 0;
								cmdsHu.isOtherPriority = 1;
							}

                            Json::Value msgHu;
                            msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                            msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
							CmdHelper::CodeMessage(cmdsHu, msgHu);
							m_g_GameRoom->SendTableMsg(i, msgHu);
						}
					}
					else if (m_HasHuTouch == -1)
					{
						for (int i = 0; i < GAME_PLAYER; ++i)
						{
							if (HasMal)
							{
								if (i == m_iOperateUer)
								{
									cmdsHu.bOperation = m_OperPriority[m_iOperateUer];
									cmdsHu.isOtherPriority = 0;

									if (cmdsHu.bOperation & Operation_Gang)
									{
										cmdsHu.bGangType = Operation_Gang_Type_Ming;
										cmdsHu.bMJGang[0] = cmdsHu.bUserHuMJ;
										cmdsHu.iGangCount = 1;
									}
									//开启一个定时器
									int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
									m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
								}
								else
								{
									cmdsHu.bOperation = 0;
									cmdsHu.isOtherPriority = 1;
								}
							}
							else
							{
								cmdsHu.bOperation = 0;
								cmdsHu.isOtherPriority = 0;
							}

                            Json::Value msgHu;
                            msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                            msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
							CmdHelper::CodeMessage(cmdsHu, msgHu);
							m_g_GameRoom->SendTableMsg(i, msgHu);
						}
					}

					//如果没有其他玩家胡，那么就让出牌下家摸牌
					if (cmdsHu.isOtherHu == 0 && !HasMal)
					{
						//清空所有操作
						memset(m_OperPriority, 0, sizeof(m_OperPriority));
						int itmpNexter = 0;
						for (int i = 0; i < GAME_PLAYER; ++i)
						{
							itmpNexter = (i+m_iOldCurrentUer) % GAME_PLAYER;
							if (itmpNexter == m_iOldCurrentUer || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo || m_isForceQuit[itmpNexter] == 1)
							{
								continue;
							}
							m_iOperateUer = itmpNexter;
							if (m_iOperateUer == itmpNexter)
							{
								break;
							}
						}
						if (m_iHandMJCount[m_iOperateUer] % 3 != 2)
						{
							if (m_iTouchPos >= MAX_ORG_MJ)
							{
								//流局
								OperGameOver(EndGameType_LiuJu);
								return;
							}
							else
							{
								OperTouch(m_iOperateUer);
							}
						}
					}
				}
				//其次看看是不是碰，杠操作
				else if (m_OperPriority[playerId])
				{
					//首先判断完是什么操作之后，就要把强退玩家操作清空
					m_OperPriority[playerId] = 0;
					cmdsHu.isNowOper = 1;
					//然后看看其他玩家有没有碰杠操作
					bool HasMal = false;
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						if (m_OperPriority[i])
						{
							cmdsHu.bUserHuMJ = m_bOutMJ[m_iOldOperateUer][m_iOutMJCount[m_iOldOperateUer]-1];
							HasMal = true;
							break;
						}
					}
					int itmpNexter = 0;
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						itmpNexter = (i+m_iOldCurrentUer) % GAME_PLAYER;
						if (itmpNexter == m_iOldCurrentUer || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo || m_isForceQuit[itmpNexter] == 1)
						{
							continue;
						}
						if (HasMal)
						{
							if (m_OperPriority[itmpNexter])
							{
								m_iOperateUer = itmpNexter;
								break;
							}
						}
						else
						{
							m_iOperateUer = itmpNexter;
							if (m_iOperateUer == itmpNexter)
							{
								break;
							}
						}

					}
					//发送消息
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						if (HasMal)
						{
							if (m_OperPriority[i])
							{
								cmdsHu.bOperation = m_OperPriority[i];
								cmdsHu.isOtherPriority = 0;
								if (cmdsHu.bOperation & Operation_Gang)
								{
									cmdsHu.bGangType = Operation_Gang_Type_Ming;
									cmdsHu.bMJGang[0] = cmdsHu.bUserHuMJ;
									cmdsHu.iGangCount = 1;
								}
							}
							else
							{
								cmdsHu.bOperation = 0;
								cmdsHu.isOtherPriority = 1;
							}
						}
						else
						{
							m_OperPriority[i] = 0;
							cmdsHu.bOperation = 0;
							cmdsHu.isOtherPriority = 0;
						}

                        Json::Value msgHu;
                        msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                        msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
						CmdHelper::CodeMessage(cmdsHu, msgHu);
						m_g_GameRoom->SendTableMsg(i, msgHu);
					}
					//如果有操作开一个定时器
					if (HasMal)
					{
						int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
						m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
					}
					//没有操作则出牌的下家摸牌
					else
					{
						if (m_iHandMJCount[m_iOperateUer] % 3 != 2)
						{
							if (m_iTouchPos >= MAX_ORG_MJ)
							{
								//流局
								OperGameOver(EndGameType_LiuJu);
								return;
							}
							else
							{
								OperTouch(m_iOperateUer);
							}
						}
					}
				}
			}
			//放弃之后没有操作了，强退人为放弃人
			else
			{
				//胡操作，那么检测还有没有人胡
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					//有人胡，isotherHu标志先赋值
					if (m_OperPriority[i] & Operation_Hu)
					{
						cmdsHu.isOtherHu = 1;
						break;
					}
				}
				//所有的操作都是在这个基础上，这个情况是一炮多响强退玩家点放弃后强退
				if (cmdsHu.isOtherHu == 1)
				{
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						if (m_OperPriority[i] & Operation_Hu)
						{
							cmdsHu.bOperation = m_OperPriority[i];
							cmdsHu.isOtherPriority = 0;
						}
						else
						{
							cmdsHu.bOperation = 0;
							cmdsHu.isOtherPriority = 1;
						}

                        Json::Value msgHu;
                        msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                        msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
						CmdHelper::CodeMessage(cmdsHu, msgHu);
						m_g_GameRoom->SendTableMsg(i, msgHu);
					}
				}
			}
		}
		//如果强退玩家不是当前玩家
		else
		{
			//首先看看有没有操作，这时候有操作，只可能是胡操作
			if (m_OperPriority[playerId])
			{
				//首先把isNowOper置为1
				cmdsHu.isNowOper = 1;
				m_OperPriority[playerId] = 0;
				//看看其他人有没有胡操作
				bool HasHu = false;bool HasMal = false;
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (m_OperPriority[i] & Operation_Hu)
					{
						cmdsHu.isOtherHu = 1;
						HasHu = true;
						break;
					}
					else if (m_OperPriority[i])
					{
						HasMal = true;
						//break;
					}
				}
				if (HasHu)
				{
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						if (m_OperPriority[i] & Operation_Hu)
						{
							cmdsHu.bOperation = Operation_Hu;
							cmdsHu.isOtherPriority = 0;

							int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
							m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
						}
						else
						{
							cmdsHu.bOperation = 0;
							if (cmdsHu.isOtherHu == 1)
							{
								cmdsHu.isOtherPriority = 1;
							}
							else
							{
								cmdsHu.isOtherPriority = 0;
							}
						}

                        Json::Value msgHu;
                        msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                        msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
						CmdHelper::CodeMessage(cmdsHu, msgHu);
						m_g_GameRoom->SendTableMsg(i, msgHu);
					}
				}
				else if (HasMal && m_HasHuTouch == -1)
				{
					int iNextOperUser = 0;
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						iNextOperUser = (i + m_iOldCurrentUer) % GAME_PLAYER;
						if (iNextOperUser == m_iOldCurrentUer || enUserStatu[iNextOperUser] >= PlayerStatu_ZiMo)
						{
							continue;
						}
						if (m_OperPriority[iNextOperUser])
						{
							m_iOperateUer = iNextOperUser;
							break;
						}
					}
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						if (i == iNextOperUser)
						{
							cmdsHu.bOperation = m_OperPriority[i];
							cmdsHu.isOtherPriority = 0;
							if (cmdsHu.bOperation & Operation_Gang)
							{
								cmdsHu.bGangType = Operation_Gang_Type_Ming;
								cmdsHu.bMJGang[0] = cmdsHu.bUserHuMJ;
								cmdsHu.iGangCount = 1;
							}
						}
						else
						{
							cmdsHu.bOperation = 0;
							cmdsHu.isOtherPriority = 1;
						}

                        Json::Value msgHu;
                        msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                        msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
						CmdHelper::CodeMessage(cmdsHu, msgHu);
						m_g_GameRoom->SendTableMsg(i, msgHu);
					}

					if (enUserStatu[playerId] == PlayerStatu_Trustee)
					{
						m_g_GameRoom->StartTimer(TIMER_BUFFER, 3);
					}
					else
					{
						int itime = (TIME_MaxOut+TIME_ServerDelay)/1000;
						m_g_GameRoom->StartTimer(TIMER_Nomal, itime);
					}
				}
				//如果没有人胡,也就是说，一炮说想的时候，强退玩家是最后一个胡的玩家,那么让下出牌的下一个玩家摸牌
				else
				{
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						cmdsHu.bOperation = 0;
						cmdsHu.isOtherPriority = 0;

                        Json::Value msgHu;
                        msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                        msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
						CmdHelper::CodeMessage(cmdsHu, msgHu);
						m_g_GameRoom->SendTableMsg(i, msgHu);
					}
					int itmpNexter = 0;
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						itmpNexter = (i+m_iOldCurrentUer) % GAME_PLAYER;
						if (itmpNexter == m_iOldCurrentUer || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo || m_isForceQuit[itmpNexter] == 1)
						{
							continue;
						}
						m_iOperateUer = itmpNexter;
						if (m_iOperateUer == itmpNexter)
						{
							break;
						}
					}
					if (m_iHandMJCount[m_iOperateUer] % 3 != 2)
					{
						if (m_iTouchPos >= MAX_ORG_MJ)
						{
							//流局
							OperGameOver(EndGameType_LiuJu);
							return;
						}
						else
						{
							OperTouch(m_iOperateUer);
						}
					}
				}
			}
			//如果强退玩家没有操作
			else if (playerId == m_iOldCurrentUer || playerId == m_BeiQiangUser)
			{
				//////注意这里有个比较纠结的问题，就是强退玩家未点炮玩家，那么就屏蔽掉所有玩家的胡操作，并让出牌玩家的下家摸牌
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (enUserStatu[i] >= PlayerStatu_ZiMo)
					{
						continue;
					}
					if (m_OperPriority[i] && (m_iHandMJCount[i] % 3 != 2) && playerId == m_iOldCurrentUer)
					{
						//强退玩家打出一张牌，其他玩家有操作，就关闭定时器
						m_g_GameRoom->StopTimer(TIMER_Nomal);
						m_g_GameRoom->StopTimer(TIMER_BUFFER);
						m_g_GameRoom->StopTimer(TIMER(TIMER_DHU,i));
						m_g_GameRoom->StopTimer(TIMER(TIMER_DHUNomal,i));
						m_OperPriority[i] = 0;
					}
					else if (m_OperPriority[i] && m_bQiangGangMJ[i] && playerId == m_BeiQiangUser)
					{
						//强退玩家打出一张牌，其他玩家有操作，就关闭定时器
						m_g_GameRoom->StopTimer(TIMER_Nomal);
						m_g_GameRoom->StopTimer(TIMER_BUFFER);
						m_g_GameRoom->StopTimer(TIMER(TIMER_DHU,i));
						m_g_GameRoom->StopTimer(TIMER(TIMER_DHUNomal,i));
						m_OperPriority[i] = 0;
					}
				}

				bool HasMal = false;
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (m_OperPriority[i])
					{
						HasMal = true;
						break;
					}
				}
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (HasMal)
					{
						if (m_OperPriority[i])
						{
							cmdsHu.bOperation = m_OperPriority[i];
							cmdsHu.isOtherPriority = 0;
							if (m_iHandMJCount[i] % 3 == 2)
							{
								if (cmdsHu.bOperation & Operation_Gang)
								{
									bool bIsZiMoGang = m_pGameLogic->IsSelfMingGang(m_bHUMJ[m_iOperateUer], m_iHUMJCount[m_iOperateUer], m_bAllMJ[m_iTouchPos-1]);
									if (bIsZiMoGang && (m_iHandMJCount[m_iOperateUer]>=1))
									{
										MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
										MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
										MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
										memcpy(btmpHuMJ, m_bHUMJ[m_iOperateUer], sizeof(MJBlock)*MAX_BLOCK_COUNT);
										itmpHuCount = m_iHUMJCount[m_iOperateUer];
										memcpy(btmpHandMJ, m_bHandMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_HAND_MJ);
										itmpHandCount = m_iHandMJCount[m_iOperateUer];
										memcpy(btmpOutMJ, m_bOutMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_OUT_MJ);
										itmpOutCount = m_iOutMJCount[m_iOperateUer];

										bIsZiMoGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, m_bAllMJ[m_iTouchPos-1], Operation_Gang_Type_ZiMoMing);
										if (bIsZiMoGang)
										{
											cmdsHu.iGangCount = 1;
											cmdsHu.bMJGang[0] = m_bAllMJ[m_iTouchPos-1];
											cmdsHu.bGangType = Operation_Gang_Type_ZiMoMing;
											//cmdsHu.bOperation |= Operation_Gang;
										}
									}
									else
									{
										bool bIsAnGang = m_pGameLogic->IsAnGang(m_bHUMJ[m_iOperateUer], m_iHUMJCount[m_iOperateUer], m_bHandMJ[m_iOperateUer], m_iHandMJCount[m_iOperateUer], cmdsHu.bMJGang, cmdsHu.iGangCount);
										if (bIsAnGang && (m_iHandMJCount[m_iOperateUer]>=1))
										{
											for (int iCount=0; iCount<cmdsHu.iGangCount; iCount++)
											{
												MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
												MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
												MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
												memcpy(btmpHuMJ, m_bHUMJ[m_iOperateUer], sizeof(MJBlock)*MAX_BLOCK_COUNT);
												itmpHuCount = m_iHUMJCount[m_iOperateUer];
												memcpy(btmpHandMJ, m_bHandMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_HAND_MJ);
												itmpHandCount = m_iHandMJCount[m_iOperateUer];
												memcpy(btmpOutMJ, m_bOutMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_OUT_MJ);
												itmpOutCount = m_iOutMJCount[m_iOperateUer];

												bIsAnGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, cmdsHu.bMJGang[iCount], Operation_Gang_Type_An);
												if (bIsAnGang)
												{
													cmdsHu.bGangType |= Operation_Gang_Type_An;
												}
											}
										}
									}
								}
							}
						}
						else
						{
							cmdsHu.bOperation = 0;
							cmdsHu.isOtherPriority = 1;
						}
					}
					else
					{
						cmdsHu.bOperation = 0;
						cmdsHu.isOtherPriority = 0;
					}

                    Json::Value msgHu;
                    msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                    msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
					CmdHelper::CodeMessage(cmdsHu, msgHu);
					m_g_GameRoom->SendTableMsg(i, msgHu);
				}
				//如果没有操作，那么下家摸牌
				if (!HasMal)
				{
					int itmpNexter = 0;
					for (int i = 0; i < GAME_PLAYER; ++i)
					{
						itmpNexter = (i+m_iOldCurrentUer) % GAME_PLAYER;
						if (itmpNexter == m_iOldCurrentUer || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo)
						{
							continue;
						}
						m_iOperateUer = itmpNexter;
						if (m_iOperateUer == itmpNexter)
						{
							break;
						}
					}
					if (m_iHandMJCount[m_iOperateUer] % 3 != 2)
					{
						if (m_iTouchPos >= MAX_ORG_MJ)
						{
							//流局
							OperGameOver(EndGameType_LiuJu);
							return;
						}
						else
						{
							if (m_HasHuTouch >= 0)
							{
								m_iOperateUer = m_HasHuTouch;
								m_HasHuTouch = -1;
							}
							OperTouch(m_iOperateUer);
						}
					}
				}
			}
			else
			{
				int iCurrentUser = 0;
				bool HasMal = false;bool HasCurrentMal = false;
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (enUserStatu[i] >= PlayerStatu_ZiMo)
					{
						continue;
					}
					if (m_iHandMJCount[i] % 3 == 2)
					{
						HasCurrentMal = true;
						m_iOperateUer = i;
						break;
					}
				}
				if (HasCurrentMal)
				{
					bool bIsZiMoGang = m_pGameLogic->IsSelfMingGang(m_bHUMJ[m_iOperateUer], m_iHUMJCount[m_iOperateUer], m_bAllMJ[m_iTouchPos-1]);
					if (bIsZiMoGang && (m_iHandMJCount[m_iOperateUer]>=1))
					{
						MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
						MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
						MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
						memcpy(btmpHuMJ, m_bHUMJ[m_iOperateUer], sizeof(MJBlock)*MAX_BLOCK_COUNT);
						itmpHuCount = m_iHUMJCount[m_iOperateUer];
						memcpy(btmpHandMJ, m_bHandMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_HAND_MJ);
						itmpHandCount = m_iHandMJCount[m_iOperateUer];
						memcpy(btmpOutMJ, m_bOutMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_OUT_MJ);
						itmpOutCount = m_iOutMJCount[m_iOperateUer];

						bIsZiMoGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, m_bAllMJ[m_iTouchPos-1], Operation_Gang_Type_ZiMoMing);
						if (bIsZiMoGang)
						{
							cmdsHu.iGangCount = 1;
							cmdsHu.bMJGang[0] = m_bAllMJ[m_iTouchPos-1];
							cmdsHu.bGangType = Operation_Gang_Type_ZiMoMing;
							//cmdsHu.bOperation |= Operation_Gang;
						}
					}
					else
					{
						bool bIsAnGang = m_pGameLogic->IsAnGang(m_bHUMJ[m_iOperateUer], m_iHUMJCount[m_iOperateUer], m_bHandMJ[m_iOperateUer], m_iHandMJCount[m_iOperateUer], cmdsHu.bMJGang, cmdsHu.iGangCount);
						if (bIsAnGang && (m_iHandMJCount[m_iOperateUer]>=1))
						{
							for (int iCount=0; iCount<cmdsHu.iGangCount; iCount++)
							{
								MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
								MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
								MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
								memcpy(btmpHuMJ, m_bHUMJ[m_iOperateUer], sizeof(MJBlock)*MAX_BLOCK_COUNT);
								itmpHuCount = m_iHUMJCount[m_iOperateUer];
								memcpy(btmpHandMJ, m_bHandMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_HAND_MJ);
								itmpHandCount = m_iHandMJCount[m_iOperateUer];
								memcpy(btmpOutMJ, m_bOutMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_OUT_MJ);
								itmpOutCount = m_iOutMJCount[m_iOperateUer];

								bIsAnGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, cmdsHu.bMJGang[iCount], Operation_Gang_Type_An);
								if (bIsAnGang)
								{
									cmdsHu.bGangType |= Operation_Gang_Type_An;
								}
							}
						}
					}
				}
				else if (!HasCurrentMal)
				{
					for (int i= 0; i < GAME_PLAYER; ++i)
					{
						iCurrentUser = (i + m_iOldCurrentUer) % GAME_PLAYER;
						if (iCurrentUser == m_iOldCurrentUer || enUserStatu[i] >= PlayerStatu_ZiMo)
						{
							continue;
						}
						if (m_OperPriority[iCurrentUser])
						{
							HasMal = true;
							m_iOperateUer = iCurrentUser;
							break;
						}
					}
				}

				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (HasMal)
					{
						if (m_OperPriority[i])
						{
							cmdsHu.bOperation = m_OperPriority[i];
							cmdsHu.isOtherPriority = 0;
							if (m_iHandMJCount[i] % 3 != 2 && (cmdsHu.bOperation & Operation_Gang))
							{
								cmdsHu.bGangType = Operation_Gang_Type_Ming;
								cmdsHu.bMJGang[0] = cmdsHu.bUserHuMJ;
								cmdsHu.iGangCount = 1;
							}
						}
						else
						{
							cmdsHu.bOperation = 0;
							cmdsHu.isOtherPriority = 1;
						}
					}
					else if (HasCurrentMal)
					{
						if (m_OperPriority[m_iOperateUer])
						{
							if (i == m_iOperateUer)
							{
								cmdsHu.bOperation = m_OperPriority[m_iOperateUer];
								cmdsHu.isOtherPriority = 0;
							}
							else
							{
								cmdsHu.bOperation = 0;
								cmdsHu.isOtherPriority = 1;
							}
						}
						else
						{
							cmdsHu.bOperation = 0;
							cmdsHu.isOtherPriority = 0;
						}
					}
					else
					{
						cmdsHu.bOperation = 0;
						cmdsHu.isOtherPriority = 0;
					}

                    Json::Value msgHu;
                    msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                    msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
					CmdHelper::CodeMessage(cmdsHu, msgHu);
					m_g_GameRoom->SendTableMsg(i, msgHu);
				}
			}
		}
	}
	else if (enhuType == HuType_ZiMo || enhuType == HuType_QiangGang || enhuType == HuType_PingHu)
	{
		if (enhuType == HuType_ZiMo)
		{
			//发送消息
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				m_OperPriority[i] = 0;
				cmdsHu.bOperation = 0;
				cmdsHu.isOtherPriority = 0;

                Json::Value msgHu;
                msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
				CmdHelper::CodeMessage(cmdsHu, msgHu);
				m_g_GameRoom->SendTableMsg(i, msgHu);
			}
			//自摸玩家的下家摸牌
			int itmpNexter = 0;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				itmpNexter = (i+playerId) % GAME_PLAYER;
				if (itmpNexter == playerId || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo || m_isForceQuit[itmpNexter] == 1)
				{
					continue;
				}
				m_iOperateUer = itmpNexter;
				if (m_iOperateUer == itmpNexter)
				{
					break;
				}
			}
			if (m_iHandMJCount[m_iOperateUer] % 3 != 2)
			{
				if (m_iTouchPos >= MAX_ORG_MJ)
				{
					//流局
					OperGameOver(EndGameType_LiuJu);
					return;
				}
				else
				{
					OperTouch(m_iOperateUer);
				}
			}
		}
		else
		{
			//检测是不是还有胡，如果没有胡操作，那么出牌的下家摸牌,记住这里只能有胡操作
			bool HasHu = false;
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (m_OperPriority[i] & Operation_Hu)
				{
					cmdsHu.isOtherHu = 1;
					HasHu = true;
					break;
				}
			}
			if (HasHu)
			{
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					int itmpNexter = (i+playerId) % GAME_PLAYER;
					if (itmpNexter == playerId || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo || m_isForceQuit[itmpNexter] == 1)
					{
						continue;
					}
					m_HasHuTouch = itmpNexter;
					if (m_HasHuTouch == itmpNexter)
					{
						break;
					}
				}
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					if (m_OperPriority[i] & Operation_Hu)
					{
						//这个时候只能是胡，因为有人应经胡过了,不能说放弃之后还有其他操作
						m_OperPriority[i] = Operation_Hu;
						cmdsHu.bOperation = Operation_Hu;
						cmdsHu.isOtherPriority = 0;
					}
					else
					{
						m_OperPriority[i] = 0;
						cmdsHu.bOperation = 0;
						cmdsHu.isOtherPriority = 1;
					}

                    Json::Value msgHu;
                    msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                    msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
					CmdHelper::CodeMessage(cmdsHu, msgHu);
					m_g_GameRoom->SendTableMsg(i, msgHu);
				}
			}
			//没有胡操作，让下家摸牌
			else
			{
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					m_OperPriority[i] = 0;
					cmdsHu.bOperation = 0;
					cmdsHu.isOtherPriority = 1;

                    Json::Value msgHu;
                    msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                    msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
					CmdHelper::CodeMessage(cmdsHu, msgHu);
					m_g_GameRoom->SendTableMsg(i, msgHu);
				}
				int itmpNexter = 0;
				for (int i = 0; i < GAME_PLAYER; ++i)
				{
					itmpNexter = (i+playerId) % GAME_PLAYER;
					if (itmpNexter == playerId || enUserStatu[itmpNexter] >= PlayerStatu_ZiMo || m_isForceQuit[itmpNexter] == 1)
					{
						continue;
					}
					m_iOperateUer = itmpNexter;
					if (m_iOperateUer == itmpNexter)
					{
						break;
					}
				}
				if (m_iHandMJCount[m_iOperateUer] % 3 != 2)
				{
					if (m_iTouchPos >= MAX_ORG_MJ)
					{
						//流局
						OperGameOver(EndGameType_LiuJu);
						return;
					}
					else
					{
						OperTouch(m_iOperateUer);
					}
				}
			}
		}
	}
	else if (enhuType == HuType_GameOver || enhuType == HuType_ForceQuit)
	{
		if (enhuType == HuType_GameOver)
		{
			if (m_AllTotal[playerId] > 0)
			{
				double iTmpShui = m_AllTotal[playerId]*m_g_GameRoom->GetChouShui();
				int iChouShui = 0;
				if (iTmpShui >= 1)
				{
					iChouShui = (int)ceil(iTmpShui);
				}
				else
				{
					iChouShui = int(iTmpShui);
				}
				m_AllTotal[playerId] -= iChouShui;
				if (m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
				{
					m_g_GameRoom->WriteUserScore(playerId, m_AllTotal[playerId], iChouShui, enScoreKind_Win);
				}
			}
			else
			{
				if (m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
				{
					m_g_GameRoom->WriteUserScore(playerId, m_AllTotal[playerId], 0, enScoreKind_Lost);
				}
			}
		}
		bool HasMal = false;
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			if (m_OperPriority[i])
			{
				HasMal = true;
				break;
			}
		}
		if (HasMal)
		{
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				if (m_OperPriority[i])
				{
					cmdsHu.bOperation = m_OperPriority[i];
					cmdsHu.isOtherPriority = 0;
					if (m_iHandMJCount[i] % 3 == 2)
					{
						if (cmdsHu.bOperation & Operation_Gang)
						{
							bool bIsZiMoGang = m_pGameLogic->IsSelfMingGang(m_bHUMJ[m_iOperateUer], m_iHUMJCount[m_iOperateUer], m_bAllMJ[m_iTouchPos-1]);
							if (bIsZiMoGang && (m_iHandMJCount[m_iOperateUer]>=1))
							{
								MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
								MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
								MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
								memcpy(btmpHuMJ, m_bHUMJ[m_iOperateUer], sizeof(MJBlock)*MAX_BLOCK_COUNT);
								itmpHuCount = m_iHUMJCount[m_iOperateUer];
								memcpy(btmpHandMJ, m_bHandMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_HAND_MJ);
								itmpHandCount = m_iHandMJCount[m_iOperateUer];
								memcpy(btmpOutMJ, m_bOutMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_OUT_MJ);
								itmpOutCount = m_iOutMJCount[m_iOperateUer];

								bIsZiMoGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, m_bAllMJ[m_iTouchPos-1], Operation_Gang_Type_ZiMoMing);
								if (bIsZiMoGang)
								{
									cmdsHu.iGangCount = 1;
									cmdsHu.bMJGang[0] = m_bAllMJ[m_iTouchPos-1];
									cmdsHu.bGangType = Operation_Gang_Type_ZiMoMing;
									//cmdsHu.bOperation |= Operation_Gang;
								}
							}
							else
							{
								bool bIsAnGang = m_pGameLogic->IsAnGang(m_bHUMJ[m_iOperateUer], m_iHUMJCount[m_iOperateUer], m_bHandMJ[m_iOperateUer], m_iHandMJCount[m_iOperateUer], cmdsHu.bMJGang, cmdsHu.iGangCount);
								if (bIsAnGang && (m_iHandMJCount[m_iOperateUer]>=1))
								{
									for (int iCount=0; iCount<cmdsHu.iGangCount; iCount++)
									{
										MJBlock btmpHuMJ[MAX_BLOCK_COUNT]; int itmpHuCount;
										MJBYTE btmpHandMJ[MAX_HAND_MJ]; int itmpHandCount;
										MJBYTE btmpOutMJ[MAX_OUT_MJ]; int itmpOutCount;
										memcpy(btmpHuMJ, m_bHUMJ[m_iOperateUer], sizeof(MJBlock)*MAX_BLOCK_COUNT);
										itmpHuCount = m_iHUMJCount[m_iOperateUer];
										memcpy(btmpHandMJ, m_bHandMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_HAND_MJ);
										itmpHandCount = m_iHandMJCount[m_iOperateUer];
										memcpy(btmpOutMJ, m_bOutMJ[m_iOperateUer], sizeof(MJBYTE)*MAX_OUT_MJ);
										itmpOutCount = m_iOutMJCount[m_iOperateUer];

										bIsAnGang = m_pGameLogic->Gang(btmpHuMJ, itmpHuCount, btmpHandMJ, itmpHandCount, btmpOutMJ, itmpOutCount, cmdsHu.bMJGang[iCount], Operation_Gang_Type_An);
										if (bIsAnGang)
										{
											cmdsHu.bGangType |= Operation_Gang_Type_An;
										}
									}
								}
							}
						}
					}
					else
					{
						cmdsHu.bGangType = Operation_Gang_Type_Ming;
						cmdsHu.bMJGang[0] = cmdsHu.bUserHuMJ;
						cmdsHu.iGangCount = 1;
					}
				}
				else
				{
					cmdsHu.bOperation = 0;
					cmdsHu.isOtherPriority = 1;
				}

                Json::Value msgHu;
                msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
				CmdHelper::CodeMessage(cmdsHu, msgHu);
				m_g_GameRoom->SendTableMsg(i, msgHu);
			}
		}
		else
		{
			for (int i = 0; i < GAME_PLAYER; ++i)
			{
				cmdsHu.bOperation = 0;
				cmdsHu.isOtherPriority = 0;

                Json::Value msgHu;
                msgHu["MsgId"] = IDD_MESSAGE_ROOM;
                msgHu["MsgSubId"] = ID_S_GAMEMSG_HU;
				CmdHelper::CodeMessage(cmdsHu, msgHu);
				m_g_GameRoom->SendTableMsg(i, msgHu);
			}
		}
	}

	for (int i = 0; i < GAME_PLAYER; ++i)
	{
		if (m_isForceQuit[i] == 1)
		{
			m_ForceUserID[i]++;
			m_isForceQuit[i] = 0;
			Player *pPlayer = m_g_GameRoom->GetPlayer(i);
			if(pPlayer != NULL && m_g_GameRoom->GetRoomType() != ROOMTYPE_BISAI)
			{
				int64 pPlayerMoney = (-1)*pPlayer->GetMoney();
				m_g_GameRoom->WriteUserScore(i, pPlayerMoney, 0, enScoreKind_Lost);
			}
			OperHu(i, INVALID_CHAIR, HuType_ForceQuit);
		}
	}
	return;
}

void CServerLogicFrame::OperGameOver(enEndGameType endGameType)
{
	RoomState rs = m_g_GameRoom->GetRoomState();
	if (rs!=  ROOMSTATE_GAMING)return;

	memset(&m_CmdsGameOver, 0, sizeof(m_CmdsGameOver));
	CMD_S_GameOver cmdsgameover;
	memset(&cmdsgameover, 0, sizeof(CMD_S_GameOver));
	memset(cmdsgameover.iDianPaoUser, INVALID_CHAIR, sizeof(int)*GAME_PLAYER);
	cmdsgameover.enEndType = endGameType;

	for (int i=0; i<GAME_PLAYER; ++i)
	{
		cmdsgameover.bHandMjCount[i] = m_iHandMJCount[i];
		cmdsgameover.bHUMjCount[i] = m_iHUMJCount[i];
		cmdsgameover.bOutMjCount[i] = m_iOutMJCount[i];
		memcpy(cmdsgameover.bHandMj[i], m_bHandMJ[i], sizeof(MJBYTE)*MAX_HAND_MJ);
		memcpy(cmdsgameover.bHUMj[i], m_bHUMJ[i], sizeof(MJBlock)*MAX_BLOCK_COUNT);
		memcpy(cmdsgameover.bOutMj[i], m_bOutMJ[i], sizeof(MJBYTE)*MAX_OUT_MJ);
	}
	//正常结束游戏
	if (endGameType == EndGameType_Hu)
	{
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			//////////////////////////////////////////////////////////////////////////
			//两大实时胡，杠更新运用
			cmdsgameover.iFanScore[i] = m_AllHuSorce[i] + m_QuitSorce[i];
			cmdsgameover.iGangScore[i] = m_AllGangSorce[i];
			cmdsgameover.iTotalScore[i] += m_AllTotal[i];
			//////////////////////////////////////////////////////////////////////////

			if (m_HuInfo[i].iHuInfoType == HuType_PingHu || m_HuInfo[i].iHuInfoType == HuType_QiangGang)
			{
				//胡的人所有信息
				int iPHuUser = m_HuInfo[i].iHuUser;
				cmdsgameover.iDianPaoUser[iPHuUser] = m_HuInfo[i].iDianPaoUser;
				cmdsgameover.enhuType[iPHuUser] = m_HuInfo[i].iHuInfoType;
				cmdsgameover.iNomalFan[iPHuUser] = m_HuInfo[i].iHuFan;
				cmdsgameover.iExtraFan[iPHuUser] = m_HuInfo[i].iExtraFan;
				cmdsgameover.enUSEndType[iPHuUser] = UserEndType_PingHu;
				for (int j = 0; j < MAX_FAN_STYLE; ++j)
				{
					if (m_HuInfo[i].iFanStyle[j] > 0)
					{
						cmdsgameover.iFanStyle[iPHuUser] = j;
						break;
					}
				}
			}
			else if (m_HuInfo[i].iHuInfoType == HuType_ZiMo)
			{
				cmdsgameover.iNomalFan[i] = m_HuInfo[i].iHuFan;
				cmdsgameover.iExtraFan[i] = m_HuInfo[i].iExtraFan;
				cmdsgameover.enhuType[i] = m_HuInfo[i].iHuInfoType;
				cmdsgameover.enUSEndType[i] = UserEndType_ZiMo;
				for (int j = 0; j < MAX_FAN_STYLE; ++j)
				{
					if (m_HuInfo[i].iFanStyle[j] > 0)
					{
						cmdsgameover.iFanStyle[i] = j;
						break;
					}
				}
			}
			else if (m_HuInfo[i].iHuInfoType == HuType_QiangTui)
			{
				//返回刮风下雨得分
				/*for (int j = 0; j < GuaFengMaxCount; ++j)
				{
					if (m_GuaFengInfo[j].eGangPlayer == i)
					{
						int tmpYinGanger = INVALID_CHAIR;
						int iGangBackCount = 0;
						for (int z = 0; z < GAME_PLAYER; ++z)
						{
							tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
							if ((tmpYinGanger != -1) && (enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu))
							{
								iGangBackCount++;
							}
						}
						if (m_GuaFengInfo[j].eGangSorce[i] >= m_UserMoney[i])
						{
							m_GuaFengInfo[j].eGangSorce[i] = m_UserMoney[i];
						}
						cmdsgameover.iGangScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
						cmdsgameover.iTotalScore[i] -= m_GuaFengInfo[j].eGangSorce[i];
						m_UserMoney[i] -= m_GuaFengInfo[j].eGangSorce[i];
						assert(m_UserMoney[i] >= 0);

						for (int z = 0; z < GAME_PLAYER; ++z)
						{
							tmpYinGanger = m_GuaFengInfo[j].eYinGanger[z] ;
							if (tmpYinGanger != -1 && enUserStatu[tmpYinGanger] <= PlayerStatu_PingHu)
							{
								cmdsgameover.iGangScore[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
								cmdsgameover.iTotalScore[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
								m_UserMoney[tmpYinGanger] += m_GuaFengInfo[j].eGangSorce[i]/iGangBackCount;
							}
						}
					}
				}*/
				cmdsgameover.enUSEndType[i] = UserEndType_Escape;
			}
			else if (m_HuInfo[i].iHuInfoType == HuType_Null)
			{
			}
		}
	}
	//流局结束游戏
	else if (endGameType == EndGameType_LiuJu)
	{
		LookHuaZhuDaJiao(cmdsgameover);
		m_zuozhuang = m_iBankerUser;
	}
	else if (endGameType == EndGameType_QiangTui)
	{
		for (int i = 0; i < GAME_PLAYER; ++i)
		{
			//////////////////////////////////////////////////////////////////////////
			//两大实时胡，杠更新运用
			cmdsgameover.iFanScore[i] = m_AllHuSorce[i] + m_QuitSorce[i];
			cmdsgameover.iGangScore[i] = m_AllGangSorce[i];
			cmdsgameover.iTotalScore[i] += m_AllTotal[i];
			//////////////////////////////////////////////////////////////////////////
			if (enUserStatu[i] == PlayerStatu_Escape)
			{
				cmdsgameover.enUSEndType[i] = UserEndType_Escape;
			}
		}
	}

	Room *pRoom = m_g_GameRoom;
	if(pRoom->GetRoomType() == ROOMTYPE_BISAI)
	{
		/*bool isok=false;
		memcpy(&m_CmdsGameOver, &cmdsgameover, sizeof(cmdsgameover));
		int64 MatchScore[GAME_PLAYER];//比赛玩家积分
		memset(MatchScore,0,sizeof(MatchScore));
		for(int i=0;i<GAME_PLAYER;i++)
		{
			Player *pPlayer = pRoom->GetPlayer(i);
			if(pPlayer == NULL) continue;

			pPlayer->SetMatchCount(pPlayer->GetMatchCount()-1);

			if(pPlayer->GetMatchCount() > 0)
			{
				isok=true;
			}
			m_CmdsGameOver.iTotalScore[i] = m_CmdsGameOver.iTotalScore[i];
			//每局结束计算玩家积分
			MatchScore[i] =1*m_CmdsGameOver.iTotalScore[i];//1为比赛场基shu
			cmdsgameover.iTotalScore[i] =cmdsgameover.iTotalScore[i];
			int64 sorce = pPlayer->GetMatchResult();
			pPlayer->SetMatchResult(pPlayer->GetMatchResult()+MatchScore[i]);

		}

		for(int i=0;i<GAME_PLAYER;i++)
		{
			Player *pPlayer = pRoom->GetPlayer(i);
			if(pPlayer == NULL) continue;
			// 得到比赛排名
			pPlayer->GetPlayerRanking();
		}

		//发送信息
        Json::Value msgGameOver;
        msgGameOver["MsgId"] = IDD_MESSAGE_ROOM;
        msgGameOver["MsgSubId"] = ID_S_GAMEMSG_GAMEOVER;
		CmdHelper::CodeMessage(cmdsgameover, msgGameOver);

		m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgGameOver);

		m_iBankerUser = m_zuozhuang;

		//关闭定时器
		m_g_GameRoom->StopAllTimer();
		if(isok)
		{
			m_g_GameRoom->StartTimer(TIMER_DELAY_READY, 8);	//延时发准备消息
		}*/
	}
	else
	{
		float fChouShui = m_g_GameRoom->GetChouShui();

		for (int i = 0; i < GAME_PLAYER; i++)
		{
			Player *myPlayer = m_g_GameRoom->GetPlayer(i);
			if (myPlayer == NULL)
			{
				continue;
			}
			int64 iresultSorce = myPlayer->GetMoney();
			if((enUserStatu[i] >= PlayerStatu_Escape) || m_UserMoney[i] <= 0)
			{
				continue;
			}
			enScoreKind enKind;
			int iChouShui = 0;
			int64 tmpSorce = cmdsgameover.iTotalScore[i];
			if (tmpSorce > 0)
			{
				//扣除抽水
				double iTmpShui = tmpSorce*m_g_GameRoom->GetChouShui();
				if (iTmpShui >= 1)
				{
					iChouShui = (int)ceil(iTmpShui);
				}
				else
				{
					iChouShui = int(iTmpShui);
				}
				cmdsgameover.iTotalScore[i] = tmpSorce - iChouShui;
				enKind = enScoreKind_Win;
			}
			else
			{
				if (endGameType == EndGameType_LiuJu)
				{
					enKind = enScoreKind_Draw;
				}
				else
				{
					enKind = enScoreKind_Lost;
				}
			}
			if (enUserStatu[i] == PlayerStatu_ForceQuit)
			{
			}
			else
			{
				m_g_GameRoom->WriteUserScore(i, cmdsgameover.iTotalScore[i], iChouShui, enKind);
			}
		}

        Json::Value msgGameOver;
        msgGameOver["MsgId"] = IDD_MESSAGE_ROOM;
        msgGameOver["MsgSubId"] = ID_S_GAMEMSG_GAMEOVER;
		CmdHelper::CodeMessage(cmdsgameover, msgGameOver);
		m_g_GameRoom->SendTableMsg(INVALID_CHAIR, msgGameOver);

		m_iBankerUser = m_zuozhuang;

		//本局结束
		m_g_GameRoom->GameEnd(true);
		//关闭定时器
		m_g_GameRoom->StopAllTimer();
		//开启一个30秒的定时器，如果都没有开始，那么就设置本桌位为空，释放桌子
		m_g_GameRoom->StartTimer(TIMER_CHANGETABLE , 20);
	}
	//结束录像
	//m_g_GameRoom->EndVideoTape();
	return;
}

void CServerLogicFrame::UpdateOperation(int &iChairID, MJBYTE &iOutMJ, MJBlock bHuMJ[], MJBYTE iHuMJCount, MJBYTE bHandMJ[], MJBYTE iHandMJCount, MJBYTE bOutMJ[], MJBYTE iOutMJCount, int &OperationType, bool iWalkType)
{

	if (enUserStatu[iChairID] >= PlayerStatu_ZiMo)
	{
		return;
	}
	bool isHu = this->isHu(iChairID, m_bHUMJ[iChairID], m_iHUMJCount[iChairID], m_bHandMJ[iChairID], m_iHandMJCount[iChairID], iOutMJ, iWalkType);
	if (isHu && (m_operationGaveUp[iChairID] & Operation_Hu))
	{
		if (m_iTouchPos <= MAX_ORG_MJ)
		{
			OperationType |= Operation_Hu;
			//return;
		}
	}
	//判断是否有杠
	bool isOutMingGang = m_pGameLogic->IsOutMingGang(bHuMJ, iHuMJCount, bHandMJ, iHandMJCount, iOutMJ);
	if(isOutMingGang) OperationType |= Operation_Gang;

	//判断是否有碰
	bool isPeng = m_pGameLogic->IsPeng(bHuMJ, iHuMJCount, bHandMJ, iHandMJCount, iOutMJ);
	if (isPeng) OperationType |= Operation_Peng;

	return;
}

int  CServerLogicFrame::UpdatePriority(int &iChairID, int &iOutPlayer)
{
	//找出优先级高的人
	int btmpOperPriority[GAME_PLAYER];
	memset(btmpOperPriority, 0,  sizeof(int)*GAME_PLAYER);
	memcpy(btmpOperPriority, m_OperPriority, sizeof(int)*GAME_PLAYER);
	std::sort(btmpOperPriority, btmpOperPriority+4);

	int itmpChairID = 0;
	for (int i=0; i<GAME_PLAYER; ++i)
	{
		itmpChairID = (i+iChairID) % GAME_PLAYER;
		if (itmpChairID == iOutPlayer)
		{
			continue;
		}
		if (m_OperPriority[itmpChairID] == btmpOperPriority[3])
		{
			return itmpChairID;
		}
	}
	return iChairID;
}


void CServerLogicFrame::GoodMJToRobot(MJBYTE bAllMJ[], int iAllMJCount, int iChairID)
{
	m_pGameLogic->RandomMJ(bAllMJ, iAllMJCount);

	MJBYTE bHandMJ[MAX_HAND_MJ];
	int iHandMJCount = 0;
	memset(bHandMJ, 0, sizeof(bHandMJ));

	//打乱三色
	int iThreeSeJudge[3] = {0, 1, 2};
	for (int i = 0; i < 3; ++i)
	{
		int index = m_pGameLogic->GetRandomNum(0, 2);
		if (index != i)
		{
			int iStore = iThreeSeJudge[i];
			iThreeSeJudge[i] = iThreeSeJudge[index];
			iThreeSeJudge[index] = iStore;
		}
	}

	//生成3色 生成一个顺子
	int iShunNum = m_pGameLogic->GetRandomNum(1, 7);
	MJBYTE bMJ = (iThreeSeJudge[0]<<4) + iShunNum;
	bHandMJ[iHandMJCount++] = bMJ;
	bHandMJ[iHandMJCount++] = bMJ+1;
	bHandMJ[iHandMJCount++] = bMJ+2;

	int iSeCountRate = m_pGameLogic->GetRandomNum(1, 100);

	//天湖
	if (iSeCountRate < 2)
	{
		//顺子
		iShunNum = m_pGameLogic->GetRandomNum(1, 7);
		bMJ = (iThreeSeJudge[0]<<4) + iShunNum;
		bHandMJ[iHandMJCount++] = bMJ;
		bHandMJ[iHandMJCount++] = bMJ+1;
		bHandMJ[iHandMJCount++] = bMJ+2;
		//顺子
		iShunNum = m_pGameLogic->GetRandomNum(1, 7);
		bMJ = (iThreeSeJudge[0]<<4) + iShunNum;
		bHandMJ[iHandMJCount++] = bMJ;
		bHandMJ[iHandMJCount++] = bMJ+1;
		bHandMJ[iHandMJCount++] = bMJ+2;
		//顺子
		iShunNum = m_pGameLogic->GetRandomNum(1, 7);
		bMJ = (iThreeSeJudge[1]<<4) + iShunNum;
		bHandMJ[iHandMJCount++] = bMJ;
		bHandMJ[iHandMJCount++] = bMJ+1;
		bHandMJ[iHandMJCount++] = bMJ+2;
	}
	//这种情况下是接近一色
	else if (iSeCountRate < 10)
	{
		//生成一个对
		int iAddRandom = m_pGameLogic->GetRandomNum(0,2);
		bHandMJ[iHandMJCount++] = bMJ + iAddRandom;
		bHandMJ[iHandMJCount++] = bMJ + iAddRandom;

		iShunNum = m_pGameLogic->GetRandomNum(1, 7);
		bMJ = (iThreeSeJudge[0]<<4) + iShunNum;
		bHandMJ[iHandMJCount++] = bMJ;
		bHandMJ[iHandMJCount++] = bMJ+1;
		bHandMJ[iHandMJCount++] = bMJ+2;

		for (int i = 1; i < 10; ++i)
		{
			int iCount = m_pGameLogic->GetMJCount(bHandMJ, iHandMJCount, ((iThreeSeJudge[0]<<4)+i));
			if (iCount <= 1)
			{
				bHandMJ[iHandMJCount++] = (iThreeSeJudge[0]<<4)+i;
				bHandMJ[iHandMJCount++] = (iThreeSeJudge[0]<<4)+i;
				bHandMJ[iHandMJCount++] = (iThreeSeJudge[0]<<4)+i;
				break;
			}
		}

		int iSeRandom = m_pGameLogic->GetRandomNum(1, 2);
		int iSeNum = m_pGameLogic->GetRandomNum(1, 9);
		bMJ = (iThreeSeJudge[iSeRandom]<<4) + iSeNum;
		bHandMJ[iHandMJCount++] = bMJ;
	}
	//这种情况下是接近两色
	else if (iSeCountRate <= 100)
	{
		iShunNum = m_pGameLogic->GetRandomNum(1, 7);
		bMJ = (iThreeSeJudge[0]<<4) + iShunNum;
		bHandMJ[iHandMJCount++] = bMJ;
		bHandMJ[iHandMJCount++] = bMJ+1;
		bHandMJ[iHandMJCount++] = bMJ+2;

		iShunNum = m_pGameLogic->GetRandomNum(1, 7);
		bMJ = (iThreeSeJudge[1]<<4) + iShunNum;
		bHandMJ[iHandMJCount++] = bMJ;
		bHandMJ[iHandMJCount++] = bMJ+1;
		bHandMJ[iHandMJCount++] = bMJ+2;

		iShunNum = m_pGameLogic->GetRandomNum(1, 7);
		bMJ = (iThreeSeJudge[1]<<4) + iShunNum;
		bHandMJ[iHandMJCount++] = bMJ;
		bHandMJ[iHandMJCount++] = bMJ+1;
		bHandMJ[iHandMJCount++] = bMJ+2;
	}

	//重组麻将数据
	for (int i = 0; i < iHandMJCount; ++i)
	{
		m_pGameLogic->DeleteMJ(bAllMJ, iAllMJCount, bHandMJ[i]);
	}

	int iIndex = 0;
	for (int i=0; i<MAX_ORG_MJ; i++)
	{
		int iPlayID = (i/GAME_PLAYER)%GAME_PLAYER;
		if (iPlayID == iChairID)
		{
			bAllMJ[iAllMJCount] = bAllMJ[i];
			bAllMJ[i] = bHandMJ[iIndex];
			iIndex++;
			iAllMJCount ++;
			if (iIndex >= 12)
			{
				break;
			}
		}
	}
	return;
}

void CServerLogicFrame::Reset()
{
	if(m_g_GameRoom != NULL)
	{
		m_g_GameRoom->StopAllTimer();
	}
	memset(m_GuaFengInfo, 0, sizeof(m_GuaFengInfo));
	memset(m_OperationInfo, 0, sizeof(m_OperationInfo));
	memset(m_GuoShuiInfo, 0, sizeof(m_GuoShuiInfo));
	memset(m_HuInfo, 0, sizeof(m_HuInfo));
	memset(m_AllGangSorce, 0, sizeof(m_AllGangSorce));
	memset(m_AllHuSorce, 0, sizeof(m_AllHuSorce));
	memset(m_AllTotal, 0, sizeof(m_AllTotal));
	//////////////////////////////////////////////////////////////////////////
	m_HuInfo[0].iDianPaoUser = -1;m_HuInfo[1].iDianPaoUser = -1;
	m_HuInfo[2].iDianPaoUser = -1;m_HuInfo[3].iDianPaoUser = -1;
	//////////////////////////////////////////////////////////////////////////
	m_iBisicCore = 0;
	memset(m_bAllMJ, 0, sizeof(m_bAllMJ));

	m_iTouchPos = 0;
	m_iDiceNum01 = 0;
	m_iDiceNum02 = 0;
	m_iDiceNum11 = 0;
	m_iDiceNum12 = 0;

	memset(m_OperPriority, 0, sizeof(m_OperPriority));
	memset(m_TruBuffer, 0, sizeof(int)*GAME_PLAYER);

	m_iOldOperateUer = 0;
	m_iOperateUer = 0;
	m_iOldCurrentUer = 0;
	m_StartDrop = true;
	m_bTouchMJ = 0;
	memset(m_bGangKaiMJ, 0, sizeof(m_bGangKaiMJ));
	m_bGangTouchMJ = 0;
	m_isLiuLie = false;
	memset(m_isLiuLieUser, 0, sizeof(m_isLiuLieUser));
	memset(m_bHUMJ, 0, sizeof(m_bHUMJ));
	memset(m_iHUMJCount, 0, sizeof(m_iHUMJCount));
	memset(m_bHandMJ, 0, sizeof(m_bHandMJ));
	memset(m_iHandMJCount, 0, sizeof(m_iHandMJCount));
	memset(m_bOutMJ, 0, sizeof(m_bOutMJ));
	memset(m_iOutMJCount, 0, sizeof(m_iOutMJCount));
	memset(m_iAUTO_OutCount, 0, sizeof(int)*GAME_PLAYER);

	m_TouchPlayer = 0;

	memset(enUserStatu, 0, sizeof(enPlayerStatu));
	memset(enUserStatuOld, 0, sizeof(enPlayerStatu));

	memset(m_operationGaveUp, 8, sizeof(m_operationGaveUp));
	memset(m_bQiangGangMJ, 0, sizeof(m_bQiangGangMJ));
	m_isAlreadyHu = 0;
	memset(m_UserTouchFirst, 0, sizeof(m_UserTouchFirst));
	memset(m_isForceQuit, 0, sizeof(m_isForceQuit));
	m_StartQuit = true;
	memset(m_QuitSorce, 0, sizeof(m_QuitSorce));
	m_huMJ = 0;

	m_zuozhuang = -1;
	m_twice = 2;
	m_DoubleHuPlayer = 0;

	memset(m_iOldHandCount, 0, sizeof(m_iOldHandCount));
	m_HasHuTouch = -1;
	m_QiangHuCount = 0;
	m_GiveQiangHuCount = 0;
	m_BeiQiangUser = -1;
	m_ReadyCount = 0;
	memset(m_UserMoney, 0, sizeof(m_UserMoney));
	memset(&m_CmdsGameOver, 0, sizeof(m_CmdsGameOver));
	memset(m_StartUserID, 0, sizeof(m_StartUserID));
	memset(m_ForceUserID, 0, sizeof(m_ForceUserID));
	memset(m_huRanderUser, 0, sizeof(m_huRanderUser));
	m_HuPos = 0;
}

#ifdef ENABLE_DEBUG_TOOL
BOOL CServerLogicFrame::ForTest()
{
	TCHAR cIsDebug[5]=TEXT("");
	GetPrivateProfileString(TEXT("XZMJ_Test"), TEXT("IsDebug"), TEXT(""), cIsDebug, 5, TEXT(".\\XZMJTest.ini"));
	int iIsDebug = _ttoi(cIsDebug);
	if (iIsDebug)
	{
		//骰子
		TCHAR DICE01[5]=TEXT("");
		TCHAR DICE02[5]=TEXT("");
		TCHAR DICE11[5]=TEXT("");
		TCHAR DICE12[5]=TEXT("");
		GetPrivateProfileString(TEXT("XZMJ_Test"), TEXT("Dice01"), TEXT(""), DICE01, 5, TEXT(".\\XZMJTest.ini"));
		m_iDiceNum01 = _ttoi(DICE01);
		GetPrivateProfileString(TEXT("XZMJ_Test"), TEXT("Dice02"), TEXT(""), DICE02, 5, TEXT(".\\XZMJTest.ini"));
		m_iDiceNum02 = _ttoi(DICE02);
		GetPrivateProfileString(TEXT("XZMJ_Test"), TEXT("Dice11"), TEXT(""), DICE11, 5, TEXT(".\\XZMJTest.ini"));
		m_iDiceNum11 = _ttoi(DICE11);
		GetPrivateProfileString(TEXT("XZMJ_Test"), TEXT("Dice12"), TEXT(""), DICE12, 5, TEXT(".\\XZMJTest.ini"));
		m_iDiceNum12 = _ttoi(DICE12);

		m_iOperateUer = m_iBankerUser = GetPrivateProfileInt(TEXT("XZMJ_Test"), TEXT("BankerNum"), 0, TEXT(".\\XZMJTest.ini"));
		assert(m_iOperateUer>=0 && m_iOperateUer<GAME_PLAYER);
		TCHAR cData[5*MAX_ORG_MJ] = TEXT("");
		GetPrivateProfileString(TEXT("XZMJ_Test"), TEXT("MJ"), TEXT(""), cData, 5*MAX_ORG_MJ, TEXT(".\\XZMJTest.ini"));
		TCHAR cTmp[5] = TEXT("");
		TCHAR ccTmp[1] = TEXT("");
		for (int i=0;i<MAX_ORG_MJ;++i)
		{
			memcpy(cTmp,cData+i*5, 5 * sizeof(TCHAR));

			ccTmp[0] = cTmp[2];
			int aaa = _ttoi(ccTmp)<<4;
			ccTmp[0] = cTmp[3];
			aaa += _ttoi(ccTmp);
			m_bAllMJ[i] = aaa;
		}
	}
	return iIsDebug;
}
#endif
