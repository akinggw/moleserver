#ifndef _C_ROOM_H_INCLUDE_
#define _C_ROOM_H_INCLUDE_

#include "../../include/molnet/Network.h"
#include "../../include/Common/serverlogicframe.h"

#include "../../include/molnet/netallocator/NedAllocatedObject.h"

#include "CRoom.h"
#include "CPlayer.h"

class Player;
class Room;

class CRoom : public Room , public NedAllocatedObject
{
public:
	CRoom(RoomType type=ROOMTYPE_NULL);
	virtual ~CRoom();

	/// 设置房间ID
	virtual void SetID(int id) { m_Id = id; }
	/// 得到房间ID
	virtual int GetID(void) { return m_Id; }
	/// 设置房间所属游戏类型
	virtual void SetGameType(int type) { m_GameType = type; }
	/// 得到房间所属游戏类型
	virtual int GetGameType(void) { return m_GameType; }
	/// 设置房间所属游戏
	virtual void SetGameId(int gameid) { m_gameid = gameid; }
	/// 得到房间所属游戏
	virtual int GetGameId(void) { return m_gameid; }
	/// 设置房间标识
	virtual void SetRoomMarking(int marking) { m_RoomMarking = marking; }
	/// 得到房间标识
	virtual int GetRoomMarking(void) { return m_RoomMarking; }
	/// 设置游戏逻辑处理接口
	virtual void SetServerLogicFrame(ServerLogicFrame *frame);
	/// 得到游戏逻辑处理接口
	virtual ServerLogicFrame* GetServerLogicFrame(void) { return m_ServerLogicFrame; }
	/// 设置房间类型
	virtual void SetRoomType(RoomType type) { m_RoomType = type; }
	/// 得到房间类型
	virtual RoomType GetRoomType(void) { return m_RoomType; }
	/// 设置房间玩家人数
	virtual void SetMaxPlayer(int max)
	{
		if(max > ROOM_MAX_PLAYERS)
			m_MaxCount = ROOM_MAX_PLAYERS;
		else
			m_MaxCount = max;
	}
	/// 得到房间玩家人数
	virtual int GetMaxPlayer(void) { return m_MaxCount; }
	/// 设置房间状态
	virtual void SetRoomState(RoomState state) { m_RoomState = state; }
	/// 得到房间状态
	virtual RoomState GetRoomState(void) { return m_RoomState; }
	/// 设置房间游戏类型
	virtual void SetRoomGameType(enStartMode type) { m_RoomGameType = type; }
	/// 得到房间游戏类型
	virtual enStartMode GetRoomGameType(void) { return m_RoomGameType; }
	/// 设置房间名称
	virtual void SetName(std::string name) { m_Name = name; }
	/// 得到房间名称
	virtual std::string GetName(void) { return m_Name; }
	/// 设置游戏抽水值
	virtual void SetChouShui(float cs) { m_RoomRevenue = cs; }
	/// 得到游戏抽水值
	virtual float GetChouShui(void) { return m_RoomRevenue; }
	/// 设置游戏单元积分
	virtual void SetGamePielement(int pielement) { m_Pielement = pielement; }
	/// 得到游戏单元积分
	virtual int GetGamePielement(void) { return m_Pielement; }
	/// 设置游戏需要的最少金币
	virtual void SetLastMoney(unsigned int money) { m_RoomLastMoney = money; }
	/// 得到游戏需要的最少金币
	virtual unsigned int GetLastMoney(void) { return m_RoomLastMoney; }
	/// 当前房间是否为空
	virtual bool IsEmpty(void);
	/// 得到指定索引的玩家
	virtual Player* GetPlayer(int index);
	/// 得到显示时指定索引的玩家(客户端使用)
	virtual int SwitchViewChairID(int chairIndex) { return 0; }
	/// 得到当前房间人数
	virtual int GetPlayerCount(void);
	/// 得到准备好状态下玩家的人数
	virtual int GetReadyPlayerCount(void);
	/// 设置当前房间的房主
	virtual void SetMaster(int playerId);
	/// 得到当前房间的房主，房主为当前第一准备开始游戏的玩家
	virtual inline int GetMaster(void) { return m_masterId; }
	/// 得到房间中当前的玩家
	virtual inline int GetCurrentPlayer(void) { return m_curPlayerId; }
	/// 根据用户连接ID得到用户在房间中的ID
	virtual int GetPlayerRoomId(uint32 connId);
	/// 得到当前房间指定状态的玩家个数
	virtual int GetPlayerCount(PlayerState state);
	/// 设置当前房间所有玩家的状态
	virtual void SetAllPlayerState(PlayerState state);
	/// 设置指定玩家的状态
	virtual void SetPlayerState(int index,PlayerState state);
	/// 设置房间中当前玩家
	virtual void SetCurrentPlayer(int playerId) { m_curPlayerId = playerId; }
	/// 得到当前房间中下一个玩家
	virtual int GetCurNextPlayer(void);
	/// 得到当前房间中下一个出牌玩家
	virtual int GetCurNextWorkPlayer(void);
	/// 设置房间进入密码
	virtual void SetEnterPassword(std::string pwd) { m_roomenterpwd = pwd; }
	/// 得到房间进入密码
	virtual std::string getEnterPassword(void) { return m_roomenterpwd; }
	/// 设置房间进入金币范围
	virtual void SetEnterMoneyRect(int64 first,int64 second) { m_roomentermoneyfirst = first; m_roomentermoneysecond = second; }
	/// 得到房间进入金币范围
	virtual void GetEnterMoneyRect(int64 *first,int64 *second) { *first = m_roomentermoneyfirst; *second = m_roomentermoneysecond; }

