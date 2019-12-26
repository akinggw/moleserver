#include "CBasicMatchingMode.h"

#include <algorithm>

#include "../GameFrameManager.h"
#include "../gameframe/PlayerManager.h"
#include "../gameframe/RoomManager.h"

initialiseSingleton(CTabelFrameManager);

//////////////////////////////////////////////////////////////////////////

/**
 * 构造函数
 */
CTabelFrameManager::CTabelFrameManager()
	: m_matchMode(MATCHMODE_NULL),m_MaxStartPlayerCount(0),m_MaxMatchingLoopCount(0),m_BasicMatchingMode(NULL)
{

}

/**
 * 析构函数
 */
CTabelFrameManager::~CTabelFrameManager()
{
	if(m_BasicMatchingMode) SafeDelete(m_BasicMatchingMode);
	m_BasicMatchingMode = NULL;
}

/// 清除所有数据
void CTabelFrameManager::Clear(void)
{
	//ASSERT(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return;

	m_BasicMatchingMode->Clear();

	if(m_BasicMatchingMode) SafeDelete(m_BasicMatchingMode);
	m_BasicMatchingMode = NULL;
}

/// 检测指定的玩家是否在比赛中
bool CTabelFrameManager::IsExist(CPlayer *pPlayer)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->IsExist(pPlayer);
}

/// 从比赛场中删除玩家
bool CTabelFrameManager::DeletePlayer(CPlayer *pPlayer,bool isMatching)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->DeletePlayer(pPlayer,isMatching);
}

/// 添加比赛用的桌子
bool CTabelFrameManager::AddRoom(CRoom *pRoom)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->AddRoom(pRoom);
}

/// 添加玩家到比赛场中
bool CTabelFrameManager::AddPlayer(CPlayer *pPlayer)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->AddPlayer(pPlayer);
}

/// 得到整个比赛人数
int CTabelFrameManager::GetMatchPlayerCount(void)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return false;

	return m_BasicMatchingMode->GetMatchPlayerCount();
}

/// 获取指定玩家比赛中排名
void CTabelFrameManager::GetPlayerRanking(CPlayer *pPlayer)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return;

	return m_BasicMatchingMode->GetPlayerRanking(pPlayer);
}

/// 比赛场更新
void CTabelFrameManager::Update(void)
{
	assert(m_BasicMatchingMode != NULL);
	if(m_BasicMatchingMode == NULL) return;

	return m_BasicMatchingMode->Update();
}

