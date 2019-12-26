#ifndef _DATA_PROVIDER_H_INCLUDE
#define _DATA_PROVIDER_H_INCLUDE

/**
* MolNet网络引擎
*
* 描述:数据库操作的基类，所有的实现类从这个上面派生
* 作者:akinggw
* 日期:2010.2.28
*/

#include <string>
#include <stdexcept>

#include "recordset.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"

/**
 * 我们要操作的数据库类型
 */
typedef enum
{
	DB_BKEND_MYSQL = 0,
	DB_BKEND_END
} DbBackends;

/**
 * 数据库操作基类
 */
class DataProvider
{
public:
	/// 构造函数
	DataProvider(void)
		throw();
	/// 析构函数
	virtual ~DataProvider(void)
		throw();

	/// 得到数据库连接状态
	bool isConnected(void) const
		throw();
	/// 得到数据库的类型
	virtual DbBackends getDbBackend(void) const
		throw() = 0;
	/// 建立与数据库的连接
	virtual bool connect(std::string host,std::string username,std::string password,
		std::string dbName,unsigned int port) = 0;
	/// 执行SQL语句
	virtual const RecordSetList execSql(const std::string& sql,const bool refresh=false) = 0;
	/// 关闭与数据库的连接
	virtual void disconnect(void) = 0;
	/// 得到数据库的名称
	std::string getDbName(void);
	/// 开始一项事物
	virtual void beginTransaction(void)
		throw (std::runtime_error) = 0;
	/// 提交一项事物
	virtual void commitTransaction(void)
		throw (std::runtime_error) = 0;
	/// 回滚一项事物
	virtual void rollbackTransaction(void)
		throw (std::runtime_error) = 0;
	/// 得到最近执行SQL语句后改变的行的个数
	virtual unsigned int getModifiedRows(void) = 0;
	/// 得到最近插入的数据的行号
	virtual unsigned int getLastId(void) = 0;
	/// 规则化SQL语句
	std::string& escapeSQL(std::string& sql);
	/// 用于维护数据库连接
	virtual void Update(void) = 0;

protected:
	std::string mDbName;                /**< 数据库的名称 */
	bool mIsConnected;                  /**< 数据库的连接状态 */
	std::string mSql;                   /**< 用于存储最近的SQL语句 */
};

#pragma clang diagnostic pop

#endif
