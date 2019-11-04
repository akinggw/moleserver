#ifndef _ROOM_H_INCLUDE_
#define _ROOM_H_INCLUDE_

#include <vector>
#include <string>
#include <map>

#include "Player.h"
#include "ccommon.h"

// 最大房间人数
#define ROOM_MAX_PLAYERS  500

/**
 * 房间类型
 */
enum RoomType
{
	ROOMTYPE_BISAI = 0,                  // 比赛房间
	ROOMTYPE_JIFEN,                      // 积分房间
	ROOMTYPE_JINBIN,                     // 金币房间
	ROOMTYPE_LIANXI,                     // 练习房间
	ROOMTYPE_NULL
};

/**
 * 房间当前状态
 */
enum RoomState
{
	ROOMSTATE_WAITING = 0,             // 等待中
	ROOMSTATE_GAMING,                  // 游戏中
	ROOMSTATE_NULL
};

//分数类型
enum enScoreKind
{
	enScoreKind_Win,					//胜
	enScoreKind_Lost,					//输
	enScoreKind_Draw,					//和
	enScoreKind_Flee,					//逃
	enScoreKind_Service,				//服务
	enScoreKind_Present,				//赠送
};

class Player;
class ServerLogicFrame;

class Room
{
public:
	/// 设置房间ID
	virtual void SetID(int id) = 0;
	/// 得到房间ID
	virtual int GetID(void) = 0;
	/// 设置房间所属游戏类型
	virtual void SetGameType(int type) = 0;
	/// 得到房间所属游戏类型
	virtual int GetGameType(void) = 0;
	/// 设置房间所属游戏
	virtual void SetGameId(int gameid) = 0;
	/// 得到房间所属游戏
	virtual int GetGameId(void) = 0;
	/// 设置房间标识
	virtual void SetRoomMarking(int marking) = 0;
	/// 得到房间标识
	virtual int GetRoomMarking(void) = 0;
	/// 设置游戏逻辑处理接口
	virtual void SetServerLogicFrame(ServerLogicFrame *frame) = 0;
	/// 设置游戏逻辑处理接口
	virtual ServerLogicFrame* GetServerLogicFrame(void) = 0;
	/// 设置房间类型
	virtual void SetRoomType(RoomType type) = 0;
	/// 得到房间类型
	virtual RoomType GetRoomType(void) = 0;
	/// 设置房间玩家人数
	virtual void SetMaxPlayer(int max) = 0;
	/// 得到房间玩家人数
	virtual int GetMaxPlayer(void) = 0;
	/// 设置房间状态
	virtual void SetRoomState(RoomState state) = 0;
	/// 得到房间状态
	virtual RoomState GetRoomState(void) = 0;
	/// 设置房间游戏类型
	virtual void SetRoomGameType(enStartMode type) = 0;
	/// 得到房间游戏类型
	virtual enStartMode GetRoomGameType(void) = 0;
	/// 设置房间名称
	virtual void SetName(std::string name) = 0;
	/// 得到房间名称
	virtual std::string GetName(void) = 0;
	/// 设置游戏抽水值
	virtual void SetChouShui(float cs) = 0;
	/// 得到游戏抽水值
	virtual float GetChouShui(void) = 0;
	/// 设置游戏单元积分
	virtual void SetGamePielement(int pielement) = 0;
	/// 得到游戏单元积分
	virtual int GetGamePielement(void) = 0;
	/// 设置游戏需要的最少金币
	virtual void SetLastMoney(unsigned int money) = 0;
	/// 得到游戏需要的最少金币
	virtual unsigned int GetLastMoney(void) = 0;
	/// 当前房间是否为空
	virtual bool IsEmpty(void) = 0;
	/// 得到指定索引的玩家
	virtual Player* GetPlayer(int index) = 0;
	/// 得到显示时指定索引的玩家(客户端使用)
	virtual int SwitchViewChairID(int chairIndex) = 0;
	/// 得到当前房间人数
	virtual int GetPlayerCount(void) = 0;
	/// 得到准备好状态下玩家的人数
	virtual int GetReadyPlayerCount(void) = 0;
	/// 设置当前房间的房主
	virtual void SetMaster(int playerId) = 0;
	/// 得到当前房间的房主，房主为当前第一准备开始游戏的玩家
	virtual int GetMaster(void) = 0;
	/// 得到房间中当前的玩家
	virtual int GetCurrentPlayer(void) = 0;
	/// 根据用户连接ID得到用户在房间中的ID
	virtual int GetPlayerRoomId(uint32 connId) = 0;
	/// 得到当前房间指定状态的玩家个数
	virtual int GetPlayerCount(PlayerState state) = 0;
	/// 设置当前房间所有玩家的状态
	virtual void SetAllPlayerState(PlayerState state) = 0;
	/// 设置指定玩家的状态
	virtual void SetPlayerState(int index,PlayerState state) = 0;
	/// 设置房间中当前玩家
	virtual void SetCurrentPlayer(int playerId) = 0;
	/// 得到当前房间中下一个玩家
	virtual int GetCurNextPlayer(void) = 0;
	/// 得到当前房间中下一个出牌玩家
	virtual int GetCurNextWorkPlayer(void) = 0;
	/// 设置房间进入密码
	virtual void SetEnterPassword(std::string pwd) = 0;
	/// 得到房间进入密码
	virtual std::string getEnterPassword(void) = 0;
	/// 设置房间进入金币范围
	virtual void SetEnterMoneyRect(int64 first,int64 second) = 0;
	/// 得到房间进入金币范围
	virtual void GetEnterMoneyRect(int64 *first,int64 *second) = 0;

