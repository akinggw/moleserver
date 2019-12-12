#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include "czhuangmanager.h"
#include <map>
#include <vector>

#define IDD_TIMER_GAME_STARTING   IDD_TIMER_GAME_START+1                   // 游戏卡开始定时器
#define IDD_TIMER_GAME_XIAZHU     IDD_TIMER_GAME_START+2                   // 下注
#define IDD_TIMER_GAME_KAIPAI     IDD_TIMER_GAME_START+3                   // 开牌
#define IDD_TIMER_GAME_JIESUAN    IDD_TIMER_GAME_START+4                   // 结算

struct tagGamingRecord
{
	tagGamingRecord():decCardType(CARTYPE_NULL) {}
	tagGamingRecord(tagCarType pCardType,std::string jhstr)
		: decCardType(pCardType)
	{
		strncpy(JvHaoStr,jhstr.c_str(),256);
	}

	tagCarType decCardType;
	char JvHaoStr[256];
};

class CServerLogicFrame : public ServerLogicFrame
{
public:
	/// 构造函数
	CServerLogicFrame();
	/// 析构函数
	~CServerLogicFrame();

	/// 设置当前应用房间
	virtual void SetGameRoom(Room* pRoom) { m_g_GameRoom = pRoom; }
	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes();
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
	/// 处理用户进入房间消息
	virtual void OnProcessEnterRoomMsg(int playerId);
	/// 处理用户离开房间消息
	virtual void OnProcessLeaveRoomMsg(int playerId);
	/// 处理用户断线重连消息
	virtual void OnProcessReEnterRoomMes(int playerId);
	/// 处理用户断线消息
	virtual void OnProcessOfflineRoomMes(int playerId);
	/// 处理用户定时器消息
	virtual void OnProcessTimerMsg(int timerId,int curTimer);

private:
	/// 导入服务器配置
	void LoadServerConfig(void);
	/// 统计各个区域的下注总输赢
	int64 GetRectJettonTotal(tagCarType pCarType,bool isRobot);
	int64 GetRectJettonTotal2(tagCarType pCarType);
	/// 统计指定玩家的下注总额
	int64 GetUserJettonTotal(uint32 pUserId);
	int64 getusertotalresult(void);
	int64 getusertotalresultbytype(tagCarType pCarType);
	int64 GetUserRectJetton(uint32 pUserId,tagCarType pCarType);
	int getLastMoneyResult(int64 pMoney,bool isRobot,bool isWinOk=false);
	int getLastMoneyResult2(int64 pMoney,bool isWinOk=false);
	int getLastRobtolMoneyResult(int64 pMoneystart,int64 pMoneyTwo);
	/// 得到结果值
	int GetGamingResult(void);
	/// 游戏结束
	void GameEnd(void);
	void OnProcessOpenRate(void);

private:
	Room *m_g_GameRoom;                                  /**< 游戏房间 */

private:
	std::map<tagCarType,int>       m_carBeiLv;               /**< 车型的倍率 */
	std::map<int32,std::map<tagCarType,int64> >      m_xiazhus;          /**< 所有人的下注 */
	GameState                      m_GameState;
	CGameLogic                     m_GameLogic;
	CZhuangManager                 m_ZhuangManager;

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int							   m_timecurlost;
	bool                           m_issupercontrol;
	int                            m_superzhuangstate;
	tagCarType                     m_supercontroltype;

	int                            m_curGamingResult;
	std::vector<tagGamingRecord>   m_gamerecords;

	int                            m_curMonthDay;
	int                            m_curJvHao;
	std::string                    m_curStrJvHao;

	bool                           m_isEnableOpenRate;
	std::map<int,std::vector<tagCarType> >       m_openrates;
};

#endif
