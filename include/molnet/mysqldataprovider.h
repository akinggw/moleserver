#ifndef _MYSQL_DATA_PROVIDER_H_INCLUDE
#define _MYSQL_DATA_PROVIDER_H_INCLUDE

/**
* MolNet网络引擎
*
* 描述:mysql的数据库操作实现类
* 作者:akinggw
* 日期:2010.2.28
*/

#include <iosfwd>
#include <string>

#include "common.h"
#include <mysql/mysql.h>

#include "connectPool.h"
#include "dataprovider.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"

/**
 * mysql的数据库操作实现类
 */
class MySqlDataProvider : public DataProvider
{
public:
	/**
	 * 用于替代mysql_autocommit()函数中my_bool类型
	 */
	enum
	{
		AUTOCOMMIT_OFF = 0,
		AUTOCOMMIT_ON
	};

	/// 构造函数
	MySqlDataProvider(void)
		throw();
	/// 析构函数
	~MySqlDataProvider(void)
		throw();

	/// 得到数据库的类型
	virtual DbBackends getDbBackend(void) const
		throw();
	/// 建立与数据库的连接
	virtual bool connect(std::string host,std::string username,std::string password,
		std::string dbName,unsigned int port);
	/// 执行SQL语句
	virtual const RecordSetList execSql(const std::string& sql,const bool refresh=false);
	/// 关闭与数据库的连接
	virtual void disconnect(void);
	/// 开始一项事物
	virtual void beginTransaction(void)
		throw (std::runtime_error);
	/// 提交一项事物
	virtual void commitTransaction(void)
		throw (std::runtime_error);
	/// 回滚一项事物
	virtual void rollbackTransaction(void)
		throw (std::runtime_error);
	/// 得到最近执行SQL语句后改变的行的个数
	virtual unsigned int getModifiedRows(void);
	/// 得到最近插入的数据的行号
	virtual unsigned int getLastId(void);
	/// 用于维护数据库连接
	virtual void Update(void);

private:
	ConnectPool m_ConnectPool;                             /**< 数据的连接池 */
};

#pragma clang diagnostic pop

#endif
