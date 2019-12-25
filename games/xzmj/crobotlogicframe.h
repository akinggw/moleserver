#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "CmdHelper.h"

#define TIMER_ROBOT_OUT				1
#define TIMER_ROBOT_PENG			2
#define TIMER_ROBOT_GANG			3
#define TIMER_ROBOT_CHI				4
#define TIMER_ROBOT_TING			5
#define TIMER_ROBOT_HU				6
#define TIMER_ROBOT_GIVEUP			7
#define TIMER_ROBOT_READY			8
#define TIMER_ROBOT_START			9

class CRobotLogicFrame : public RobotLogicFrame
{
public:
	/// 构造函数
	CRobotLogicFrame();
	/// 析构函数
	~CRobotLogicFrame();

	/// 设置当前玩家
	virtual void SetGamePlayer(Player* pPlayer,Room* pRoom) { m_g_myself = pPlayer; m_g_myselfRoom = pRoom;}

	/// 用于处理用户准备后的消息
	virtual void OnProcessPlayerReadyMes(int playerId);
	/// 用于处理用户开始游戏开始消息
	virtual void OnProcessPlayerGameStartMes();
	/// 用于处理用户结束游戏消息
	virtual void OnProcessPlayerGameOverMes(void);
	/// 用于处理用户进入游戏房间后的消息
	virtual void OnProcessPlayerRoomMes(Json::Value &mes);
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

public:
	///发牌消息
	void OnDealMJ(CMD_S_DealMJ cmdsDealMJ);

	///出牌消息
	void OnOutMJ(CMD_S_OutMJ cmdsOutMJ);

	///摸牌消息
	void OnTouchMJ(CMD_S_TouchMJ cmdsTouchMJ);

	///碰牌消息
	void OnPengMJ(CMD_S_PengMJ cmdsPengMJ);

	///杠牌消息
	void OnGangMJ(CMD_S_GangMJ cmdsGangMJ);

	///放弃消息
	void OnGiveUp(CMD_S_GiveUp cmdsGiveUp);

	///胡牌消息
	void OnHu(CMD_S_HU cmdsHu);

	///机器人逻辑，找出一张最差的牌打出去
	MJBYTE GetUselessMj();

	///重置
	void ReSet();

	int GetRestMjCount(MJBYTE bMj);

//自定义成员变量  麻将权重，凑成结构的概率
private:
	typedef struct tagMjWeight
	{
		MJBYTE		bLessMj;						//牌
		int			bMinCount;						//凑成结构需要的牌里最小个数（除开已出现的，凑成刻子和顺子之和）这个值越大，说明越可能凑成胡牌结构
	}MjWeight, *PMjWeight;

private:
	static const MJBYTE				bIsHuMJ[MAX_ORG_MJ/GAME_PLAYER];					/*!< 原始的麻将数据 */
	MJBlock							m_bHUMJ[GAME_PLAYER][MAX_BLOCK_COUNT];				/*!< 亮出的牌块(吃碰杠的牌) */
	int								m_iHUMJCount[GAME_PLAYER];							/*!< 亮出牌块的数量 */
	MJBYTE							m_bHandMJ[GAME_PLAYER][MAX_HAND_MJ];				/*!< 手中的牌 */
	int								m_iHandMJCount[GAME_PLAYER];						/*!< 手中的牌的数量 */
	MJBYTE							m_bOutMJ[GAME_PLAYER][MAX_OUT_MJ];					/*!< 已经出的牌 */
	int								m_iOutMJCount[GAME_PLAYER];

	MJBYTE							m_bExistMjCount[MAX_ARRAY_MJ];						/*!< 已出现麻将的个数 */
	enOperationGangType				enGangType;											/*!< 杠牌类型 */

	MJBYTE							iOutMJ;												/*!< 打出的牌 */

	int								m_iOldOperUser;										/*!< 上次当前玩家 */
	int								m_iOperUser;										/*!< 当前操作玩家 */

	int								m_iSeOut;											/*!< 打出哪一色 */
	MJFLAG							m_BankerOper;										/*!< 开局玩家操作 */
	MJBYTE							m_GangMJ;
	enum time_range
	{
		MIN_TIME = 2,
		MAX_TIME = 5,
	};

	CMahJongLogic				   *m_pGameLogic;

private:
	Player *m_g_myself;
    Room *m_g_myselfRoom;
};

#endif
