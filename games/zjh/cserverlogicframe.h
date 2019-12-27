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
	Room *m_g_GameRoom;                                  /**< 游戏房间 */

	CzjhGameLogic				m_GameLogic;			//游戏逻辑

	//状态变量
private:
	PlayStates					m_PlayState[IDD_MAX_ROOM_USERCOUNT];							//是否在玩
	bool						m_bLook[IDD_MAX_ROOM_USERCOUNT];								//是否看牌
	bool						m_bCanThan;														//是否可以比牌
	int							m_nCanThanUser;

	//游戏变量
private:
	WORD						m_wCardData[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_HAND_COUNT];		//玩家手牌
	std::map<uint32,std::string>m_strUserName;							//玩家名字
	int64						m_nUserScore[IDD_MAX_ROOM_USERCOUNT];							//玩家金币（用于判断什么时候必须比牌）
	int64						m_nAddScore[IDD_MAX_ROOM_USERCOUNT];							//当前所下金额
	int							m_nBasicAddId;													//最低下注
	int64						m_nBasicScore;													//单位下注

	//比牌变量
	int							m_nThanUser[IDD_MAX_ROOM_USERCOUNT];

	//运行变量
	int							m_nBankUser;													//庄家
	int							m_nCurrentUser;													//当前玩家
	int							m_nAllUserNum;													//当前玩家数

	bool						m_bGameOver;
	int64						m_nWinScore[IDD_MAX_ROOM_USERCOUNT];
	bool						m_bOffLine[IDD_MAX_ROOM_USERCOUNT];

private:
	//加注
	void OnUserAddScore(int playerID, int addId);
	//看牌
	void OnUserWatchCard(int playerID);
	//比牌
	void OnUserThanCard(int player1,int player2);
	//放弃
	void OnUserGiveUp(int playerID);
	//游戏结束
	void OnUserGameEnd(bool isGiveUp);
	//发送可以比牌消息
	void SendCanThan();

private:
	//是否必须比牌
	bool isMustThan(int playerID);
	//获取下一个操作玩家
	int GetNextUser(int PlayerID);

private:
	//清除数据
	void ClearData();
	//测试
	bool ForTest();
};

#endif
