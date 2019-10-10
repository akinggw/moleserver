#ifndef _C_PLAYER_H_INCLUDE_
#define _C_PLAYER_H_INCLUDE_

#include <string>

#include "../gameserver_common.h"

#include "../../include/molnet/Network.h"
#include "../../include/Common/Player.h"
#include "../../include/Common/Robotlogicframe.h"

#include "../../include/molnet/netallocator/NedAllocatedObject.h"

//class CPlayer;
//class RobotLogicFrame;

class CPlayer : public Player , public NedAllocatedObject
{
public:
	CPlayer(PlayerType type);
	CPlayer(int id,uint32 conid);
	~CPlayer();

	/// 设置玩家ID
	virtual void SetID(int id) { m_Id = id; }
	/// 得到玩家ID
	virtual int GetID(void) { return m_Id; }
	/// 设置玩家连接ID
	virtual void SetConnectID(uint32 id) { m_ConnectId = id; }
	/// 得到玩家连接ID
	virtual uint32 GetConnectID(void) { return m_ConnectId; }
	/// 设置玩家所在的房间ID
	virtual void SetRoomId(int id) { m_roomId = id; }
	/// 得到玩家所在的房间ID
	virtual int GetRoomId(void) { return m_roomId; }
	/// 设置玩家类型
	virtual void SetType(PlayerType type) { m_PlayerType = type; }
	/// 得到玩家类型
	virtual PlayerType GetType(void) { return m_PlayerType; }
	/// 设置玩家设备类型
	virtual void SetDeviceType(PlayerDeviceType type) { m_PlayerDeviceType = type; }
	/// 得到玩家设备类型
	virtual PlayerDeviceType GetDeviceType(void) { return m_PlayerDeviceType; }
	/// 设置玩家在房间中的索引
	virtual void SetChairIndex(int index) { m_ChairIndex = index; }
	/// 得到玩家在房间中的索引
	virtual int GetChairIndex(void) { return m_ChairIndex; }
	/// 设置玩家名称
	virtual void SetName(std::string name) { m_Name = name; }
	/// 得到玩家名称
	virtual std::string GetName(void) { return m_Name; }
	/// 得到玩家状态
	virtual PlayerState GetState(void) { return m_PlayerState; }
	/// 设置玩家状态
	virtual void SetState(PlayerState state)
	{
		m_PlayerState = state;
	}
	/// 设置是否旁观
	virtual void SetLookOn(bool isLook) { m_isLookOn = isLook; }
	/// 得到是否旁观
	virtual bool IsLookOn(void) { return m_isLookOn; }

