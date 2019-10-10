#ifndef _MOL_CONNECT_POOL_H_INCLUDE_
#define _MOL_CONNECT_POOL_H_INCLUDE_

/**
* MolNet网络引擎
*
* 描述:用于建立一个数据库的多个连接
* 作者:akinggw
* 日期:2010.8.17
*/

#include "Mutex.h"
#include "AtomicCounter.h"
#include <mysql/mysql.h>

#include <list>
#include <string>

class ConnectPool
{
public:
	/// 构造函数
	ConnectPool();
	/// 析构函数
	~ConnectPool();

	/// 初始化连接池
	bool Init(int max,std::string host,std::string user,std::string password,std::string db,int port);
	/// 关闭连接池
	void Close(void);

	/// 重新建立数据库连接
	bool ResetConnet(MYSQL* pMysql);
	/// 得到数据库连接
	MYSQL* GetConnet();
	/// 为系统新建一个新的连接
	bool NewConnect(void);
	/// 释放数据库连接
	void PutConnet(MYSQL* pMysql);
	/// 用于维护数据库连接
	void Update(void);

private:
	std::list<MYSQL*> m_connlist;                /**< mysql的连接操作列表 */
	Mutex m_connlistlock;                        /**< 连接池锁 */

	int m_max;                                   /**< 数据库连接的最大个数 */
	AtomicCounter m_BusyCount;                   /**< 当前使用的连接的个数 */

	std::string m_host;                          /**< 数据库IP地址 */
	std::string m_user;                          /**< 数据库用户 */
	std::string m_password;                      /**< 数据库用户密码 */
	std::string m_db;                            /**< 要连接的服务器的名称 */
	int m_port;                                  /**< 数据库端口号 */
};

#endif
