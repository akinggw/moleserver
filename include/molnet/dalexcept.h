#ifndef _DAL_EXCEPT_H_INCLUDE
#define _DAL_EXCEPT_H_INCLUDE

/**
* MolNet网络引擎
*
* 描述:用于处理数据库操作过程中可能的异常
* 作者:akinggw
* 日期:2010.2.27
*/

#include "common.h"
#include <string>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated"


/**
 * 基本的数据库异常
 */
class DbException : public std::exception
{
public:
	/**
	 * 构造函数
	 *
	 * @param msg 要抛出的错误消息
	 */
	DbException(const std::string& msg)
		throw()
			: mMsg(msg)
	{

	}

	/**
	 * 析构函数
	 */
	~DbException(void)
		throw()
	{

	}

	/**
	 * 得到抛出的错误消息
	 *
	 * @return 返回抛出异常的错误消息
	 */
	virtual const char* what(void) const
		throw()
	{
		return mMsg.c_str();
	}

private:
	std::string mMsg;
};

/**
 * 数据库连接失败
 */
class DbConnectionFailure : public DbException
{
public:
	/**
	 * 初始的构造函数
	 */
	DbConnectionFailure(void)
		throw()
			: DbException("")
	{

	}

	/**
	 * 构造函数
	 *
	 * @param msg 抛出异常的错误消息
	 */
	DbConnectionFailure(const std::string& msg)
		throw()
			: DbException(msg)
	{
		LOG_ERROR(msg.c_str());
	}
};

/**
 * 数据库断开连接失败
 */
class DbDisConnectionFailure : public DbException
{
public:
	/**
	 * 初始的构造函数
	 */
	DbDisConnectionFailure(void)
		throw()
			: DbException("")
	{

	}

	/**
	 * 带参数的构造函数
	 *
	 * @param msg 要抛出的异常错误消息
	 */
	DbDisConnectionFailure(const std::string& msg)
		throw()
			: DbException(msg)
	{
		LOG_ERROR(msg.c_str());
	}
};

/**
 * SQL 查询执行失败
 */
class DbSqlQueryExecFailure : public DbException
{
public:
	/**
	 * 初始的构造函数
	 */
	DbSqlQueryExecFailure(void)
		throw()
			: DbException("")
	{

	}

	/**
	 * 带参数的构造函数
	 *
	 * @param msg 要抛出的异常错误消息
	 */
	DbSqlQueryExecFailure(const std::string& msg)
		throw()
			: DbException(msg)
	{
		LOG_ERROR(msg.c_str());
	}
};

/**
 * 已经设置异常
 */
class AlreadySetException : public std::exception
{

};

/**
 * 缺少列头异常
 */
class RsColumnHeadersNotSet : public std::exception
{

};

#pragma clang diagnostic pop

#endif
