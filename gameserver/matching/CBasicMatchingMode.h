#ifndef C_BASIC_MATCHING_MODE_HEAD_FILE
#define C_BASIC_MATCHING_MODE_HEAD_FILE

#include <vector>
#include <list>

#include "../../include/molnet/common.h"
#include "../gameframe/CPlayer.h"
#include "../gameframe/CRoom.h"

class CPlayer;
typedef CPlayer* CPlayerPtr;
class CRoom;

//////////////////////////////////////////////////////////////////////////

/**
 * 比赛模式
 */
enum matchMode
{
	MATCHMODE_LOOPMATCH = 0,                // 循环赛
	MATCHMODE_GROUPSMATCH,              // 分组淘汰赛
	MATCHMODE_NULL
};

/**
 * 比赛场基础类
 */
class BasicMatchingMode
{
public:
	/// 构造函数
	BasicMatchingMode(matchMode pmatchMode):m_matchMode(pmatchMode) {}
	/// 析构函数
	virtual ~BasicMatchingMode() {}

	/// 设置比赛模式
	inline void SetMatchingMode(matchMode pmode) { m_matchMode = pmode; }
	/// 得到比赛模式
	inline matchMode GetMatchingMode(void) { return m_matchMode; }

	/// 清除所有数据
	virtual void Clear(void) {}
	/// 检测指定的玩家是否在比赛中
	virtual bool IsExist(CPlayer *pPlayer) { return true; }
	/// 添加玩家到比赛场中
	virtual bool AddPlayer(CPlayer *pPlayer) { return true; }
	/// 获取指定玩家比赛中排名
	virtual void GetPlayerRanking(CPlayer *pPlayer) {}
	/// 从比赛场中删除玩家
	virtual bool DeletePlayer(CPlayer *pPlayer,bool isMatching=false) { return true; }
	/// 得到整个比赛人数
	virtual int GetMatchPlayerCount(void) { return 0; }
	/// 添加比赛用的桌子
	virtual bool AddRoom(CRoom *pRoom) { return true; }

	/// 比赛场更新
	virtual void Update(void) {}

private:
	matchMode m_matchMode;                         /**< 比赛模式 */
};

/**
 * 椅子管理类，用于管理所有的椅子，然后实现各种各样的比赛
 */
class CTabelFrameManager : public Singleton<CTabelFrameManager>
{
public:
	/// 构造函数
	CTabelFrameManager();
	/// 析构函数
	~CTabelFrameManager();

	/// 设置当前使用的比赛
	inline void SetMatchingMode(BasicMatchingMode *pBasicMatchingMode) { m_BasicMatchingMode = pBasicMatchingMode; }
	/// 得到当前使用的比赛
	inline BasicMatchingMode* GetMatchingMode(void) { return m_BasicMatchingMode; }

	/// 设置比赛类型
	inline void SetMatchingType(matchMode type) { m_matchMode = type; }
	/// 得到比赛类型
	inline matchMode GetMatchingType(void) { return m_matchMode; }
	/// 设置最大开赛人数
	inline void SetMaxStartPlayerCount(int count) { m_MaxStartPlayerCount = count; }
	/// 得到最大开赛人数
	inline int GetMaxStartPlayerCount(void) { return m_MaxStartPlayerCount; }
	/// 设置比赛循环轮数
	inline void SetMatchingLoopCount(int count) { m_MaxMatchingLoopCount = count; }
	/// 得到比赛循环轮数
	inline int GetMatchingLoopCount(void) { return m_MaxMatchingLoopCount; }

	/// 清除所有数据
	void Clear(void);
	/// 检测指定的玩家是否在比赛中
	bool IsExist(CPlayer *pPlayer);
	/// 添加玩家到比赛场中
	bool AddPlayer(CPlayer *pPlayer);
	/// 获取指定玩家比赛中排名
	void GetPlayerRanking(CPlayer *pPlayer);
	/// 从比赛场中删除玩家
	bool DeletePlayer(CPlayer *pPlayer,bool isMatching=false);
	/// 得到整个比赛人数
	int GetMatchPlayerCount(void);
	/// 添加比赛用的桌子
	bool AddRoom(CRoom *pRoom);

	/// 比赛场更新
	void Update(void);

private:
	matchMode m_matchMode;                                                /**< 比赛模式 */
	int m_MaxStartPlayerCount;                                            /**< 最大开赛人数 */
	int m_MaxMatchingLoopCount;                                           /**< 比赛循环轮数 */

	BasicMatchingMode *m_BasicMatchingMode;                               /**< 我们当前使用的比赛 */
};

//////////////////////////////////////////////////////////////////////////

#endif
