#include "DBOperator.h"

initialiseSingleton(DBOperator);

/**
 * 构造函数
 */
DBOperator::DBOperator()
: m_DataProvider(NULL)
{
	m_DataProvider = DataProviderFactory::createDataProvider();
}

/**
 * 析构函数
 */
DBOperator::~DBOperator()
{
	Shutdown();

	if(m_DataProvider)
		delete m_DataProvider;
	m_DataProvider = NULL;
}

/// 用于维护当前数据库连接
void DBOperator::Update(void)
{
	if(m_DataProvider == NULL) return;

	m_DataProvider->Update();
}


/**
 * 初始数据库
 *
 * @param host 要连接的数据库的IP地址
 * @param user 连接数据库的用户名
 * @param pass 连接数据库的用户密码
 * @param db 要连接的数据库名称
 * @param port 数据库端口号
 *
 * @return 如果数据库连接成功返回真，否则返回假
 */
bool DBOperator::Initilize(std::string host,std::string user,std::string pass,std::string db,int port)
{
	if(m_DataProvider == NULL)
		return false;

	return m_DataProvider->connect(host,user,pass,db,port);
}

/**
 * 关闭数据库连接
 */
void DBOperator::Shutdown(void)
{
	if(m_DataProvider == NULL)
		return;

	m_DataProvider->disconnect();
}

/**
 * 根据玩家名称和密码检测这个玩家是否存在
 *
 * @param name 要检测的玩家的姓名
 * @param password 要检测的玩家的密码
 *
 * @return 如果玩家存在返回玩家的ID，不存在就返回-1
 */
unsigned int DBOperator::IsExistUser(std::string name,std::string password)
{
	if(m_DataProvider == NULL || name.empty() || password.empty()) return 0;

	std::ostringstream sqlstr;
	sqlstr << "call isexistuser('" << name << "','" << password << "');";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atol(pRecord(0)(0,0).c_str());
}

/// 得到当前注册玩家数量
unsigned int DBOperator::GetTotalUserCount(void)
{
	if(m_DataProvider == NULL) return 0;

	std::ostringstream sqlstr;
	sqlstr << "select count(uid) from mol_member;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atol(pRecord(0)(0,0).c_str());
}

/// 根据玩家名称检测这个玩家是否存在
unsigned int DBOperator::IsExistUser(std::string name)
{
	if(m_DataProvider == NULL || name.empty()) return 0;

	std::ostringstream sqlstr;
	sqlstr << "select uid from mol_member where username='" << name << "' and gtype=0 limit 1;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atol(pRecord(0)(0,0).c_str());
}