	/// 设置玩家金钱数量
	virtual void SetMoney(int64 money) { m_Money = money; }
	/// 得到玩家金币数量
	virtual int64 GetMoney(void) { return m_Money; }
	/// 设置玩家银行金钱数量
	virtual void SetBankMoney(int64 money) { m_BankMoney = money; }
	/// 得到玩家银行金币数量
	virtual int64 GetBankMoney(void) { return m_BankMoney; }
	/// 设置玩家税收
	virtual void SetRevenue(int64 revenue) { m_Revenue = revenue; }
	/// 得到玩家税收
	virtual int64 GetRevenue(void) { return m_Revenue; }
	/// 设置玩家输赢值
	virtual void SetTotalResult(int64 result) { m_TotalResult = result; }
	/// 得到玩家输赢值
	virtual int64 GetTotalResult(void) { return m_TotalResult; }
	/// 设置玩家等级
	virtual void SetLevel(int level) { m_level = level; }
	/// 得到玩家等级
	virtual int GetLevel(void) { return m_level; }
	/// 设置玩家经验值
	virtual void SetExperience(unsigned int exp) { m_experience = exp; }
	/// 得到玩家经验值
	virtual unsigned int GetExperience(void) { return m_experience; }
	/// 设置玩家总局
	virtual void SetTotalBureau(int bureau) { m_totalbureau = bureau; }
	/// 得到玩家总局
	virtual int GetTotalBureau(void) { return m_totalbureau; }
	/// 设置玩家赢局
	virtual void SetSuccessBureau(int bureau) { m_successbureau = bureau; }
	/// 得到玩家赢局
	virtual int GetSuccessBureau(void) { return m_successbureau; }
	/// 设置玩家输局
	virtual void SetFailBureau(int bureau) { m_failbureau = bureau; }
	/// 得到玩家输局
	virtual int GetFailBureau(void) { return m_failbureau; }
	/// 设置逃跑次数
	virtual void SetRunawayBureau(int bureau) { m_RunawayBureau = bureau; }
	/// 得到逃跑次数
	virtual int GetRunawayBureau(void) { return m_RunawayBureau; }
	/// 设置玩家胜率
	virtual void SetSuccessRate(float rate) { m_successrate = rate; }
	/// 得到玩家胜率
	virtual float GetSuccessRate(void) { return m_successrate; }
	/// 设置玩家逃跑率
	virtual void SetRunawayrate(float rate) { m_runawayrate = rate; }
	/// 得到玩家逃跑率
	virtual float GetRunawayrate(void) { return m_runawayrate; }
	/// 设置玩家性别
	virtual void SetSex(int psex) { sex = psex; }
	/// 得到玩家性别
	virtual int GetSex(void) { return sex; }
	/// 设置玩家真实姓名
	virtual void SetRealName(std::string rn) { realname = rn; }
	/// 得到玩家真实姓名
	virtual std::string GetRealName(void) { return realname; }
	/// 设置玩家系统类型
	virtual void SetSysType(int type) { gtype = type; }
	/// 得到玩家系统类型
	virtual int GetSysType(void) { return gtype; }
	/// 设置IP地址
	virtual void SetLoginIP(uint32 ip) { ipaddress = ip; }
	/// 得到IP地址
	virtual uint32 GetLoginIP(void) { return ipaddress; }

	/// 设置玩家当前所在游戏ID
	virtual void setCurGameID(uint32 gameid) { m_CurGameType = gameid; }
	/// 得到玩家当前所在游戏ID
	virtual uint32 getCurGameID(void) { return m_CurGameType; }
	/// 设置玩家当前所在桌子ID
	virtual void setCurTableIndex(int32 tableindex) { m_CurTableIndex = tableindex; }
	/// 得到玩家当前所在桌子ID
	virtual int32 getCurTableIndex(void) { return m_CurTableIndex; }
	/// 得到玩家当前所在椅子ID
	virtual void setCurChairIndex(int32 chairindex) { m_CurChairIndex = chairindex; }
	/// 得到玩家当前所在椅子ID
	virtual int32 getCurChairIndex(void) { return m_CurChairIndex; }
	/// 设置玩家当前所在服务器ID
	virtual void setCurServerId(uint32 serverid) { m_CurServerPort = serverid; }
	/// 得到玩家当前所在服务器ID
	virtual uint32 getCurServerId(void) { return m_CurServerPort; }
	/// 设置玩家当前是否在游戏中
	virtual void setCurGamingState(bool state) { m_CurGamingState = state; }
	/// 得到玩家当前是否在游戏中
	virtual bool getCurGamingState(void) { return m_CurGamingState; }
	/// 设置是否可以抽奖
	virtual void setIsChouJiang(bool iscj) { m_isChoujiang = iscj; }
	/// 得到是否可以抽奖
	virtual bool IsChouJiang(void) { return m_isChoujiang; }

	/// 设置房间进入密码
	inline void SetEnterPassword(std::string pwd) { m_roomenterpwd = pwd; }
	/// 得到房间进入密码
	inline std::string getEnterPassword(void) { return m_roomenterpwd; }
	/// 设置房间进入金币范围
	inline void SetEnterMoneyRect(int64 first,int64 second) { m_roomentermoneyfirst = first; m_roomentermoneysecond = second; }
	/// 得到房间进入金币范围
	inline void GetEnterMoneyRect(int64 *first,int64 *second) { *first = m_roomentermoneyfirst; *second = m_roomentermoneysecond; }


	/// 设置准备时间
	inline void SetReadyTime(DWORD time) { m_RealyTime = time; }
	/// 得到准备时间
	inline DWORD GetReadyTime(void) { return m_RealyTime; }

	/// 开始一个定时器
	virtual bool StartTimer(int timerId,int space);
	/// 关闭一个定时器
	virtual void StopTimer(int id);
	/// 关闭所有的定时器
	virtual void StopAllTimer(void);

