#ifndef _C_ZHUANG_MANAGER_H_INCLUDE_
#define _C_ZHUANG_MANAGER_H_INCLUDE_

#include <vector>

#include "cdefines.h"

class CZhuangManager : public Singleton<CZhuangManager>
{
public:
	CZhuangManager();
	CZhuangManager(Room *proom);
	~CZhuangManager();

	inline void setMinShangZhuangMoney(int64 pmoney) { m_minShangZhuangMoney = pmoney; }
	inline int64 getMinShangZhuangMoney(void) { return m_minShangZhuangMoney; }
	inline void setGameRoom(Room *proom) { m_g_myselfRoom = proom; }
	inline Room* getGameRoom(void) { return m_g_myselfRoom; }
	inline int32 getCurrentZhuang(void) { return m_curZhuangId; }
	inline std::vector<int32> & getShangZhuangList(void) { return m_zhuanglist; }
	inline void setTotalShangZhuangCount(int pcount) { m_totalShangZhuangCount = pcount; }
	inline int getTotalShangZhuangCount(void) { return m_totalShangZhuangCount; }
	inline int GetCurShangZhuangCount(void) { return m_curShangZhuangCount; }
	inline int64 GetZhuangTotalWinResult(void) { return m_zhuangtotalwinresult; }
	inline void SetZhuangTotalWinResult(int zhuangid,int64 pResult)
	{
		if(m_curZhuangId != zhuangid)
			m_zhuangtotalwinresult=0;

		m_zhuangtotalwinresult+=pResult;
	}
	inline void clear(void)
	{
		m_zhuanglist.clear();
		m_curZhuangId=-1;
		m_minShangZhuangMoney=0;
		m_totalShangZhuangCount=m_curShangZhuangCount=0;
	}

	bool addZhuang(int32 pplayerId);
	bool decZhuang(int32 pplayerId,bool isGaming);
	bool findZhuang(int32 pplayerId);

	int OnProcessGameStart(void);
	void OnProcessGameEnd(void);
	void OnProcessUserLeave(int32 pplayerId);

private:
	Room *m_g_myselfRoom;
	std::vector<int32> m_zhuanglist;
	int32 m_curZhuangId;
	int64 m_minShangZhuangMoney;
	int m_totalShangZhuangCount,m_curShangZhuangCount;
	int64 m_zhuangtotalwinresult;
};

#endif
