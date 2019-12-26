#ifndef _PLAYER_H_INCLUDE_
#define _PLAYER_H_INCLUDE_

#include <string>

#include "ccommon.h"

/**
 * 玩家状态
 */
enum PlayerState
{
	PLAYERSTATE_NORAML = 0,             // 正常状态
	PLAYERSTATE_READY,                  // 准备状态
	PLAYERSTATE_GAMING,                 // 游戏中状态
	PLAYERSTATE_LOSTLINE,               // 掉线状态
	PLAYERSTATE_QUEUE                   // 排队状态
};

/**
 * 玩家类型
 */
enum PlayerType
{
	PLAYERTYPE_NORMAL = 0,              // 普通用户
	PLAYERTYPE_ROBOT                    // 机器人用户
};

/**
 * 玩家设备类型
 */
enum PlayerDeviceType
{
	PLAYERDEVICETYPE_PC = 0,            // PC用户
	PLAYERDEVICETYPE_ANDROID,           // 安卓用户
	PLAYERDEVICETYPE_IOS,               // ios用户
	PLAYERDEVICETYPE_FLASH,             // flash用户
	PLAYERDEVICETYPE_NULL
};

class Player
{
public:
	/// 设置玩家ID
	virtual void SetID(int id) = 0;
	/// 得到玩家ID
	virtual int GetID(void) = 0;
	/// 设置玩家连接ID
	virtual void SetConnectID(uint32 id) = 0;
	/// 得到玩家连接ID
	virtual uint32 GetConnectID(void) = 0;
	/// 设置玩家所在的房间ID
	virtual void SetRoomId(int id) = 0;
	/// 得到玩家所在的房间ID
	virtual int GetRoomId(void) = 0;
	/// 设置玩家类型
	virtual void SetType(PlayerType type) = 0;
	/// 得到玩家类型
	virtual PlayerType GetType(void) = 0;
	/// 设置玩家设备类型
	virtual void SetDeviceType(PlayerDeviceType type) = 0;
	/// 得到玩家设备类型
	virtual PlayerDeviceType GetDeviceType(void) = 0;
	/// 设置玩家在房间中的索引
	virtual void SetChairIndex(int index) = 0;
	/// 得到玩家在房间中的索引
	virtual int GetChairIndex(void) = 0;
	/// 设置玩家名称
	virtual void SetName(std::string name) = 0;
	/// 得到玩家名称
	virtual std::string GetName(void) = 0;
	/// 得到玩家状态
	virtual PlayerState GetState(void) = 0;
	/// 设置玩家状态
	virtual void SetState(PlayerState state) = 0;
	/// 设置是否旁观
	virtual void SetLookOn(bool isLook) = 0;
	/// 得到是否旁观
	virtual bool IsLookOn(void) = 0;

	/// 设置玩家金钱数量
	virtual void SetMoney(int64 money) = 0;
	/// 得到玩家金币数量
	virtual int64 GetMoney(void) = 0;
	/// 设置玩家银行金钱数量
	virtual void SetBankMoney(int64 money) = 0;
	/// 得到玩家银行金币数量
	virtual int64 GetBankMoney(void) = 0;
	/// 设置玩家税收
	virtual void SetRevenue(int64 revenue) = 0;
	/// 得到玩家税收
	virtual int64 GetRevenue(void) = 0;
	/// 设置玩家输赢值
	virtual void SetTotalResult(int64 result) = 0;
	/// 得到玩家输赢值
	virtual int64 GetTotalResult(void) = 0;
	/// 设置玩家等级
	virtual void SetLevel(int level) = 0;
	/// 得到玩家等级
	virtual int GetLevel(void) = 0;
	/// 设置玩家经验值
	virtual void SetExperience(unsigned int exp) = 0;
	/// 得到玩家经验值
	virtual unsigned int GetExperience(void) = 0;
	/// 设置玩家总局
	virtual void SetTotalBureau(int bureau) = 0;
	/// 得到玩家总局
	virtual int GetTotalBureau(void) = 0;
	/// 设置玩家赢局
	virtual void SetSuccessBureau(int bureau) = 0;
	/// 得到玩家赢局
	virtual int GetSuccessBureau(void) = 0;
	/// 设置玩家输局
	virtual void SetFailBureau(int bureau) = 0;
	/// 得到玩家输局
	virtual int GetFailBureau(void) = 0;
	/// 设置逃跑次数
	virtual void SetRunawayBureau(int bureau) = 0;
	/// 得到逃跑次数
	virtual int GetRunawayBureau(void) = 0;
	/// 设置玩家胜率
	virtual void SetSuccessRate(float rate) = 0;
	/// 得到玩家胜率
	virtual float GetSuccessRate(void) = 0;
	/// 设置玩家逃跑率
	virtual void SetRunawayrate(float rate) = 0;
	/// 得到玩家逃跑率
	virtual float GetRunawayrate(void) = 0;
	/// 设置玩家性别
	virtual void SetSex(int psex) = 0;
	/// 得到玩家性别
	virtual int GetSex(void) = 0;
	/// 设置玩家真实姓名
	virtual void SetRealName(std::string rn) = 0;
	/// 得到玩家真实姓名
	virtual std::string GetRealName(void) = 0;
	/// 设置玩家系统类型
	virtual void SetSysType(int type) = 0;
	/// 得到玩家系统类型
	virtual int GetSysType(void) = 0;
	/// 设置IP地址
	virtual void SetLoginIP(uint32 ip) = 0;
	/// 得到IP地址
	virtual uint32 GetLoginIP(void) = 0;

