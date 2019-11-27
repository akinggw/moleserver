#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include "czhuangmanager.h"

#include <map>
#include <vector>

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
	/// 得到结果值
	void GetGamingResult(void);
	/// 计算所有玩家的输赢值
	int64 getWinMoney(void);
	//计算指定玩家赢了多少钱
	int64 getWinMoneyByUserId(int userid,int64 *lRevenue);
	//得到指定玩家指定区域的输赢值
	int64 getWinMoneyByUserIdAndArea(int userid,YanZhuType type,int64 *lRevenue);
	/// 游戏结束
	void GameEnd(void);
	int64 GetAllUserJettons(void);
	int64 getusertotalresult(void);
	int masterwinorlost(bool iswin,int64 pMaxMoney=-1);
	int GetGamingResultByUserId(int userid,int64 pMoney);

private:
	Room *m_g_GameRoom;                                  /**< 游戏房间 */

private:
	std::map<YanZhuType,int>       m_carBeiLv;               /**< 车型的倍率 */
	GameState                      m_GameState;
	CZhuangManager                 m_ZhuangManager;
	CXiaZhuManager                 m_BeiLvXiaZhuManager;
	std::map<int,CXiaZhuManager>   m_UserXiaZhuManager;
	std::vector<tagGamingRecord>   m_gamerecords;
	int                            m_curGamingResult;

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int                            m_timecurlost;

	bool                           m_issupercontrol;
	bool                           m_issuperZhuangWinControl;
	int                            m_supercontroltype;

	int                            m_curMonthDay;
	int                            m_curJvHao;
	std::string                    m_curStrJvHao;

	tagSuperUserControl            m_SuperUserControl;
	int64                          m_maxtotaljettons,m_maxpeoplejettons;
};


#endif
