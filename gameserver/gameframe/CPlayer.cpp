#include "CPlayer.h"

#include "../GameFrameManager.h"
#include "PlayerManager.h"
#include "../matching/CDieOutMatchingMode.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////

CPlayer::CPlayer(PlayerType type)
	: m_Id(-1),m_ConnectId(0),m_roomId(-1),m_PlayerState(PLAYERSTATE_NORAML),
	m_ChairIndex(-1),m_level(0),m_Money(0),m_experience(0),
	m_totalbureau(0),m_successbureau(0),m_failbureau(0),m_successrate(0),m_runawayrate(0),
	m_PlayerType(type),m_isLookOn(false),m_BankMoney(0),m_RunawayBureau(0),
	m_Revenue(0),m_RobotLogicFrame(NULL),m_TotalResult(0),sex(0),m_RealyTime(0),
	gtype(0),ipaddress(0),m_PlayerDeviceType(PLAYERDEVICETYPE_NULL),m_CurGameType(0),m_CurServerPort(0),m_CurTableIndex(-1),m_CurChairIndex(-1),
	m_CurGamingState(false),m_roomentermoneyfirst(0),m_roomentermoneysecond(0),m_curGamingResult(0)
{
	SetType(type);

	m_MatchCount=0;
	m_TotalMatchCount=0;
	m_MatchResult=0;
	m_IsMatching=false;
	m_MatchRoomIndex=-1;
	m_IsMatchingLostOnline=false;
	m_IsMatchSignUp=false;

	if(m_PlayerType == PLAYERTYPE_ROBOT)
	{

	}
}

CPlayer::CPlayer(int id,uint32 conid)
	: m_Id(id),m_ConnectId(conid),m_roomId(-1),m_PlayerState(PLAYERSTATE_NORAML),
	m_ChairIndex(-1),m_level(0),m_Money(0),m_experience(0),
	m_totalbureau(0),m_successbureau(0),m_failbureau(0),m_successrate(0),m_runawayrate(0),
	m_PlayerType(PLAYERTYPE_NORMAL),m_isLookOn(false),m_BankMoney(0),m_RunawayBureau(0),
	m_Revenue(0),m_RobotLogicFrame(NULL),m_TotalResult(0),sex(0),m_RealyTime(0),
	gtype(0),ipaddress(0),m_PlayerDeviceType(PLAYERDEVICETYPE_NULL),
	m_offlineConnectId(0),m_CurGameType(0),m_CurServerPort(0),m_CurTableIndex(-1),m_CurChairIndex(-1),
	m_CurGamingState(false),m_roomentermoneyfirst(0),m_roomentermoneysecond(0),m_curGamingResult(0)
{
	SetType(PLAYERTYPE_NORMAL);

	m_MatchCount=0;
	m_TotalMatchCount=0;
	m_MatchResult=0;
	m_IsMatching=false;
	m_MatchRoomIndex=-1;
	m_IsMatchingLostOnline=false;
	m_IsMatchSignUp=false;
}

CPlayer::~CPlayer()
{
	DeleteAllTimer();

	if(m_PlayerType == PLAYERTYPE_ROBOT)
	{
		if(m_RobotLogicFrame)
			SafeDelete(m_RobotLogicFrame);
	}
}

/// 设置机器人游戏逻辑处理接口
void CPlayer::SetRobotLogicFrame(RobotLogicFrame *frame)
{
	assert(frame != NULL);
	if(frame == NULL) return;

	m_RobotLogicFrame = frame;
	m_RobotLogicFrame->SetGamePlayer(this,NULL);
}

/// 设置当前游戏房间
void CPlayer::SetCurGameRoom(Room* pRoom)
{
	assert(m_RobotLogicFrame != NULL);
	if(m_RobotLogicFrame == NULL) return;

	m_RobotLogicFrame->SetGamePlayer(this,pRoom);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// 删除所有的定时器
void CPlayer::DeleteAllTimer(void)
{
	m_TimerItemActive.clear();
}

/**
 * 开始一个定时器
 *
 * @param timerId 要开启的定时器ID
 * @param space 定时间隔
 *
 * @return 如果开启成功返回真，否则返回假
 */
bool CPlayer::StartTimer(int timerId,int space)
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
void CPlayer::StopTimer(int id)
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
void CPlayer::StopAllTimer(void)
{
	m_TimerLock.Acquire();
	CTimerItemArray::iterator iter = m_TimerItemActive.begin();
	for(;iter != m_TimerItemActive.end();++iter)
	{
		(*iter).second.nIsEnable=false;
	}
	m_TimerLock.Release();
}

///时间事件
bool CPlayer::OnEventTimer(DWORD dwTimerID, DWORD wBindParam)
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

		if(GetRobotLogicFrame())
		{
			GetRobotLogicFrame()->OnProcessTimerMsg(pTimerItem->nTimerID,pTimerItem->nTimeLeave);
		}
	}
	m_TimerLock.Release();

	return true;
}

/// 发送游戏数据
void CPlayer::SendGameMsg(Json::Value &msg)
{
	CMolMessageIn pInData = CMolMessageIn((const char*)msg.toStyledString().c_str(),msg.toStyledString().length());
	ServerGameFrameManager.OnProcessNetMes(GetConnectID(),&pInData);
}

/// 发送准备消息
void CPlayer::SendReadyMsg(void)
{
	ServerGameFrameManager.OnProcessGameReadyMatchingMes(this);
}

/// 获取指定玩家比赛中排名
void CPlayer::GetPlayerRanking(void)
{
	if(m_ServerSet.GameType == ROOMTYPE_BISAI)
		CTabelFrameManager::getSingleton().GetPlayerRanking(this);
}
