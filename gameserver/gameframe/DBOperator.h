#ifndef _DB_OPERATOR_H_INCLUDE_
#define _DB_OPERATOR_H_INCLUDE_

#include "../../include/molnet/Network.h"
#include "../gameserver_common.h"

#include <string>

using namespace network;

class CPlayer;

/**
 * 玩家用户信息
 */
struct UserDataStru
{
	UserDataStru():UserId(0),Money(0),BankMoney(0),Revenue(0),TotalResult(0),
		Level(0),Experience(0),TotalBureau(0),SBureau(0),FailBureau(0),RunawayBureau(0),
		SuccessRate(0),RunawayRate(0),sex(0),gType(0)
	{
		memset(realName,0,sizeof(realName));
	}

	int UserId;                   // 玩家ID
	int64 Money;                // 玩家金钱
	int64 BankMoney;            // 银行金钱
	int64 Revenue;               // 税收
	int64 TotalResult;           // 玩家总输赢
	int Level;                    // 玩家等级
	int Experience;               // 玩家经验值
	int TotalBureau;              // 玩家总的局数
	int SBureau;                  // 玩家胜利局数
	int FailBureau;               // 玩家失败局数
	int RunawayBureau;            // 逃跑次数
	float SuccessRate;            // 玩家胜利概率
	float RunawayRate;            // 玩家逃跑概率

	int sex;                      // 玩家性别
	int gType;                    // 玩家类型
	char realName[256];           // 玩家昵称
	char UserName[256];           // 玩家用户名
	char UserIP[256];             // 玩家IP
};

/**
 * 游戏房间结构
 */
struct tagGameRoom
{
    int gameid;
    int serverport;
    char serverip[256];
    char processname[256];
    char servername[256];
    int tablecount;
    int tableplayercount;
    int gamingtype;
    int64 lastmoney;
    int64 pielement;
    uint32 roomrevenue;
    int QueueGaming;
    int state;                     // 0-正常；
};

/**
 * 这个类用于游戏中所有的数据库操作
 */
class DBOperator : public Singleton<DBOperator>
{
public:
	/// 构造函数
	DBOperator();
	/// 析构函数
	~DBOperator();

	/// 初始数据库
	bool Initilize(std::string host,std::string user,std::string pass,std::string db,int port);
	/// 关闭数据库连接
	void Shutdown(void);
	/// 用于维护当前数据库连接
	void Update(void);

	/// 设置当前玩家游戏状态
	bool SetPlayerGameState(CPlayer *pPlayer);
	/// 插入一条游戏记录到数据库中
	bool InsertPlayerGameRecord(uint32 UserId,int64 Score,int64 Revenue,uint32 gameId,uint32 ServerID,
		std::string RoomName,int tableid,int chairid,int64 lastmoney,std::string gametip="",int64 pAgentmoney=0,int64 pcurJetton=0);
    /// 更新玩家信息
    bool UpdateUserData(Player *pPlayer);
    /// 更新指定玩家的身上金币数据
    bool UpdateUserMoney(CPlayer *pPlayer);
    /// 检测指定玩家是否在游戏中
    bool IsExistUserGaming(uint32 UserId,uint32 *serverid,int32 *roomid,int32 *chairid,uint32 *gametype);
    /// 根据用户ID得到用户的游戏数据
    bool GetUserData(unsigned int UserId,UserDataStru &UserData);
    /// 根据玩家名称和密码检测这个玩家是否存在
    uint32 IsExistUser(std::string name,std::string password);
    /// 得到游戏房间信息
    bool GetGameRoomInfo(uint32 pRoomId,tagGameRoom *ptagGameRoom);
    /// 更新游戏房间在线人数
    bool UpdateGameRoomOnlinePlayerCount(uint32 pRoomId,int currealplayercount,int currobotplayercount);

private:
	DataProvider *m_DataProvider;                               /**< 用于访问本地数据库 */
};

#define ServerDBOperator DBOperator::getSingleton()

#endif
