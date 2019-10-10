#include "../../include/molnet/connectPool.h"
#include "../../include/molnet/dalexcept.h"

/**
 * 构造函数
 */
ConnectPool::ConnectPool()
	: m_max(0),m_BusyCount(0),m_port(3306)
{

}

/**
 * 析构函数
 */
ConnectPool::~ConnectPool()
{
	Close();
}

/**
 * 初始化连接池
 *
 * @param max 连接池中连接数量
 * @param host 数据库IP地址
 * @param user 访问数据库用户名
 * @param password 用户密码
 * @param db 要访问的数据库名称
 * @param port 要访问的数据库端口
 *
 * @return 如果数据库连接池建立成功返回真，否则返回假
 */
bool ConnectPool::Init(int max,std::string host,std::string user,std::string password,std::string db,int port)
{
	m_max = max;
	m_host = host;
	m_user = user;
	m_password = password;
	m_db = db;
	m_port = port;

	for(int i=0;i<m_max;i++)
		NewConnect();

	return true;
}

/// 为系统新建一个新的连接
bool ConnectPool::NewConnect(void)
{
	m_connlistlock.Acquire();

	MYSQL *mDb = mysql_init(NULL);

	if(!mDb)
	{
		m_connlistlock.Release();
		throw DbConnectionFailure("不能够初始MySql:没有足够的内存。");
		return false;
	}

	int g_iTimeOut = 60;
	mysql_options(mDb, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&g_iTimeOut);
	mysql_options(mDb, MYSQL_OPT_READ_TIMEOUT, (const char *)&g_iTimeOut);
	mysql_options(mDb, MYSQL_OPT_WRITE_TIMEOUT, (const char *)&g_iTimeOut);

	if(!mysql_real_connect(mDb,
		m_host.c_str(),
		m_user.c_str(),
		m_password.c_str(),
		m_db.c_str(),
		m_port,
		NULL,
		CLIENT_MULTI_STATEMENTS))
	{
		std::string msg(mysql_error(mDb));
		mysql_close(mDb);

		m_connlistlock.Release();
		throw DbConnectionFailure(msg);
		return false;
	}

	//mysql_query(mDb,"set names utf8");

	my_bool my_true = true;
	mysql_options(mDb, MYSQL_OPT_RECONNECT, &my_true);

	// 设定字符集为"utf-8"
	mysql_options(mDb,MYSQL_SET_CHARSET_NAME,"utf8");
	mysql_set_character_set(mDb,"utf8");

	m_connlist.push_back(mDb);
	m_connlistlock.Release();

	return true;
}

/**
 * 关闭连接池
 */
void ConnectPool::Close(void)
{
	m_connlistlock.Acquire();
	MYSQL *pcurConn = NULL;
	while(!m_connlist.empty())
	{
		pcurConn = m_connlist.front();
		m_connlist.pop_front();
		if(pcurConn)
			mysql_close(pcurConn);
	}
	m_connlistlock.Release();
}

/**
 * 重新建立数据库连接
 *
 * @param pMysql 要建立的数据库连接
 *
 * @return 如果数据库连接建立成功返回真，否则返回假
 */
bool ConnectPool::ResetConnet(MYSQL* pMysql)
{
	mysql_close(pMysql);

	pMysql = mysql_init(NULL);

	if(!pMysql)
	{
		throw DbConnectionFailure("不能够初始MySql:没有足够的内存。");
		return false;
	}

	int g_iTimeOut = 60;
	mysql_options(pMysql, MYSQL_OPT_CONNECT_TIMEOUT, (const char *)&g_iTimeOut);
	mysql_options(pMysql, MYSQL_OPT_READ_TIMEOUT, (const char *)&g_iTimeOut);
	mysql_options(pMysql, MYSQL_OPT_WRITE_TIMEOUT, (const char *)&g_iTimeOut);

	if(!mysql_real_connect(pMysql,
						   m_host.c_str(),
						   m_user.c_str(),
						   m_password.c_str(),
						   m_db.c_str(),
						   m_port,
						   NULL,
						   CLIENT_MULTI_STATEMENTS))
	{
		std::string msg(mysql_error(pMysql));
		mysql_close(pMysql);

		throw DbConnectionFailure(msg);
		return false;
	}

	//mysql_query(pMysql,"set names utf8");

	my_bool my_true = true;
	mysql_options(pMysql, MYSQL_OPT_RECONNECT, &my_true);

	// 设定字符集为"utf-8"
	mysql_options(pMysql,MYSQL_SET_CHARSET_NAME,"utf8");
	mysql_set_character_set(pMysql,"utf8");

	return true;
}

/**
 * 得到当前空闲的数据库连接
 */
MYSQL* ConnectPool::GetConnet()
{
	MYSQL *pmySql = NULL;

	m_connlistlock.Acquire();

	// 如果池中没有连接的话，就新建连接
	if(m_connlist.empty())
		NewConnect();

	// 存在空闲的线程
	if(!m_connlist.empty())
	{
		//连接出池
		++m_BusyCount;

		pmySql = m_connlist.front();
		m_connlist.pop_front();

		// 如果这个连接已经断开，就迅速的为它建立连接！
		if(mysql_ping(pmySql))
			ResetConnet(pmySql);
	}

	m_connlistlock.Release();

	return pmySql;
}

/// 用于维护数据库连接
void ConnectPool::Update(void)
{
	if(m_connlist.empty()) return;

	m_connlistlock.Acquire();

	// 先清除多余的连接
	if((int)m_connlist.size() > MOL_CONN_POOL_MAX)
	{
		for(int index=0;index < (int)m_connlist.size()-MOL_CONN_POOL_MAX;index++)
		{
			if((*m_connlist.begin()) != NULL)
				mysql_close((*m_connlist.begin()));

			m_connlist.erase(m_connlist.begin());
		}
	}

	std::list<MYSQL*>::iterator iter = m_connlist.begin();
	for(;iter != m_connlist.end();++iter)
	{
		if((*iter) == NULL) continue;

		try
		{
			if(mysql_ping((*iter)))
				ResetConnet((*iter));
			//if(mysql_ping((*iter)) == 0)
			//	mysql_query((*iter),"set names utf8");
		}
		catch(std::exception e)
		{
			LOG_ERROR(mysql_error((*iter)));
		}
	}
	m_connlistlock.Release();
}

/**
 * 释放数据库连接
 *
 * @param pMysql 工作完后要释放的连接
 */
void ConnectPool::PutConnet(MYSQL* pMysql)
{
	if(pMysql == NULL) return;

	m_connlistlock.Acquire();

	--m_BusyCount;
	m_connlist.push_back(pMysql);

	m_connlistlock.Release();
}