	/// 清除房间中所有的玩家
	void ClearAllPlayers(void);
	/// 添加一个玩家到当前房间中
	int AddPlayer(CPlayer *pPlayer,int index=-1);
	/// 添加一个旁观玩家到当前房间中
	int AddLookOnPlayer(CPlayer *pPlayer,int index);
	/// 从当前房间中清除指定的玩家
	void ClearPlayer(CPlayer *pPlayer);
	/// 从当前房间中删除指定的玩家
	void DeletePlayer(CPlayer *pPlayer);
	/// 从当前房间中清除指定房间ID的玩家
	void ClearPlayerById(int Id);
	/// 检测指定玩家是否存在当前房间
	bool IsExist(CPlayer *pPlayer);
	/// 检测指定的旁观玩家是否存在当前房间中
	bool IsExistLookOn(CPlayer *pPlayer);
	/// 检测房间是否已经满员
	virtual bool IsFull(void);
	/// 检测房间是否全部准备完毕
	virtual bool IsReadyed(void);
	/// 根据玩家连接id得到玩家在当前房间中的索引
	virtual int GetPlayerIndex(uint32 connId);
	/// 得到当前房间中掉线玩家人数
	virtual inline int GetLostPlayerCount(void) { return (int)m_lostPlayerList.size(); }
	/// 清除掉线玩家列表中玩家
	void ClearLostPlayerById(int id);

	/// 清空当前房间数据
	virtual void Clear(void);
	/// 剔除指定的玩家，必须在游戏空闲时间才能剔除
	virtual bool EliminatePlayer(Player *pPlayer);
	/// 写入用户积分
	virtual bool WriteUserScore(int wChairID, int64 lScore, int64 lRevenue, enScoreKind ScoreKind,int64 pAgentmoney=0,bool isCumulativeResult=true,int64 pcurJetton=0,const char* pgametip="");
	/// 游戏结束时调用
	virtual void GameEnd(bool isupdateuserdata=true);
	/// 游戏开始是调用
	virtual void GameStart(void);
    /// 更新指定玩家的数据
    virtual void UpdateUserScore(Player *pPlayer);

	/// 检测当前玩家输赢情况(2:表示玩家赢了；1：表示玩家输了；0：表示不输不赢随机）
	virtual int IsUserWin(int64 *result = NULL, int64 *sysMax = NULL, int64 *sysMin = NULL, bool isUpdate = false);
	/// 检测指定玩家的输赢情况(1:表示玩家输了；2：表示玩家赢了；0：表示不输不赢随机；-1：不控制）
	virtual int IsSuperPlayerWin(Player *pPlayer,int64 *curresult=NULL,int64 *decResult=NULL);

