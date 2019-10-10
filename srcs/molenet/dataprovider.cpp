#include "../../include/molnet/dataprovider.h"

/**
 * 构造函数
 */
DataProvider::DataProvider(void)
	throw()
	: mIsConnected(false)
{

}

/**
 * 析构函数
 */
DataProvider::~DataProvider(void)
	throw()
{

}

/**
 * 得到数据库连接状态
 *
 * @return 如果数据库连接中返回真，否则返回假
 */
bool DataProvider::isConnected(void) const
	throw()
{
	return mIsConnected;
}

/**
 * 规则化SQL语句
 *
 * @param sql 要规则化的SQL语句
 *
 * @return 返回规则化后的sql语句
 */
std::string& DataProvider::escapeSQL(std::string& sql)
{
	size_t pos = 0;

	pos = sql.find("'", pos);
	while (pos != std::string::npos)
	{
		sql.replace(pos, 1, "\'\'");
		pos += 2;
		pos = sql.find("'", pos);
	}

	return sql;
}