	/// 检测房间是否已经满员
	virtual bool IsFull(void) = 0;
	/// 检测房间是否全部准备完毕
	virtual bool IsReadyed(void) = 0;
	/// 根据玩家连接id得到玩家在当前房间中的索引
	virtual int GetPlayerIndex(uint32 connId) = 0;
	/// 得到当前房间中掉线玩家人数
	virtual inline int GetLostPlayerCount(void) = 0;
	/// 清空当前房间数据
	virtual void Clear(void) = 0;
	/// 剔除指定的玩家，必须在游戏空闲时间才能剔除
	virtual bool EliminatePlayer(Player *pPlayer) = 0;
	/// 写入用户积分
	virtual bool WriteUserScore(int wChairID, int64 lScore, int64 lRevenue, enScoreKind ScoreKind,int64 pAgentmoney=0,bool isCumulativeResult=true,int64 pcurJetton=0,const char* pgametip="") = 0;
	/// 游戏结束时调用
	virtual void GameEnd(bool isupdateuserdata=true) = 0;
	/// 游戏开始是调用
	virtual void GameStart(void) = 0;

	/// 检测当前玩家输赢情况(2:表示玩家赢了；1：表示玩家输了；0：表示不输不赢随机）
	virtual int IsUserWin(int64 *result = NULL, int64 *sysMax = NULL, int64 *sysMin = NULL, bool isUpdate = false) = 0;
	/// 检测指定玩家的输赢情况(1:表示玩家输了；2：表示玩家赢了；0：表示不输不赢随机；-1：不控制）
	virtual int IsSuperPlayerWin(Player *pPlayer,int64 *curresult=NULL,int64 *decResult=NULL) = 0;

	/// 向指定的玩家发送数据
	virtual void SendTableMsg(int index,Json::Value &msg) = 0;
	/// 向指定的玩家发送旁观数据
	virtual void SendLookOnMes(int index,Json::Value &msg) = 0;

	/// 开始一个定时器
	virtual bool StartTimer(int timerId,int space) = 0;
	/// 关闭一个定时器
	virtual void StopTimer(int id) = 0;
	/// 关闭所有的定时器
	virtual void StopAllTimer(void) = 0;
	//virtual std::string WideCharConverToUtf8(CString& str)=NULL;

	/// 打印日志
	virtual void Room_Log(enLogLevel loglevel,std::string log) = 0;
	/// 得到房间配置参数
	virtual std::string GetRoomParameters(void) = 0;
};

#endif