	/// 向指定的玩家发送数据
	virtual void SendTableMsg(int index,Json::Value &msg);
	/// 向指定的玩家发送旁观数据
	virtual void SendLookOnMes(int index,Json::Value &msg);

	/// 开始一个定时器
	virtual bool StartTimer(int timerId,int space);
	/// 关闭一个定时器
	virtual void StopTimer(int id);
	/// 关闭所有的定时器
	virtual void StopAllTimer(void);

	/// 打印日志
	virtual void Room_Log(enLogLevel loglevel,std::string log);
	/// 得到房间配置参数
	virtual std::string GetRoomParameters(void);

	///时间事件
	bool OnEventTimer(DWORD dwTimerID, DWORD wBindParam);

	/// 用于处理接收到的网络消息
	virtual void OnProcessNetMes(uint32 connId,Json::Value &mes);

	/// 用于处理用户准备后的消息
	void OnProcessPlayerReadyMes(int playerId);
	/// 用于处理用户开始游戏开始消息
	void OnProcessPlayerGameStartMes(void);
	/// 用于处理用户进入游戏房间后的消息
	void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
	/// 处理用户进入房间消息
	void OnProcessEnterRoomMsg(int playerId);
	/// 处理用户离开房间消息
	void OnProcessLeaveRoomMsg(int playerId);
	/// 处理用户断线消息
	void OnProcessOfflineRoomMes(int playerId);
	/// 处理用户断线重连消息
	void OnProcessReEnterRoomMes(int playerId);

	/// 是否继续游戏
	void OnProcessContinueGaming(void);

private:
	/// 向所有玩家发送数据
	void SendAllPlayerMsg(Json::Value &sg);
	/// 向当前房间所有玩家发送用户离开服务器消息
	void AllPlayerLeveaServer(void);
	/// 向所有旁观玩家发送数据
	void SendAllLookOnPlayerMsg(Json::Value &msg);
	/// 检测指定的索引是否在掉线玩家列表中
	bool IsExistLostPlayer(int index);
	/// 删除所有的定时器
	void DeleteAllTimer(void);

	//时间组件
protected:
	CTimerItemArray					m_TimerItemActive;					//活动数组
	Mutex                           m_TimerLock;                        /**< 用于保护玩家中的定时器 */

private:
	int m_Id;                   /**< 房间ID */
	int m_GameType;             /**< 房间所属的游戏类型 */
	int m_gameid;               /**< 房间所属游戏 */
	int m_RoomMarking;          /**< 房间标识，一般用它的端口号表示 */
	std::string m_Name;         /**< 房间名称 */
	int m_MaxCount;             /**< 房间中最大玩家个数 */
	RoomType m_RoomType;        /**< 房间类型 */
	RoomState m_RoomState;      /**< 房间当前状态 */
	enStartMode m_RoomGameType; /**< 房间游戏类型 */
	float m_RoomRevenue;        /**< 房间税收 */
	int m_RoomLastMoney;        /**< 房间最小金币 */
	int m_Pielement;            /**< 房间单元积分 */
	//int64 m_JackPot;            /**< 奖池 */

	std::string m_roomenterpwd; /**< 房间进入密码 */
	int64 m_roomentermoneyfirst,m_roomentermoneysecond;  /**< 房间进入金币范围 */

	int m_masterId;             /**< 当前房主ID */
	int m_curPlayerId;          /**< 当前玩家ID */

	CPlayer *m_PlayerList[ROOM_MAX_PLAYERS];       /**< 房间中游戏玩家列表 */
	std::map<int,std::vector<CPlayer*> > m_LookonPlayerList;       /**< 房间中旁观玩家列表 */
	std::vector<int> m_lostPlayerList;            /**< 房间中掉线玩家列表 */

	Mutex m_playersLock;                          /**< 用于玩家操作 */

	ServerLogicFrame* m_ServerLogicFrame;               /**< 用于处理游戏逻辑 */
};

#endif