	/// 发送游戏数据
	virtual void SendGameMsg(Json::Value &msg);
	/// 发送准备消息
	virtual void SendReadyMsg(void);

	/// 设置机器人游戏逻辑处理接口
	void SetRobotLogicFrame(RobotLogicFrame *frame);
	/// 得到机器人游戏逻辑处理接口
	RobotLogicFrame* GetRobotLogicFrame(void) { return m_RobotLogicFrame; }
	///时间事件
	bool OnEventTimer(DWORD dwTimerID, DWORD wBindParam);

	/// 设置当前游戏房间
	void SetCurGameRoom(Room* pRoom);
	/// 设置掉线后的连接ID
	inline void SetOffLineConnectId(uint32 connid) { m_offlineConnectId = connid; }
	/// 得到掉线后的连接ID
	inline uint32 GetOffLineConnectId(void) { return m_offlineConnectId; }

	/// 设置当前游戏结果输赢值
	inline void setCurrentGamingResult(int64 presult) { m_curGamingResult = presult; }
	/// 得到当前游戏结果输赢值
	inline int64 getCurrentGamingResult(void) { return m_curGamingResult; }

private:
	/// 删除所有的定时器
	void DeleteAllTimer(void);

	//时间组件
protected:
	CTimerItemArray					m_TimerItemActive;					//活动数组
	Mutex                           m_TimerLock;                        /**< 用于保护玩家中的定时器 */

private:
	int m_Id;         /**< 玩家ID */
	uint32 m_ConnectId;  /**< 玩家的连接ID */
	uint32 ipaddress;    /**< 玩家IP */
	uint32 m_offlineConnectId;  /**< 掉线后的连接ID */
	int m_roomId;     /**< 玩家所在的房间ID */
	int m_ChairIndex;   /**< 用户在房间中的索引 */
	PlayerState m_PlayerState;       /**< 玩家当前状态 */
	PlayerType m_PlayerType;           /**< 玩家类型 */
	PlayerDeviceType m_PlayerDeviceType; /**< 玩家设备类型 */
	bool m_isLookOn;                   /**< 是否为旁观 */

	std::string m_Name;  /**< 玩家的名称 */

	std::string m_roomenterpwd; /**< 房间进入密码 */
	int64 m_roomentermoneyfirst,m_roomentermoneysecond;  /**< 房间进入金币范围 */

	int64 m_Money;              /**< 玩家金钱数量 */
	int64 m_BankMoney;                /**< 银行金钱数量 */
	int64 m_TotalResult;              /**< 玩家总的输赢值 */
	int64 m_Revenue;                  /**< 玩家税收 */
	int m_level;                       /**< 玩家等级 */
	unsigned int m_experience;         /**< 玩家经验值 */
	std::string m_useravatar;         /**< 用户头像 */
	int m_totalbureau;                 /**< 玩家总局 */
	int m_successbureau;               /**< 玩家赢局 */
	int m_RunawayBureau;			   /**< 玩家逃跑次数 */
	int m_failbureau;                  /**< 玩家输局 */
	float m_successrate;               /**< 玩家胜率 */
	float m_runawayrate;               /**< 玩家逃跑率 */
	int sex;                      //玩家性别
	std::string realname;           //真实姓名
	int gtype;                    //玩家类型

	int64 m_curGamingResult;                         /**< 当前游戏输赢值 */

	uint32 m_CurGameType;                            /**< 当前所在游戏类型 */
	uint32 m_CurServerPort;                          /**< 当前所在游戏服务器端口 */
	int32 m_CurTableIndex;                          /**< 当前所在游戏桌子ID */
	int32 m_CurChairIndex;                          /**< 当前所在游戏椅子ID */
	bool m_CurGamingState;                          /**< 当前玩家游戏状态 */
	bool m_IsMatchSignUp;                           /**< 是否报名当前比赛 */
	bool m_isChoujiang;                                 /**< 是否可以游戏后抽奖 */

	DWORD           m_RealyTime;                  /**< 准备时间 */

	RobotLogicFrame *m_RobotLogicFrame;             /**< 机器人处理框架 */
};

#endif
