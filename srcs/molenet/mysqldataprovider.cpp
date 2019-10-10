#include "../../include/molnet/mysqldataprovider.h"

#include "../../include/molnet/common.h"
#include "../../include/molnet/dalexcept.h"

/**
 * 构造函数
 */
MySqlDataProvider::MySqlDataProvider(void)
	throw()
{

}

/**
 * 析构函数
 */
MySqlDataProvider::~MySqlDataProvider(void)
	throw()
{
	if(mIsConnected)
		disconnect();
}

/**
 * 得到数据库的类型
 *
 * @return 用于返回当前数据库的类型
 */
DbBackends MySqlDataProvider::getDbBackend(void) const
	throw()
{
	return DB_BKEND_MYSQL;
}

/**
 * 建立与数据库的连接
 *
 * @param host 要连接的数据库的IP地址
 * @param username 要连接数据库的用户名
 * @param password 要连接数据库的用户密码
 * @param dbName 要连接的数据库名称
 * @param port 要连接的数据库的端口号
 */
bool MySqlDataProvider::connect(std::string host,std::string username,std::string password,
					 std::string dbName,unsigned int port)
{
	if(mIsConnected)
		return false;

	if(!m_ConnectPool.Init(MOL_CONN_POOL_MAX,
	                   host.c_str(),
					   username.c_str(),
					   password.c_str(),
					   dbName.c_str(),
					   port))
	    return false;

	mysql_thread_init();

	mDbName = dbName;

	mIsConnected = true;

	return true;
}

/**
 * 执行SQL语句
 *
 * @param sql 要执行的SQL语句
 * @param refresh 是否要刷新数据，也就是重新从数据库中获取数据
 *
 * @return 如果成功获取到数据返回这个数据记录，否则抛出异常
 */
const RecordSetList MySqlDataProvider::execSql(const std::string& sql,const bool refresh)
{
	if(!mIsConnected)
		throw std::runtime_error("没有与数据库建立连接。");

	RecordSetList pRecordSetList;

	try
	{
		if(refresh || (sql != mSql))
		{
			MYSQL *m_curWorkingConn = m_ConnectPool.GetConnet();
			if(m_curWorkingConn == NULL) return pRecordSetList;

			if(mysql_query(m_curWorkingConn,sql.c_str()) != 0)
			{
				m_ConnectPool.PutConnet(m_curWorkingConn);
				throw DbSqlQueryExecFailure(mysql_error(m_curWorkingConn)+sql);
			}

			do
			{
				int fieldCount = mysql_field_count(m_curWorkingConn);
				if(fieldCount > 0)
				{
					RecordSet pRecordSet;
					MYSQL_RES* res = NULL;

					if(!(res = mysql_store_result(m_curWorkingConn)))
					{
						m_ConnectPool.PutConnet(m_curWorkingConn);
						throw DbSqlQueryExecFailure(mysql_error(m_curWorkingConn));
					}

					unsigned int nFields = mysql_num_fields(res);
					//MYSQL_FIELD* fields = mysql_fetch_fields(res);
					Row fieldNames;
					for(unsigned int i=0;i<nFields;i++)
					{
						MYSQL_FIELD* fields = NULL;
						fields=mysql_fetch_field_direct(res,i);
						if(fields) fieldNames.push_back(fields->name ? fields->name : "");
					}

					pRecordSet.setColumnHeaders(fieldNames);

					MYSQL_ROW row;
					while((row = mysql_fetch_row(res)))
					{
						Row r;

						for(unsigned int i = 0;i < nFields; i++)
						{
							char *tmpStr = static_cast<char*>(row[i]);
							r.push_back(tmpStr ? tmpStr : "");
						}

						pRecordSet.add(r);
					}

					if(!pRecordSet.isEmpty())
						pRecordSetList.add(pRecordSet);

					mysql_free_result(res);
				}
			} while (!mysql_next_result( m_curWorkingConn ));

			m_ConnectPool.PutConnet(m_curWorkingConn);
		}
	}
	catch(std::exception e)
	{
		LOG_ERROR(e.what());
	}

	return pRecordSetList;
}

/**
 * 关闭与数据库的连接
 */