/// 根据玩家ID更新玩家密码
bool DBOperator::UpdateUserPassword(uint32 UserId,std::string pwd)
{
	if(m_DataProvider == NULL || UserId <= 0 || pwd.empty()) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_member set"
		<< " password='" << pwd << "',"
		<< " bankpassword='" << pwd << "'"
		<< " where uid=" << UserId << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// 更改指定玩家的银行密码
bool DBOperator::UpdateUserBankPassword(uint32 UserID,std::string decPWD)
{
	if(m_DataProvider == NULL || UserID <= 0 || decPWD.empty()) return false;

	std::ostringstream sqlstr;

	sqlstr << "update mol_member set bankpassword='" << decPWD << "' where uid=" << UserID << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// 更改指定玩家的登陆密码
bool DBOperator::UpdateUserLoginPassword(uint32 UserId,std::string decPWD)
{
	if(m_DataProvider == NULL || UserId <= 0 || decPWD.empty()) return false;

	std::ostringstream sqlstr;

	sqlstr << "update mol_member set password='" << decPWD << "' where uid=" << UserId << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}


/**
 * 注册用户
 */
int32 DBOperator::RegisterGameUser(std::string name,std::string password,std::string email,
								  int sex,std::string realname,std::string telephone,std::string AvatorIndex,std::string Referrer,std::string ipaddress,std::string cardnumber)
{
	if(m_DataProvider == NULL)
		return -1;

	std::ostringstream sqlstr;
	sqlstr << "call registergameuser('"
		   << name << "','"
		   << password << "','"
		   << email << "',"
		   << sex << ",'"
		   << realname << "','"
		   << telephone << "','"
		   << AvatorIndex << "','"
		   << Referrer << "','"
		   << ipaddress << "','"
		   << cardnumber << "');";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return -1;

	return atoi(pRecord(0)(0,0).c_str());
}

/// 更新用户最近登录IP和最近登录时间
bool DBOperator::UpdatePlayerLastLogin(uint32 UserID,std::string ipaddress,std::string machinecode)
{
	if(m_DataProvider == NULL || UserID <= 0 || ipaddress.empty()) return false;

	std::ostringstream sqlstr;
	sqlstr << "update mol_member set"
		   << " ipaddress='" << ipaddress << "',"
		   << " machinecode='" << machinecode
		   << "',lastlogintime=unix_timestamp(NOW())"
		   << " where uid=" << UserID << ";";

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// 锁定指定玩家的机器
bool DBOperator::LockMachineByUser(uint32 UserId,int operType)
{
	if(m_DataProvider == NULL || UserId <= 0 ) return false;

	std::ostringstream sqlstr;

	switch(operType)
	{
	case 1:               //锁机
		{
			sqlstr << "update mol_member set"
				<< " glockmachine=1"
				<< " where uid=" << UserId << ";";
		}
		break;
	case 2:               // 解锁
		{
			sqlstr << "update mol_member set"
				<< " glockmachine=0"
				<< " where uid=" << UserId << ";";
		}
		break;
	default:
		break;
	}

	m_DataProvider->execSql(sqlstr.str());

	return true;
}

/// 更新指定玩家的信息
bool DBOperator::UpdateUserInfo(uint32 UserID,std::string nickname,std::string email,std::string telephone,std::string qq,std::string useravatar,int sex)
{
	if(m_DataProvider == NULL || UserID <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "call updateuserinfo("
		<< UserID << ",'"
		<< nickname << "','"
		<< email << "','"
		<< telephone << "','"
		<< qq << "','"
		<< useravatar << "',"
		<< sex << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	return atoi(pRecord(0)(0,0).c_str()) > 0 ? true : false;
}

/**
 * 根据用户ID得到用户的游戏数据
 *
 * @param UserId 要取得的用户的ID
 * @param UserData 如果取得用户数据成功，这里用于存储取得的用户数据
 *
 * @return 如果取得用户数据成功返回真，否则返回假
 */
bool DBOperator::GetUserData(uint32 UserId,MemberDataStru &UserData)
{
	if(m_DataProvider == NULL || UserId <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "call getuserdata(" << UserId << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	UserData.uid = atol(pRecord(0)(0,0).c_str());
	UserData.gtype = atoi(pRecord(0)(0,1).c_str());
	strncpy(UserData.username , pRecord(0)(0,2).c_str(),CountArray(UserData.username));
	strncpy(UserData.password , pRecord(0)(0,3).c_str(),CountArray(UserData.password));
	strncpy(UserData.bankpassword , pRecord(0)(0,5).c_str(),CountArray(UserData.bankpassword));
	strncpy(UserData.email , pRecord(0)(0,6).c_str(),CountArray(UserData.email));
	UserData.sex = atoi(pRecord(0)(0,7).c_str());
	strncpy(UserData.realname , pRecord(0)(0,8).c_str(),CountArray(UserData.realname));
	strncpy(UserData.homeplace , pRecord(0)(0,9).c_str(),CountArray(UserData.homeplace));
	strncpy(UserData.telephone , pRecord(0)(0,10).c_str(),CountArray(UserData.telephone));
	strncpy(UserData.QQ , pRecord(0)(0,11).c_str(),CountArray(UserData.QQ));
	strncpy(UserData.ipaddress , pRecord(0)(0,12).c_str(),CountArray(UserData.ipaddress));
	UserData.createtime = atol(pRecord(0)(0,13).c_str());
	UserData.lastlogintime = atol(pRecord(0)(0,14).c_str());
	UserData.state = atoi(pRecord(0)(0,15).c_str());
	strncpy(UserData.machinecode , pRecord(0)(0,18).c_str(),CountArray(UserData.machinecode));
	UserData.glockmachine = atoi(pRecord(0)(0,19).c_str());

	UserData.money = atol(pRecord(0)(0,21).c_str());
	UserData.bankmoney = atol(pRecord(0)(0,22).c_str());
	UserData.level = atoi(pRecord(0)(0,23).c_str());
	UserData.experience = atoi(pRecord(0)(0,24).c_str());

	return true;
}

/// 得到当前在线的所有服务器
bool DBOperator::GetOnlineGameRooms(std::vector<tagGameRoom> &pgamerooms)
{
	if(m_DataProvider == NULL) return false;

	std::ostringstream sqlstr;
	sqlstr << "select * from mol_gameroom where state=0;";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	for(int i=0;i<pRecord.Count();i++)
    {
        tagGameRoom ptagGameRoom;
        ptagGameRoom.gameid = atoi(pRecord(i)(0,1).c_str());
        ptagGameRoom.gamingtype = atoi(pRecord(i)(0,7).c_str());
        ptagGameRoom.serverport = atoi(pRecord(i)(0,3).c_str());
        ptagGameRoom.lastmoney = atol(pRecord(i)(0,8).c_str());
        ptagGameRoom.pielement = atol(pRecord(i)(0,9).c_str());
        ptagGameRoom.roomrevenue = atoi(pRecord(i)(0,10).c_str());
        ptagGameRoom.tablecount = atoi(pRecord(i)(0,4).c_str());
        ptagGameRoom.tableplayercount = atoi(pRecord(i)(0,5).c_str());
        ptagGameRoom.state = atoi(pRecord(i)(0,14).c_str());

        strncpy(ptagGameRoom.serverip , pRecord(i)(0,6).c_str(),CountArray(ptagGameRoom.serverip));
        strncpy(ptagGameRoom.servername , pRecord(i)(0,2).c_str(),CountArray(ptagGameRoom.servername));

        pgamerooms.push_back(ptagGameRoom);
    }

	return true;
}

/// 玩家之间转钱
int32 DBOperator::TransferAccounts(uint32 UserID,std::string receiverUser,int64 money)
{
	if(m_DataProvider == NULL || UserID <= 0 || money <= 0 || receiverUser.empty()) return 0;

	std::ostringstream sqlstr;
	sqlstr << "call transferaccounts("
		<< UserID << ",'"
		<< receiverUser << "',"
		<< money << ");";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return 0;

	return atoi(pRecord(0)(0,0).c_str());
}

/// 得到玩家的金币
bool DBOperator::GetUserMoney(uint32 UserId,int64 *money,int64 *bankmoney)
{
	if(m_DataProvider == NULL || UserId <= 0) return false;

	std::ostringstream sqlstr;
	sqlstr << "select money,bankmoney from mol_userdata where userid=" << UserId << ";";

	RecordSetList pRecord = m_DataProvider->execSql(sqlstr.str());
	if(pRecord.isEmpty()) return false;

	*money = _atoi64(pRecord(0)(0,0).c_str());
	*bankmoney = _atoi64(pRecord(0)(0,1).c_str());

	return true;
}