	/// 设置玩家当前所在游戏ID
	virtual void setCurGameID(uint32 gameid) = 0;
	/// 得到玩家当前所在游戏ID
	virtual uint32 getCurGameID(void) = 0;
	/// 设置玩家当前所在桌子ID
	virtual void setCurTableIndex(int32 tableindex) = 0;
	/// 得到玩家当前所在桌子ID
	virtual int32 getCurTableIndex(void) = 0;
	/// 得到玩家当前所在椅子ID
	virtual void setCurChairIndex(int32 chairindex) = 0;
	/// 得到玩家当前所在椅子ID
	virtual int32 getCurChairIndex(void) = 0;
	/// 设置玩家当前所在服务器ID
	virtual void setCurServerId(uint32 serverid) = 0;
	/// 得到玩家当前所在服务器ID
	virtual uint32 getCurServerId(void) = 0;
	/// 设置玩家当前是否在游戏中
	virtual void setCurGamingState(bool state) = 0;
	/// 得到玩家当前是否在游戏中
	virtual bool getCurGamingState(void) = 0;
	/// 设置是否可以抽奖
	virtual void setIsChouJiang(bool iscj) = 0;
	/// 得到是否可以抽奖
	virtual bool IsChouJiang(void) = 0;

	/// 开始一个定时器
	virtual bool StartTimer(int timerId,int space) = 0;
	/// 关闭一个定时器
	virtual void StopTimer(int id) = 0;
	/// 关闭所有的定时器
	virtual void StopAllTimer(void) = 0;

	/// 发送游戏数据(此函数只限于机器人使用，正常用户使用请使用房间消息发送函数)
	virtual void SendGameMsg(Json::Value &msg) = 0;
	/// 发送准备消息
	virtual void SendReadyMsg(void) = 0;

	/// 是否处于比赛中
	virtual bool IsMatching(void) = 0;
	/// 设置是否比赛中
	virtual void SetMatching(bool im) = 0;
	/// 是否比赛报名
	virtual bool IsMatchSignUp(void) = 0;
	/// 设置是否比赛报名
	virtual void SetMatchSignUp(bool ms) = 0;
	/// 比赛局数
	virtual void SetMatchCount(int count) = 0;
	/// 得到比赛局数
	virtual int GetMatchCount(void) = 0;
	/// 比赛总局数
	virtual void SetTotalMatchCount(int count) = 0;
	/// 得到比赛总局数
	virtual int GetTotalMatchCount(void) = 0;
	/// 比赛分数
	virtual void SetMatchResult(int64 result) = 0;
	/// 得到比赛分数
	virtual int64 GetMatchResult(void) = 0;
	/// 获取指定玩家比赛中排名
	virtual void GetPlayerRanking(void) = 0;
	/// 设置比赛房间索引
	virtual void SetMatchRoomIndex(int index) = 0;
	/// 得到比赛房间索引
	virtual int GetMatchRoomIndex(void) = 0;
	/// 是否比赛中掉线
	virtual bool IsMatchingLostLine(void) = 0;
	/// 设置是否比赛中掉线
	virtual void SetMatchingLostLine(bool mlo) = 0;
};

#endif