void MySqlDataProvider::disconnect(void)
{
	if(!mIsConnected)
		return;

	m_ConnectPool.Close();

	mysql_thread_end();
	mysql_library_end();

	mIsConnected = false;
}

/**
 * 开始一项事物
 */
void MySqlDataProvider::beginTransaction(void)
	throw (std::runtime_error)
{
	if(!mIsConnected)
	{
		const std::string error = "在没有连接数据库的情况下开始一项事务!";

		throw std::runtime_error(error);
	}

	MYSQL *m_curWorkingConn = m_ConnectPool.GetConnet();
	if(m_curWorkingConn == NULL) return;

	mysql_autocommit(m_curWorkingConn,AUTOCOMMIT_OFF);
	execSql("BEGIN");

	m_ConnectPool.PutConnet(m_curWorkingConn);
}

/**
 * 提交一项事物
 */
void MySqlDataProvider::commitTransaction(void)
	throw (std::runtime_error)
{
	if(!mIsConnected)
	{
		const std::string error = "在没有连接数据库的情况下提交一项事务!";

		throw std::runtime_error(error);
	}

	MYSQL *m_curWorkingConn = m_ConnectPool.GetConnet();
	if(m_curWorkingConn == NULL) return;

	if(mysql_commit(m_curWorkingConn) != 0)
		throw DbSqlQueryExecFailure(mysql_error(m_curWorkingConn));

	mysql_autocommit(m_curWorkingConn,AUTOCOMMIT_ON);

	m_ConnectPool.PutConnet(m_curWorkingConn);
}

/**
 * 回滚一项事物
 */
void MySqlDataProvider::rollbackTransaction(void)
	throw (std::runtime_error)
{
	if(!mIsConnected)
	{
		const std::string error = "在没有连接数据库的情况下回滚一项事务!";

		throw std::runtime_error(error);
	}

	MYSQL *m_curWorkingConn = m_ConnectPool.GetConnet();
	if(m_curWorkingConn == NULL) return;

	if(mysql_rollback(m_curWorkingConn) != 0)
		throw DbSqlQueryExecFailure(mysql_error(m_curWorkingConn));

	mysql_autocommit(m_curWorkingConn,AUTOCOMMIT_ON);

	m_ConnectPool.PutConnet(m_curWorkingConn);
}

/**
 * 得到最近执行SQL语句后改变的行的个数
 *
 * @return 返回最近改变的行数
 */
unsigned int MySqlDataProvider::getModifiedRows(void)
{
	if(!mIsConnected)
	{
		const std::string error = "在没有连接数据库的情况下试图执行getModifiedRows!";

		throw std::runtime_error(error);
	}

	MYSQL *m_curWorkingConn = m_ConnectPool.GetConnet();
	if(m_curWorkingConn == NULL) return 0;

	const my_ulonglong affected = mysql_affected_rows(m_curWorkingConn);

	if(affected > INT_MAX)
		throw std::runtime_error("MySqlDataProvider: getModifiedRows 超出范围.");

	if(affected == (my_ulonglong)-1)
	{
		throw DbSqlQueryExecFailure(mysql_error(m_curWorkingConn));
	}

	m_ConnectPool.PutConnet(m_curWorkingConn);

	return (unsigned int)affected;
}

/**
 * 得到最近插入的数据的行号
 *
 * @return 返回改变数据的最新的行号
 */
unsigned int MySqlDataProvider::getLastId(void)
{
	if(!mIsConnected)
	{
		const std::string error = "在没有连接数据库的情况下试图执行getLastId!";

		throw std::runtime_error(error);
	}

	MYSQL *m_curWorkingConn = m_ConnectPool.GetConnet();
	if(m_curWorkingConn == NULL) return 0;

	const my_ulonglong lastId = mysql_insert_id(m_curWorkingConn);
	if(lastId > UINT_MAX)
		throw std::runtime_error("MySqlDataProvider: getLastId 超出范围.");

	m_ConnectPool.PutConnet(m_curWorkingConn);

	return (unsigned int)lastId;
}

/// 用于维护数据库连接
void MySqlDataProvider::Update(void)
{
	m_ConnectPool.Update();
}
