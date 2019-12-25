#ifndef _C_SERVER_SERVICE_MANAGER_H_INCLUDE_
#define _C_SERVER_SERVICE_MANAGER_H_INCLUDE_

#include "cdefines.h"

/**
 * 用于管理整个游戏逻辑
 */
class CServerServiceManager : public ServerServiceManager
{
public:
	/// 构造函数
	CServerServiceManager();
	/// 析构函数
	virtual ~CServerServiceManager();

	/// 设置玩家人数
	virtual void SetGamePlayerCount(int count);
	/// 得到玩家人数
	virtual int GetGamePlayerCount(void);
	/// 设置房间标识
	virtual void SetRoomMarking(int marking);
	/// 得到房间标识
	virtual int GetRoomMarking(void);
	/// 设置游戏名称
	virtual void SetGameName(const char* name);
	/// 得到游戏名称
	virtual const char* GetGameName(void);
	/// 设置游戏客户端组件名称
	virtual void SetGameModuleName(const char* name);
	/// 得到游戏客户端组件名称
	virtual const char* GetGameModuleName(void);
	/// 设置房间游戏类型
	virtual void SetRoomGameType(enStartMode type) { m_GameStartMode = type; }
	/// 得到房间游戏类型
	virtual enStartMode GetRoomGameType(void) { return m_GameStartMode; }
	/// 设置房间最小进入倍数
	virtual void SetRoomLastDouble(int pdouble);
	/// 得到房间最小进入倍数
	virtual int GetRoomLastDouble(void);
	/// 设置服务器端口号
	virtual void SetServerPort(int pport) { m_serverport = pport; }
	/// 得到服务器端口号
	virtual int GetServerPort(void) { return m_serverport; }

	//创建接口
public:
	//创建桌子
	virtual void * CreateTableFrameSink(void);
	//创建机器
	virtual void * CreateAndroidUserItemSink(void);

private:
	int m_maxPlayerCount;                         /**< 玩家人数 */
	int m_GameMarking;                            /**< 游戏标识 */
	std::string m_GameName;                       /**< 游戏名称 */
	std::string m_GameModuleName;                 /**< 游戏客户端组件名称 */
	enStartMode m_GameStartMode;                  /**< 游戏开始模式 */
	int m_RoomLastDouble;                         /**< 游戏房间最小进入倍数 */
	int m_serverport;
};

#endif
