#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
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
	/// 计算某种花色总的下注量
	int64 ComputerCardColorTotalJetton(uint8 cardcolor);
	/// 得到指定玩家总的下注
	int64 ComputerUserTotalJetton(uint32 pUserId);
	/// 得到指定玩家指定花色的下注
	int64 ComputerUserCardColorTotalJetton(uint32 pUserId,uint8 cardcolor);
	/// 得到当前赚钱最少的花色
	int ComputerCardColorTotalResult(int64 presult);
	/// 得到当前最亏的花色
	int ComputerCardColorFailTotalResult(int64 *presult);
	/// 根据玩家下注得到最终牌
	uint8 GetResultCard(void);
	/// 结算游戏
	void TradGame(void);
	/// 清空所有人下注记录
	void ClearJettonRecord(void);
	/// 加载游戏配置
	void LoadGameConfig(void);

private:
	Room *m_g_GameRoom;                                  /**< 游戏房间 */

	CGameLogic m_CGameLogic;
	GameState m_GameState;
	uint8 m_resultCard;                                   /**< 结果牌 */
	int64 m_GamePielement;                                    /**< 游戏基础分 */

	std::map<uint8,float>     m_jettonTrad;                    /**< 花色倍率 */
	std::map<uint8,int>       m_colorrecordcount;              /**< 花色出现次数 */
	std::map<uint32,tagJettons> m_userjettonresult;              /**< 用户押注结果 */
	std::vector<uint8>        m_cardrecord;                    /**< 游戏记录 */
	int                      m_gamejvcount;                   /**< 游戏局数 */

	float m_fangkuai, m_meihua, m_heitao, m_hongtao, m_wang;
	int m_timegamestart, m_timexiazhu, m_timekaipai, m_timejiesuan;
	int m_unitmoney;
	float m_choushui;
};

#endif
