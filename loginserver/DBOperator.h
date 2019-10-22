#ifndef _DB_OPERATOR_H_INCLUDE_
#define _DB_OPERATOR_H_INCLUDE_

#include "../include/molnet/Network.h"
using namespace network;

#include <string>

/**
 * 玩家用户信息
 */
struct MemberDataStru
{
	uint32 uid;                   //玩家ID
	int gtype;                    //玩家类型
	char username[256];           //玩家名称
	char password[256];           //玩家密码
	char bankpassword[256];       //玩家银行密码
	char email[256];              //玩家email
	int sex;                      //玩家性别
	char realname[256];           //真实姓名
	char homeplace[256];          //主页
	char telephone[256];          //电话
	char QQ[256];                 //QQ
	char ipaddress[256];          //IP地址
	uint32 createtime;            //建立时间
	uint32 lastlogintime;         //最后登录时间
	char useravatar[256];         //用户头像
	int state;                      //是否被封号

	int64 money;                  //手上的钱
	int64 bankmoney;              //银行的钱
	uint32 level;                 //等级
	uint32 experience;            //经验

	int glockmachine;             //是否锁机
	char machinecode[256];        //机器码
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
    int state;                     // 0-正常；
};

/**
 * 游戏信息结构
 */
struct GameDataInfo
{
	GameDataInfo():GameID(0),GameType(0),MaxVersion(0),GameState(0),showindex(0)
	{
		memset(GameName,0,sizeof(GameName));
		memset(ProcessName,0,sizeof(ProcessName));
		memset(GameLogo,0,sizeof(GameLogo));
	}
	GameDataInfo(int gid,const char *gn,int gt,int mv,char *pn,char *gl,int ps,int si)
		: GameID(gid),GameType(gt),MaxVersion(mv),GameState(ps),showindex(si)
	{
		if(gn) strncpy(GameName,gn,sizeof(GameName));
		if(pn) strncpy(ProcessName,pn,sizeof(ProcessName));
		if(gl) strncpy(GameLogo,gl,sizeof(GameLogo));
	}

	int GameID;                   // 游戏ID
	char GameName[128];           // 游戏名称
	int GameType;                 // 游戏类型
	int MaxVersion;               // 版本号
	char ProcessName[128];        // 游戏进程名称
	char GameLogo[128];           // 游戏logo
	int GameState;                // 游戏状态：0-正常；1-维护；2-新；3-热,4-赛
	int showindex;                // 显示顺序
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

	/// 根据玩家名称和密码检测这个玩家是否存在
	unsigned int IsExistUser(std::string name,std::string password);
	/// 根据玩家名称检测这个玩家是否存在
	unsigned int IsExistUser(std::string name);
    /// 得到当前注册玩家数量
    unsigned int GetTotalUserCount(void);
    /// 锁定指定玩家的机器
    bool LockMachineByUser(uint32 UserId,int operType);
	/// 根据玩家ID更新玩家密码
	bool UpdateUserPassword(uint32 UserId,std::string pwd);
	/// 根据用户ID得到用户的游戏数据
	bool GetUserData(uint32 UserId,MemberDataStru &UserData);
	/// 根据用户账号得到用户ID
	bool GetUserID(std::string username,uint32 *userId);
	/// 更改指定玩家的银行密码
	bool UpdateUserBankPassword(uint32 UserID,std::string decPWD);
	/// 更改指定玩家的登陆密码
	bool UpdateUserLoginPassword(uint32 UserId,std::string decPWD);
	/// 更新用户最近登录IP和最近登录时间
	bool UpdatePlayerLastLogin(uint32 UserID,std::string ipaddress,std::string machinecode);
	/// 更新指定玩家的信息
	bool UpdateUserInfo(uint32 UserID,std::string nickname,std::string email,std::string telephone,std::string qq,std::string useravatar,int sex);
	/// 注册用户
	int32 RegisterGameUser(std::string name,std::string password,std::string email,
		int sex,std::string realname,std::string telephone,std::string AvatorIndex,std::string Referrer,std::string ipaddress,std::string cardnumber);
    /// 得到当前在线的所有服务器
    bool GetOnlineGameRooms(std::vector<tagGameRoom> &pgamerooms);
    /// 得到所有的游戏信息
    bool GetGameInfos(std::vector<GameDataInfo> &pGameDataInfos);
    /// 玩家之间转钱
    int32 TransferAccounts(uint32 UserID,std::string receiverUser,int64 money);
    /// 得到玩家的金币
    bool GetUserMoney(uint32 UserId,int64 *money,int64 *bankmoney);

	/// 用于维护当前数据库连接
	void Update(void);

private:
	DataProvider *m_DataProvider;                               /**< 用于访问本地数据库 */
};

#define ServerDBOperator DBOperator::getSingleton()

#endif
