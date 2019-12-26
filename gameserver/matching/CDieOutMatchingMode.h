#ifndef _C_DIE_OUT_MATCHING_MODE_H_INCLUDE_
#define _C_DIE_OUT_MATCHING_MODE_H_INCLUDE_

#include "CBasicMatchingMode.h"
#include "../../include/molnet/Mutex.h"

/**
 * 比赛场结构
 */
struct MatchStruct
{
	MatchStruct():isMatching(false),matchcount(0),roomIndex(-1),isEnable(false),matchendcount(0) {}
	MatchStruct(bool im,std::vector<CPlayer*> &ppl,int mc,int ri,bool enable,int mec)
		: isMatching(im),playerlist(ppl),matchcount(mc),roomIndex(ri),isEnable(enable),matchendcount(mec) {}

	void clear(void)
	{
		isMatching = false;
		playerlist.clear();
		matchcount = 0;
		matchendcount = 0;
		roomIndex = -1;
		isEnable=false;
		oldroomlist.clear();
		curroomlist.clear();
		oldroomlisttimer.clear();
		oldroomlistString.clear();
	}

	bool isMatching;                 // 是否在比赛中
	std::vector<CPlayer*> playerlist; // 比赛的所有人
	int matchcount;                    // 比赛总的轮数
	int roomIndex;                     // 比赛场使用的房间索引
	bool isEnable;                     // 这场比赛是否可用
	int matchendcount;                 // 当前比赛结束人数
	std::map<int,int> oldroomlist,curroomlist;        // 当前比赛用的房间（主要用于监控当前比赛进度
	std::map<int,DWORD> oldroomlisttimer;
	std::map<std::string,int> oldroomlistString;
};

/**
 * 淘汰赛
 */
class CDieOutMatchingMode : public BasicMatchingMode
{
public:
	/// 构造函数
	CDieOutMatchingMode(matchMode pmatchMode);
	/// 析构函数
	virtual ~CDieOutMatchingMode();

	/// 清除所有数据
	virtual void Clear(void);
	/// 检测指定的玩家是否在比赛中
	virtual bool IsExist(CPlayer *pPlayer);
	/// 添加玩家到比赛场中
	virtual bool AddPlayer(CPlayer *pPlayer);
	/// 获取指定玩家比赛中排名
	virtual void GetPlayerRanking(CPlayer *pPlayer);
	/// 从比赛场中删除玩家
	virtual bool DeletePlayer(CPlayer *pPlayer,bool isMatching=false);
	/// 得到整个比赛人数
	virtual int GetMatchPlayerCount(void);
	/// 添加比赛用的桌子
	virtual bool AddRoom(CRoom *pRoom);

	/// 比赛场更新
	virtual void Update(void);

private:
	/// 锁住玩家链表
	inline void LockTableFramePlayerList(void) { m_tableframePlayerListLock.Acquire(); }
	/// 解锁玩家链表
	inline void UnLockTableFramePlayerList(void) { m_tableframePlayerListLock.Release(); }
	/// 得到玩家链表
	inline std::vector<MatchStruct>& GetTablePlayerList(void) { return m_tableframePlayerList; }
	/// 得到一个空的比赛房间
	int GetEmptyMatchingRoom(void);
	/// 根据比赛场索引得到比赛场
	MatchStruct* GetMatchStructById(int index);

	/// 开始处理比赛
	void OnProcessMatching(MatchStruct *pMatchStruct);
	/// 给指定比赛场的玩家发送比赛排名
	void SendMatchingRanking(MatchStruct *pMatchStruct,bool issort=true);
	/// 开始比赛
	void StartMatching(MatchStruct *pMatchStruct,bool isConntinue=false);
	/// 检测一轮比赛是否结束
	bool IsOverOneLoopMatching(MatchStruct *pMatchStruct);
	/// 比赛场所有用户全部离开房间，准备进行下一轮
	void AllPlayerLeaveMatchingToNext(MatchStruct *pMatchStruct);
	/// 结束比赛
	void MatchingOver(MatchStruct *pMatchStruct);
	/// 比赛继续
	void MatchingContinue(MatchStruct *pMatchStruct);
	/// 发送指定比赛场的等待人数
	void SendMatchingWaitingPlayerCount(MatchStruct *pMatchStruct);

private:
	std::vector<MatchStruct> m_tableframePlayerList;             /**< 用于管理所有的玩家 */
	std::vector<std::vector<CRoom*> > m_tableframeroomlist;      /**< 比赛用的桌子 */
	Mutex  m_tableframePlayerListLock;                                    /**< 用于保护所有玩家 */
	bool m_isAutoMatchingPlayerCount;                            /**< 是否自动匹配玩家参赛人数，一次只能匹配一次 */
};

#endif
